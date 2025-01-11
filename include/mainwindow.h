#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <leepickerview.h>
#include <QPointer>
#include <QLineEdit>
#include <leeGlobal.hpp>
#include <singleton.h>
#include <QProperty>
#include <LeePickerPython.h>

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

    void AddToLog(const LogType inLog,QString inMessage,bool isClear=0);



    static SoftWareApp RemoteApp;
signals:
    void OnColorChanged(QColor &Color);

private:

    Ui::leePicker *ui;

    QPointer<LeePickerView> pView=nullptr;

    void InitializeFuns();

    void CustomNewTab(QString newName,int type=0);

    void AddTabSimple(QString tabname);

    void tabSetup(bool newfile=0);

    ///View
    LeePickerView* getView(QWidget* tabIndex);

    LeePickerScene* getScene(const QWidget* tabIndex);

    int currentTab;

    QColor MColor;


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
    void OnPickerExit();
    void OnColorChoise();
    void OnSave();


};

typedef Singleton<MainWindow> Picker;
#endif // MAINWINDOW_H
