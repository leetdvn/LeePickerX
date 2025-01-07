#ifndef LEEGLOBAL_H
#define LEEGLOBAL_H


#include <QWidgetAction>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>

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


#endif
