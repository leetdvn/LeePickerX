#include "leepickeritem.h"
#include <QPainter>
#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QMenu>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPixmap>
#include <mainwindow.h>
#include "../ui/ui_ScriptEditor.h"
#include "Definations.h"
#include <QGraphicsWidget>
#include <LeePickerPython.h>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QStyledItemDelegate>
#include <qobject.h>


LeePickerItem::LeePickerItem(QString itemName, QString Image, int objID, QRectF inRectF)
    :imgfile(Image)
    ,iName(itemName)
    ,itemId(objID)
    ,SEditor(new Ui::ScriptEditor)
    ,iColor(Qt::blue)
    ,iRectF(inRectF)
    ,iZLayer(1)
    ,isize(25)
{

    //Initialization
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    QImage img(imgfile);
    SetItemPixmap(img);

    setProperty("Pin",false);
    //Default Bgr Layer 1
    setZValue(iZLayer);

    rX = inRectF.x();
    rY = inRectF.y();
    rH = inRectF.height();
    rW = inRectF.width();
}

QRectF LeePickerItem::boundingRect() const
{
    if (imgfile.isNull()) {
        qreal wMin = 80;
        QFontMetrics fm(QFont(DisplayName, isize));
        double ratio{};
        int width = fm.horizontalAdvance(DisplayName);
        ratio = width < 80 ? 80 : width * 1.25;
        return QRectF(iRectF.x(),iRectF.y(), ratio, iRectF.height());
    }

    ///Reimplemented Protected
    return iRectF;
;
}

void LeePickerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    ///Reimplemented Protected Paint Function
    if (ImageIsValid())	{
        QPainter::CompositionMode mode = !iIsOverlay ?
                                             QPainter::CompositionMode_SourceOver :
                                             QPainter::CompositionMode_Overlay;
        painter->drawPixmap(iRectF.toRect(), iPixmap);
        painter->setCompositionMode(mode);
    }
    else {
        painter->fillRect(boundingRect(), iColor);
    }

    if (isSelected())
    {
        QPen boder(Qt::green);
        boder.setWidth(5);
        painter->setPen(boder);
        painter->drawRect(boundingRect());
    }

    if(isFlipHorizontal || isFlipVertical)
        painter->setTransform(transform().inverted(),true);

    ///Size
    UpdateDisplayName(painter);
    painter->setOpacity(iAlpha);

}

void LeePickerItem::UpdateDisplayName(QPainter *painter)
{
    ///Update DisplayName
    painter->setPen(Qt::white);
    QFont font("Arial",isize, QFont::Bold);
    painter->setFont(font);
    painter->drawText(boundingRect(), DisplayName, QTextOption(Qt::AlignCenter));

}

void LeePickerItem::InitVariant()
{
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i)
    {
        qDebug() << metaObject()->property(i).name() << metaObject()->property(i).read(this) << Qt::endl;

        QString img = property(metaObject()->property(i).name()).toString();

        qDebug() << img << Qt::endl;
    }
}

QString LeePickerItem::PyExecResultStr(const char* inCmd)
{
    ///Call Python Function
    MainWindow* LeePicker=MainWindow::Instance();

    SoftWareApp interactApp = LeePicker->GetInteractionApp();
    const char* ModuleFile = interactApp == Maya ? "MayaCommandPort" : "BlenderCommandPort";
    const char* funcName = "send_command";

    QString selections ;
    auto future = QtConcurrent::run([&](){selections = PyExecResultString(LEESCRIPTPATH,ModuleFile,funcName,inCmd);});
    future.waitForFinished();

    return selections;
}

void LeePickerItem::PyExec(const char *inCmd)
{
    ///Call Python Void Function
    MainWindow* LeePicker=MainWindow::Instance();

    SoftWareApp interactApp = LeePicker->GetInteractionApp();
    ///Module File
    const char* ModuleFile = interactApp == Maya ? "MayaCommandPort" : "BlenderCommandPort";
    const char* funcName = "send_command";

    PyExecFuncAsVoid(funcName,interactApp,inCmd);

}

QJsonObject LeePickerItem::toJsonObject()
{

    ///Convert Data Iem To Json Data
    InitVariant();
    QJsonObject obj;
    int count=0;
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i)
    {
        const QString iKey = metaObject()->property(i).name();
        QVariant iv = metaObject()->property(i).read(this);
        qDebug() << "Name : " << metaObject()->property(i).name() << "Property : " <<  metaObject()->property(i).read(this) << Qt::endl;
        LEEJOBJ(obj,iKey,iv.toJsonValue());

    }

    return obj;
}

