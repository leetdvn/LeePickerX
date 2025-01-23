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


#define AUTHOR "LeePicker"
#define INTERACTAPP "InteractApp"

///init MainWindow // Implementation Singleton Pattern
class MainWindow : public QMainWindow , public Singleton<MainWindow>
{
    Q_OBJECT

    const quint16 mPort =54322;
    const quint16 bPort =5000;

    const QString PickerLocation = "C:/Users/" + qgetenv("USERNAME") + "/AppData/LocalLow/LeePicker/LeePicker.picker";

public:
    explicit  MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ///Log Main
    void AddToLog(const LogType inLog,QString inMessage,bool isClear=0,int inSecond=6000);

    ///Server App
    SoftWareApp GetInteractionApp(){return RemoteApp;}

    ///Color
    QColor GetColor(){return MColor;}

    ///App Avalible
    bool IsAppAvalible();

    ///Socket Call Solution
    void ReInitSocket(const SoftWareApp inApp);

    bool CanEditBackGround(){return CanEditBgr;}

    QPointer<QTcpSocket> GetTcpSocket();

    ///Tabs
    QList<LeePickerScene*> GetAllTabScenes();

signals:

    void OnColorChanged(QColor &Color);

protected:
    void timerEvent(QTimerEvent *event) override;
    int timeId=-1;

    virtual void closeEvent ( QCloseEvent * event ) override;
    void SaveLocalData();
private:
    //static MainWindow* m_Instance;
    bool MayaHasConnected,BlenderHasConnected;
    bool IsPined=false;
    bool mIsConnected=false;
    bool CanEditBgr=false;
    ///Author
    QString _Pc,_Users,_Host,_AuthorName;
    bool IsAuthor();
    QString GetMacAddress();
    bool IsValidAPI(QString inUrl,QString inMacHost);
    void InitLeePicker();
    void LoadLocalData();


    Ui::leePicker *ui;

    QPointer<LeePickerView> pView=nullptr;
    QPointer<QTcpSocket> m_pTcpSocket=nullptr;

    QStringList recentfiles;

    QString currentPath;

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

    void LoadDataFile(QString &inPath);
    ///
    void AlignHorizontal(bool fromRight=false);
    void AlignVertical(bool fromBottom=false);

    ///Saving
    void SaveData(const QString inPath);
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
    ///On Save
    void OnSave();

    ///File Exploder Open
    void OnFileOpen();

    ///OnSaveAs
    void OnSaveAs();

    ///On Load Recent File
    void OnRecentFile();

    void OnLoadRecent();

    ///On Flip
    void OnFlip(bool isVertical);

    ///On Connected
    void OnSocketConnected();
    void OnSocketDisconneted();
    void OnConnectionError(QAbstractSocket::SocketError inError);
    void OnReadSocketData();

    ///Pin Selected
    void OnPinAction();

    /// Edit BackGround
    void OnEditBackGround();

    ///New Shape
    void OnCreateNewShape();
};



#endif // MAINWINDOW_H
