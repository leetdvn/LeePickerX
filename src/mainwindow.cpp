#include "mainwindow.h"
#include "../ui/ui_leePicker.h"
#include <QWidgetAction>
#include <QTabBar>
#include <QLineEdit>
#include <QScrollBar>
#include <QColorDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <Definations.h>

//MainWindow* MainWindow::m_Instance=nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::leePicker)
{

    if(m_Instance==nullptr)
    {
        m_Instance=this;
    }

    ui->setupUi(this);

    setDockOptions(QMainWindow::AllowTabbedDocks);
    setContextMenuPolicy(Qt::NoContextMenu);
    pView = new LeePickerView(this);
    ui->GraphicsLayout->addWidget(pView);
    //tabwight moveable
    ui->tabWidget->setMovable(true);
    //Tab Create New
    InitializeFuns();
    RemoteApp = Maya;
    inItLog();

    QHostAddress host("127.0.0.1");
    quint16 bPort = 5000;
    quint16 mPort = 54322;
    InitSocket(host,mPort);

}

MainWindow::~MainWindow()
{
    delete ui;
    m_Instance = nullptr;
    deleteLater();
}

void MainWindow::AddToLog(const LogType inLog, QString inMessage, bool isClear)
{
    if(inMessage =="") return;
    QStringList splitMes =  inMessage.split(" : ");
    QString title = splitMes.count() >=0 ? splitMes[0] : inMessage;

    QString mess = splitMes.count() >=2  ? splitMes[1] : splitMes[0];
    QString result;
    switch (inLog) {
    case Log:{
        result = QString("<font color=\"white\">%1</font>").arg(mess);
        break;
    }
    case Warning:{
        result = QString("<font color=\"yellow\">%1</font>").arg(title);
        result += mess;
        break;
    }
    case Error:{
        result = QString("<font color=\"red\">%1</font>").arg(title);
        result += mess;
        break;
    }
    case Completed:{
        result = QString("<font color=\"cyan\">%1</font>").arg(title);
        result += mess;
        break;
    }
    };


    QString CLog = !isClear ? ui->LogPicker->toHtml() : "";
    CLog += result;
    ui->LogPicker->setHtml(CLog);

    //new Logs
    //set value max down to new Log
    int valueMax = ui->LogPicker->verticalScrollBar()->maximum();
    ui->LogPicker->verticalScrollBar()->setValue(valueMax);
    ui->LogPicker->update();
}

bool MainWindow::IsAppAvalible()
{

    QString app = RemoteApp == Maya ? "maya.exe" : "blender.exe";

    return isRunning(app);
}



void MainWindow::InitializeFuns()
{
    //Signal SLot Window
    connect(ui->tabWidget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(OnNewTab(int)));

    //Signals slot Action
    connect(ui->NewShapeAct,SIGNAL(triggered(bool)),this,SLOT(CreateNewShape(bool)));

    connect(ui->NewScene,SIGNAL(triggered(bool)),this,SLOT(OnNewFile()));

    connect(ui->Open,&QAction::triggered,this,[&](){OnFileOpen();});

    connect(ui->SaveAs,&QAction::triggered,this,[&](){OnSaveAs();});

    connect(ui->Quit,SIGNAL(triggered(bool)),this,SLOT(OnPickerExit()));
    //toogle grid
    connect(ui->actionLeeGrid, SIGNAL(triggered()), this, SLOT(OnToogleGrid()));

    connect(ui->actionConnectApp,SIGNAL(triggered(bool)),this,SLOT(OnConnectAppChanged(bool)));

    connect(ui->tabWidget,SIGNAL(tabBarClicked(int)),this,SLOT(OnTabBarClicked(int)));

    connect(ui->ColorAct,SIGNAL(triggered(bool)),this,SLOT(OnColorChoise()));

    connect(ui->Save,SIGNAL(triggered(bool)),this,SLOT(OnSave()));

    //Flip Action
    connect(ui->actionFlip_Horizontal,&QAction::triggered,this,[&](){OnFlip(false);});

    connect(ui->actionFlip_Vertical ,&QAction::triggered,this,[&](){OnFlip(true);});

}

