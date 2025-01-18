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

    ///Get Pid ProcessId
    qint64 GetPID(){return processId;}

    ///Send Command To Server App
    void SendCommand();

    ///Send Command To Server App
    void ExecuteCmd(const QString inCmd);

    /// Get IsEroor
    bool isError(){return iError;}

    ///Get ILog
    QString GetILog(){return iLog.replace("\r\n","");}
private:
    QPointer<QProcess> iProcess=Q_NULLPTR;

    qint64 processId;

    SoftWareApp iApp;
    QString ICommand;
    bool iError=false;
    QString iLog;
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
