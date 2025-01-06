#include "leepickerscene.h"

LeePickerScene::LeePickerScene(QObject *parent)
    : QGraphicsScene{parent}
{
    CreateItem("ABC");
}

void LeePickerScene::CreateItem(const QString inItemName)
{
    QString inImage(":/icons/color.png");
    LeePickerItem* item = new LeePickerItem(inItemName,inImage,0);
    addItem(item);
}
