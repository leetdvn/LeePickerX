#ifndef LEEPICKERPYTHON_H
#define LEEPICKERPYTHON_H

#include <QApplication>
#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")


static PyObject* pModule = nullptr;
static PyObject* pFunc = nullptr;
static PyObject* pResult = nullptr;
static PyObject* pArg=nullptr;

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

static void closePy(){
    // Decrement the reference count for python object, prevent memory leak

    // PyObject must NOT be NULL. you may add if check.
    Py_DECREF(pModule);
    //Py_DECREF(pParam);
    Py_DECREF(pFunc);
    Py_DECREF(pResult);

    Py_Finalize();
}
static QString PyExecResultString(const char* inScriptPath,const char* inFileName,const char* inFunc)
{
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
    pModule = PyImport_ImportModule(inFileName);

    pFunc = PyObject_GetAttrString(pModule, inFunc);

    //pArg=Py_BuildValue("(s)","");
    //pResult= PyObject_CallFunction(pFunc,"send_command");
    pResult = PyObject_CallObject(pFunc,NULL);

    PyObject* repr = PyObject_Repr(pResult);
    char const * str = PyUnicode_AsUTF8(repr);

    QString data(str);
    //data.replace("b\"","");
    data.replace("\\n\\x00","");
    data.replace("\"","");
    data.replace("b'","");
    data.replace("'","");

    closePy();

    return data;
}




#endif // LEEPICKERPYTHON_H
