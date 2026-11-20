import { useEffect, useMemo, useRef, useState } from "react";
import {
  ResponsiveContainer,
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
} from "recharts";

// =========================
// Configuración general
// =========================
const TS_URL = (import.meta.env.VITE_TIMESERIES_URL || "").trim();
const CLIENT_ID = (import.meta.env.VITE_CLIENT_ID || "ESP32C6-AWS").trim();

// =========================
// Catálogo de variables
// =========================
const METRICS = {
  temperature: { label: "Temperatura", unit: "°C", color: "#2ea8ff", decimals: 2 },
  humidity: { label: "Humedad", unit: "%", color: "#39d98a", decimals: 2 },
  pressure: { label: "Presión", unit: "hPa", color: "#f5b342", decimals: 2 },
  altitude: { label: "Altitud", unit: "m", color: "#c084fc", decimals: 2 },
  wind_speed: { label: "Velocidad del viento", unit: "m/s", color: "#00d4ff", decimals: 2 },
  rpm: { label: "RPM", unit: "rpm", color: "#ff7b72", decimals: 0 },
  voltage: { label: "Voltaje", unit: "V", color: "#ffd166", decimals: 2 },
  memory_usage: { label: "Memoria usada", unit: "%", color: "#7ee787", decimals: 2 },
};

// =========================
// Rangos (simple y útil)
// =========================
const RANGE_OPTIONS = [
  { value: "15m", label: "Últimos 15 min", minutes: 15, autoRefresh: true },
  { value: "1h", label: "Última hora", minutes: 60, autoRefresh: true },
  { value: "24h", label: "Últimas 24 horas", minutes: 1440, autoRefresh: false },
  { value: "7d", label: "Últimos 7 días", minutes: 10080, autoRefresh: false },
];

// =========================
// Tooltip personalizado
// =========================
function CustomTooltip({ active, payload, label, metricInfo }) {
  if (!active || !payload || !payload.length) return null;

  const value = payload[0]?.value;
  const point = payload[0]?.payload;

  return (
    <div
      style={{
        background: "#f3f4f6",
        color: "#0f172a",
        padding: "10px 14px",
        borderRadius: 8,
        border: "1px solid #d1d5db",
        boxShadow: "0 4px 14px rgba(0,0,0,0.2)",
        minWidth: 170,
      }}
    >
      <div style={{ fontWeight: 800, marginBottom: 6 }}>{metricInfo.label}</div>
      <div style={{ marginBottom: 4 }}>
        <strong>Hora:</strong> {label}
      </div>
      <div>
        <strong>Valor:</strong>{" "}
        {Number(value).toFixed(metricInfo.decimals)} {metricInfo.unit}
      </div>
      {point?.fullTs && (
        <div style={{ marginTop: 6, fontSize: 12, color: "#475569" }}>
          {point.fullTs}
        </div>
      )}
    </div>
  );
}

