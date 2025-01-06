#ifndef PICKERSCENE_H
#define PICKERSCENE_H

#include <QObject>
#include <QGraphicsScene>

class PickerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PickerScene(QObject *parent = nullptr);
signals:
};

#endif // PICKERSCENE_H