void LeePickerItem::LoadDataFromJsObject(const QJsonObject inObject)
{
    ///Null Obj
    if(inObject.isEmpty()) return;

    ///Load Metadata Properties
    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i)
    {
        const QString iKey = metaObject()->property(i).name();
        QVariant iv = inObject[iKey].toVariant();
        metaObject()->property(i).write(this,iv);

    }

    ///Update Item Posision
    QGraphicsItem* item = qobject_cast<QGraphicsItem*>(this);
    if(item !=nullptr){
        item->setPos(QPoint(iX,iY));
        item->update();
    }
    this->update();
}

#pragma region Item Sets Functions {

void LeePickerItem::SetImagePath(const QString infile)
{
    ///Set Image Path Func
    if(infile.isEmpty() || infile.isNull()) return;

    if(!QFile(infile).exists()) {
        qDebug() << "file does not exists " <<Qt::endl;
    }

    imgfile=infile;

    return this->update();
}

void LeePickerItem::UpdateRect()
{
    QImage img(imgfile);
    SetItemPixmap(img);
    SetRectX(img.rect().left());
    SetRectY(img.rect().top());
    SetRectH(img.rect().height());
    SetRectW(img.rect().width());
    update();
}

void LeePickerItem::SetItemName(const QString inItemName)
{
    ///Set Item Name Func
    if(inItemName.isEmpty() || inItemName.isNull()) return;

    iName = inItemName;
    emit nameChanged(inItemName);
    return update();
}

void LeePickerItem::SetItemColor(const QString inColor)
{
    ///Set item Color Func
    if(!inColor.isEmpty() || inColor.isNull()) return;

    iColor=QColor(inColor);
    emit iColorChanged(inColor);
    return update();
}

void LeePickerItem::SetItemId(const int newId)
{
    ///Set Item Id
    if(newId < 0 || newId == itemId) return ;

    itemId = newId;
    emit IdChanged(newId);
    return update();
}

void LeePickerItem::SetItemPixmap(const QPixmap inPixmap)
{
    ///Set Image Pixmap
    if(inPixmap.isNull()) return;
    iPixmap=inPixmap;

    return update();
}

void LeePickerItem::SetItemPixmap(const QImage inImage)
{
    ///Set Item Pixmap Overload2
    if(inImage.isNull()) return;

    iPixmap = QPixmap::fromImage(inImage);

    return update();
}

void LeePickerItem::SetDisplayName(const QString inText)
{
    ///Set DisplayName
    if(inText.isEmpty() || inText.isNull()) return;

    DisplayName=inText;
    emit DisplayNameChanged(inText);
    return update();
}

void LeePickerItem::SetRemoteAppScript(const SoftWareApp inApp)
{
    ///Set Remote Application Changed
    iApp = inApp;
}

void LeePickerItem::SetMayaActive(bool isActive,bool isAdd)
{
    ///Set Mayaa Active Change
   return OnSelectionClicked(isActive,isAdd);
}

bool LeePickerItem::IsAssigned()
{
    ///Is Assign Connect App == Check Properties

    return iDataSelect.isEmpty() || iDataSelect.isNull() ? false : true;
}

void LeePickerItem::SetFlip(bool isVertical)
{
    ///Set Flip Image by Matrix 4x4
    QTransform current(this->transform());

    qreal m11 = current.m11();    // Horizontal scaling
    qreal m12 = current.m12();    // Vertical shearing
    qreal m13 = current.m13();    // Horizontal Projection
    qreal m21 = current.m21();    // Horizontal shearing
    qreal m22 = current.m22();    // vertical scaling
    qreal m23 = current.m23();    // Vertical Projection
    qreal m31 = current.m31();    // Horizontal Position (DX)
    qreal m32 = current.m32();    // Vertical Position (DY)
    qreal m33 = current.m33();    // Addtional Projection Factor

    // We need this in a minute
    qreal Hscale = m11;
    qreal Vscale = m22;

    //texTransform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
    if (!isVertical) {
        // Horizontal flip
        m11 = -m11;
        if (m31 > 0)
            m31 = 0;
        else
            m31 = (boundingRect().width() * Hscale);
        isFlipHorizontal = !isFlipHorizontal;
    }
    else {
        // Vertical flip
        m22 = -m22;
        if (m32 > 0)
            m32 = 0;
        else
            m32 = (boundingRect().height() * Vscale);
        isFlipVertical = !isFlipVertical;
    }
    current.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
    setTransform(QTransform(current));
    update();

}

#pragma region Mouse HoverEvent {

void LeePickerItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{

    ///Mouse Press Event
    if (ev->button() == Qt::LeftButton)
    {
        //this->~leePatternItem();

        OnSelectionClicked();
    }
    else if (ev->button() & Qt::RightButton && isHover)
    {
        //     QMenu *menu = new QMenu();
        //     QAction  *act = new QAction("abc");
        //     menu->addAction(act);

        SetUpMenuPolicy(iLastScenePos);//iItemMenus->exec(iItemMenus->mapToParent(iLastScenePos));
    }
    return QGraphicsItem::mousePressEvent(ev);
}

void LeePickerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    ///Mouse Hover Enter Event
    isHover=true;
    iLastScenePos = e->screenPos();

    return QGraphicsItem::hoverEnterEvent(e);
}

void LeePickerItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{

    ///Hover Move Event
    iLastScenePos = e->screenPos();

    return QGraphicsItem::hoverMoveEvent(e);
}

void LeePickerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    ///Hover Leave Event
    isHover=false;
    return QGraphicsItem::hoverLeaveEvent(e);
}

void LeePickerItem::InitItemMenus(const QPoint inPosi)
{

    ///Init Menu Item Righclick
    iItemMenus = new QMenu();

    QString pinText = property("Pin").toBool() ? "UnPin Item" : "Pin Item";
    PinAct = new QAction(pinText);

    if(iZLayer > 0){
        //_menu->setAttribute(Qt::WA_DeleteOnClose,true);
        //QMenu* edit = _menu->addMenu("edit button");
        iItemMenus->addAction("assign selection", [&]() { OnAssignSelection();});

        //_menu->addSeparator();
        //_menu->addAction("test Action", [&]() {on_testCommand(); });
        iItemMenus->addAction("scripts editor", [&]() { OnInitScriptEditor(); });

        iItemMenus->addAction(pinText, [&]() { OnPinItem(); });

        iItemMenus->addAction("test Maya Cmds", [&]() { OnTestMayaCmds(); });

        iItemMenus->addAction("test Blender Cmds", [&]() { OnTestBlenderCmds(); });

        //PinActionSetUp();
        QString  menuStyle(
            "QMenu::item{"
            "background-color: rgb(0, 170, 0);"
            "color: rgb(255, 255, 255);"
            "}"
            "QMenu::item:selected{"
            "background-color: rgb(0, 127, 85);"
            "color: rgb(255, 255, 255);"
            "}"
            );
        iItemMenus->setStyleSheet(menuStyle);
        iItemMenus->addAction("change shape", [this]() {OnShapeChanged();});
        iItemMenus->addSeparator();
        iItemMenus->addAction("delete item", [&]() {OnDelete(); });
    }
    ZLayerSetup();
    iItemMenus->exec(iItemMenus->mapToParent(inPosi));

}

void LeePickerItem::SetUpMenuPolicy(const QPoint inPos)
{
    MainWindow* LeePicker=MainWindow::Instance();

    if(!LeePicker->CanEditBackGround() && iZLayer == 0) return;

    InitItemMenus(inPos);
}

void LeePickerItem::Message(LogType inType,const QString msg,int timeDelete)
{
    MainWindow* LeePicker=MainWindow::Instance();

    return LeePicker->AddToLog(inType,msg,true,timeDelete);
}

#pragma endregion }

bool LeePickerItem::ImageIsValid()
{
    ///Check Valid Image
    if(imgfile.isEmpty()||imgfile.isNull() || !QFile(imgfile).exists()) return false;

    QImage img(imgfile);
    iPixmap = QPixmap::fromImage(img);

    return !iPixmap.isNull();
}

SoftWareApp LeePickerItem::GetInteractApp()
{
    ///Get Interact Application Connect
    MainWindow* LeePicker=MainWindow::Instance();

    return LeePicker->GetInteractionApp();
}


