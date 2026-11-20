import os
import time
import boto3
from decimal import Decimal


# =========================
# Configuración
# =========================
TABLE_NAME = os.environ.get("TABLE_NAME", "turbina_latest")

ddb = boto3.resource("dynamodb")
table = ddb.Table(TABLE_NAME)


# =========================
# Helpers
# =========================
def to_decimal(x):
    """
    DynamoDB no acepta float nativo; requiere Decimal.
    Este helper convierte números (int/float/str numérico) a Decimal.
    """
    if x is None:
        return None
    try:
        # str() evita problemas de precisión con float
        return Decimal(str(x))
    except Exception:
        return None


def safe_bool(x):
    """
    Normaliza booleans que pueden llegar como True/False, "true"/"false", 1/0.
    """
    if x is None:
        return None
    if isinstance(x, bool):
        return x
    if isinstance(x, (int, float)):
        return bool(x)
    if isinstance(x, str):
        v = x.strip().lower()
        if v in ("true", "t", "1", "yes", "y"):
            return True
        if v in ("false", "f", "0", "no", "n"):
            return False
    return None


# =========================
# Handler
# =========================
def lambda_handler(event, context):
    """
    Espera un 'event' JSON proveniente de AWS IoT Rule:
      SELECT *, clientid() as clientId FROM 'sensor/data'

    Ejemplo:
    {
      "timestamp":"15/05/26 12:46:27",
      "wind_speed":0.0,
      "voltage":0.26,
      "temperature":21.58,
      "humidity":83.44,
      "pressure":1016.46,
      "altitude":1395.49,
      "lat":0,
      "lon":0,
      "rpm":-1,
      "esp_now_connected":true,
      "memory_usage":18.41,
      "clientId":"ESP32C6-AWS"
    }
    """

    # 1) Determinar clientId (clave primaria)
    client_id = event.get("clientId") or event.get("clientid") or event.get("client_id") or "UNKNOWN"

    # 2) Construir el item "último estado" (se sobrescribe siempre)
    now_epoch = int(time.time())

    item = {
        # PK de la tabla (según tu DynamoDB): clientId (String)
        "clientId": client_id,

        # Metadatos
        "updatedAtEpoch": now_epoch,
        "timestamp": event.get("timestamp"),

        # Variables numéricas (Decimal)
        "wind_speed": to_decimal(event.get("wind_speed")),
        "voltage": to_decimal(event.get("voltage")),
        "temperature": to_decimal(event.get("temperature")),
        "humidity": to_decimal(event.get("humidity")),
        "pressure": to_decimal(event.get("pressure")),
        "altitude": to_decimal(event.get("altitude")),
        "lat": to_decimal(event.get("lat")),
        "lon": to_decimal(event.get("lon")),
        "rpm": to_decimal(event.get("rpm")),
        "memory_usage": to_decimal(event.get("memory_usage")),

        # Boolean
        "esp_now_connected": safe_bool(event.get("esp_now_connected")),
    }

    # 3) Limpieza: elimina claves con None para mantener el item limpio
    item = {k: v for k, v in item.items() if v is not None}

    # 4) Guardar (PutItem sobrescribe por PK → siempre queda el último dato)
    table.put_item(Item=item)

    return {
        "ok": True,
        "table": TABLE_NAME,
        "clientId": client_id,
        "updatedAtEpoch": now_epoch
    }