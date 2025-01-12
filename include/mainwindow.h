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

class MainWindow : public QMainWindow , public Singleton<MainWindow>
{
    Q_OBJECT

public:
    explicit  MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void AddToLog(const LogType inLog,QString inMessage,bool isClear=0);


    SoftWareApp GetInteractionApp(){return RemoteApp;}

    QColor GetColor(){return MColor;}

    bool IsAppAvalible();
    // static MainWindow* Instance(){
    //     if(m_Instance == nullptr)
    //         m_Instance=new MainWindow();
    //     return m_Instance;
    // }

signals:

    void OnColorChanged(QColor &Color);

private:

    //static MainWindow* m_Instance;

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

    void inItLog();

    SoftWareApp RemoteApp;

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



#endif // MAINWINDOW_H
