#include <leeSendCommands.h>

LeeSendCommand::LeeSendCommand(QObject* parent,const SoftWareApp inApp, const QString Cmds)
    :iApp(inApp)
    ,ICommand(Cmds)
{

    this->moveToThread(&CommandThread);
    CommandThread.start();
    iProcess = new QProcess(parent);
    //iProcess->setProcessChannelMode(QProcess::ForwardedOutputChannel);
    //connections
    connect(&CommandThread,&QThread::finished, this, &LeeSendCommand::OnThreadFinished);
    connect(iProcess,&QProcess::finished,this,&LeeSendCommand::OnThreadFinished);

    connect(iProcess,&QProcess::errorOccurred,this,&LeeSendCommand::OnError);

    connect(iProcess,&QProcess::readyReadStandardOutput,this,&LeeSendCommand::OnReadyReadLog);

    //QProcess::execute(QString("taskkill /f /pid %1").arg(ProcessID));

}

LeeSendCommand::~LeeSendCommand()
{
    CommandThread.quit();
    CommandThread.wait();
}

void LeeSendCommand::SendCommand()
{
    if(iProcess == Q_NULLPTR) return;
    QString workingDir(QDir::currentPath() + "/Scripts/");

    qint64 ProcessID;
    ///get path and app name from identity
    const char* AppEnv = GetAppEnv(iApp);
    const char* pyApp = GetAppExec(iApp);
    QString AppCmd = GetAppCommand(iApp);

    //init environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PYTHONPATH",AppEnv);

    iProcess->setWorkingDirectory(workingDir);

    iProcess->setProcessEnvironment(env);


    QStringList params;

    params << "-c" << AppCmd.arg(ICommand);
    iProcess->start(pyApp,params,QIODevice::ReadWrite);
    qDebug() <<"Command : " << AppCmd.arg(ICommand) <<  iProcess->readAllStandardOutput() << Qt::endl;

    iProcess->waitForFinished(-1);
    if(iProcess->error()){
        qDebug() <<"error : "  << Qt::endl;

    }

}

void LeeSendCommand::OnFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process : Finished"  << Qt::endl;
    iProcess->close();
    emit Finished();
}

void LeeSendCommand::OnError()
{
    qDebug() << "process Error : " << iProcess->errorString() << Qt::endl;

    QProcess::ProcessError error = iProcess->error();
    iError=true;
    emit Errored(error);
}

void LeeSendCommand::OnReadyReadLog()
{
    QString log = iProcess->readAllStandardOutput();

    qDebug() << "Process Ready : " << log << Qt::endl;



    emit ReadyOutPut(log);
}

void LeeSendCommand::OnThreadFinished()
{
    qDebug() << "Process : Thread Finished"  << Qt::endl;

    //deleteLater();
}
