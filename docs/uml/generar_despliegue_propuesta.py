"""Dibuja la vista UML de despliegue con coordenadas estables y texto editable.

El modelo PlantUML equivalente está en fuentes/01_despliegue.puml.
Este diseño fija la disposición editorial para mantener la legibilidad.
"""
from pathlib import Path
from html import escape

ROOT = Path(__file__).resolve().parent
W, H = 820, 1080
out = []

def text(x, y, lines, size=16, weight='normal', anchor='middle', color='#183D4C'):
    if isinstance(lines,str): lines=[lines]
    for i, s in enumerate(lines):
        out.append(f'<text x="{x}" y="{y+i*(size+5)}" font-size="{size}" font-weight="{weight}" text-anchor="{anchor}" fill="{color}">{escape(s)}</text>')

def rect(x,y,w,h,fill='#F4F8FA',stroke='#5A7D8E',r=0):
    out.append(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{r}" fill="{fill}" stroke="{stroke}" stroke-width="1.2"/>')

def group(x,y,w,h,label,fill):
    rect(x,y,w,h,fill,'#8CA7B2',5)
    text(x+16,y+25,label,18,'bold','start')

def node(x,y,w,h,label,stereo='servicioAdministrado',fill='#F4F8FA'):
    d=8
    out.append(f'<path d="M{x},{y} l{d},-{d} h{w} v{h} l-{d},{d} Z" fill="#EDF4F7" stroke="#5A7D8E"/>')
    out.append(f'<path d="M{x+w},{y} l{d},-{d} M{x+w},{y} v{h}" fill="none" stroke="#5A7D8E"/>')
    rect(x,y,w,h,fill)
    if stereo: text(x+w/2,y+21,f'«{stereo}»',13)
    text(x+w/2,y+44,label,17,'bold')

def artifact(x,y,w,h,lines):
    fold=9
    out.append(f'<path d="M{x},{y} H{x+w-fold} L{x+w},{y+fold} V{y+h} H{x} Z" fill="white" stroke="#789AA8"/>')
    out.append(f'<path d="M{x+w-fold},{y} V{y+fold} H{x+w}" fill="none" stroke="#789AA8"/>')
    text(x+w/2,y+22,lines,15)

def line(path, dashed=False, arrow=False):
    # Halo blanco en los cruces: un cruce sin punto no es una unión.
    out.append(f'<path d="{path}" fill="none" stroke="white" stroke-width="5"/>')
    dash=' stroke-dasharray="6 5"' if dashed else ''
    tip=' marker-end="url(#open)"' if arrow or dashed else ''
    out.append(f'<path d="{path}" fill="none" stroke="#355E71" stroke-width="1.5"{dash}{tip}/>')

def main():
    out.clear()
    out.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}">')
    out.append('<defs><marker id="open" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="8" markerHeight="8" orient="auto-start-reverse"><path d="M1,1 L9,5 L1,9" fill="none" stroke="#355E71" stroke-width="1.5"/></marker></defs>')
    out.append('<rect width="100%" height="100%" fill="white"/><g font-family="DejaVu Sans, sans-serif">')
    group(12,8,796,238,'Sistema local · IoT Edge','#F0F8F3')
    for x, name, lines in [
        (28,'ESP1 : ESP32-C6',['Firmware de adquisición','Voltaje · RPM · GPS']),
        (296,'ESP2 : ESP32-C6',['Firmware de integración','Ambiente · RTC · SD']),
        (564,'ESP3 : ESP32-C6',['Firmware de enlace cloud','UART · Wi-Fi · MQTT'])]:
        node(x,57,224,130,name,'device')
        artifact(x+10,113,204,61,lines)
    line('M140,187 V210 H408 V187',arrow=True)
    text(270,205,'ESP-NOW',15)
    line('M452,187 V233 H676 V187',arrow=True)
    text(565,226,'UART · 9600 bit/s',15)

    group(12,307,796,503,'AWS · Servicios administrados','#F3F6FC')
    node(28,355,216,124,'AWS IoT Core','executionEnvironment')
    artifact(39,415,194,48,['Regla IoT'])
    node(292,355,250,210,'AWS Lambda','executionEnvironment')
    artifact(302,410,230,39,['ActualizarUltimoDatoDynamo'])
    artifact(302,458,230,39,['GetLatest'])
    artifact(302,506,230,39,['GetTimeseries'])
    node(596,355,192,95,['DynamoDB','turbina_latest'])
    node(28,615,216,83,['Amazon S3','Histórico JSON'])
    node(304,615,220,83,['Amazon Athena','Consulta SQL'])
    node(584,615,204,83,['Glue Data Catalog','Metadatos'])
    node(28,735,216,60,'CloudFront')
    node(584,735,204,60,'S3 · Sitio web')

    # Dispositivos a servicios y dependencias de la nube.
    line('M760,187 V279 H20 V397 H28')
    text(433,272,'Wi-Fi + Internet · MQTT/TLS · 8883',16)
    line('M244,389 H292',dashed=True)
    text(268,381,'invoca',13)
    line('M542,483 H686 V450',dashed=True)
    text(679,504,['Actualizar / consultar','por clientId'],14)
    line('M136,479 V615',dashed=True)
    text(145,546,['Guardar','JSON'],15,anchor='start')
    line('M417,565 V615',dashed=True)
    text(428,595,'SQL',14,anchor='start')
    line('M304,663 H244',dashed=True)
    text(262,652,'lee',13)
    line('M524,663 H584',dashed=True)
    text(554,652,'esquema',12)
    line('M244,768 H584',dashed=True)
    text(414,758,'Origen del sitio · HTTPS',15)

    node(294,855,248,124,'Navegador del usuario','executionEnvironment','#F0F8F3')
    artifact(304,916,228,47,['Dashboard React / Vite'])
    line('M294,900 H136 V795')
    text(173,858,['HTTPS','Cargar sitio'],15)
    # Corredor central sin conexiones a las líneas que cruza.
    line('M294,926 H282 V534 H292')
    text(260,835,'HTTPS · Function URLs',15)
    text(418,1009,'El navegador ejecuta el dashboard; S3 aloja sus archivos compilados.',14)
    text(25,1040,'Nodo 3D: dispositivo o entorno. Pliegue: artefacto desplegado.',14,anchor='start')
    text(25,1062,'Línea continua: comunicación. Flecha discontinua: dependencia o uso.',14,anchor='start')
    out.append('</g></svg>')
    dest=ROOT/'figuras'/'01_despliegue.svg'
    dest.parent.mkdir(parents=True,exist_ok=True)
    dest.write_text('\n'.join(out),encoding='utf-8')
    return dest

if __name__=='__main__':
    print(main())