void MainWindow::CreateNewShape(bool ischecked)
{
    QWidget* curentWidget = ui->tabWidget->currentWidget();

    if(currentTab == ui->tabWidget->count()-1) return;

    LeePickerView* pView = curentWidget->findChild<LeePickerView*>();

    LeePickerScene* pScene = dynamic_cast<LeePickerScene*>(pView->scene());

    if(pScene == Q_NULLPTR) return;

    pScene->CreateItem("ABC");
    qDebug() << "Create New Shape" << ischecked << Qt::endl;
}

void MainWindow::OnNewFile()
{
    int idx = ui->tabWidget->count() - 1;

    if(idx < 2) {
        CleanUpScene(0);

        qDebug() << "scene " << idx << Qt::endl;
        return;
    }

    for (int i = idx; i >0; i--)
    {
        QWidget* subtab = ui->tabWidget->widget(i);
        if (subtab->objectName() == "leeTabAdditional" || ui->tabWidget->tabText(i)=="+") continue;
        LeePickerView* v = getView(subtab);
        if (v) {
            v->resetCachedContent();
            delete v;
        }
        subtab->deleteLater();
    }

    CleanUpScene(0);
    ui->tabWidget->setCurrentIndex(0);
    return ui->tabWidget->update();

}

#pragma region TabWidget Funtion {

void MainWindow::CustomNewTab(QString newName,int index)
{
    // create new tab 2 option is addtional and read file open
    QWidget* curr = ui->tabWidget->currentWidget();
    if (curr->objectName() == "leeTabAdditional") {
        newName += QString::number(index);
        return AddTabSimple(newName);
    }

    if(curr->objectName()=="mainTab") {
        return;
    }
    //if (index)

    QMenu* menu = new QMenu();
    menu->setAttribute(Qt::WA_DeleteOnClose,true);
    QWidgetAction* w = new QWidgetAction(menu);
    QPointer<QLineEdit> lineTabName = new QLineEdit(menu);
    lineTabName->setAttribute(Qt::WA_DeleteOnClose,true);
    lineTabName->setPlaceholderText("new name");
    qreal textlegth = ui->tabWidget->tabBar()->tabRect(index).width();
    lineTabName->setFixedWidth(textlegth);
    lineTabName->setText(ui->tabWidget->tabText(index));
    w->setDefaultWidget(lineTabName);
    menu->addAction(w);
    double px = ui->tabWidget->tabBar()->tabRect(index).x();
    double py = ui->tabWidget->tabBar()->tabRect(index).y();
    connect(lineTabName,SIGNAL(textChanged(QString)), this,SLOT(OnTabRename(QString)));
    menu->exec(mapToGlobal(QPoint(px, py + 60)));
    lineTabName->setFocusPolicy(Qt::StrongFocus);
    lineTabName->setFocusProxy(menu);
}

void MainWindow::AddTabSimple(QString tabname)
{
    int idx = ui->tabWidget->count() - 1;
    tabname +=QString::number(idx);
    QWidget* tabX = new QWidget();
    ui->tabWidget->insertTab(idx, tabX, tabname);
    ui->tabWidget->widget(idx);
    ui->tabWidget->setCurrentIndex(idx);
    tabX->setObjectName(tabname);
    tabSetup();
}

void MainWindow::tabSetup(bool newfile)
{
    // set  up new tab , add leegraphicsviewport on tab
    QWidget* curr = ui->tabWidget->currentWidget();

    int index = ui->tabWidget->currentIndex();
    QString title = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (title == "+") {
        curr = ui->tabWidget->widget(index - 1);
    };

    QHBoxLayout* layout = new QHBoxLayout(curr); // fix new file if tab ==
    layout->setContentsMargins(0, 0, 0, 0);
    curr->setLayout(layout);
    LeePickerView* view = new LeePickerView(curr);
    view->setObjectName("Main");
    QString graphName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    view->setObjectName(graphName);
    LeePickerScene* scene = new LeePickerScene();
    scene->setObjectName("leeScene");
    view->setScene(scene);
    if (curr->objectName() =="mainTab"){
        ui->GraphicsLayout->addWidget(view);
    }
    else
        layout->addWidget(view);
}

