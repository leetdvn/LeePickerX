#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <leepickerview.h>
#include <QPointer>
#include <QLineEdit>

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

    QPointer<LeePickerView> pView=nullptr;

    void InitializeFuns();

    void CustomNewTab(QString newName,int type=0);

    void AddTabSimple(QString tabname);

    void tabSetup(bool newfile=0);

    ///View
    LeePickerView* getView(QWidget* tabIndex);

    int currentTab;

private slots:
    ///Connection Slot
    void CreateNewShape(bool ischecked=0);
    void OnNewTab(int inIdx){   return CustomNewTab("NewTab",currentTab);}
    void OnNewFile();
    void OnTabRename(QString inNewName);
    void OnTabBarClicked(int index);
    void OnToogleGrid();
    void OnTabChanged(int index);
    void OnConnectAppChanged(bool checkable);
    void OnNewItem();
};
#endif // MAINWINDOW_H
