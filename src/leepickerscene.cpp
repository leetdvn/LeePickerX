#include "leepickerscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMoveEvent>
#include <QRect>
#include <qforeach.h>
#include <mainwindow.h>

LeePickerScene::LeePickerScene(QObject *parent)
    : QGraphicsScene{parent}
{
    //initialization
    QString itemName = "Demo";
    QString inImage(":/icons/color.png");
    //create item demo
    CreateItem(itemName,inImage);

    //connect selection change
    connect(this,SIGNAL(selectionChanged()),this,SLOT(OnSelectionChanged()));
}

LeePickerItem* LeePickerScene::CreateItem(const QString inItemName, QString inImage, int itemId)
{
    ///Factory pattern
    LeePickerItem* item = new LeePickerItem(inItemName,inImage,itemId,QRectF(0,0,80,80));
    item->SetItemId(items().length() + 1);
    addItem(item);
    return item;
}

QList<LeePickerItem*> LeePickerScene::GetSelectedItems()
{
    ///@Get Item Selections
    QList<LeePickerItem*> pItems = QList<LeePickerItem*>();
    QList<QGraphicsItem*> Items = selectedItems();

    for(auto it: Items)
    {
        QPointer<LeePickerItem> castIt = qgraphicsitem_cast<LeePickerItem*>(it);
        if(castIt==Q_NULLPTR) continue;
        pItems.append(castIt);
    }

    return pItems;
}

QList<LeePickerItem *> LeePickerScene::GetAllItems()
{
    //@get All Item in Scene
    QList<LeePickerItem*> pItems = QList<LeePickerItem*>();
    QList<QGraphicsItem*> Items = items();

    for(auto it: Items)
    {
        QPointer<LeePickerItem> castIt = qgraphicsitem_cast<LeePickerItem*>(it);
        if(castIt==Q_NULLPTR) continue;
        pItems.append(castIt);
    }

    return pItems;
}

QJsonArray LeePickerScene::GetDataAllObject()
{
    //get JsonObect all Item
    QList<LeePickerItem*> AllItems = GetAllItems();

    QJsonArray jsArray{};
    if(AllItems.length() <=0) return jsArray;

    for(const auto It : AllItems)
    {
        QJsonObject obj = It->toJsonObject();
        jsArray.append(obj);
    }

    return jsArray;
}

QJsonObject LeePickerScene::GetSceneData(const QString &inViewName)
{
    //Get Scene Data return Json Object
    QJsonArray jsArray = GetDataAllObject();
    MainWindow* LeePicker=MainWindow::Instance();

    QJsonObject sceneData;

    QString sceneApp = LeePicker->GetInteractionApp() == Maya ? "Maya" : "Blender";

    LEEJOBJ(sceneData,"App",sceneApp);
    LEEJOBJ(sceneData,inViewName,jsArray);

    return sceneData;
}

void LeePickerScene::LoadSceneData(QJsonObject &inItems)
{

    ///Check Item Obj
    if(inItems.isEmpty()) return;

    QJsonArray JsItems = inItems["Items"].toArray();

    ///Check Array Json
    if(JsItems.isEmpty()) return;


    ///Load Item
    foreach(const QJsonValue& obj, JsItems){
        ///Create Item and load Value
        LeePickerItem* newItem = CreateItem("newItem");
        newItem->LoadDataFromJsObject(obj.toObject());
    }

}

void LeePickerScene::SetISceneName(const QString inNewName)
{
    if(inNewName.isEmpty() || inNewName.isNull()) return ;

    if(inNewName != iSceneName)
        iSceneName = inNewName;
    emit SceneNameChanged(inNewName);
}

void LeePickerScene::AlignSelectedItems(bool isLeftToRight, bool isVertical)
{
    QList<QGraphicsItem*> Selections = selectedItems();

    if(Selections.length() <= 0) return;

    int count=0;
    bool isBigest = isLeftToRight ? true : false;
    qreal baseXY = GetBigestSmallest(!isBigest,isVertical);

    for(auto &it : Selections)
    {
        qreal valueXY = isVertical ? it->pos().x() : it->pos().y();
        if(valueXY ==baseXY) continue;
        isVertical ?
            it->setPos(QPoint(it->pos().x(),baseXY)) :
            it->setPos(QPoint(baseXY,it->pos().y()));
        it->update();
    }
    qDebug() << "base XY " << baseXY << Qt::endl;

}

qreal LeePickerScene::GetBigestSmallest(bool isBigest, bool isVertical)
{
    QList<LeePickerItem*> Selections = GetSelectedItems();

    if(Selections.length() <= 0) return 0;

    qreal result=0;
    int count=0;
    for(auto &it : Selections)
    {
        qreal posXY = isVertical ? it->PosY() : it->PosX();
        if(count ==0) result=posXY;

        if(isBigest){
            if(posXY > result) result = posXY;
        }
        else{
            if(posXY < result) result = posXY;
        }
        count++;
    }
    return result;
}

void LeePickerScene::ClearSelectionProcess()
{
    //Clear Process
    MainWindow* LeePicker=MainWindow::Instance();
    bool isAvalible = LeePicker->IsAppAvalible();

    if(!isAvalible) {
        LeePicker->AddToLog(Error,"Maya is Not Runing..",true);
        return;
    }

    SoftWareApp app = LeePicker->GetInteractionApp();

    LeePicker->AddToLog(Log,"Clear Selections",true,2);

    PyExecFuncAsVoid("PickerClearSelection",app);

    // qDebug() << "Commnad " << Cmds << Qt::endl;
}


void LeePickerScene::OnSelectionChanged(){

    //OnSelection Changed
    QList<LeePickerItem*> Items = GetSelectedItems();
    if(Items.length() <= 0){
        return ClearSelectionProcess();
    }
}

