#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include <QStyleFactory>


void StyleFusion(const QString &inStyle)
{
    QApplication::setStyle(QStyleFactory::create(inStyle));
    QPalette p;
    p = qApp->palette();

    //Base Color
    p.setColor(QPalette::Base,QColor(53,53,53));
    //Window Color
    p.setColor(QPalette::Window,QColor(53,53,53));
    //Button Color
    p.setColor(QPalette::Button, QColor(53,53,53));
    //Button Text Color
    p.setColor(QPalette::ButtonText, Qt::white);
    //Tooltip Text Color
    p.setColor(QPalette::ToolTipText, Qt::white);
    //Window Text Color
    p.setColor(QPalette::WindowText, Qt::white);
    //Highlight Color
    p.setColor(QPalette::Highlight, QColor(142,45,197));
    qApp->setPalette(p);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StyleFusion("Fusion");
    MainWindow w;
    w.show();
    return a.exec();
}
