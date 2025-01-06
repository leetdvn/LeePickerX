#ifndef LEEPICKERITEM_H
#define LEEPICKERITEM_H

#include <QGraphicsItem>

class LeePickerItem : public QGraphicsItem
{
public:
    LeePickerItem(QString itemName = NULL, QString Image = nullptr, int objID = -1);

    //get rectF object x,y ,width , height
    virtual QRectF RectF() { return iRectF; }
    //get rect return other rect x,y,width,height
    virtual QRect Rect() { return iRectF.toRect(); }

    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QRectF iRectF= QRectF(0, 0, 80, 80);
    QString imgfile;
    QString iName;
    int itemId;
signals:
};

#endif // LEEPICKERITEM_H
