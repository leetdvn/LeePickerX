#include <leepickerview.h>
#include <QWheelEvent>

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

QList<LeePickerItem *> LeePickerView::SelectedItems()
{
    QList<LeePickerItem*> leeObj{};
    if (leeScene)
    {
        QList<QGraphicsItem*> ilist = leeScene->selectedItems();
        if (!ilist.isEmpty())
        {
            for (QGraphicsItem* it : ilist)
            {
                LeePickerItem* mObj = qgraphicsitem_cast<LeePickerItem*>(it);
                if (mObj)
                    leeObj.append(mObj);
            }
        }
    }
    return leeObj;
}

void LeePickerView::InitializePolicy()
{
    const QString bgrfile=":/icons/author.png";
    QImage image(bgrfile);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setAcceptDrops(true);
    setAttribute(Qt::WA_AcceptDrops, true);
    setMouseTracking(true);
    setUpdatesEnabled(true);
    setBackgroundBrush(image);
    setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
}


void LeePickerView::wheelEvent(QWheelEvent *e)
{
    switch (e->modifiers())
    {
        case Qt::NoModifier:{
            return Zoom(e);
        }
        default: {
            return QGraphicsView::wheelEvent(e);
        }
    }
    QGraphicsView::wheelEvent(e);
}

void LeePickerView::Zoom(QWheelEvent *e)
{
    if (isLocked)return e->ignore();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPointF target = sScenePos- cursorPos;
    qreal deltaScale = 1;
    qreal temple = transform().m11();
    deltaScale += e->angleDelta().y() > 0 ? 0.25 : -0.25;
    if (temple > 5 && deltaScale > .75) { return; }
    scale(deltaScale, deltaScale);
    translate(target.x(), target.y());

}

bool LeePickerView::HasItemUnderMouse()
{
    QGraphicsItem* item= scene()->itemAt(mapToScene(cursorPos), transform());

    bool result = item== nullptr ? false : true;

    return result == true ? item->isUnderMouse() : false;
}

void LeePickerView::mousePressEvent(QMouseEvent *e)
{
    if (HasItemUnderMouse()) return QGraphicsView::mousePressEvent(e);

    if (e->button() & Qt::LeftButton){
        //Show Rubberband Square
        iOrigin = e->globalPosition();
        if (Rubberband == nullptr)
            Rubberband = new QRubberBand(QRubberBand::Rectangle,Q_NULLPTR);
        Rubberband->show();

    }
    else if (e->button() == Qt::RightButton)
    {
        e->accept();
    }

    QGraphicsView::mousePressEvent(e);
}

void LeePickerView::mouseMoveEvent(QMouseEvent *e)
{
    cursorPos = e->pos();
    if (Rubberband != nullptr && Rubberband->isVisible())
    {
        QRect rect(iOrigin.toPoint(),e->globalPosition().toPoint());
        Rubberband->setGeometry(rect.normalized());
    }

    if(sScenePos.isNull() && !isLocked)
    {
        sScenePos = e->globalPosition().toPoint();
    }
    QGraphicsView::mouseMoveEvent(e);

}

void LeePickerView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {

        QRectF rectScene = mapToScene(Rubberband->geometry()).boundingRect();
        QList<QGraphicsItem*> inside = scene()->items(rectScene, Qt::IntersectsItemBoundingRect);
        if (inside.length() > 0) {
            for (QGraphicsItem* it : inside)
            {
                qDebug() << "Name " << it  <<  Qt::endl;
                LeePickerItem* obj = qgraphicsitem_cast<LeePickerItem*>(it);
                if (obj) {
                    obj->setSelected(!obj->isSelected());
                    // if(obj->isSelected())
                    //     obj->leftClicked(true);
                }
                //toogleItemSelection(it);
            }
        }

        Rubberband->setGeometry(QRect());
        Rubberband->hide();

    }
    QGraphicsView::mouseReleaseEvent(e);

}
