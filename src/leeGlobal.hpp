#ifndef LEEGLOBAL_H
#define LEEGLOBAL_H


#include <QWidgetAction>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <Definations.h>
#include <QTextEdit>
#include <QScrollBar>
#include <QProcess>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <windows.h>
#include <QMetaEnum>
#include <string>
#include <QChar>
#include <QTcpSocket>


enum LogType{
    Log,
    Warning,
    Error,
    Completed
};

enum SoftWareApp{
    NONE,
    Maya,
    Blender,
};

static QString OLDFOLDER = "./icons/";

static bool isDir(const QString dir) { return QDir(dir).exists(); }

static QString getCurrentFolder(QString path) {

    if(!QDir(path).exists()) return QString();
    if (!isDir(path)) {
        QString folder(path);
        QStringList split = folder.split('/');
        folder.remove(split[split.length() - 1]);
        OLDFOLDER = folder;
        return OLDFOLDER;
    }
    return OLDFOLDER;
}

template<class T>
QWidgetAction* TemplateAction(void*& action, QString label = NULL, QString name = NULL)
{
    QWidget* w = new QWidget();
    w->setAttribute(Qt::WA_DeleteOnClose, true);
    QHBoxLayout* layout = new QHBoxLayout(w);
    layout->setContentsMargins(5, 5, 5, 5);
    QWidgetAction* _nAct = new QWidgetAction(w);
    action = new T(w);
    if (!label.isEmpty()) {
        QLabel* lbel = (QLabel*&)action;
        lbel->setText(label);
    }
    w->setObjectName(name);
    layout->setAlignment(Qt::AlignCenter);
    w->setLayout(layout);
    _nAct->setDefaultWidget(w);
    layout->addWidget((T*)action);
    return _nAct;
}

//Instance Browser Image
static QString BrowserImage()
{
    QString filter = "All files (*.*);;JPEG (*.jpg *.jpeg);;TIFF (*.tif);;PNG (*.png)";
    QString ImgPath = QFileDialog::getOpenFileName(nullptr, " image choise ", OLDFOLDER,filter);
    getCurrentFolder(ImgPath);
    return ImgPath;
}

static void MakeBinaryData(QFile &file){
    if (!file.exists()) { return; }


    if (file.open(QIODevice::ReadWrite ))
    {
        QByteArray copyData = file.readAll();

        QDataStream in(&file);
        in << copyData;

        file.close();
    }
}

static QByteArray ReadBinaryData(QFile &file){

    if (!file.exists()) { return QByteArray(); }

    QByteArray copyData{};
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in >> copyData;
        file.close();
        return copyData;
    }
    return copyData;
}

//Json Export File
static void JsonExport(QFile &file, QByteArray data, bool Hex=0)
{
    if (!file.exists() && data.isEmpty() || !file.exists() && data.isNull()) { return; }

    QByteArray copyData=*&data;
    if (!file.open(QIODevice::WriteOnly)) return;

    if(Hex) copyData = copyData.toHex();
    file.write(copyData);
    // QDataStream in(&file);
    // in << copyData;
    file.close();

}



//Json Import File
static QByteArray JsonImport(QFile &file, bool Hex=0)
{
    if (!file.exists()) { QByteArray(); };

    QByteArray data{};
    if (!file.open(QIODevice::ReadOnly)) return QByteArray();

    // QDataStream in(&file);
    // in >> data;
    data = file.readAll();
    file.close();

    QString copyData{};
    if(Hex){
        copyData= data;
        copyData = QByteArray::fromHex(copyData.toUtf8());

        // data = QByteArray::fromRawData(data,sizeof(data));
        // QJsonDocument doc = QJsonDocument::fromJson(copyData.toUtf8());
        // QJsonObject rootObj = doc.object();
        // QJsonValue vStr = rootObj.value("Main");

        // for(const auto it : vStr.toArray())
        // {
        //     QJsonObject obj = it.toObject();
        //     QJsonValue color = obj.value("Color");
        //     qDebug() << "data : " << color.toString() <<Qt::endl;

        // }
        qDebug() << "data : " << copyData.toUtf8() <<Qt::endl;

    }

    //QString cData= QString::fromUtf16((char16_t*)hexEncode.data());
    return copyData.toUtf8();
}


