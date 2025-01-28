#ifndef LEEPICKERITEM_H
#define LEEPICKERITEM_H

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <Definations.h>
#include <QPointer>
#include <leeGlobal.hpp>
#include <QJsonObject>
#include <QProcess>
#include <QComboBox>

///Ui File Name Space
QT_BEGIN_NAMESPACE
namespace Ui {
class ScriptEditor;
}
QT_END_NAMESPACE

class MainWindow;


#define MESSAGE(Type,msg,time) Message(Type,msg,time)


class LeePickerItem : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString imgfile READ LeeImagePath WRITE SetImagePath NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString DisplayName READ LeeDisplayName WRITE SetDisplayName NOTIFY DisplayNameChanged FINAL)
    Q_PROPERTY(QString iScript READ IScript WRITE SetIScript NOTIFY IScriptChanged FINAL)
    Q_PROPERTY(bool isPined READ LeePined WRITE SetPin NOTIFY isPinedChanged FINAL)
    Q_PROPERTY(QString iColorStr READ LeeColor WRITE SetItemColor NOTIFY iColorChanged FINAL)
    Q_PROPERTY(int itemId READ Id WRITE SetItemId NOTIFY IdChanged FINAL)
    Q_PROPERTY(bool isFlipHorizontal READ isHorizontal WRITE SetHorizontal NOTIFY HorizontalChanged FINAL)
    Q_PROPERTY(bool isFlipVertical READ isVertial WRITE SetVertical NOTIFY FlipVerticalChanged FINAL)
    Q_PROPERTY(int iZLayer READ ZDeepthLayer WRITE SetZLayer NOTIFY LayerZChanged FINAL)
    Q_PROPERTY(qreal iX READ PosX WRITE SetItemPosX NOTIFY PosXChanged FINAL)
    Q_PROPERTY(qreal iY READ PosY WRITE SetItemPosY NOTIFY PosYChanged FINAL)
    Q_PROPERTY(QString iServerApp READ DataServerApp WRITE SetDataServStr NOTIFY DataServChanged FINAL)
    Q_PROPERTY(QString iDataSelect READ DataSelect WRITE SetDataStr NOTIFY DataSelectChanged FINAL)
    Q_PROPERTY(qreal isize READ iSize WRITE SetItemSize NOTIFY SizeChanged FINAL)
    Q_PROPERTY(qreal rX READ IRectX WRITE SetRectX NOTIFY RectXChanged FINAL)
    Q_PROPERTY(qreal rY READ IRectY WRITE SetRectY NOTIFY RectYChanged FINAL)
    Q_PROPERTY(qreal rH READ IRectH WRITE SetRectH NOTIFY RectHChanged FINAL)
    Q_PROPERTY(qreal rW READ IRectW WRITE SetRectW NOTIFY RectWChanged FINAL)

public:
    LeePickerItem(QString itemName = NULL, QString Image = nullptr, int objID = -1 ,QRectF inRectF = QRectF(0, 0, 80, 80));
    ~LeePickerItem(){deleteLater();}
    //Virtual Pure override function
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    //Public Function
    void SetImagePath(const QString infile);

    void UpdateRect();
    //Read Property
