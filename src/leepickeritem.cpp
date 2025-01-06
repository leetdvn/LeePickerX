#include "leepickeritem.h"
#include <QPainter>

LeePickerItem::LeePickerItem(QString itemName, QString Image, int objID)
    :imgfile(Image)
    ,iName(itemName)
    ,itemId(objID)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
}

QRectF LeePickerItem::boundingRect() const
{
    return iRectF;
}

void LeePickerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QImage image(imgfile);

    painter->setCompositionMode(QPainter::CompositionMode_Overlay);
    painter->drawPixmap(iRectF.toRect(), QPixmap::fromImage(image));
    //QPainter::CompositionMode mode = !status.isOverlay ? QPainter::CompositionMode_SourceOver : QPainter::CompositionMode_Overlay;
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    //return QGraphicsItem::paint(painter,option,widget);
}