//check Maya Running on Task process
static bool isRunning(const QString &process) {
    QProcess tasklist;
    tasklist.start(
        "tasklist",
        QStringList() << "/NH"
                      << "/FO" << "CSV"
                      << "/FI" << QString("IMAGENAME eq %1").arg(process));
    tasklist.waitForFinished();
    QString output = tasklist.readAllStandardOutput();
    return output.startsWith(QString("\"%1").arg(process));
}


template<typename QEnum>
static QString QtEnumToString(const QEnum value)
{
    return std::string(QMetaEnum::fromType<QEnum>().valueToKey(value)).c_str();
}

// static int QtStringToEnum(QString inEnumStr){
//     const char* str = inEnumStr.toUtf8();
//     return QMetaType::type(str);
// }

static void SaveAssignObject(QObject* inObj,SoftWareApp inApp,const QString inValue)
{
    if(inObj==nullptr) return;
    //save value to assign
    inObj->setProperty("select",inValue);
    inObj->setProperty("App",inApp);

}


//file dialog
static QString fileDialog(QWidget* main)
{
    QString filter = "LeePicker (*.Leetdvn)";
    return QFileDialog::getOpenFileName(main, ("Lee Picker file Open Window"), OLDFOLDER, filter, &filter);
}

static QString GetAppCommand(const SoftWareApp inApp){
    switch (inApp) {
        LEECASE(Maya,MAYACMDS)
        LEECASE(Blender,BLENDERCMDS)
        LEECASE(NONE,QString())
    }
    return NULL;
}

static const char* GetAppEnv(const SoftWareApp inApp){
    switch (inApp) {
        LEECASE(Maya,LEEMAYAENV)
        LEECASE(Blender,LEEBLENDERENV)
        LEECASE(NONE,NULL)
    }
    return NULL;
}

static const char* GetAppExec(const SoftWareApp inApp){
    switch (inApp) {
        LEECASE(Maya,"mayapy")
        LEECASE(Blender,"python")
        LEECASE(NONE,NULL)
    }
    return NULL;
}

static bool isRunning(const SoftWareApp inApp){
    switch (inApp) {
        LEECASE(Maya,isRunning("maya.exe"))
        LEECASE(Blender,isRunning("blender.exe"))
        LEECASE(NONE,false)
    }
    return false;
}

///python command process app
static void PythonProcessCmd(QObject* obj, const SoftWareApp inApp,const QString inCmd)
{
    QProcess* process = new QProcess(obj);
    QString workingDir(QDir::currentPath() + "/Scripts/");
    qDebug() << "path" << workingDir << Qt::endl;

    ///get path and app name from identity
    const char* AppEnv = GetAppEnv(inApp);
    const char* pyApp = GetAppExec(inApp);
    QString AppCmd = GetAppCommand(inApp);

    //init environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONPATH",AppEnv);

    process->setWorkingDirectory(workingDir);

    process->setProcessEnvironment(env);

    QStringList params;

    params << "-c" << AppCmd.arg(inCmd);
    process->start(pyApp,params);

    // process->start(
    //     pyApp,
    //     QStringList() << "-c"
    //                   << LEECMDS.arg(inCmd));

    //process->start("python",params);
    process->waitForFinished(5);
    qDebug() << "App " << AppEnv << "app2 " << pyApp <<Qt::endl;

    qDebug() << process->readAllStandardOutput() << Qt::endl;
    qDebug() << process->readAllStandardError() << Qt::endl;
    qDebug() << AppCmd.arg(inCmd) << Qt::endl;

    process->close();
}
#endif