void MainWindow::CleanUpScene(int index)
{
    LeePickerScene* sceneIdx = getScene(ui->tabWidget->widget(index));
    if(sceneIdx == nullptr) return;
    QList<LeePickerItem*> Items = sceneIdx->GetAllItems();
    if(Items.length() <= 0) return ;

    for(const auto it : Items)
        it->deleteLater();

    qDebug() << "CleanUp Scene Items" << Qt::endl;
}

LeePickerView *MainWindow::getView(QWidget *tabIndex)
{
    QList<LeePickerView*> llui = tabIndex->findChildren<LeePickerView*>();
    if (llui.length() > 0) {
        for (LeePickerView* view : llui) {
            if (view) {
                return view;
            }
        }
    }
    return nullptr;
}

LeePickerScene *MainWindow::getScene(const QWidget *tabIndex)
{
    QPointer<LeePickerScene> sceneResult=nullptr;
    QList<LeePickerView*> llui = tabIndex->findChildren<LeePickerView*>();
    if (llui.length() > 0) {
        for (LeePickerView* view : llui) {
            if (view) {
                return sceneResult = qobject_cast<LeePickerScene*>(view->scene());
            }
        }
    }
    return sceneResult;
}

QList<LeePickerItem *> MainWindow::GetSelectedItems()
{
    QList<LeePickerItem*> selectedItems{};
    LeePickerScene* mScene = getScene(ui->tabWidget->currentWidget());
    if (mScene)
    {
        QList<QGraphicsItem*> ilist = mScene->selectedItems();
        if (!ilist.isEmpty())
        {
            for (QGraphicsItem* it : ilist)
            {
                LeePickerItem* mObj = qgraphicsitem_cast<LeePickerItem*>(it);

                /*  leePatternItem* pItem = qgraphicsitem_cast<leePatternItem*>(it);
                if (pItem) leeObj.clear(); break;*/
                if (mObj)
                    selectedItems.append(mObj);
            }
        }
    }
    return selectedItems;
}

void MainWindow::inItLog()
{
}

void MainWindow::OnTabBarClicked(int index)
{
    currentTab= index;
}

void MainWindow::OnTabRename(const QString &inNewName)
{

    qDebug() << "info " << inNewName << Qt::endl;

    if (!inNewName.isEmpty()) {
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), inNewName);
    }
}

void MainWindow::OnToogleGrid()
{
    //Toolge Show Hide Grid BackGround Brush
    QImage image(":/icons/author.png");

    QWidget* curentWidget = ui->tabWidget->currentWidget();

    LeePickerView* current = curentWidget->findChild<LeePickerView*>();

    QAction* grid = qobject_cast<QAction*>(sender());

    if (current)
    {
        if (grid && !grid->isChecked()) {
            current->setBackgroundBrush(QBrush());
        }
        else {
            current->setBackgroundBrush(image);
            current->setCacheMode(QGraphicsView::CacheBackground);
        }
    }
}

void MainWindow::OnTabChanged(int index)
{

    QImage image(":/icons/author.png");

    QWidget* curentWidget = ui->tabWidget->currentWidget();
    LeePickerView* current = curentWidget->findChild<LeePickerView*>();
    if(ui->actionLeeGrid->isChecked()){
        current->setBackgroundBrush(image);
        current->setCacheMode(QGraphicsView::CacheBackground);

    }
    else{
        current->setBackgroundBrush(QBrush());
    }
}

#pragma endregion }

void MainWindow::OnConnectAppChanged(bool checkable)
{
    //On Connect With Maya Or Blender
    QString img = !checkable ? (":/icons/maya.png") : (":/icons/blender.png");

    RemoteApp = !checkable ? Maya  : Blender;
    QString message = !checkable ? "     Connect To Maya " : "      Connect To Blender ";
    AddToLog(LogType::Log,message,true);
    QImage image(img);

    QAction* appAct = qobject_cast<QAction*>(sender());
    appAct->setIcon(QPixmap::fromImage(image));

}

void MainWindow::OnNewItem()
{
    //Create New Item
}

void MainWindow::OnPickerExit()
{
    //On Quit
    QCoreApplication::exit();
}

void MainWindow::OnColorChoise()
{
    //Change Color
    QColor color= QColorDialog::getColor(Qt::white, this, "choise Color");
    MColor=color;
    emit OnColorChanged(color);
}

