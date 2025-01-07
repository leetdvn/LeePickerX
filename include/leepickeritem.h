#ifndef LEEPICKERITEM_H
#define LEEPICKERITEM_H

#include <QGraphicsItem>
#include <Definations.h>

class LeePickerItem : public QGraphicsItem
{
public:
    LeePickerItem(QString itemName = NULL, QString Image = nullptr, int objID = -1 ,QRectF inRectF = QRectF(0, 0, 80, 80));

    //Virtual Pure override function
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    //Public Function
    void SetImagePath(const QString infile);

    void SetItemName(const QString inItemName);

    void SetItemColor(const QColor inColor);

    void SetItemId(const int newId);

    void SetItemPixmap(const QPixmap inPixmap);

    void SetItemPixmap(const QImage inImage);

protected:

    QRectF iRectF;
    QString imgfile;
    QString iName;
    int itemId;
    QColor iColor;
    QPixmap iPixmap;

    //private Funtion
    bool ImageIsValid();
signals:


};

#endif // LEEPICKERITEM_H