void LeePickerItem::ZLayerSetup()
{

    ///SetUp Menu Changed ZLayer

    QPointer<QWidgetAction> layer = new QWidgetAction(this);
    layerOder = new QComboBox();
    layerOder->setContextMenuPolicy(Qt::NoContextMenu);
    layerOder->addItem("background",Qt::AlignHCenter);
    layerOder->addItem("on background",Qt::AlignHCenter);

    for (unsigned i = 0; i < 4; i++)
        layerOder->addItem("Ground " + QString::number(i+1),Qt::AlignHCenter);

    layer->setDefaultWidget(layerOder);
    if (iItemMenus !=nullptr)
        iItemMenus->addAction(layer);
    layerOder->setCurrentIndex(zValue());

    ///Sort Alignment
    layerOder->setEditable(true);
    layerOder->lineEdit()->setReadOnly(true);
    layerOder->lineEdit()->setAlignment(Qt::AlignCenter);
    for (int i = 0 ; i < layerOder->count() ; ++i) {
        layerOder->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    connect(layerOder,SIGNAL(currentIndexChanged(int)),this,SLOT(OnZLayerChanged(int)));
}

void LeePickerItem::OnDelete()
{
    if(!iDataSelect.isEmpty() ||
        !iDataSelect.isNull() ||
        !iScript.isEmpty() ||
        !iScript.isNull())
    {
        if(ShowDialog("Do You Wanna Delele this Button")){
            deleteLater();
        }
        else{
            return;
        }

    }
    ///Destructor
    deleteLater();
    ///free(this);
}

void LeePickerItem::OnZLayerChanged(int idx)
{
    ///OnZLayer Changed
    if(layerOder == nullptr) return;

    iZLayer = idx;
    setZValue(iZLayer);

    if (iZLayer == 0) {
        // if (MaskColor != nullptr)
        //     MaskColor = nullptr;
        // status.isMoving = false;
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
    }
    else {
        // status.isMoving = true;
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
    qDebug() << "ZLayer Changed" << iZLayer << Qt::endl;
    update();


}

void LeePickerItem::OnShapeChanged()
{
    ///OnShaped Changed
    QString image = BrowserImage();
    if (!QFile(image).exists()) return;

    QImage img(image);
    SetItemPixmap(img);
    SetRectX(img.rect().left());
    SetRectY(img.rect().top());
    SetRectH(img.rect().height());
    SetRectW(img.rect().width());
    imgfile = image;
    update();
}

void LeePickerItem::OnAssignSelection()
{
    ///On Assign Selection
    MainWindow* LeePicker=MainWindow::Instance();

    // if(LeePicker->IsAppAvalible())
    //     LeePicker->ReInitSocket(LeePicker->GetInteractionApp());

    switch (LeePicker->GetInteractionApp()) {
        case Maya: return AssignSelection();
        case Blender:{
            qDebug() << "Blender..." << Qt::endl;
            return AssignSelection();}
        case NONE:
            break;
    }
}

void LeePickerItem::OnDisplayChanged(QString inText)
{
    //On Display Name Changed
    if(inText.isNull() || inText.isEmpty()) return;

    DisplayName = inText;
    update();
}

void LeePickerItem::OnColorChanged(QColor inColor)
{
    ///OnColor Changed
    if (!inColor.isValid() && !isSelected()) return;

    iColor=inColor;
    return update();
}

void LeePickerItem::OnAppConnectChanged(bool isChecked)
{
    //Do Something
    //QRadioButton* radio = qobject_cast<QRadioButton*>(sender());

}

void LeePickerItem::OnTestBlenderCmds()
{

    ///Test Command Blender
    //MainWindow* LeePicker=MainWindow::Instance();


    const char* pfile="C:/Users/leepl/Documents/GitHub/LeePickerX/Scripts/connectPort.py";

    QFile pyfile(pfile);

    QByteArray data{};
    if(pyfile.open(QIODevice::ReadOnly)){
        data=pyfile.readAll();
    }
    //PyExecString(data.constData());

    // QString Cmd = "import BlenderCommandPort\nPickerClearSelection()";
    // //PythonProcessCmd(this,Maya,Cmd);
    // QPointer<LeeSendCommand> process = new LeeSendCommand(this,Blender,Cmd);
    // process->SendCommand();


    qDebug() << data << Qt::endl;
}

void LeePickerItem::OnTestMayaCmds()
{
    ///Test Command Maya
    if(ShowDialog("taoday"))
    {
        ///Test Log Define
        MESSAGE(Log,"taoday Comfirm",3);
    }
    else{
        MESSAGE(Log,"taoday Canel",3);

    }
}

void LeePickerItem::OnPinItem()
{

    isPined = !property("Pin").toBool();
    //Toogle Change Item Pin
    setFlag(QGraphicsItem::ItemIsMovable,!isPined);
    setProperty("Pin",isPined);
    MESSAGE(Log,isPined ? QString("Pined"):QString("UnPined"),3);
}

void LeePickerItem::OnScriptChanged()
{
    QPointer<QTextEdit> textEdit = qobject_cast<QTextEdit*>(sender());
    QString textChanged = textEdit->toPlainText();

    if(textChanged.isNull() || textChanged.isEmpty()) return;

    SetIScript(textChanged);
    update();

    qDebug () << "Text : " << textChanged << Qt::endl;
}

void LeePickerItem::OnInitScriptEditor()
{

    ///Show Script Editor UI
    QWidget* widget=new QWidget();
    SEditor->setupUi(widget);

    if(!DisplayName.isEmpty())
        SEditor->lineEdit->setText(DisplayName);
    if(!iScript.isEmpty()){
        SEditor->ScriptEdit->setText(iScript);
    }

    connect(SEditor->lineEdit,SIGNAL(textChanged(QString)),SLOT(OnDisplayChanged(QString)));

    MainWindow* LeePicker=MainWindow::Instance();

    SoftWareApp interactApp = LeePicker->GetInteractionApp();

    bool AppC = interactApp == Maya ? false : true;

    AppC == false ? SEditor->MayaRadio->setChecked(!AppC) :
        SEditor->BlenderRadio->setChecked(AppC) ;

    switch (interactApp) {
        case Maya:{
            SEditor->MayaRadio->setChecked(true);
            break;
        }
        case Blender:{
            SEditor->BlenderRadio->setChecked(true);
            break;
        }
        case NONE:
            return;
    }

    connect(SEditor->BlenderRadio,SIGNAL(clicked(bool)),this,SLOT(OnAppConnectChanged(bool)));
    connect(SEditor->MayaRadio,SIGNAL(clicked(bool)),this,SLOT(OnAppConnectChanged(bool)));
    connect(SEditor->ScriptEdit,SIGNAL(textChanged()),this,SLOT(OnScriptChanged()));
    widget->show();

}

void LeePickerItem::AssignSelection()
{
    ///Assign Selection
    MainWindow* LeePicker=MainWindow::Instance();

    SoftWareApp interactApp = LeePicker->GetInteractionApp();
    const char* appUltils = interactApp == Maya ? "MayaCommandPort" : "BlenderCommandPort";

    QString msgApp = interactApp == Maya ?
                       "Maya is not Runing" :
                       "Blender is not Runing";

    QString msgPort = interactApp == Maya ?
                         "Port not found exec Mel commandPort -name \"localhost:54322\" -sourceType \"python\";" :
                         "Command Port Addon Not Found..";

    //Check Maya Running
    if(isRunning(interactApp)){
        //check Port Commend
        bool result{};

        result = LeePicker->IsAppAvalible();
        if(!result){
            LeePicker->AddToLog(Error,msgApp,true,2);
            return;
        }

        ///Init Maya Command
        const char* Cmd = interactApp == Maya ?
                                        "cmds.ls(sl=1)" :
                                        "import bpy\nselected_obj = [obj.name for obj in bpy.context.selected_objects]\nprint(selected_obj)";

        QString selections  = PyExecResultStr(Cmd);

        if(selections.isEmpty() || selections.isNull()){
            LeePicker->AddToLog(Error,msgPort,true,2);
            return;
        }
        //save value to assign
        if(!selections.isEmpty() && !selections.startsWith("[]")){
            //SaveAssignObject(this,interactApp,selections);
            SetDataStr(selections);
            SetDataServStr(interactApp == Maya ? "Maya" : "Blender");
            SetDisplayName(DisplayName);
        }

        //Log Result
        LeePicker->AddToLog(Log,QString("select %1").arg(selections),true);
        qDebug() << selections << Qt::endl;
    }
    else{
        //report error software not running
        QString info = "Error :  " + msgApp;
        LeePicker->AddToLog(Error,info,false);
    }

}

void LeePickerItem::OnSelectionClicked(bool isSelect, bool isAdd)
{
    if(!IsAssigned() && iScript.isEmpty()) return ;

    SoftWareApp interactApp = GetInteractApp();

    if(!isRunning(interactApp)){
        qDebug() << "App Not Running";
        return;
    }
    ///check empty property


    auto Args = iDataSelect.toStdString();
    const char* Cmd = !isAdd ?
                          isSelect ?
                          "PickerSelect" :
                          "PickerDeSelect" :
                          "PickerAddSelect";

    MainWindow* LeePicker=MainWindow::Instance();

    if(IsAssigned()){
        PyExecFuncAsVoid(Cmd,interactApp,iDataSelect.toUtf8());
        LeePicker->AddToLog(Log,QString("Select %1").arg(iDataSelect),true);
    }

    if(!iScript.isEmpty())
    {
        const char* ModuleFile = interactApp == Maya ? "MayaCommandPort" : "BlenderCommandPort";
        const char* funcName = "send_command";

        const char* Cmd = iScript.toLocal8Bit();
        QString result = PyExecResultString(LEESCRIPTPATH,ModuleFile,funcName,Cmd);
        //PyExec(Cmd);
    }
    qDebug() << "Click " <<  Cmd << testCount <<  Qt::endl;
}


#pragma endregion }
