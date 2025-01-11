#ifndef LEEPICKERPYTHON_H
#define LEEPICKERPYTHON_H

#include <QApplication>
#include <QDir>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")


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


static QString PyExecResultString(const char* inScriptFolder,const char* inFileName,const char* inFunc)
{
    if (!QDir(inScriptFolder).exists()){
        qDebug() << "Folder Does not exists" << Qt::endl;
        return QString();
    }

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs=nullptr, *pValue;
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
            pValue = PyObject_CallObject(pFunc,pArgs);
            str = PyUnicode_AsUTF8(pValue);
            qDebug() << "info " << str << Qt::endl;
        }

    }
    return QString(str).replace("\n","");
}


static bool PyExecResultAsBool(const char* inScriptPath,const char* inFileName,const char* inFunc)
{
    bool result=false;
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
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

            pValue = PyObject_CallObject(pFunc,NULL);
            PyObject* repr = PyObject_Repr(pValue);
            if (repr != NULL) {
                result = PyObject_IsTrue(pValue);
                if(!result){
                    qDebug() << "abc : " << PyObject_IsTrue(pValue) << Qt::endl;
                }
                Py_DECREF(pValue);
                //         PyObject* repr = PyObject_Repr(pValue);
                //         char const * str = PyUnicode_AsUTF8(repr);
            }
        }

    }

    return result;
}

#endif // LEEPICKERPYTHON_H
