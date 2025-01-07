#ifndef LEEPICKERVIEW_H
#define LEEPICKERVIEW_H
#include <QObject>
#include <QGraphicsView>
#include <leepickerscene.h>
#include <QPointer>

class LeePickerView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LeePickerView(QWidget *parent = nullptr);

    int numberItemOfScene();

protected:

    QPointer<LeePickerScene> leeScene=nullptr;
    void InitializePolicy();

private:


signals:
};
#endif // LEEPICKERVIEW_H
