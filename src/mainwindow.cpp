#include "mainwindow.h"
#include "../ui/ui_leePicker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::leePicker)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
