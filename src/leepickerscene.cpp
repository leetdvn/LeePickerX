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
    //Factory pattern
    LeePickerItem* item = new LeePickerItem(inItemName,inImage,itemId);
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

void LeePickerScene::ClearSelectionProcess()
{
    //Clear Process
    MainWindow* LeePicker=MainWindow::Instance();
    bool isAvalible = LeePicker->IsAppAvalible();

    if(!isAvalible) {
        //LeePicker->AddToLog(Error,"Maya is Not Runing..",true);
        return;
    }

    SoftWareApp app = LeePicker->GetInteractionApp();
    QString str = app == Maya ? "maya" : "blender";

    LeePicker->AddToLog(Log,"Clear Selections",true,2);
    QString Cmds = app == Maya ?
                        "cmds.select(cl=1)" :
                        "bpy.ops.object.select_all(action='DESELECT')";

    PyExecFuncAsVoid("PickerClearSelection",app);
    // PythonProcessCmd(this,app,Cmds);

    // qDebug() << "Commnad " << Cmds << Qt::endl;
}


void LeePickerScene::OnSelectionChanged(){

    //OnSelection Changed
    QList<LeePickerItem*> Items = GetSelectedItems();
    if(Items.length() <= 0){
        return ClearSelectionProcess();
    }
}

