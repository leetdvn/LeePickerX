#include <leepickerview.h>

LeePickerView::LeePickerView(QWidget *parent)
    :QGraphicsView(parent)
    ,leeScene(new LeePickerScene)
{

    InitializePolicy();
    setScene(leeScene);

}

void LeePickerView::InitializePolicy()
{
    const QString bgrfile=":/icons/author.png";
    QImage image(bgrfile);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setAcceptDrops(true);
    setAttribute(Qt::WA_AcceptDrops, true);
    setMouseTracking(true);
    setUpdatesEnabled(true);
    setBackgroundBrush(image);

}
