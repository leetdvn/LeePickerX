#include "leepickeritem.h"
#include <QPainter>
#include <QFile>

LeePickerItem::LeePickerItem(QString itemName, QString Image, int objID, QRectF inRectF)
    :imgfile(Image)
    ,iName(itemName)
    ,itemId(objID)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    iRectF =  inRectF;
    iColor = Qt::blue;

    QImage img(imgfile);
    SetItemPixmap(img);
}

QRectF LeePickerItem::boundingRect() const
{
    return iRectF;
}

void LeePickerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Pixmap file image link

    painter->setCompositionMode(QPainter::CompositionMode_Overlay);
    painter->drawPixmap(iRectF.toRect(), iPixmap);
    //QPainter::CompositionMode mode = !status.isOverlay ? QPainter::CompositionMode_SourceOver : QPainter::CompositionMode_Overlay;
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    //return QGraphicsItem::paint(painter,option,widget);
}

#pragma region Item Functions {

void LeePickerItem::SetImagePath(const QString infile)
{
    if(infile.isEmpty() || infile.isNull()) return;

    if(!QFile(infile).exists()) {
        qDebug() << "file does not exists " <<Qt::endl;
    }

    imgfile=infile;

    return this->update();
}

void LeePickerItem::SetItemName(const QString inItemName)
{
    if(inItemName.isEmpty() || inItemName.isNull()) return;

    iName = inItemName;
    return this->update();
}

void LeePickerItem::SetItemColor(const QColor inColor)
{
    if(!inColor.isValid()) return;

    iColor=inColor;
    return this->update();
}

void LeePickerItem::SetItemId(const int newId)
{
    if(newId < 0 || newId == itemId) return ;

    itemId = newId;
    return this->update();
}

void LeePickerItem::SetItemPixmap(const QPixmap inPixmap)
{
    if(inPixmap.isNull()) return;
    iPixmap=inPixmap;

    return this->update();
}

void LeePickerItem::SetItemPixmap(const QImage inImage)
{
    if(inImage.isNull()) return;

    iPixmap = QPixmap::fromImage(inImage);
    return this->update();
}

bool LeePickerItem::ImageIsValid()
{
    if(iPixmap.isNull()) return false;

    QPixmap pm;
    return pm.load(imgfile);
}

#pragma endregion }
