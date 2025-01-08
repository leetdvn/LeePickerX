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

enum LogType{
    Log,
    Warning,
    Error,
    Completed
};


static QString OLDFOLDER = "C:/Users/" + LEEUSER + "/Documents/";


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


static QString BrowserImage()
{
    QString filter = "All files (*.*);;JPEG (*.jpg *.jpeg);;TIFF (*.tif);;PNG (*.png)";
    QString ImgPath = QFileDialog::getOpenFileName(nullptr, " image choise ", OLDFOLDER,filter);
    getCurrentFolder(ImgPath);
    return ImgPath;
}


// void AddToLog(QTextEdit*& LogObj , const LogType inLog, QString inMessage, bool isClear=0)
// {
//     if(inMessage =="") return;
//     QStringList splitMes =  inMessage.split(" : ");
//     QString title = splitMes.count() >=0 ? splitMes[0] : inMessage;

//     QString mess = splitMes.count() >=2  ? splitMes[1] : splitMes[0];
//     QString result;
//     switch (inLog) {
//     case Log:{
//         result = QString("<font color=\"white\">%1 : </font>").arg(mess);
//         break;
//     }
//     case Warning:{
//         result = QString("<font color=\"yellow\">%1 : </font>").arg(title);
//         result += mess;
//         break;
//     }
//     case Error:{
//         result = QString("<font color=\"red\">%1 : </font>").arg(title);
//         result += mess;
//         break;
//     }
//     case Completed:{
//         result = QString("<font color=\"green\">%1 : </font>").arg(title);
//         result += mess;
//         break;
//     }
//     };

//     QString CLog = !isClear ? LogObj->toHtml() : "";
//     CLog += result;
//     LogObj->setHtml(CLog);

//     //new Logs
//     //set value max down to new Log
//     int valueMax = LogObj->verticalScrollBar()->maximum();
//     LogObj->verticalScrollBar()->setValue(valueMax);

// }

#endif
