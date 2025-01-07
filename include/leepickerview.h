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

protected:
    LeePickerScene* leeScene=nullptr;

    void InitializePolicy();

signals:
};
#endif // LEEPICKERVIEW_H
