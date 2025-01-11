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

QJsonObject LeePickerItem::toJsonObject()
{
    QJsonObject obj;
    LEEJOBJ(obj,"abc",isPined)
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

void LeePickerItem::SetDisplayName(const QString inText)
{
    if(inText.isEmpty() || inText.isNull()) return;

    DisplayName=inText;
    return this->update();
}

void LeePickerItem::SetRemoteAppScript(const SoftWareApp inApp)
{
    iSoftWareApp = inApp;
}

void LeePickerItem::SetMayaActive(bool isActive,bool isAdd)
{
   return OnSelectionClicked(isActive,isAdd);
}

#pragma region Mouse HoverEvent {

void LeePickerItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{

    if (ev->button() == Qt::LeftButton)
    {
        //this->~leePatternItem();
        QString select = property("select").toString();
        if(select.isEmpty()) return QGraphicsItem::mousePressEvent(ev);

        OnSelectionClicked();
        qDebug() << select << Qt::endl;
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
    int max = scale() * 100;
    if (max > 1000)
        max = 1000;
    iItemMenus = new QMenu();
    //_menu->setAttribute(Qt::WA_DeleteOnClose,true);
    //QMenu* edit = _menu->addMenu("edit button");
    iItemMenus->addAction("assign selection", [&]() { OnAssignSelection();});

    //_menu->addSeparator();
    //_menu->addAction("test Action", [&]() {on_testCommand(); });
    iItemMenus->addAction("scripts editor", [&]() { OnInitScriptEditor(); });

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

    switch (LeePicker->RemoteApp) {
        case Maya: return AssignMayaSelection();
        case Blender:{return;}
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
    const char* pfile="C:/Users/leepl/Documents/GitHub/LeePickerX/Scripts/connectPort.py";

    QFile pyfile(pfile);

    QByteArray data{};
    if(pyfile.open(QIODevice::ReadOnly)){
        data=pyfile.readAll();
    }
    //PyExecString(data.constData());

    QProcess* process = new QProcess(this);

    process->setWorkingDirectory(LEEPYTHONPATH);

    // tasklist.start(
    //     "tasklist",
    //     QStringList() << "/NH"
    //                   << "/FO" << "CSV"
    //                   << "/FI" << QString("IMAGENAME eq %1").arg(process));

    process->start(
        "mayapy.exe",
        QStringList() << "-i"
                      << "-c" << QString("%1").arg(data)
        );

    process->waitForFinished();

    qDebug() << data << Qt::endl;
}

void LeePickerItem::OnTestMayaCmds()
{
    MainWindow* LeePicker=MainWindow::Instance();

    bool AppC = LeePicker->RemoteApp == Maya ? false : true;

    switch (LeePicker->RemoteApp) {
    case Maya: return AssignMayaSelection();
    default:
        break;
    }
}

void LeePickerItem::OnInitScriptEditor()
{

    QWidget* widget=new QWidget();
    SEditor->setupUi(widget);

    connect(SEditor->lineEdit,SIGNAL(textChanged(QString)),SLOT(OnDisplayChanged(QString)));
    MainWindow* LeePicker=MainWindow::Instance();

    bool AppC = LeePicker->RemoteApp == Maya ? false : true;

    AppC == false ? SEditor->MayaRadio->setChecked(!AppC) :
        SEditor->BlenderRadio->setChecked(AppC) ;

    switch (LeePicker->RemoteApp) {
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

    //Check Maya Running
    if(isRunning("maya.exe")){

        //check Port Commend
        bool result{};
        auto future = QtConcurrent::run([&](){result = PyExecResultAsBool(LEESCRIPTPATH,LEEPYTHON_ULTILS,funcName);});
        future.waitForFinished();

        if(!result){
            LeePicker->AddToLog(Error,"Error :  Port not found exec Mel commandPort -name \"localhost:54322\" -sourceType \"python\";",true);
            return;
        }
        ///Send MayA Command
        funcName = "send_command";

        ///Init Maya Command
        const char* MayaPyCmd = "cmds.ls(sl=1)";
        QString selections = PyExecResultString(LEESCRIPTPATH,LEEPYTHON_ULTILS,funcName,MayaPyCmd);

        //save value to assign
        if(!selections.isEmpty() && !selections.startsWith("[]"))
            SaveAssignObject(this,Maya,selections);

        //Log Result
        LeePicker->AddToLog(Log,QString("select %1").arg(selections),true);
        qDebug() << selections << Qt::endl;
    }
    else{
        //report error software not running
        QString AppC = LeePicker->RemoteApp == Maya ?
                           "Maya is not Runing" :
                           "Blender is not Runing";

        QString info = "Error :  " + AppC;
        LeePicker->AddToLog(Error,info,true);
    }

}

void LeePickerItem::OnSelectionClicked(bool isSelect, bool isAdd)
{
    ///check empty property
    auto pro = property("select").toString();
    if(pro.isEmpty() || pro.isNull()) return;

    const char* funcName = isSelect ?
                            isAdd ?
                            "PickerAddSelect" :
                            "PickerSelect" :
                            "PickerDeSelect";
    auto Args = pro.toStdString();
    PyExecFuncAsVoid(funcName,Args.c_str());
}
#pragma endregion }
