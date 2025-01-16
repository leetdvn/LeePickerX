#ifndef LEESENDCOMMANDS_H
#define LEESENDCOMMANDS_H

#include <QObject>
#include <Definations.h>
#include <leeGlobal.hpp>
#include <QThread>
#include <QProcess>
#include <QPointer>

class LeeSendCommand : public QObject
{
    Q_OBJECT
    QThread CommandThread;
public:
    LeeSendCommand(QObject* parent,const SoftWareApp inApp,const QString Cmds);
    ~LeeSendCommand();

    QPointer<QProcess> GetProcess(){return iProcess;}

    qint64 GetPID(){return processId;}

    void SendCommand();

    bool isError(){return iError;}
private:
    QPointer<QProcess> iProcess=Q_NULLPTR;

    qint64 processId;

    SoftWareApp iApp;
    QString ICommand;
    bool iError=false;
signals:
    void Finished();
    void ReadyOutPut(const QString& output);
    void Errored(QProcess::ProcessError error);

public slots:
    //on finish
    void OnFinished(int exitCode, QProcess::ExitStatus exitStatus = QProcess::NormalExit);
    //Error
    void OnError();
    //log
    void OnReadyReadLog();
    //Thread Finished
    void OnThreadFinished();

};

typedef LeeSendCommand Super;

#endif // LEESENDCOMMANDS_H
