#include "leepickerscene.h"

LeePickerScene::LeePickerScene(QObject *parent)
    : QGraphicsScene{parent}
{
    QString itemName = "Demo";
    QString inImage(":/icons/color.png");
    CreateItem(itemName,inImage);
}

LeePickerItem* LeePickerScene::CreateItem(const QString inItemName, QString inImage, int itemId)
{
    LeePickerItem* item = new LeePickerItem(inItemName,inImage,itemId);
    addItem(item);
    return item;
}
