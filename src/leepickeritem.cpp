#include "leepickeritem.h"
#include <QPainter>
#include <QFile>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QMenu>
#include <QWidgetAction>
#include <QComboBox>
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


LeePickerItem::LeePickerItem(QString itemName, QString Image, int objID, QRectF inRectF)
    :imgfile(Image)
    ,iName(itemName)
    ,itemId(objID)
    ,SEditor(new Ui::ScriptEditor)
    ,iColor(Qt::blue)
    ,iRectF(inRectF)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    QImage img(imgfile);
    SetItemPixmap(img);

    setProperty("Pin",false);

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
    if (ImageIsValid())	{
        QPainter::CompositionMode mode = !iIsOverlay ? QPainter::CompositionMode_SourceOver : QPainter::CompositionMode_Overlay;
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

    UpdateDisplayName(painter);
    painter->setOpacity(iAlpha);

}

void LeePickerItem::UpdateDisplayName(QPainter *painter)
{
    painter->setPen(Qt::white);
    QFont font("Arial",10, QFont::Bold);
    painter->setFont(font);
    if(!DisplayName.isEmpty())
        painter->drawText(iRectF, DisplayName, QTextOption(Qt::AlignCenter));

}

void LeePickerItem::InitVariant()
{
    VItems.insert(0,DisplayName);
    VItems.insert(1,imgfile);
    VItems.insert(2,iColor);
    VItems.insert(3,pos().x());
    VItems.insert(4,pos().y());
    VItems.insert(5,isPined);
    VItems.insert(6,itemId);
    QByteArray scr = iScript.toUtf8();
    VItems.insert(7,scr.toHex());
    VItems.insert(8,property("select"));

    for(int i = metaObject()->propertyOffset(); i < metaObject()->propertyCount(); ++i)
    {
        qDebug() << metaObject()->property(i).name() << metaObject()->property(i).read(this) << Qt::endl;
        metaObject()->property(i);

        QString img = property(metaObject()->property(i).name()).toString();

        qDebug() << img << Qt::endl;
    }
}

QJsonObject LeePickerItem::toJsonObject()
{

    InitVariant();
    QJsonObject obj;
    int count=0;
    for(auto& iv : ItemVaribles){
        if(count >= VItems.count()) break;
        if(VItems[count].isNull() || !VItems[count].isValid()) continue;
        QString str = VItems[count].toString();
        LEEJOBJ(obj,iv,str)
        count++;
    }

    return obj;
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
    emit nameChanged(inItemName);
    return update();
}

void LeePickerItem::SetItemColor(const QString inColor)
{
    if(!inColor.isEmpty() || inColor.isNull()) return;

    iColor=QColor(inColor);
    emit iColorChanged(inColor);
    return update();
}

void LeePickerItem::SetItemId(const int newId)
{
    if(newId < 0 || newId == itemId) return ;

    itemId = newId;
    emit IdChanged(newId);
    return update();
}

void LeePickerItem::SetItemPixmap(const QPixmap inPixmap)
{
    if(inPixmap.isNull()) return;
    iPixmap=inPixmap;

    return update();
}

void LeePickerItem::SetItemPixmap(const QImage inImage)
{
    if(inImage.isNull()) return;

    iPixmap = QPixmap::fromImage(inImage);

    return update();
}

void LeePickerItem::SetDisplayName(const QString inText)
{
    if(inText.isEmpty() || inText.isNull()) return;

    DisplayName=inText;
    emit DisplayNameChanged(inText);
    return update();
}

void LeePickerItem::SetRemoteAppScript(const SoftWareApp inApp)
{
    iApp = inApp;
}

void LeePickerItem::SetMayaActive(bool isActive,bool isAdd)
{
   return OnSelectionClicked(isActive,isAdd);
}

bool LeePickerItem::IsAssigned()
{
    QVariant selectV = property("select");

    return selectV.toString().isEmpty() ? false : true;
}

