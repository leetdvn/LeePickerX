#include "leepickerscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMoveEvent>
#include <QRect>
#include <qforeach.h>
#include <mainwindow.h>


LeePickerScene::LeePickerScene(QObject *parent)
    : QGraphicsScene{parent}
{
    QString itemName = "Demo";
    QString inImage(":/icons/color.png");
    //create item demo
    CreateItem(itemName,inImage);

    //connect selection change
    connect(this,SIGNAL(selectionChanged()),this,SLOT(OnSelectionChanged()));
}

LeePickerItem* LeePickerScene::CreateItem(const QString inItemName, QString inImage, int itemId)
{
    LeePickerItem* item = new LeePickerItem(inItemName,inImage,itemId);
    item->SetItemId(items().length() + 1);
    addItem(item);
    return item;
}

QList<LeePickerItem*> LeePickerScene::GetSelectedItems()
{
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
    MainWindow* LeePicker=MainWindow::Instance();
    QList<LeePickerItem*> Items = GetSelectedItems();

    SoftWareApp app = LeePicker->GetInteractionApp();
    QString str = app == Maya ? "maya" : "blender";
    if(Items.length() <= 0 && LeePicker->IsAppAvalible()){

        QPointer<QTcpSocket> socket = LeePicker->GetTcpSocket();

        if(!socket->isValid()) return;

        if(QTcpSocket::ConnectingState == socket->state()){
            socket->waitForConnected(-1);
        }

        if(QTcpSocket::ConnectedState == socket->state()){

            QString workingDir = QString("import sys\nsys.path.append('%1')\n%2").arg(LEESCRIPTPATH,BLENDERCMDS.arg("PickerClearSelection()"));
            QTextStream T(socket);
            T << "import bpy\nbpy.ops.object.select_all(action='DESELECT')" ;


            // T << QString("sys.append('%1')").arg(LEESCRIPTPATH);
            // T << BLENDERCMDS.arg("PickerClearSelection()");
            socket->flush();
            qDebug() << "send" << socket->state() <<  socket->peerPort() <<  Qt::endl;
        }
        else{
            qDebug() << "Not Connected" << socket->state() << socket->peerPort() <<  Qt::endl;
        }
    }
}


void LeePickerScene::OnSelectionChanged()
{

    ClearSelectionProcess();
}

