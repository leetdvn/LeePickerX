#include "leepickerscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMoveEvent>
#include <QRect>
#include <qforeach.h>

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

QList<LeePickerItem*> LeePickerScene::GetSelectedItems()
{
    QList<LeePickerItem*> pItems = QList<LeePickerItem*>();
    QList<QGraphicsItem*> Items = selectedItems();

    for(auto it: Items)
    {
        QPointer<LeePickerItem> castIt = qgraphicsitem_cast<LeePickerItem*>(it);
        if(castIt==Q_NULLPTR) continue;
        pItems.append(castIt);
    }

    return pItems;
}
