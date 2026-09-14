"""Disposición horizontal del diagrama aprobado, para papel carta."""
from pathlib import Path
import generar_despliegue_propuesta as g

ROOT=Path(__file__).resolve().parent

def main():
    g.out.clear()
    g.out.append('<svg xmlns="http://www.w3.org/2000/svg" width="860" height="606" viewBox="0 0 860 606">')
    g.out.append('<defs><marker id="open" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="8" markerHeight="8" orient="auto-start-reverse"><path d="M1,1 L9,5 L1,9" fill="none" stroke="#355E71" stroke-width="1.5"/></marker></defs>')
    g.out.append('<rect width="100%" height="100%" fill="white"/><g font-family="DejaVu Sans, sans-serif">')
    g.group(12,8,612,172,'Sistema local · IoT Edge','#F0F8F3')
    for x,name,words in [(24,'ESP1 : ESP32-C6',['Adquisición','Voltaje · RPM · GPS']),
                         (228,'ESP2 : ESP32-C6',['Integración','Ambiente · RTC · SD']),
                         (432,'ESP3 : ESP32-C6',['Enlace cloud','UART · Wi-Fi · MQTT'])]:
        g.node(x,46,180,96,name,'device')
        g.artifact(x+6,98,168,40,[])
        g.text(x+90,113,words,14)
    g.line('M100,142 V158 H310 V142',arrow=True)
    g.text(206,154,'ESP-NOW',13)
    g.line('M356,142 V175 H520 V142',arrow=True)
    g.text(440,170,'UART · 9600 bit/s',13)
    g.node(660,46,184,97,'','executionEnvironment','#F0F8F3')
    g.text(752,90,'Navegador del usuario',14,'bold')
    g.artifact(666,99,172,38,[]);g.text(752,124,'Dashboard React / Vite',14)

    g.group(12,221,832,335,'AWS · Servicios administrados','#F3F6FC')
    g.node(24,266,186,89,'AWS IoT Core','executionEnvironment')
    g.artifact(30,317,174,33,[]);g.text(117,339,'Regla IoT',15)
    g.node(318,266,244,160,'AWS Lambda','executionEnvironment')
    for yy,lab in [(315,'ActualizarUltimoDatoDynamo'),(351,'GetLatest'),(387,'GetTimeseries')]:
        g.artifact(324,yy,232,31,[]);g.text(440,yy+21,lab,15)
    g.node(634,266,198,78,['DynamoDB','turbina_latest'])
    g.node(634,370,198,78,['Amazon Athena','Consulta SQL'])
    g.node(634,477,198,74,['Glue Data Catalog','Metadatos'])
    g.node(322,477,220,74,['Amazon S3','Histórico JSON'])
    g.node(24,386,186,54,'CloudFront')
    g.node(24,489,186,54,'S3 · Sitio web')

    # Caminos de comunicación. El corredor exterior evita atravesar rótulos.
    g.line('M596,142 V208 H16 V302 H24')
    g.text(203,202,'Wi-Fi + Internet · MQTT/TLS · 8883',14)
    g.line('M660,79 H638 V190 H582 V385 H562')
    g.text(590,241,'HTTPS · Function URLs',14,anchor='start')
    g.line('M808,143 V202 H852 V442 H236 V414 H210')
    g.text(790,196,'HTTPS · Sitio',13)
    g.line('M210,303 H318',dashed=True)
    g.text(264,297,'Invoca',14)
    g.line('M562,350 H732 V344',dashed=True)
    g.text(730,359,'Actualiza / consulta',12)
    g.line('M562,399 H634',dashed=True)
    g.text(598,391,'SQL',14)
    g.line('M732,448 V477',dashed=True)
    g.text(795,466,'Catálogo',12)
    g.line('M634,421 H606 V514 H542',dashed=True)
    g.text(574,506,'Lee datos',12)
    g.line('M117,355 V365 H260 V514 H322',dashed=True)
    g.text(284,506,'JSON',13)
    g.line('M117,440 V489',dashed=True)
    g.text(167,471,'Origen',12)
    g.text(20,580,'Nodo 3D: dispositivo o entorno. Pliegue: artefacto de software o configuración.',14,anchor='start')
    g.text(20,601,'Línea continua: comunicación. Flecha discontinua: dependencia o uso.',14,anchor='start')
    g.out.append('</g></svg>')
    dest=ROOT.parent.parent/'tesis/fig/uml/01_despliegue.svg'
    dest.parent.mkdir(parents=True,exist_ok=True)
    dest.write_text('\n'.join(g.out),encoding='utf-8')
    return dest

if __name__=='__main__': print(main())
