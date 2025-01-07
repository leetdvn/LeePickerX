#ifndef PICKERSCENE_H
#define PICKERSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <leepickeritem.h>

class LeePickerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit LeePickerScene(QObject *parent = nullptr);

    LeePickerItem* CreateItem(const QString inItemName,QString inImage=QString(),int itemId = -1);

private:

signals:
};

#endif // PICKERSCENE_H
