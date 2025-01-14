#ifndef DEFINATIONS_H
#define DEFINATIONS_H

#include <QDir>

#define FOLDINGSTART {
#define FOLDINGEND }

#define LEEUSER qgetenv("USERNAME")

#define LEEJOBJ(Obj,Key,value)  Obj[Key]=value;

#define LEEPYTHONPATH "C:/Program Files/Autodesk/Maya2022/bin/"

#define LEESCRIPTPATH  QString("%1/Scripts/").arg(QDir::currentPath()).toUtf8()

#define LEEMAYA_ULTILS "MayaCommandPort"

#define LEEBLENDER_ULTILS "BlenderCommandPort"


#endif // DEFINATIONS_H
