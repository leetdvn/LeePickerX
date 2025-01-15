#include <leeSendCommands.h>

LeeSendCommand::LeeSendCommand(const SoftWareApp inApp, const QString Cmds)
    :iProcess(new QProcess)
    ,iApp(inApp)
    ,ICommand(Cmds)
{
    iProcess->moveToThread(&CommandThread);
    CommandThread.start();
    //QProcess::execute(QString("taskkill /f /pid %1").arg(ProcessID));

}

LeeSendCommand::~LeeSendCommand()
{
    CommandThread.quit();
    CommandThread.wait();
}

void LeeSendCommand::SendCommand()
{
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

    //connections
    connect(&CommandThread,&QThread::finished, this, &LeeSendCommand::OnThreadFinished);
    connect(iProcess,&QProcess::finished,this,&LeeSendCommand::OnThreadFinished);
    connect(iProcess,&QProcess::errorOccurred,this,&LeeSendCommand::OnError);
    connect(iProcess,&QProcess::readyReadStandardOutput,this,&LeeSendCommand::OnReadyReadLog);

    QStringList params;

    params << "-c" << AppCmd.arg(ICommand);
    iProcess->startDetached(pyApp,params,workingDir,&processId);


    iProcess->waitForStarted();

    qDebug() <<"Command : " << AppCmd.arg(ICommand) << Qt::endl;

}

void LeeSendCommand::OnFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process : Finished"  << Qt::endl;
    iProcess->close();
    emit Finished();
}

void LeeSendCommand::OnError(QProcess::ProcessError error)
{
    qDebug() << "process Erro : " << error << Qt::endl;

    emit Errored(error);
}

void LeeSendCommand::OnReadyReadLog()
{
    QString log = iProcess->readAllStandardOutput();

    qDebug() << "Process : " << log << Qt::endl;
    emit ReadyOutPut(log);
}

void LeeSendCommand::OnThreadFinished()
{
    qDebug() << "Process : Thread Finished"  << Qt::endl;

    //deleteLater();
}
