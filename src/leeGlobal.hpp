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


static QString BrowserImage()
{
    QString filter = "All files (*.*);;JPEG (*.jpg *.jpeg);;TIFF (*.tif);;PNG (*.png)";
    QString ImgPath = QFileDialog::getOpenFileName(nullptr, " image choise ", OLDFOLDER,filter);
    getCurrentFolder(ImgPath);
    return ImgPath;
}

static void JsonExport(QFile &file, QByteArray data, bool Hex=0)
{
    if (!file.exists() && data.isEmpty() || !file.exists() && data.isNull()) { return; }

    QByteArray copyData=*&data;
    if (file.open(QIODevice::WriteOnly))
    {
        if(Hex) copyData = copyData.toHex();
        file.write(copyData);
        file.close();
    }

}

#endif
