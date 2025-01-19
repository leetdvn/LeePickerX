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

    QList<LeePickerItem*> SelectedItems();

protected:

    QPointer<LeePickerScene> leeScene=nullptr;
    void InitializePolicy();

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseMoveEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void wheelEvent(QWheelEvent* e) override;

private:
    void Zoom(QWheelEvent *e);

    void MoveSceneRect(QMouseEvent* e);

    QPoint sScenePos,cursorPos;
    QPointF iOrigin;

    QPointer<QRubberBand> Rubberband=Q_NULLPTR;

    bool isLocked=false;
    bool isRightClicked=false;
    bool HasItemUnderMouse();
signals:
};
#endif // LEEPICKERVIEW_H
