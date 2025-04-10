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
#include <QAbstractSocket>
#include <qtimer.h>
#include <QtSerialPort/QSerialPortInfo>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHostInfo>


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
    // QHostAddress host("127.0.0.1");

    // InitSocket(host,mPort);

    QString message = IsAppAvalible() ?
                                        "Maya is Runing":
                                        "Maya is Not Running";
    AddToLog(Log,message,true);


    //init Default
    _Users = qgetenv("USERNAME");
    _Host = QHostInfo::localDomainName();
    _Pc = qgetenv("COMPUTERNAME");

    IsAuthor();

    LoadLocalData();

    //connect(this,&QMainWindow::close,this,[&](){OnPickerExit();});
}

MainWindow::~MainWindow()
{
    delete ui;
    m_Instance = nullptr;
    m_pTcpSocket->flush();
    m_pTcpSocket->close();
    free(m_pTcpSocket);
    m_pTcpSocket = Q_NULLPTR;
    deleteLater();
}

void MainWindow::AddToLog(const LogType inLog, QString inMessage, bool isClear, int inSecond)
{
    if(inMessage =="") return;
    QString title;

    QString result;
    switch (inLog) {
    case Log:{
        title = "Log : ";
        result = LEEWHITE_HTML.arg(title);
        break;
    }
    case Warning:{
        title = "Warning : ";
        result = LEEYELLOW_HTML.arg(title);
        break;
    }
    case Error:{
        title = "Error : ";
        result = LEERED_HTML.arg(title);
        break;
    }
    case Completed:{
        title = "Completed : ";
        result = LEECYAN_HTML.arg(title);
        break;
    }

    };
    result += inMessage;

    qDebug () << "Reuslt : " << result << Qt::endl;
    QString CLog = !isClear ? ui->LogPicker->toHtml() : QString();
    CLog += result;
    ui->LogPicker->setHtml(CLog);
    //new Logs
    //set value max down to new Log
    int valueMax = ui->LogPicker->verticalScrollBar()->maximum();
    ui->LogPicker->verticalScrollBar()->setValue(valueMax);
    ui->LogPicker->update();

    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, [&](){ui->LogPicker->clear();});
    // timer->start(5000);
    if(timeId > 0)
        killTimer(timeId);
    timeId = startTimer(inSecond*1000);
    qDebug() << "TimerId : " <<  timeId << Qt::endl;
}

bool MainWindow::IsAppAvalible()
{
    QString app = RemoteApp == Maya ? "maya.exe" : "blender.exe";

    return isRunning(app);
}

void MainWindow::ReInitSocket(const SoftWareApp inApp)
{
    QHostAddress host("127.0.0.1");

    InitSocket(host,inApp == Maya ? mPort : bPort);

    qDebug() << m_pTcpSocket->peerPort() << "check port " << Qt::endl;
}

QPointer<QTcpSocket> MainWindow::GetTcpSocket()
{
    //Get pointer TcpSocket
    if(m_pTcpSocket->state() != QAbstractSocket::ConnectedState){
        delete m_pTcpSocket;
        m_pTcpSocket = nullptr;
        ReInitSocket(RemoteApp);
    }
    //Get pointer TcpSocket

    return m_pTcpSocket;
}



void MainWindow::timerEvent(QTimerEvent *event)
{
    ///Timer Event
    ui->LogPicker->clear();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ///Close Event...
    SaveLocalData();
    qDebug () << "Closed Window" << Qt::endl;

}

void MainWindow::SaveLocalData()
{
    ///Save Data
    QFile savefile(PickerLocation);

    QJsonObject obj;QJsonDocument jdoc;

    obj[INTERACTAPP]= RemoteApp  == ui->actionConnectApp->isChecked() ? "Maya" : "Blender";

    qDebug () << ui->actionConnectApp->isChecked() << "remote" << Qt::endl;
    obj["CanEditBgr"] = ui->BgrAct->isChecked();
    jdoc.setObject(obj);

    if(savefile.open(QIODeviceBase::ReadWrite | QIODevice::Truncate))
    {
        savefile.write(jdoc.toJson());
        savefile.close();
    }
}

