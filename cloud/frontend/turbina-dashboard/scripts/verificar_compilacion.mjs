// Comprueba una compilacion local contra la copia recuperada de S3.
// No compila, no despliega y no se conecta a AWS.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import { fileURLToPath } from 'node:url';

const proyecto = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const raizRepo = path.resolve(proyecto, '../../..');
const args = process.argv.slice(2);
if (args.length && !(args.length === 2 && args[0] === '--directorio')) {
  console.error('Uso: node scripts/verificar_compilacion.mjs [--directorio RUTA]');
  process.exit(2);
}
const dist = path.resolve(proyecto, args.length ? args[1] : 'dist');
const publicado = path.resolve(proyecto, '../publicado');
const referencia = JSON.parse(fs.readFileSync(path.join(proyecto, 'referencia_publicada.json'), 'utf8'));
const sha = p => crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex');
function seguro(base, relative) {
  if (typeof relative !== 'string' || path.isAbsolute(relative) || relative.split(/[\\/]/).includes('..')) {
    throw new Error('Ruta no valida en la referencia');
  }
  return path.join(base, relative);
}
function archivos(base) {
  if (!fs.existsSync(base)) return [];
  const result = [];
  function walk(dir, prefix = '') {
    for (const entry of fs.readdirSync(dir, {withFileTypes: true})) {
      if (entry.isSymbolicLink()) throw new Error('Enlace no admitido: ' + entry.name);
      const relative = prefix + entry.name;
      if (entry.isDirectory()) walk(path.join(dir, entry.name), relative + '/');
      else if (entry.isFile()) result.push(relative);
    }
  }
  walk(base);
  return result.sort();
}
const filas = referencia.archivos_publicados.map(r => {
  const a = seguro(dist, r.archivo);
  const b = seguro(publicado, r.archivo);
  const obtenido = fs.existsSync(a) ? sha(a) : null;
  const respaldo = fs.existsSync(b) ? sha(b) : null;
  return { archivo: r.archivo, sha256_esperado: r.sha256, sha256_dist: obtenido,
    coincide: obtenido === r.sha256, referencia_s3_intacta: respaldo === r.sha256 };
});
const nombres = new Set(filas.map(r => r.archivo));
const extras = archivos(dist).filter(p => !nombres.has(p));
const fuentes = referencia.fuentes_originales.map(r => {
  const f = seguro(proyecto, r.archivo);
  return { archivo: r.archivo, coincide: fs.existsSync(f) && sha(f) === r.sha256 };
});
const referenciaIntacta = filas.every(r => r.referencia_s3_intacta);
const identidad = filas.every(r => r.coincide) && extras.length === 0;
const fecha = new Date().toISOString();
const dirInforme = path.join(raizRepo, 'trazabilidad/verificaciones_frontend_local');
fs.mkdirSync(dirInforme, {recursive: true});
const destino = path.join(dirInforme, 'comparacion_' + fecha.replace(/[^0-9TZ]/g, '') + '_' + process.pid + '.json');
const informe = {
  fecha_utc: fecha, node: process.version, plataforma: process.platform,
  directorio_evaluado: dist,
  metodo: 'Comparacion SHA-256 de archivos existentes; el script no ejecuta el compilador.',
  estado: !fs.existsSync(dist) ? 'DIST_NO_EXISTE' : !referenciaIntacta ? 'REFERENCIA_ALTERADA_O_AUSENTE' : identidad ? 'ARCHIVOS_IDENTICOS_AL_RESPALDO_S3' : 'ARCHIVOS_DIFERENTES_AL_RESPALDO_S3',
  archivos_publicados: filas, archivos_adicionales: extras, fuentes_originales: fuentes,
  fuentes_sin_modificacion: fuentes.every(r => r.coincide),
  llamadas_a_aws: 0,
};
fs.writeFileSync(destino, JSON.stringify(informe, null, 2) + '\n');
console.log(informe.estado);
console.log('Archivos coincidentes:', filas.filter(r => r.coincide).length + '/' + filas.length);
console.log('Archivos adicionales:', extras.length);
console.log('Fuentes originales sin modificar:', informe.fuentes_sin_modificacion);
console.log('Informe:', destino);
if (!identidad || !referenciaIntacta || !informe.fuentes_sin_modificacion) process.exitCode = 1;
