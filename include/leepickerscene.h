#ifndef PICKERSCENE_H
#define PICKERSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <leepickeritem.h>
#include <QRubberBand>
#include <LeePickerPython.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class LeePickerScene : public QGraphicsScene
{
    Q_OBJECT

    Q_PROPERTY(QString iSceneName READ GetISceneName WRITE SetISceneName NOTIFY SceneNameChanged FINAL)
    Q_PROPERTY(bool isLocked READ GetISceneLocked WRITE SetISceneLocked NOTIFY SceneLockChanged FINAL)
public:
    explicit LeePickerScene(QObject *parent = nullptr);

    LeePickerItem* CreateItem(const QString inItemName,QString inImage=QString(),int itemId = -1);

    QList<LeePickerItem*> GetSelectedItems();

    QList<LeePickerItem*> GetAllItems();

    QJsonArray GetDataAllObject();

    QJsonObject GetSceneData(const QString &inViewName);

    void LoadSceneData(QJsonObject &inItems);

    QString GetISceneName(){return iSceneName;}
    void SetISceneName(const QString inNewName);

    bool GetISceneLocked(){return isLocked;}
    void SetISceneLocked(const bool inLock){
        isLocked = inLock;
        emit SceneLockChanged(inLock);
    }

signals:
    void SceneNameChanged(QString newName);
    void SceneLockChanged(bool isLock);
protected:

    void ClearSelectionProcess();
    QString iSceneName;
    bool isLocked;
private slots:

    void OnSelectionChanged();

};

#endif // PICKERSCENE_H
