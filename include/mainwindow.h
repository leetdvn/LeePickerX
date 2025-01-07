#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <leepickerview.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class leePicker;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::leePicker *ui;
    LeePickerView* pView=nullptr;

    void InitializeFuns();

public slots:
    ///Connection Slot
    void CreateNewShape(bool ischecked=0);
};
#endif // MAINWINDOW_H
