#ifndef LEEPICKERITEM_H
#define LEEPICKERITEM_H

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <Definations.h>
#include <QPointer>
#include <leeGlobal.hpp>
#include <QJsonObject>


QT_BEGIN_NAMESPACE
namespace Ui {
class ScriptEditor;
}
QT_END_NAMESPACE

class LeePickerItem : public QGraphicsObject
{
    Q_OBJECT
public:
    LeePickerItem(QString itemName = NULL, QString Image = nullptr, int objID = -1 ,QRectF inRectF = QRectF(0, 0, 80, 80));
    ~LeePickerItem(){deleteLater();}
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

    void SetDisplayName(const QString inText);

    QJsonObject toJsonObject();

protected:

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* ev) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* e) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* e) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* e) override;
    virtual void InitItemMenus(const QPoint inPosi);

    QRectF iRectF;
    QString imgfile;
    QString iName;
    int itemId;
    QColor iColor;
    QPixmap iPixmap;

    //private Funtion
    bool ImageIsValid();
    bool iIsOverlay;

    QPoint iLastScenePos= QPoint();

    bool isHover=false;

    bool isPined=false;
    QMenu* iItemMenus = Q_NULLPTR;

    QString DisplayName;

    qreal iAlpha;


private:
    Ui::ScriptEditor*SEditor;

    void ZLayerSetup();

    void UpdateDisplayName(QPainter* paint);


private slots:
    void OnDelete();

    void OnZLayerChanged(int idx);

    void OnShapeChanged();

    void OnInitScriptEditor();

    void OnAssignSelection();

    void OnDisplayChanged(QString inText);

    void OnColorChanged(QColor inColor);

    void OnAppConnectChanged(bool isChecked);
};

#endif // LEEPICKERITEM_H
