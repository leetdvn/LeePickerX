#include "mainwindow.h"
#include "../ui/ui_leePicker.h"

static QString USER = qgetenv("USER").isEmpty() ? qgetenv("USERNAME") : qgetenv("USER");
static QString PICKERICON = "C:/Users/" + USER + "/Documents/GitHub/LeePickerX/build/Debug/icons/";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::leePicker)
{

    ui->setupUi(this);
    pView = new LeePickerView(this);
    ui->GraphicsLayout->addWidget(pView);

}

MainWindow::~MainWindow()
{
    delete ui;
}
