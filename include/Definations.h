#ifndef DEFINATIONS_H
#define DEFINATIONS_H

#include <QDir>

#define FOLDINGSTART {
#define FOLDINGEND }

#define LEEUSER qgetenv("USERNAME")

///JSON
#define LEEJOBJ(Obj,Key,value)  Obj[Key]=value;

#define LEEMAYAENV "C:/Program Files/Autodesk/Maya2022/bin/"

#define LEEBLENDERENV "C:/Program Files/Blender Foundation/Blender 3.4/3.4/python/bin/"

#define LEESCRIPTPATH  QString("%1/Scripts/").arg(QDir::currentPath()).toUtf8()

#define MAYACMDS QString("import MayaCommandPort as LeeCmds\nLeeResultStr=LeeCmds.%1\nprint(LeeResultStr)")//BlenderCommandPort

#define BLENDERCMDS QString("import BlenderCommandPort as LeeCmds\nLeeResultStr=LeeCmds.%1\nprint(LeeResultStr)")

///Nomal CASE RETURN
#define LEECASE(X,A) case X:{ return A;}
///CASE REF COPY
#define LEECASEB(X,R,A) case X:{ R=A;break;}

#define LEEARTURL QString("https://raw.githubusercontent.com/leetdvn/UEFakeAPIData/refs/heads/main/LeeArtLisences.json")

#endif // DEFINATIONS_H