#pragma region Q PROPERTY {
    QString LeeImagePath(){return imgfile;}
    QString LeeDisplayName(){return DisplayName;}
    bool LeePined() {return isPined;}
    void SetPin(bool newPin){
        isPined = newPin;
        setFlag(QGraphicsItem::ItemIsMovable, isPined ? false : true);
        emit isPinedChanged(newPin);
        update();
    }
    QString LeeColor(){return iColor.name();}
    int Id(){return itemId;}

    bool isHorizontal(){return isFlipHorizontal;}
    bool isVertial() {return isFlipVertical;}
    double PosX(){return pos().x();}
    double PosY(){return pos().y();}
    int ZDeepthLayer(){return iZLayer;}
    qreal iSize(){return isize;}
    QString DataSelect(){return iDataSelect;}
    QString DataServerApp() {return iServerApp;}

    QString IScript() {return iScript;}

    qreal IRectX(){return rX;}
    qreal IRectY(){return rY;}
    qreal IRectH(){return rH;}
    qreal IRectW(){return rW;}

    //WRITE
    void SetHorizontal(bool isHz){
        isFlipHorizontal=isHz;
        emit HorizontalChanged(isHz);
        update();
    }
    void SetVertical(bool isHz){
        isFlipVertical=isHz;
        emit FlipVerticalChanged(isHz);
        update();
    }
    void SetZLayer(int newDeepth) {
        iZLayer=newDeepth;
        setZValue(iZLayer);
        emit LayerZChanged(iZLayer);
        update();
    }

    void SetItemName(const QString inItemName);
    void SetIScript(const QString inScript){
        iScript = inScript;
        emit IScriptChanged(inScript);
    }

    void SetItemColor(const QString inColor);

    void SetItemId(const int newId);

    void SetItemPixmap(const QPixmap inPixmap);

    void SetItemPixmap(const QImage inImage);

    void SetDisplayName(const QString inText);

    void SetRemoteAppScript(const SoftWareApp inApp);

    void SetMayaActive(bool isActive, bool isAdd=false);

    void SetItemPosX(qreal newX){
        iX=newX;
        emit PosXChanged(newX);
    }
    void SetItemSize(qreal newsize){
        isize=newsize;
        emit SizeChanged(newsize);
    }
    void SetItemPosY(qreal newY){
        iY=newY;
        emit PosYChanged(newY);
    }

    void SetDataStr(const QString newStr){
        iDataSelect = newStr;
        emit DataSelectChanged(newStr);
    }

    void SetDataServStr(const QString newStr){
        iServerApp = newStr;
        emit DataServChanged(newStr);
    }
    ///set Rect
    void SetRectX(const qreal newx)
    {
        rX=newx;
        iRectF.setLeft(newx);
        emit RectXChanged(newx);
        update();
    }
    void SetRectY(const qreal newy)
    {
        rY=newy;
        iRectF.setTop(newy);
        emit RectYChanged(newy);
        update();
    }
    void SetRectH(const qreal newH)
    {
        rH=newH;
        iRectF.setHeight(newH);
        emit RectHChanged(newH);
        update();
    }
    void SetRectW(const qreal newW)
    {
        rW=newW;
        iRectF.setWidth(newW);
        emit RectXChanged(newW);
        update();
    }

    void SetEditBackGround(bool isEdit){
        ///Set Edit Bgr
        isEditBgr=isEdit;
        OnZLayerChanged(1);
        this->update();
    }


#pragma endregion Q PROPERTIES }
    ///check Item Has Assinged
    bool IsAssigned();

    //Set Flip
    void SetFlip(bool isVertial);

    QJsonObject toJsonObject();

    void LoadDataFromJsObject(const QJsonObject inObject);
signals:
    //Notify
    void nameChanged(const QString& newName);
    void DisplayNameChanged(const QString& newName) ;
    void isPinedChanged(const bool& newName) ;
    void iColorChanged(const QString& newName);//{qDebug() << "Color changed";}
    void IScriptChanged(const QString& newScript);
    void IdChanged(const int newId);
    void HorizontalChanged(const bool newHoz);
    void FlipVerticalChanged(const bool isVertical);
    void LayerZChanged(const int inZLayer);

    void PosXChanged(const qreal newX);
    void PosYChanged(const qreal newY);
    void DataSelectChanged(const QString newStr);
    void DataServChanged(const QString newServ);
    void SizeChanged(const qreal newsize);
    //Rect Signals
    void RectXChanged(const qreal newX);
    void RectYChanged(const qreal newY);
    void RectHChanged(const qreal newH);
    void RectWChanged(const qreal newW);
protected:

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* ev) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* e) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* e) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* e) override;
    virtual void InitItemMenus(const QPoint inPosi);

    void SetUpMenuPolicy(const QPoint inPos);

    QRectF iRectF;
    QString imgfile;
    QString iName;
    int itemId;
    QColor iColor;
    QString iColorStr;
    QString iDataSelect;
    QString iServerApp;
    QPixmap iPixmap;

    ///Item Message
    void Message(LogType inType,const QString msg,int timeDelete);
    //private Funtion
    bool ImageIsValid();
    bool iIsOverlay;

    QPoint iLastScenePos= QPoint();

    bool isHover=false;

    bool isPined=false;
    bool isFlipHorizontal=false;
    bool isFlipVertical=false;
    bool isEditBgr=false;
    QMenu* iItemMenus = Q_NULLPTR;

    QString DisplayName,iScript;

    qreal isize;
    qreal rX,rY,rH,rW;
    qreal iAlpha;
    int iZLayer;
    qreal iX,iY;
    SoftWareApp iApp;

    QPointer<QAction> PinAct;

    SoftWareApp GetInteractApp();

    QPointer<QComboBox> layerOder;


private:
    Ui::ScriptEditor*SEditor;

    void ZLayerSetup();

    void UpdateDisplayName(QPainter* paint);

    void AssignSelection();

    void OnSelectionClicked(bool isSelect=true,bool isAdd=false);

    void InitVariant();

    QString PyExecResultStr(const char* inCmd);

    void PyExec(const char* inCmd);

    int testCount=0;
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

    ///Script Changed
    void OnScriptChanged();
};

#endif // LEEPICKERITEM_H
