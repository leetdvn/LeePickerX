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

    const quint16 mPort =54322;
    const quint16 bPort =5000;
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
    bool IsConnectedWithMaya(){return MayaHasConnected;}

    bool isConnectedWIthBlender(){return BlenderHasConnected;}

    void ReInitSocket(const SoftWareApp inApp);

signals:

    void OnColorChanged(QColor &Color);

protected:
    void timerEvent(QTimerEvent *event) override;
    int timeId=-1;

private:
    //static MainWindow* m_Instance;
    bool MayaHasConnected,BlenderHasConnected;
    Ui::leePicker *ui;

    QPointer<LeePickerView> pView=nullptr;
    QPointer<QTcpSocket> m_pTcpSocket=nullptr;

    QStringList recentfiles;

    void InitializeFuns();

    void CustomNewTab(QString newName,int type=0);

    void AddTabSimple(QString tabname);

    void tabSetup(bool newfile=0);

    void CleanUpScene(int index);

    ///View
    LeePickerView* getView(QWidget* tabIndex);

    LeePickerScene* getScene(const QWidget* tabIndex);

    QList<LeePickerItem*> GetSelectedItems();

    int currentTab;

    QColor MColor;

    void inItLog();

    SoftWareApp RemoteApp;
    Q_ENUM(SoftWareApp)

    void AddRecentFile(const QString inName);

    void LoadRecent();

    void InitRecent();

    void InitSocket(QHostAddress inhost,quint16 inPort);

    ///

private slots:
    ///Connection Slot
    void CreateNewShape(bool ischecked=0);
    void OnNewTab(int inIdx){   return CustomNewTab("NewTab",currentTab);}
    void OnNewFile();

    void OnTabRename(const QString &inNewName);

    void OnTabBarClicked(int index);
    void OnToogleGrid();
    void OnTabChanged(int index);
    void OnConnectAppChanged(bool checkable);
    void OnNewItem();
    void OnPickerExit();
    void OnColorChoise();
    void OnSave();

    void OnFileOpen();

    void OnSaveAs();

    void OnRecentFile();

    void OnLoadRecent();

    void OnFlip(bool isVertical);

    void OnSocketConnected();
    void OnSocketDisconneted();

    void OnConnectionError(QAbstractSocket::SocketError inError);



};



#endif // MAINWINDOW_H