void LeePickerItem::SetFlip(bool isVertical)
{
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

    if (ev->button() == Qt::LeftButton)
    {
        //this->~leePatternItem();
        QVariant selectV = property("select");
        if(!selectV.isValid() || selectV.isNull()) return QGraphicsItem::mousePressEvent(ev);

        qDebug() << "info left mouse click." << Qt::endl;
        OnSelectionClicked();
    }
    else if (ev->button() & Qt::RightButton && isHover)
    {
        //     QMenu *menu = new QMenu();
        //     QAction  *act = new QAction("abc");
        //     menu->addAction(act);

        InitItemMenus(iLastScenePos);//iItemMenus->exec(iItemMenus->mapToParent(iLastScenePos));
    }
    return QGraphicsItem::mousePressEvent(ev);
}

void LeePickerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
    isHover=true;
    iLastScenePos = e->screenPos();

    return QGraphicsItem::hoverEnterEvent(e);
}

void LeePickerItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{

    iLastScenePos = e->screenPos();

    return QGraphicsItem::hoverMoveEvent(e);
}

void LeePickerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    isHover=false;
    return QGraphicsItem::hoverLeaveEvent(e);
}

void LeePickerItem::InitItemMenus(const QPoint inPosi)
{

    iItemMenus = new QMenu();

    QString pinText = property("Pin").toBool() ? "UnPin Item" : "Pin Item";
    PinAct = new QAction(pinText);
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
    ZLayerSetup();
    iItemMenus->exec(iItemMenus->mapToParent(inPosi));

}

#pragma endregion }

bool LeePickerItem::ImageIsValid()
{
    if(iPixmap.isNull()) return false;

    QPixmap pm;
    return pm.load(imgfile);
}

SoftWareApp LeePickerItem::GetInteractApp()
{
    MainWindow* LeePicker=MainWindow::Instance();

    return LeePicker->GetInteractionApp();
}


void LeePickerItem::ZLayerSetup()
{


    QPointer<QWidgetAction> layer = new QWidgetAction(this);
    QPointer<QComboBox> layerOder = new QComboBox();


    layerOder->addItem("background",Qt::AlignHCenter);
    layerOder->addItem("on background",Qt::AlignHCenter);

    for (unsigned i = 0; i < 4; i++)
        layerOder->addItem("Ground " + QString::number(i+1),Qt::AlignHCenter);

    layer->setDefaultWidget(layerOder);
    if (iItemMenus !=nullptr)
        iItemMenus->addAction(layer);
    layerOder->setCurrentIndex(zValue());

    //Sort Alignment
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
    deleteLater();
    ///free(this);
}

