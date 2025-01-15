#ifndef DEFINATIONS_H
#define DEFINATIONS_H

#include <QDir>

#define FOLDINGSTART {
#define FOLDINGEND }

#define LEEUSER qgetenv("USERNAME")

#define LEEJOBJ(Obj,Key,value)  Obj[Key]=value;

#define LEEMAYAENV "C:/Program Files/Autodesk/Maya2022/bin/"

#define LEEBLENDERENV "C:/Program Files/Blender Foundation/Blender 3.4/3.4/python/bin/"

#define LEESCRIPTPATH  QString("%1/Scripts/").arg(QDir::currentPath()).toUtf8()

#define MAYACMDS QString("import MayaCommandPort as LeeCmds\nLeeCmds.%1")//BlenderCommandPort

#define BLENDERCMDS QString("import BlenderCommandPort as LeeCmds\nLeeCmds.%1")

#define LEECASE(X,A) case X:{ return A;}
#define LEECASEB(X,R,A) case X:{ R=A;break;}

#endif // DEFINATIONS_H
