import React, { useEffect, useState } from "react";
import Historico from "./Historico";

const API_URL = import.meta.env.VITE_LATEST_URL;

function Card({ title, value, unit }) {
  return (
    <div
      style={{
        background: "#1f2937",
        color: "white",
        padding: 18,
        borderRadius: 14,
        minHeight: 90,
        display: "flex",
        flexDirection: "column",
        justifyContent: "space-between",
      }}
    >
      <div style={{ fontSize: 13, opacity: 0.85 }}>{title}</div>
      <div style={{ fontSize: 28, fontWeight: 700 }}>
        {value ?? "--"}{" "}
        <span style={{ fontSize: 14, opacity: 0.8 }}>{unit}</span>
      </div>
    </div>
  );
}

export default function App() {
  const [data, setData] = useState(null);
  const [error, setError] = useState(null);
  const [lastFetch, setLastFetch] = useState(null);

  useEffect(() => {
    let alive = true;

    const load = async () => {
      try {
        setError(null);
        const r = await fetch(API_URL, { cache: "no-store" });
        if (!r.ok) throw new Error(`HTTP ${r.status}`);
        const json = await r.json();
        if (!alive) return;
        setData(json);
        setLastFetch(new Date());
      } catch (e) {
        if (!alive) return;
        setError(String(e));
      }
    };

    load();
    const id = setInterval(load, 30000); // refresh cada 30 s
    return () => {
      alive = false;
      clearInterval(id);
    };
  }, []);

  return (
    <div style={{ background: "#0b1220", minHeight: "100vh", padding: 24 }}>
      <div style={{ color: "white", fontSize: 22, fontWeight: 700 }}>
        Sistema de Datos – Generador Eólico Axial
      </div>

      <div style={{ color: "#9ca3af", marginTop: 6 }}>
        Última actualización (dato): {data?.timestamp ?? "--"}{" "}
        {data?.esp_now_connected === false ? " (ESP-NOW: desconectado)" : ""}
      </div>

      <div style={{ color: "#9ca3af", marginTop: 6 }}>
        Refresh: 30 s | Última lectura API:{" "}
        {lastFetch ? lastFetch.toLocaleString() : "--"}
      </div>

      {error && (
        <div
          style={{
            marginTop: 16,
            padding: 12,
            background: "#7f1d1d",
            color: "white",
            borderRadius: 10,
          }}
        >
          Error: {error}
        </div>
      )}

      <div
        style={{
          marginTop: 18,
          display: "grid",
          gridTemplateColumns: "repeat(auto-fit, minmax(220px, 1fr))",
          gap: 14,
        }}
      >
        <Card title="Temperatura" value={data?.temperature} unit="°C" />
        <Card title="Humedad" value={data?.humidity} unit="%" />
        <Card title="Presión" value={data?.pressure} unit="hPa" />
        <Card title="Altitud" value={data?.altitude} unit="m" />
        <Card title="Velocidad viento" value={data?.wind_speed} unit="m/s" />
        <Card title="RPM" value={data?.rpm} unit="rpm" />
        <Card title="Voltaje" value={data?.voltage} unit="V" />
        <Card title="Memoria" value={data?.memory_usage} unit="%" />
      </div>
      <Historico />
    </div>
  );
}