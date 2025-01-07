#include "mainwindow.h"
#include "../ui/ui_leePicker.h"
#include <QWidgetAction>
#include <QTabBar>
#include <QLineEdit>

static QString USER = qgetenv("USER").isEmpty() ? qgetenv("USERNAME") : qgetenv("USER");
static QString PICKERICON = "C:/Users/" + USER + "/Documents/GitHub/LeePickerX/build/Debug/icons/";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::leePicker)
{

    ui->setupUi(this);
    setDockOptions(QMainWindow::AllowTabbedDocks);
    setContextMenuPolicy(Qt::NoContextMenu);
    pView = new LeePickerView(this);
    ui->GraphicsLayout->addWidget(pView);
    //tabwight moveable
    ui->tabWidget->setMovable(true);
    //Tab Create New
    InitializeFuns();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitializeFuns()
{
    //Signal SLot Window
    connect(ui->tabWidget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(OnNewTab(int)));

    //Signals slot Action
    connect(ui->NewShapeAct,SIGNAL(triggered(bool)),this,SLOT(CreateNewShape(bool)));

    connect(ui->NewScene,SIGNAL(triggered(bool)),this,SLOT(OnNewFile()));

    connect(ui->tabWidget,SIGNAL(tabBarClicked(int)),this,SLOT(OnTabBarClicked(int)));

    //toogle grid
    connect(ui->actionLeeGrid, SIGNAL(triggered()), this, SLOT(OnToogleGrid()));

    connect(ui->actionConnectApp,SIGNAL(triggered(bool)),this,SLOT(OnConnectAppChanged(bool)));
}

void MainWindow::CreateNewShape(bool ischecked)
{
    qDebug() << "Create New Shape" << ischecked << Qt::endl;
    OnNewItem();
}

void MainWindow::OnNewFile()
{
    int idx = ui->tabWidget->count() - 1;
    if (idx > 1) {
        for (int i = idx; i >0; i--)
        {
            QWidget* subtab = ui->tabWidget->widget(i);
            if (subtab->objectName() == "leeAddTab" || ui->tabWidget->tabText(i)=="+") continue;
            LeePickerView* v = getView(subtab);
            if (v) {
                v->resetCachedContent();
                delete v;
            }
            subtab->deleteLater();
        }
    }
    LeePickerView* leeView = getView(ui->mainTab);
    if (leeView) {
        if (leeView->numberItemOfScene() > 0) {
            delete leeView;
            ui->tabWidget->setCurrentIndex(0);
            tabSetup(true);
        }
        ui->tabWidget->setTabText(0, "Main");
    }
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
    int count = ui->tabWidget->count();

    QMenu* menu = new QMenu();
    menu->setAttribute(Qt::WA_DeleteOnClose,true);
    QWidgetAction* w = new QWidgetAction(menu);
    QLineEdit* line = new QLineEdit();
    line->setAttribute(Qt::WA_DeleteOnClose);
    line->setPlaceholderText("new name");
    qreal textlegth = ui->tabWidget->tabBar()->tabRect(index).width();
    line->setFixedWidth(textlegth);
    line->setText(ui->tabWidget->tabText(index));
    w->setDefaultWidget(line);
    menu->addAction(w);
    double px = ui->tabWidget->tabBar()->tabRect(index).x();
    double py = ui->tabWidget->tabBar()->tabRect(index).y();
    QSize sz = ui->tabWidget->tabBar()->tabRect(index).size() - ui->leeTabAdditional->size();
    connect(line,SIGNAL(textChanged(QString)), this,SLOT(OnTabRename(QString)));
    menu->exec(mapToGlobal(QPoint(px, py + 65)));
    line->setFocusPolicy(Qt::StrongFocus);
    line->setFocusProxy(menu);

}

void MainWindow::AddTabSimple(QString tabname)
{
    int idx = ui->tabWidget->count() - 1;
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
    QImage image(PICKERICON + "author.png");
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
    if (curr->objectName() =="mainTab")
        ui->GraphicsLayout->addWidget(view);
    else
        layout->addWidget(view);
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

void MainWindow::OnTabBarClicked(int index)
{
    currentTab= index;
}

void MainWindow::OnTabRename(QString inNewName)
{
    QLineEdit* line = qobject_cast<QLineEdit*>(sender());
    if (line) {
        QString old=ui->tabWidget->tabText(ui->tabWidget->currentIndex());
        if(!inNewName.isEmpty())
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), inNewName);
    }
}

void MainWindow::OnToogleGrid()
{
    QImage image( PICKERICON+ "author.png");

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

    QImage image( PICKERICON+ "author.png");

    QWidget* curentWidget = ui->tabWidget->currentWidget();

    LeePickerView* current = curentWidget->findChild<LeePickerView*>();
    QBrush brush = current->backgroundBrush();

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
    QString img = !checkable ? (":/icons/maya.jpg") : (":/icons/blender.jpg");

    QImage image(img);

    QAction* appAct = qobject_cast<QAction*>(sender());

    appAct->setIcon(QPixmap::fromImage(image));

}

void MainWindow::OnNewItem()
{
    QWidget* curentWidget = ui->tabWidget->currentWidget();

    LeePickerView* pView = curentWidget->findChild<LeePickerView*>();

    LeePickerScene* pScene = dynamic_cast<LeePickerScene*>(pView->scene());

    if(pScene ==Q_NULLPTR) return;

    pScene->CreateItem("ABC");
}

