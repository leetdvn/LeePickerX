#ifndef LEEPICKERPYTHON_H
#define LEEPICKERPYTHON_H

#include <QApplication>
#include <QDir>
#include <Definations.h>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")


using namespace std;

static void PyExecString(const char* inStr){
    Py_Initialize();
    int result = PyRun_SimpleString(inStr);

    qDebug() << "result : " << result << Qt::endl;
    Py_Finalize();
}

static void PyExecFile(const char* inFilePath)
{
    FILE* fp;
    Py_Initialize();
    fp =  _Py_fopen(inFilePath, "r");
    PyRun_SimpleFile(fp, inFilePath);
    Py_Finalize();
}

template<typename T>
static T cleanup(const QString text) {
    Py_Finalize();
    T exitcode{};
    if (!text.isEmpty())
        qDebug() << text << Qt::endl;
    return exitcode;
}

static void cleanup(const QString text) {
    Py_Finalize();
    if (!text.isEmpty())
        qDebug() << text << Qt::endl;
    return;
}

static QString PyExecResultString(const char* inScriptFolder,const char* inFileName,const char* inFunc,const char* inCmd=NULL)
{
    if (!QDir(inScriptFolder).exists()){
        qDebug() << "Folder Does not exists" << Qt::endl;
        return QString();
    }

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs=nullptr, *pValue,*pResult;
    char const *str="";
    // Initilaize Python
    Py_Initialize();
    // set system path to find correct python script
    //C:/Users/leepl/Documents/GitHub/LeePickerX/Scripts/
    QString rawpath = QString("sys.path.append(\'%1\')").arg(inScriptFolder);
    const char* chdir_cmd = rawpath.toStdString().c_str();

    const char* cstr_cmd = chdir_cmd;
    // use sys to locate the script
    PyRun_SimpleString("import sys\n");
    PyRun_SimpleString(cstr_cmd);
    // import module(predication.py), it's a python script file name
    pModule = PyImport_ImportModule(inFileName);

    if(pModule !=NULL){
        pFunc = PyObject_GetAttrString(pModule, inFunc);
        if (pFunc && PyCallable_Check(pFunc)){

            if(inCmd != NULL){
                pArgs = PyTuple_New(1);
                if(pArgs==NULL)  return cleanup<QString>("NULL tuple returned");
                pValue = PyUnicode_FromString(inCmd);
                if(pValue==NULL) {
                    Py_DECREF(pValue);
                    return cleanup<QString>("PyUnicode_FromString");
                }
                pArgs = PyTuple_Pack(1,pValue);
                if(pArgs == NULL){
                    Py_DECREF(pValue);
                    Py_DECREF(pArgs);
                    return cleanup<QString>("PyTuple_Pack returned ");
                //qDebug() << PyArg_Parse(pArgs,inCmd) << Qt::endl;
                }
            }
            pResult = PyObject_CallObject(pFunc,pArgs);
            Py_DECREF(pArgs);
            Py_DECREF(pValue);
            str = PyUnicode_AsUTF8(pResult);
            cleanup<QString>("Completed..");
            qDebug() << "Func " << inFunc  << "Cmd" << inCmd<<  Qt::endl;
        }

    }
    return QString(str).replace("\n","");
}


static bool PyExecResultAsBool(const char* inScriptPath,const char* inFileName,const char* inFunc)
{
    bool result=false;
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs=nullptr, *pValue;
    // Initilaize Python
    Py_Initialize();
    // set system path to find correct python script
    //C:/Users/leepl/Documents/GitHub/LeePickerX/Scripts/
    QString rawpath = QString("sys.path.append(\'%1\')").arg(inScriptPath);
    const char* chdir_cmd = rawpath.toStdString().c_str();

    const char* cstr_cmd = chdir_cmd;
    // use sys to locate the script
    PyRun_SimpleString("import sys\n");
    PyRun_SimpleString(cstr_cmd);
    // import module(predication.py), it's a python script file name
    pName = PyUnicode_DecodeFSDefault(inFileName);
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if(pModule !=NULL){
        pFunc = PyObject_GetAttrString(pModule, inFunc);

        if (pFunc && PyCallable_Check(pFunc)){

            pValue = PyObject_CallObject(pFunc,pArgs);
            result = PyObject_IsTrue(pValue);
            if(!result){
                qDebug() << "abc : " << PyObject_IsTrue(pValue) << Qt::endl;
            }
            // if (repr != NULL) {
            //     result = PyObject_IsTrue(pValue);
            //     if(!result){
            //         qDebug() << "abc : " << PyObject_IsTrue(pValue) << Qt::endl;
            //     }
            //     Py_DECREF(pValue);
            // }
            qDebug() << "PyFunc : " << inFunc << Qt::endl;
        }

    }

    return result;
}

static void PyExecFuncAsVoid(const char* inFunc,const char* Args=NULL)
{
    if (!QDir(LEESCRIPTPATH).exists()){
        qDebug() << "Folder Does not exists" << Qt::endl;
        return;
    }

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs=NULL, *pValue,*pResult;
    char const *str="";
    // Initilaize Python
    Py_Initialize();
    // set system path to find correct python script
    //C:/Users/leepl/Documents/GitHub/LeePickerX/Scripts/
    QString rawpath = QString("sys.path.append(\'%1\')").arg(LEESCRIPTPATH);
    const char* chdir_cmd = rawpath.toStdString().c_str();

    const char* cstr_cmd = chdir_cmd;
    // use sys to locate the script
    PyRun_SimpleString("import sys\n");
    PyRun_SimpleString(cstr_cmd);
    // import module(predication.py), it's a python script file name
    pModule = PyImport_ImportModule(LEEPYTHON_ULTILS);

    if(pModule !=NULL){
        pFunc = PyObject_GetAttrString(pModule, inFunc);
        if (pFunc && PyCallable_Check(pFunc)){

            if(Args != NULL){
                pArgs = PyTuple_New(1);
                if(pArgs==NULL)  return cleanup("NULL tuple returned");
                pValue = PyUnicode_FromString(Args);
                if(pValue==NULL) {
                    Py_DECREF(pValue);
                    return cleanup("PyUnicode_FromString");
                }
                pArgs = PyTuple_Pack(1,pValue);
                if(pArgs == NULL){
                    Py_DECREF(pValue);
                    Py_DECREF(pArgs);
                    return cleanup("PyTuple_Pack returned ");
                    //qDebug() << PyArg_Parse(pArgs,inCmd) << Qt::endl;
                }
            }
            PyObject_CallObject(pFunc,pArgs);


            cleanup("Completed..");
            qDebug() << "Func " << inFunc  << "Cmd" << Args<<  Qt::endl;
        }
    }
}

#endif // LEEPICKERPYTHON_H
