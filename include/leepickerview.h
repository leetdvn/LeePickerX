#ifndef LEEPICKERVIEW_H
#define LEEPICKERVIEW_H
#include <QObject>
#include <QGraphicsView>
#include <leepickerscene.h>

class LeePickerView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LeePickerView(QWidget *parent = nullptr);
private:
    LeePickerScene* leeScene=nullptr;
signals:
};
#endif // LEEPICKERVIEW_H
