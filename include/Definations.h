#ifndef DEFINATIONS_H
#define DEFINATIONS_H

#include <QDir>

#define FOLDINGSTART {
#define FOLDINGEND }

#define LEEUSER qgetenv("USERNAME")

#define LEEJOBJ(Obj,Key,value)  Obj[Key]=value;

#define LEEMAYA_PYTHONPATH "C:/Program Files/Autodesk/Maya2022/bin/"

#define LEEBLENDER_PYTHONPATH "C:/Program Files/Blender Foundation/Blender 3.4/3.4/python/bin"

#define LEESCRIPTPATH  QString("%1/Scripts/").arg(QDir::currentPath()).toUtf8()

#define LEEMAYA_ULTILS "MayaCommandPort"

#define LEEBLENDER_ULTILS "BlenderCommandPort"


#endif // DEFINATIONS_H