void MainWindow::OnSave()
{
    LeePickerScene* currentScene =  getScene(ui->tabWidget->currentWidget());
    if(currentScene==Q_NULLPTR) return;

    QJsonArray jsArray  = currentScene->GetDataAllObject();


    QJsonObject total;
    LEEJOBJ(total,ui->tabWidget->tabText(0),jsArray);

    QString ePath = QDir::currentPath() + "/Saved/test.Leetdvn";

    QFile jfile(ePath);
    QByteArray byteArray = QJsonDocument(total).toJson();

    JsonExport(jfile,byteArray,true);
    QString message = "Saved  %1";
    AddToLog(Log,message.arg(jfile.fileName()));

    QFile checkFile(ePath.replace("Leetdvn","json"));
    QByteArray imp =  JsonImport(jfile,true);

    JsonExport(checkFile,imp);

    qDebug() << "data : " << imp << Qt::endl;

}

void MainWindow::OnFileOpen()
{
    QString fileOpen = fileDialog(this);

    QString message = QString("Open file :  %1").arg(fileOpen);
    AddToLog(Completed,message,true);
}

void MainWindow::OnSaveAs()
{
    QString fileName = fileDialog(this);

    OnSave();
    QString message = QString("Saved  :  %1").arg(fileName);

    AddToLog(Completed,message,true);

}



void MainWindow::AddRecentFile(const QString inName)
{
    //Add Recent To Action of Recent Menu
    if (inName.isEmpty()) return;
    ui->menuRecent->addAction(inName,[&](){OnRecentFile();});

}

void MainWindow::LoadRecent()
{
    //load recent file
    std::reverse(recentfiles.begin(), recentfiles.end());
    for (QString rf : recentfiles) {
        ui->menuRecent->addAction(rf,[&](){OnLoadRecent();});
    }
}

void MainWindow::InitRecent()
{
    //init recent file from saved data file
}

void MainWindow::InitSocket(QHostAddress inhost, quint16 inPort)
{
    m_pTcpSocket = new QTcpSocket(this);

    if(!(QAbstractSocket::ConnectedState == m_pTcpSocket->state())){
        m_pTcpSocket->connectToHost(inhost,inPort,QIODevice::ReadWrite);

        // connect(m_pTcpSocket,SIGNAL(readyRead()),SLOT(readSocketData()),Qt::UniqueConnection);
        // connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),SIGNAL(connectionError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
        // connect(m_pTcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),SIGNAL(tcpSocketState(QAbstractSocket::SocketState)),Qt::UniqueConnection);

        connect(m_pTcpSocket,SIGNAL(connected()),SLOT(OnSocketConnected()),Qt::UniqueConnection);
        connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(OnSocketDisconneted()),Qt::UniqueConnection);
    }
    qDebug() << "Socket...init.." << Qt::endl;

}

void MainWindow::OnRecentFile()
{
    //signals On recent file
}

void MainWindow::OnLoadRecent()
{
    //action load Recenet
}

void MainWindow::OnFlip(bool isVertical)
{
    QList<LeePickerItem*> selectedItems = GetSelectedItems();
    if (!selectedItems.isEmpty())
    {
        for (int i=0; i < selectedItems.length(); i++)
            selectedItems[i]->SetFlip(isVertical);
    }

}

void MainWindow::OnSocketConnected()
{

    QTcpSocket* Socket = qobject_cast<QTcpSocket*>(sender());
    if(Socket==nullptr) return ;

    Socket->localPort()==5000 ?
            BlenderHasConnected=true :
            MayaHasConnected = true;

    qDebug()<< "Maya"  << MayaHasConnected
             << "Blender " << BlenderHasConnected
             << "Port" << Socket->localPort() <<   Qt::endl;

    QTextStream T(Socket);

    T << "print(\"thang dang do hoi cham gi nhanh vai dai\")";

    Socket->flush();

}

void MainWindow::OnSocketDisconneted()
{
    QTcpSocket* Socket = qobject_cast<QTcpSocket*>(sender());
    if(Socket==nullptr) return ;

    Socket->localPort()==5000 ?
        BlenderHasConnected=true :
        MayaHasConnected = true;

    qDebug()<< "Maya"  << MayaHasConnected
            << "Blender " << BlenderHasConnected
            << "Port" << Socket->localPort() <<   Qt::endl;

}