void LeePickerItem::OnZLayerChanged(int idx)
{
    QComboBox* layerOder = qobject_cast<QComboBox*>(sender());

    if(layerOder == nullptr) return;


    if (layerOder != nullptr) {
        int OrderNumber = layerOder->currentIndex();
        setZValue(OrderNumber);
        if (OrderNumber == 0) {
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
        update();
    }

}

void LeePickerItem::OnShapeChanged()
{
    QString image = BrowserImage();
    if (!QFile(image).exists()) return;

    SetItemPixmap(QImage(image));
    iRectF = QImage(image).rect();
    imgfile = image;
    update();
}

void LeePickerItem::OnAssignSelection()
{
    MainWindow* LeePicker=MainWindow::Instance();

    // if(LeePicker->IsAppAvalible())
    //     LeePicker->ReInitSocket(LeePicker->GetInteractionApp());

    switch (LeePicker->GetInteractionApp()) {
        case Maya: return AssignMayaSelection();
        case Blender:{
            qDebug() << "Blender..." << Qt::endl;
            return AssignMayaSelection();}
        case NONE:
            break;
    }
}

void LeePickerItem::OnDisplayChanged(QString inText)
{
    if(inText.isNull() || inText.isEmpty()) return;

    DisplayName = inText;
    update();
}

void LeePickerItem::OnColorChanged(QColor inColor)
{
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

    MainWindow* LeePicker=MainWindow::Instance();


    const char* pfile="C:/Users/leepl/Documents/GitHub/LeePickerX/Scripts/connectPort.py";

    QFile pyfile(pfile);

    QByteArray data{};
    if(pyfile.open(QIODevice::ReadOnly)){
        data=pyfile.readAll();
    }
    //PyExecString(data.constData());

    QString Cmd = "import BlenderCommandPort\nPickerClearSelection()";
    //PythonProcessCmd(this,Maya,Cmd);
    QPointer<LeeSendCommand> process = new LeeSendCommand(Blender,Cmd);
    process->SendCommand();


    qDebug() << data << Qt::endl;
}

void LeePickerItem::OnTestMayaCmds()
{
    SoftWareApp interactApp = GetInteractApp();

    switch (interactApp) {
    case Maya: return AssignMayaSelection();
    default:
        break;
    }
}

void LeePickerItem::OnPinItem()
{
    isPined = !property("Pin").toBool();
    //Toogle Change Item Pin
    setFlag(QGraphicsItem::ItemIsMovable,!isPined);
    setProperty("Pin",isPined);
}

void LeePickerItem::OnInitScriptEditor()
{

    QWidget* widget=new QWidget();
    SEditor->setupUi(widget);

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

    widget->show();

}

void LeePickerItem::AssignMayaSelection()
{
    const char* funcName = "PortIsOpen";//"send_command";
    MainWindow* LeePicker=MainWindow::Instance();

    SoftWareApp interactApp = LeePicker->GetInteractionApp();
    const char* appUltils = interactApp == Maya ? "MayaCommandPort" : "BlenderCommandPort";

    //Check Maya Running
    if(isRunning(interactApp)){

        //check Port Commend
        bool result{};
        // auto future = QtConcurrent::run([&](){result = PyExecResultAsBool(LEESCRIPTPATH,appUltils,funcName);});
        // future.waitForFinished();

        result = interactApp == Maya ? LeePicker->IsConnectedWithMaya() : LeePicker->isConnectedWIthBlender();
        if(!result){
            LeePicker->AddToLog(Error,"Error :  Port not found exec Mel commandPort -name \"localhost:54322\" -sourceType \"python\";",true);
            return;
        }
        ///Send MayA Command
        funcName = "send_command";

        ///Init Maya Command
        const char* Cmd = interactApp == Maya ?
                                                "cmds.ls(sl=1)" :
                                                "import bpy\nselected_obj = [obj.name for obj in bpy.context.selected_objects]\nprint(selected_obj)";
        QString selections = PyExecResultString(LEESCRIPTPATH,appUltils,funcName,Cmd);

        qDebug() << selections << Qt::endl;
        //save value to assign
        if(!selections.isEmpty() && !selections.startsWith("[]"))
            SaveAssignObject(this,Maya,selections);

        //Log Result
        LeePicker->AddToLog(Log,QString("select %1").arg(selections),true);
        qDebug() << selections << Qt::endl;
    }
    else{
        //report error software not running
        QString AppC = interactApp == Maya ?
                           "Maya is not Runing" :
                           "Blender is not Runing";

        QString info = "Error :  " + AppC;
        LeePicker->AddToLog(Error,info,true);
    }

}

void LeePickerItem::OnSelectionClicked(bool isSelect, bool isAdd)
{

    if(!IsAssigned()) return ;

    SoftWareApp interactApp = GetInteractApp();


    if(!isRunning(interactApp)){
        qDebug() << "App Not Running";
        return;
    }
    ///check empty property
    auto pro = property("select").toString();
    if(pro.isEmpty() || pro.isNull()) return;


    auto Args = pro.toStdString();
    QString Cmds = QString("PickerSelect(\"%1\")").arg(pro);
    QPointer<LeeSendCommand> process = new LeeSendCommand(interactApp,Cmds);
    process->SendCommand();



    qDebug() << Cmds << Qt::endl;
}


#pragma endregion }