bool MainWindow::IsAuthor()
{
    qDebug() << GetMacAddress() << Qt::endl;

    if(!IsOnline()){
        AddToLog(Error,"Net Message : Network Connection failse please check your internet connection..",true);

        return false;
    }

    QString HostLower = _Host.toLower();

    QStringList Ids = {GetMacAddress(),HostLower,_Pc};


    if(Ids.length() <=0) return false;

    for(auto Id : Ids)  {
        if(IsValidAPI(LEEARTURL,Id)) {
            InitLeePicker();


            return true;
        }
    }

    QString LogAuthor =  "Error : You cannot use the software without the consent of Lee.";

    AddToLog(Error,LogAuthor,true);

    setDisabled(true);

    update();

    return false;
}

QString MainWindow::GetMacAddress()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;
    for (int i = 0; i < ifaces.count(); i++){
        QNetworkInterface iface = ifaces.at(i);
        if (iface.hardwareAddress() != "") { return iface.hardwareAddress(); }
    }
    return QString();
}

bool MainWindow::IsValidAPI(QString inUrl, QString inMacHost)
{
    if(inUrl.isEmpty()) return false;

    QEventLoop eventloop;
    QNetworkAccessManager networkManager;

    QUrl url(inUrl);

    QNetworkRequest request(url);

    QNetworkReply* reply = networkManager.get(request);

    QString result="NetworkDebug : ";
    connect(reply,&QNetworkReply::finished,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    QString ReplyText= reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(ReplyText.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue value = obj.value(QString("LeeArt"));

    QJsonArray arr = value.toArray();
    for(auto elm : arr){
        QJsonObject obj = elm.toObject();
        QJsonValue val = obj.value("ID");

        if(val.toString()==inMacHost)
        {
            _AuthorName = val.toString();
            qDebug() << "Network Validated.. : " << val.toString() << Qt::endl;
            return true;
        }
    }
    return false;

}

void MainWindow::InitLeePicker()
{
    //init Defaults
    QString baseDir = "C:/Users/" + _Users + "/AppData/LocalLow/LeePicker/";

    if (!QDir(baseDir).exists()){
        QDir nDir(baseDir);
        if(!nDir.exists()) {
            nDir.mkdir(baseDir);
        }
    }



    qDebug() << "log Base Dir" <<  baseDir <<  Qt::endl;

}

void MainWindow::LoadLocalData()
{
    QFile localFile(PickerLocation);
    if(!localFile.exists()) return ;

    QByteArray localData = JsonImport(localFile,false);

    if(localData.isEmpty() || localData.isNull()) return;

    QJsonDocument jdoc = QJsonDocument::fromJson(localData);
    QJsonObject currentObj = jdoc.object();

    QString app = currentObj[INTERACTAPP].toString();

    bool isMaya = app.endsWith("Maya") ? false : true;

    QString img = !isMaya ? (":/icons/maya.png") : (":/icons/blender.png");

    RemoteApp = isMaya ? Maya  : Blender;

    QImage image(img);

    ui->actionConnectApp->setIcon(QPixmap::fromImage(image));
    ui->actionConnectApp->setChecked(isMaya);
    update();
}

void MainWindow::InitializeFuns()
{
    ///Signal SLot Window
    connect(ui->tabWidget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(OnNewTab(int)));

    ///Signals slot Action
    connect(ui->NewShapeAct,SIGNAL(triggered(bool)),this,SLOT(CreateNewShape(bool)));

    connect(ui->NewScene,SIGNAL(triggered(bool)),this,SLOT(OnNewFile()));

    connect(ui->Open,&QAction::triggered,this,[&](){OnFileOpen();});

    connect(ui->SaveAs,&QAction::triggered,this,[&](){OnSaveAs();});

    connect(ui->Quit,SIGNAL(triggered(bool)),this,SLOT(OnPickerExit()));
    ///toogle grid
    //connect(ui->actionLeeGrid, SIGNAL(triggered()), this, SLOT(OnToogleGrid()));

    connect(ui->actionConnectApp,SIGNAL(triggered(bool)),this,SLOT(OnConnectAppChanged(bool)));

    connect(ui->tabWidget,SIGNAL(tabBarClicked(int)),this,SLOT(OnTabBarClicked(int)));

    connect(ui->ColorAct,SIGNAL(triggered(bool)),this,SLOT(OnColorChoise()));

    connect(ui->Save,SIGNAL(triggered(bool)),this,SLOT(OnSave()));

    ///Flip Action
    connect(ui->actionFlip_Horizontal,&QAction::triggered,this,[&](){OnFlip(false);});

    connect(ui->actionFlip_Vertical ,&QAction::triggered,this,[&](){OnFlip(true);});

    ///Align Selections
    connect(ui->actionLefttoRight,&QAction::triggered,this,[&](){AlignHorizontal();});
    connect(ui->actionRighttoLeft,&QAction::triggered,this,[&](){AlignHorizontal(true);});
    connect(ui->actionVToptoBottom,&QAction::triggered,this,[&](){AlignVertical(false);});
    connect(ui->actionVBottomtoTop,&QAction::triggered,this,[&](){AlignVertical(true);});
    connect(ui->SortVericalAct,&QAction::triggered,this,[&](){AlignVertical(false);});
    connect(ui->SortHorizontalAct,&QAction::triggered,this,[&](){AlignHorizontal(true);});

    /// PinSelected.
    connect(ui->actionPin,&QAction::triggered,this,[&](){OnPinAction();});

    connect(ui->BgrAct,&QAction::triggered,this,[&](){OnEditBackGround();});

    ///Menu Create
    connect(ui->actionCreateCImage,&QAction::triggered,this,[&](){OnCreateNewShape();});
    connect(ui->actionCreateSingleButton,&QAction::triggered,this,[&](){CreateNewShape();});

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
    LeePickerScene* scene = new LeePickerScene();
    scene->SetISceneName(QString("scene_%1").arg(graphName));
    view->setScene(scene);
    if (curr->objectName() =="mainTab"){
        ui->GraphicsLayout->addWidget(view);
        scene->SetISceneName("Main");
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
        QWidget* curentWidget = ui->tabWidget->currentWidget();
        LeePickerScene* currentScene = getScene(curentWidget);
        if(currentScene !=Q_NULLPTR){
            currentScene->SetISceneName(QString("scene_").arg(inNewName));
        }
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
            //current->setCacheMode(QGraphicsView::CacheBackground);
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
        //current->setCacheMode(QGraphicsView::Ca);

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

    QImage image(img);

    QAction* appAct = qobject_cast<QAction*>(sender());
    if(appAct==nullptr) return;

    appAct->setIcon(QPixmap::fromImage(image));

    // QHostAddress host("127.0.0.1");
    // quint16 Port = RemoteApp == Maya ? mPort : 5000;

    //InitSocket(host,Port);
    QString messageApp = RemoteApp == Maya ? "Maya Running" : "Blender Running";
    QString message = !checkable ? "Execute in Maya " : "Execute in Blender ";

    QString finalMsg = IsAppAvalible() ? messageApp : message;
    AddToLog(Log,finalMsg,true);
}

void MainWindow::OnNewItem()
{
    //Create New Item
}


void MainWindow::OnPickerExit()
{

    SaveLocalData();
    ////On Quit
    QCoreApplication::exit();
}

void MainWindow::OnColorChoise()
{
    ///Change Color
    QColor color= QColorDialog::getColor(Qt::white, this, "choise Color");
    MColor=color;
    emit OnColorChanged(color);
}

QList<LeePickerScene *> MainWindow::GetAllTabScenes()
{
    ///Get All Tab Scene
    QList<LeePickerScene*> Scenes = QList<LeePickerScene*>();

    int tabNum = ui->tabWidget->count();
    if(tabNum <= 0) return Scenes;

    for(int i =0 ; i < tabNum ; ++i){

        QWidget* wg = ui->tabWidget->widget(i);
        LeePickerScene* scene = getScene(wg);
        if(scene == nullptr) continue;
        QString tabName = ui->tabWidget->tabText(i);
        scene->setObjectName(tabName);
        Scenes.append(scene);
        //if(wg->tab)
    }
    return Scenes;
}


void MainWindow::OnSave()
{
    ///Save Data
    if(currentPath.isEmpty()|| currentPath.isNull()) return;

    return SaveData(currentPath);
}

void MainWindow::OnFileOpen()
{
    ///File Open File Data
    QString filter = "JSON (*.json)", source{};

    QString fileOpen = fileDialog(this);

    if(fileOpen.isNull() || fileOpen.isEmpty()) return ;

    QString message = QString("Open file :  %1").arg(fileOpen);

    AddToLog(Completed,message,true);

    currentPath = fileOpen;
    return LoadDataFile(fileOpen);
}

void MainWindow::OnSaveAs()
{
    ///Save Data Ass File Diglog Save
    QString fileName = fileDialog(this,true);

    if(fileName.isEmpty() || fileName.isNull()) return;

    return SaveData(fileName);

}

void MainWindow::SaveData(const QString inPath)
{
    if(inPath.isEmpty()|| inPath.isNull()) return ;


    QList<LeePickerScene*> ListScenes = GetAllTabScenes();
    if(ListScenes.length() <= 0) {
        AddToLog(Log,"No Data To Save.",true);
        return;
    }

    QJsonArray jsScene;
    QJsonObject PickerObj;

    int count=0;
    for(auto &scene : ListScenes){
        QJsonArray jsArray  = scene->GetDataAllObject();
        QJsonObject sceneObj;
        LEEJOBJ(sceneObj,"TextName",ui->tabWidget->tabText(count));
        LEEJOBJ(sceneObj,"SceneName",scene->GetISceneName());
        LEEJOBJ(sceneObj,"SceneLock",scene->GetISceneLocked());
        LEEJOBJ(sceneObj,"Items",jsArray);
        jsScene.append(sceneObj);
        count++;
    }

    LEEJOBJ(PickerObj,"LeePicker",jsScene);
    LEEJOBJ(PickerObj,"InteractApp",RemoteApp == Maya ? "Maya" : "Blender");
    LEEJOBJ(PickerObj,"CanEditBgr",ui->BgrAct->isChecked());

    QFile jfile(inPath);
    QByteArray byteArray = QJsonDocument(PickerObj).toJson();

    JsonExport(jfile,byteArray,true);
    QString message = "Saved  %1";
    AddToLog(Log,message.arg(jfile.fileName()));

    ///Json Check Path
    QString jsPath = inPath;
    QFile checkFile(jsPath.replace("Leetdvn","json"));
    QByteArray imp =  JsonImport(jfile,true);

    JsonExport(checkFile,imp);

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
    for (auto& rf : recentfiles) {
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

        connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(OnReadSocketData()),Qt::UniqueConnection);
        connect(m_pTcpSocket,&QTcpSocket::errorOccurred,this,&MainWindow::OnConnectionError,Qt::UniqueConnection);
        connect(m_pTcpSocket,SIGNAL(connected()),SLOT(OnSocketConnected()),Qt::UniqueConnection);
        connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(OnSocketDisconneted()),Qt::UniqueConnection);
        m_pTcpSocket->waitForConnected(5000);
    }

    m_pTcpSocket->close();
}

void MainWindow::LoadDataFile(QString &inPath)
{
    QFile inDatafile(inPath);
    if(!inDatafile.exists()) {
        qDebug() << "file does not exists.." << Qt::endl;
        return ;
    }

    ///Ready All Data
    QByteArray data = JsonImport(inDatafile,true);

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject rootObj = doc.object();
    QJsonArray dataTab = rootObj[AUTHOR].toArray();
    RemoteApp = rootObj[INTERACTAPP].toVariant().toString().endsWith("Maya") ? Maya : Blender;
    bool caneditbgr = rootObj["CanEditBgr"].isBool();
    ui->BgrAct->setChecked(caneditbgr);
    int count=0;
    foreach(const QJsonValue &v, dataTab) {
        QJsonObject tabOther = v.toObject();
        QString TabName = tabOther["TextName"].toString();
        QJsonArray Items = v.toObject().value("Items").toArray();
        if(count > 0){
            AddTabSimple(TabName);
            ui->tabWidget->setTabText(count,TabName);
        }

        LeePickerScene* scene  = getScene(ui->tabWidget->widget(count));

        scene->LoadSceneData(tabOther);
        count++;
    }

    //JsonImport()
}

void MainWindow::AlignHorizontal(bool fromRight)
{
    QPointer<LeePickerScene> currentScene = getScene(ui->tabWidget->currentWidget());

    if(currentScene==nullptr) return;

    currentScene->AlignSelectedItems(fromRight,true);

    qDebug() << "Align" << Qt::endl;
}

void MainWindow::AlignVertical(bool fromBottom)
{

    QPointer<LeePickerScene> currentScene = getScene(ui->tabWidget->currentWidget());

    if(currentScene==nullptr) return;

    currentScene->AlignSelectedItems(fromBottom);
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

    RemoteApp==Blender ?
            BlenderHasConnected=true :
            MayaHasConnected != BlenderHasConnected;

    qDebug() << "Maya"  << MayaHasConnected
             << "Blender " << BlenderHasConnected
             << "Port" << Socket->peerPort() <<   Qt::endl;

    //init Connections
    // QTextStream T(Socket);

    // QString scrLocation = QDir::currentPath() + "/Scripts/";
    // T << QString("import sys\nsys.path.append('%1')\nprint(\"LeePicker Connected..%1\")").arg(scrLocation);
    // QString cmd = RemoteApp == Maya ? "import MayaCommandPort" : "import BlenderCommandPort";
    // cmd += "\nprint(\"print import completed.\")";
    // T << cmd;

    QString msg = RemoteApp == Maya ? QString("   Maya Connected") : QString("   Blender Connected");
    AddToLog(Completed,msg,true);
    //Socket->flush();
}

void MainWindow::OnSocketDisconneted()
{
    QTcpSocket* Socket = qobject_cast<QTcpSocket*>(sender());
    if(Socket==nullptr) return ;

    Socket->localPort()==bPort ?
        BlenderHasConnected=true :
        MayaHasConnected = true;

    qDebug()<< "Maya"  << MayaHasConnected
            << "Blender " << BlenderHasConnected
            << "Port" << Socket->localPort() <<   Qt::endl;



}

void MainWindow::OnConnectionError(QAbstractSocket::SocketError inError)
{
    QTcpSocket* Socket = qobject_cast<QTcpSocket*>(sender());
    if(Socket==nullptr) return ;
    Socket->peerPort()==bPort ?
        BlenderHasConnected=false :
        MayaHasConnected = false;

    QString message = RemoteApp == Maya ?
                          "Maya is Not Connected." :
                          "Blender is Not Connected.";
    if(QAbstractSocket::ConnectionRefusedError==inError){
        qDebug() << Socket->peerPort() << MayaHasConnected << BlenderHasConnected <<  Qt::endl;

        //Socket->aboutToClose();

        AddToLog(Log,message,true,10);
    }

}

void MainWindow::OnReadSocketData()
{
    qDebug() << "Ready SocketData " << Qt::endl;
    mIsConnected=true;
}

void MainWindow::OnPinAction()
{
    QPointer<QAction> Act = qobject_cast<QAction*>(sender());

    QPointer<LeePickerScene> lScene = getScene(ui->tabWidget->currentWidget());

    if (lScene == Q_NULLPTR) return;

    IsPined = !IsPined;

    lScene->OnSelectedPin(IsPined);

    Act->setText(IsPined ? "UnPined" : "Pin");
    qDebug() << "is Pin click" << IsPined <<   Qt::endl;

}

void MainWindow::OnEditBackGround()
{
    CanEditBgr = ui->BgrAct->isChecked();
    QString Text = CanEditBgr ? "EDIT" : "BGR";
    ui->BgrAct->setText(Text);

    QPointer<LeePickerScene> currentScene = getScene(ui->tabWidget->currentWidget());
    if(!CanEditBgr && currentScene !=nullptr) {
        currentScene->ClearSelection();
        return;
    }

    currentScene->SelectAllItemZLayer(0);

    qDebug() << "Bgr Edit" << Qt::endl;
}

void MainWindow::OnCreateNewShape()
{
    QPointer<LeePickerScene> currentScene = getScene(ui->tabWidget->currentWidget());

    QString image = BrowserImage();

    if (!QFile(image).exists() || currentScene == nullptr) return;

    QPointer<LeePickerItem> item =  currentScene->CreateItem(QString(),image);

    item->UpdateRect();

}
