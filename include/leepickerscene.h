#ifndef PICKERSCENE_H
#define PICKERSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <leepickeritem.h>
#include <QRubberBand>
#include <LeePickerPython.h>

class LeePickerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit LeePickerScene(QObject *parent = nullptr);

    LeePickerItem* CreateItem(const QString inItemName,QString inImage=QString(),int itemId = -1);

    QList<LeePickerItem*> GetSelectedItems();

    QList<LeePickerItem*> GetAllItems();


protected:


private slots:

    void OnSelectionChanged();

};

#endif // PICKERSCENE_H
