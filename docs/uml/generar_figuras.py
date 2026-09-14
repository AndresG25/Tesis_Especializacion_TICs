"""Recompone los tres diagramas integrados. El render de la tesis usa los PDF ya exportados.

python generar_figuras.py --jar RUTA/plantuml-1.2026.8.jar
python generar_figuras.py --solo-exportar  # usa los paneles SVG existentes
"""
from pathlib import Path
import argparse, copy, subprocess, xml.etree.ElementTree as E
import cairosvg
import generar_despliegue

ROOT=Path(__file__).resolve().parent
FIG=ROOT.parent.parent/'tesis/fig/uml'
NS='http://www.w3.org/2000/svg'
E.register_namespace('',NS)

def combine(name,pairs,foot):
    diagrams=[]
    for file,title in pairs:
        r=E.parse(ROOT/'paneles'/file).getroot()
        w=float(r.get('width').removesuffix('px'));h=float(r.get('height').removesuffix('px'))
        diagrams.append((r,w,h,title))
    gap=32;pad=8;top=38;bottom=35
    w=sum(d[1] for d in diagrams)+gap+pad*2;h=max(d[2] for d in diagrams)+top+bottom
    r=E.Element(f'{{{NS}}}svg',{'width':str(w),'height':str(h),'viewBox':f'0 0 {w} {h}'})
    E.SubElement(r,f'{{{NS}}}rect',{'width':'100%','height':'100%','fill':'white'})
    xx=pad
    for sub,sw,sh,title in diagrams:
        E.SubElement(r,f'{{{NS}}}text',{'x':str(xx),'y':'25','font-family':'DejaVu Sans','font-size':'19','font-weight':'bold','fill':'#173D4D'}).text=title
        sub.set('x',str(xx));sub.set('y',str(top));sub.set('width',str(sw));sub.set('height',str(sh))
        r.append(sub);xx+=sw+gap
    E.SubElement(r,f'{{{NS}}}text',{'x':'8','y':str(h-8),'font-family':'DejaVu Sans','font-size':'16','fill':'#173D4D'}).text=foot
    E.ElementTree(r).write(FIG/(name+'.svg'),encoding='utf-8',xml_declaration=True)

def main():
    p=argparse.ArgumentParser();p.add_argument('--jar',type=Path);p.add_argument('--solo-exportar',action='store_true');a=p.parse_args()
    FIG.mkdir(parents=True,exist_ok=True)
    if not a.solo_exportar:
        if not a.jar or not a.jar.is_file():p.error('Indique un archivo JAR de PlantUML con --jar.')
        for source in ['02a_adquisicion','02b_publicacion','03a_ultimo_dato','03b_historico']:
            subprocess.run(['java','-Djava.awt.headless=true','-jar',str(a.jar.resolve()),'-charset','UTF-8','-tsvg','-o','../paneles',str(ROOT/'fuentes'/(source+'.puml'))],check=True)
    generar_despliegue.main()
    combine('02_secuencia_telemetria',[('02a_adquisicion.svg','A · Adquisición e integración'),('02b_publicacion.svg','B · Publicación hacia AWS')], 'A y B representan etapas de un mismo ciclo; las tareas locales continúan de forma independiente.')
    combine('03_secuencia_consultas',[('03a_ultimo_dato.svg','A · Último dato'),('03b_historico.svg','B · Histórico bajo demanda')], 'Las dos consultas son independientes. El panel B representa el caso exitoso de consulta histórica.')
    for name in ['01_despliegue','02_secuencia_telemetria','03_secuencia_consultas']:
        svg=FIG/(name+'.svg')
        cairosvg.svg2pdf(url=str(svg),write_to=str(FIG/(name+'.pdf')),background_color='white')
        cairosvg.svg2png(url=str(svg),write_to=str(FIG/(name+'.png')),output_width=2600,background_color='white')
    print('Figuras exportadas a',FIG)

if __name__=='__main__':main()
