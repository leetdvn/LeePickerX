#include "mainwindow.h"
#include "../ui/ui_leePicker.h"

static QString USER = qgetenv("USER").isEmpty() ? qgetenv("USERNAME") : qgetenv("USER");
static QString PICKERICON = "C:/Users/" + USER + "/Documents/GitHub/LeePickerX/build/Debug/icons/";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::leePicker)
    , scene(new PickerScene)
{

    ui->setupUi(this);
    pView = *&ui->PickerGraphicsViews;
    pView->setScene(scene);
    const QString bgrfile=":/icons/author.png";
    QImage image(bgrfile);
    QBrush viewbrush(image);

    if(image.isNull()){
        qDebug() << QString("Fucking nullptr") << Qt::endl;
        return;
    }

    scene->addText("Tao Day");

    QPixmap pim = QPixmap(":/icons/author.png");
    ui->actionAuth->setIcon(pim);
    pView->setBackgroundBrush(pim);

}

MainWindow::~MainWindow()
{
    delete ui;
}
