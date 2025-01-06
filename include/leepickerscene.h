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

    virtual void CreateItem(const QString inItemName);

private:

signals:
};

#endif // PICKERSCENE_H
