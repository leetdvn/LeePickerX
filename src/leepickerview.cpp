#include <leepickerview.h>

LeePickerView::LeePickerView(QWidget *parent)
    :QGraphicsView(parent)
    ,leeScene(new LeePickerScene)
{
    setScene(leeScene);
    const QString bgrfile=":/icons/author.png";
    QImage image(bgrfile);
    leeScene->setBackgroundBrush(image);
}
