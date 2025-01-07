#include <leepickerview.h>

LeePickerView::LeePickerView(QWidget *parent)
    :QGraphicsView(parent)
    ,leeScene(new LeePickerScene)
{

    InitializePolicy();
    setScene(leeScene);

}

int LeePickerView::numberItemOfScene()
{
    QGraphicsScene* gScene = scene(); int number{};
    if (gScene == NULL) return 0;
    //lDebug("aa.");
    QList<QGraphicsItem*> itemList = gScene->items();

    return itemList.length();
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
    setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
}