export default function Historico() {
  const [metric, setMetric] = useState("temperature");
  const [range, setRange] = useState("1h");

  const [chartData, setChartData] = useState([]);
  const [loading, setLoading] = useState(false);
  const [errorHist, setErrorHist] = useState("");

  // Trigger manual/auto para refrescar
  const [refreshTick, setRefreshTick] = useState(0);

  // Countdown de auto-refresh
  const [nextRefreshIn, setNextRefreshIn] = useState(null);
  const intervalRef = useRef(null);

  // Ref para saber si estamos cargando (para que el auto-refresh no pise una consulta en curso)
  const loadingRef = useRef(false);

  const metricInfo = useMemo(() => METRICS[metric], [metric]);

  const rangeInfo = useMemo(() => {
    return RANGE_OPTIONS.find((r) => r.value === range) || RANGE_OPTIONS[1];
  }, [range]);

  const minutes = rangeInfo.minutes;
  const autoRefreshEnabled = rangeInfo.autoRefresh;

  function refreshNow() {
    setRefreshTick((x) => x + 1);
  }

  // =========================
  // Auto-refresh (solo 15m/1h)
  // =========================
  useEffect(() => {
    if (intervalRef.current) {
      clearInterval(intervalRef.current);
      intervalRef.current = null;
    }

    if (!autoRefreshEnabled) {
      setNextRefreshIn(null);
      return;
    }

    // Intervalo de auto-refresh
    let seconds = 60;
    setNextRefreshIn(seconds);

    intervalRef.current = setInterval(() => {
      seconds -= 1;
      setNextRefreshIn(seconds);

      if (seconds <= 0) {
        // Si hay una consulta en curso, no dispares otra; reintenta pronto
        if (!loadingRef.current) {
          refreshNow();
          seconds = 60;
          setNextRefreshIn(seconds);
        } else {
          // reintento suave
          seconds = 10;
          setNextRefreshIn(seconds);
        }
      }
    }, 1000);

    return () => {
      if (intervalRef.current) {
        clearInterval(intervalRef.current);
        intervalRef.current = null;
      }
    };
  }, [autoRefreshEnabled, metric, range]);

  // =========================
  // Carga histórico (API Athena)
  // =========================
  useEffect(() => {
    if (!TS_URL) {
      setErrorHist("No existe VITE_TIMESERIES_URL en el archivo .env");
      return;
    }

    let cancelled = false;
    const controller = new AbortController();

    // Diferenciar abort por timeout vs abort por cleanup
    let didTimeout = false;

    // Timeout adaptativo: corto vs largo
    // (Athena + Lambda puede variar, damos margen)
    const timeoutMs = minutes <= 60 ? 25000 : 60000; // 25s corto, 60s largo

    const timeoutId = setTimeout(() => {
      didTimeout = true;
      controller.abort();
    }, timeoutMs);

    async function loadHistory() {
      try {
        loadingRef.current = true;
        setLoading(true);

        // Importante: NO borramos el chartData; así si falla, la gráfica no "parpadea"
        // Solo limpiamos error al iniciar
        setErrorHist("");

        const queryUrl = `${TS_URL}?metric=${metric}&minutes=${minutes}&clientId=${CLIENT_ID}`;

        const response = await fetch(queryUrl, {
          method: "GET",
          signal: controller.signal,
          cache: "no-store",
        });

        const text = await response.text();

        let data;
        try {
          data = JSON.parse(text);
        } catch {
          throw new Error(
            `Respuesta no JSON (HTTP ${response.status}). Inicio: ${text.slice(0, 120)}`
          );
        }

        if (!response.ok || !data.ok) {
          throw new Error(data.error || `HTTP ${response.status}`);
        }

        const points = (data.points || []).map((p) => {
          let hhmm = p.ts;
          try {
            const dt = new Date(p.ts.replace(" ", "T"));
            hhmm = dt.toLocaleTimeString([], {
              hour: "2-digit",
              minute: "2-digit",
              hour12: false,
            });
          } catch {
            // se queda con p.ts
          }
          return { time: hhmm, fullTs: p.ts, value: Number(p.value) };
        });

        if (cancelled) return;

        setChartData(points);
        setErrorHist(""); // éxito: limpia cualquier error anterior
      } catch (err) {
        if (cancelled) return;

        // AbortError puede ser por timeout (real) o por cleanup (normal)
        const isAbort =
          err?.name === "AbortError" ||
          /aborted/i.test(err?.message || "") ||
          /canceled/i.test(err?.message || "");

        if (isAbort) {
          if (didTimeout) {
            // Solo mostramos error si realmente fue timeout
            setErrorHist(
              "Error histórico: la consulta tardó demasiado y se canceló (timeout). Intenta de nuevo o usa un rango menor."
            );
          } else {
            // Abort por cleanup (cambio de variable/rango/refresh): NO es error real
            // No hacemos nada.
          }
        } else {
          setErrorHist(`Error histórico: ${err?.message || String(err)}`);
        }
      } finally {
        clearTimeout(timeoutId);
        loadingRef.current = false;
        if (!cancelled) setLoading(false);
      }
    }

    loadHistory();

    return () => {
      cancelled = true;
      clearTimeout(timeoutId);
      // Abort por cleanup (normal). NO debe mostrarse como error.
      controller.abort();
    };
  }, [metric, minutes, refreshTick]);

  return (
    <div style={{ marginTop: 26 }}>
      {/* Encabezado y controles */}
      <div
        style={{
          display: "flex",
          flexWrap: "wrap",
          alignItems: "center",
          gap: 14,
          marginBottom: 12,
        }}
      >
        <h2 style={{ margin: 0, color: "white" }}>Histórico</h2>

        <div style={{ color: "#cbd5e1", fontSize: 18 }}>
          <span style={{ marginRight: 8 }}>Variable:</span>
          <select
            value={metric}
            onChange={(e) => setMetric(e.target.value)}
            style={{
              background: "#3b3b3b",
              color: "white",
              border: "1px solid #999",
              borderRadius: 4,
              padding: "4px 8px",
              fontSize: 16,
            }}
          >
            {Object.entries(METRICS).map(([key, info]) => (
              <option key={key} value={key}>
                {info.label}
              </option>
            ))}
          </select>
        </div>

        <div style={{ color: "#cbd5e1", fontSize: 18 }}>
          <span style={{ marginRight: 8 }}>Rango:</span>
          <select
            value={range}
            onChange={(e) => setRange(e.target.value)}
            style={{
              background: "#3b3b3b",
              color: "white",
              border: "1px solid #999",
              borderRadius: 4,
              padding: "4px 8px",
              fontSize: 16,
            }}
          >
            {RANGE_OPTIONS.map((opt) => (
              <option key={opt.value} value={opt.value}>
                {opt.label}
              </option>
            ))}
          </select>
        </div>

        <div style={{ color: "#cbd5e1", fontSize: 16 }}>
          Puntos: {chartData.length}
        </div>

        {autoRefreshEnabled ? (
          <div style={{ color: "#94a3b8", fontSize: 14 }}>
            Auto-refresh: 60 s{" "}
            {typeof nextRefreshIn === "number" ? `(en ${nextRefreshIn}s)` : ""}
          </div>
        ) : (
          <div style={{ color: "#94a3b8", fontSize: 14 }}>
            Auto-refresh: OFF (usa botón)
          </div>
        )}

        <button
          onClick={refreshNow}
          style={{
            marginLeft: "auto",
            background: "#334155",
            color: "white",
            border: "1px solid #64748b",
            borderRadius: 8,
            padding: "8px 12px",
            cursor: "pointer",
            fontSize: 14,
            fontWeight: 700,
            opacity: loading ? 0.7 : 1,
          }}
          disabled={loading}
          title="Forzar actualización del histórico"
        >
          {loading ? "Cargando..." : "Actualizar"}
        </button>
      </div>

      {/* Título explícito */}
      <div
        style={{
          color: "#e2e8f0",
          marginBottom: 12,
          fontSize: 18,
          fontWeight: 600,
        }}
      >
        Serie histórica de{" "}
        <span style={{ color: "#ffffff" }}>{metricInfo.label}</span>{" "}
        <span style={{ color: "#94a3b8" }}>({metricInfo.unit})</span>
      </div>

      {/* Error (solo si es error real) */}
      {errorHist && (
        <div
          style={{
            background: "#991b1b",
            color: "white",
            padding: "14px 18px",
            borderRadius: 12,
            marginBottom: 14,
            fontSize: 18,
          }}
        >
          {errorHist}
        </div>
      )}

      {/* Gráfico */}
      <div
        style={{
          background: "#1e293b",
          borderRadius: 18,
          padding: "18px 18px 10px 18px",
          minHeight: 420,
        }}
      >
        <div
          style={{
            width: "100%",
            height: 380,
            border: "2px dashed rgba(255,255,255,0.35)",
            borderRadius: 10,
            padding: 10,
            boxSizing: "border-box",
          }}
        >
          <ResponsiveContainer width="100%" height="100%">
            <LineChart
              data={chartData}
              margin={{ top: 20, right: 20, left: 25, bottom: 35 }}
            >
              <CartesianGrid
                strokeDasharray="4 4"
                stroke="rgba(255,255,255,0.5)"
              />

              <XAxis
                dataKey="time"
                stroke="#9ca3af"
                tick={{ fill: "#9ca3af", fontSize: 14 }}
                minTickGap={20}
                label={{
                  value: "Hora",
                  position: "insideBottom",
                  offset: -15,
                  fill: "#cbd5e1",
                  fontSize: 16,
                }}
              />

              <YAxis
                stroke="#9ca3af"
                tick={{ fill: "#9ca3af", fontSize: 14 }}
                label={{
                  value: `${metricInfo.label} (${metricInfo.unit})`,
                  angle: -90,
                  position: "insideLeft",
                  fill: "#cbd5e1",
                  fontSize: 16,
                  dx: -10,
                }}
              />

              <Tooltip content={<CustomTooltip metricInfo={metricInfo} />} />

              <Line
                type="monotone"
                dataKey="value"
                stroke={metricInfo.color}
                strokeWidth={2}
                dot={false}
                activeDot={{ r: 5, stroke: "#fff", strokeWidth: 2 }}
                name={`${metricInfo.label} (${metricInfo.unit})`}
              />
            </LineChart>
          </ResponsiveContainer>
        </div>
      </div>
    </div>
  );
}