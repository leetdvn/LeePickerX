#ifndef LEEPICKERITEM_H
#define LEEPICKERITEM_H

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <Definations.h>
#include <QPointer>
#include <leeGlobal.hpp>
#include <QJsonObject>
#include <QProcess>


QT_BEGIN_NAMESPACE
namespace Ui {
class ScriptEditor;
}
QT_END_NAMESPACE

class MainWindow;


static QStringList ItemVaribles={
    "DisplayName", //0
    "Image", //1
    "Color", //2
    "PosX", //3
    "PosY",//4
    "Pin",//5
    "ID",
    "Script",
    "Select",
};




class LeePickerItem : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString imgfile READ LeeImagePath WRITE SetImagePath NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString DisplayName READ LeeDisplayName WRITE SetDisplayName NOTIFY DisplayNameChanged FINAL)
    Q_PROPERTY(bool isPined READ LeePined WRITE SetPin NOTIFY isPinedChanged FINAL)
    Q_PROPERTY(QString iColorStr READ LeeColor WRITE SetItemColor NOTIFY iColorChanged FINAL)
    Q_PROPERTY(int itemId READ Id WRITE SetItemId NOTIFY IdChanged FINAL)
    Q_PROPERTY(bool isFlipHorizontal READ isHorizontal WRITE SetHorizontal NOTIFY HorizontalChanged FINAL)
    Q_PROPERTY(bool isFlipVertical READ isVertial WRITE SetVertical NOTIFY FlipVerticalChanged FINAL)
    Q_PROPERTY(qreal iZLayer READ ZDeepthLayer WRITE SetZLayer NOTIFY LayerZChanged FINAL)

public:
    LeePickerItem(QString itemName = NULL, QString Image = nullptr, int objID = -1 ,QRectF inRectF = QRectF(0, 0, 80, 80));
    ~LeePickerItem(){deleteLater();}
    //Virtual Pure override function
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    //Public Function
    void SetImagePath(const QString infile);

    //Read Property
    QString LeeImagePath(){return imgfile;}
    QString LeeDisplayName(){return DisplayName;}
    bool LeePined() {return isPined;}
    void SetPin(bool isPined){isPined = isPined;}
    QString LeeColor(){return iColor.name();}
    int Id(){return itemId;}

    bool isHorizontal(){return isFlipHorizontal;}
    bool isVertial() {return isFlipVertical;}
    double PosX(){return pos().x();}
    double PosY(){return pos().y();}
    qreal ZDeepthLayer(){return iZLayer;}

    //WRITE
    void SetHorizontal(bool isHz){
        isFlipHorizontal=isHz;
        emit HorizontalChanged(isHz);
    }
    void SetVertical(bool isHz){
        isFlipVertical=isHz;
        emit FlipVerticalChanged(isHz);
    }
    void SetZLayer(qreal newDeepth) {
        iZLayer=newDeepth;
        emit LayerZChanged(iZLayer);
    }

    void SetItemName(const QString inItemName);

    void SetItemColor(const QString inColor);

    void SetItemId(const int newId);

    void SetItemPixmap(const QPixmap inPixmap);

    void SetItemPixmap(const QImage inImage);

    void SetDisplayName(const QString inText);

    void SetRemoteAppScript(const SoftWareApp inApp);

    void SetMayaActive(bool isActive, bool isAdd=false);

    //check Item Has Assinged
    bool IsAssigned();

    //Set Flip
    void SetFlip(bool isVertial);

    QJsonObject toJsonObject();

signals:
    //Notify
    void nameChanged(const QString& newName);
    void DisplayNameChanged(const QString& newName) ;
    void isPinedChanged(const bool& newName) ;
    void iColorChanged(const QString& newName);//{qDebug() << "Color changed";}
    void IdChanged(const int newId);
    void HorizontalChanged(const bool newHoz);
    void FlipVerticalChanged(const bool isVertical);
    void LayerZChanged(const qreal inZLayer);

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
    QString iColorStr;
    QPixmap iPixmap;

    //private Funtion
    bool ImageIsValid();
    bool iIsOverlay;

    QPoint iLastScenePos= QPoint();

    bool isHover=false;

    bool isPined=false;
    bool isFlipHorizontal=false;
    bool isFlipVertical=false;
    QMenu* iItemMenus = Q_NULLPTR;

    QString DisplayName,iScript;

    qreal iAlpha;
    qreal iZLayer;

    SoftWareApp iApp;

    QPointer<QAction> PinAct;

    SoftWareApp GetInteractApp();

    QList<QVariant> VItems;


private:
    Ui::ScriptEditor*SEditor;

    void ZLayerSetup();

    void UpdateDisplayName(QPainter* paint);

    void AssignSelection();

    void OnSelectionClicked(bool isSelect=true,bool isAdd=false);

    void InitVariant();

    QString PyExecResultStr(const char* inCmd);

private slots:
    void OnDelete();

    void OnZLayerChanged(int idx);

    void OnShapeChanged();

    void OnInitScriptEditor();

    void OnAssignSelection();

    void OnDisplayChanged(QString inText);

    void OnColorChanged(QColor inColor);

    void OnAppConnectChanged(bool isChecked);

    void OnTestBlenderCmds();

    void OnTestMayaCmds();

    void OnPinItem();
};

#endif // LEEPICKERITEM_H
