#ifndef VegaTreeObject_H
#define VegaTreeObject_H

#include "node.h"

#include <QAbstractItemModel>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <functional>

class VegaTreeObject
{
public:
    // Data
    struct Data {
        Data() {  }
        QModelIndex parentIndex = QModelIndex();
        std::function<void()> afterToggle_cb;
    };
    // ItemData
    struct ItemData {
        QQmlComponent* m_delegate;
        QQuickItem* m_parentItem;
        QQmlEngine* m_engine;
        int x = 0;
        int spacing = 30;
    };

    VegaTreeObject(QModelIndex index, QAbstractItemModel* model, ItemData itemData, Data data = {});
    ~VegaTreeObject();

    QQuickItem *getItem() const;

    // настройка элемента
    void setupY(int y = 0);
    void toggle();
    void update(); // обнволяет контекст
    void chainUpdateItem(ItemData itemData);

    // chain - зависит от других items
    int chainMaxRightX();
    int chainHeight(); // работает только для root
    int chainItemHeight();
    void chainSpaceItem(std::function<void(int x, int y, int height)> cb);
    void chainRowsRemoved(const QModelIndex &parent, int first, int last);
    void chainRowsInserted(const QModelIndex &parent, int first, int last);
    void chainUpdate(QModelIndex index);

    ItemData getItemData() const;

private:
    QModelIndex m_index;
    QVector<VegaTreeObject*> objectChildren;
    QAbstractItemModel* m_model;
    QQuickItem* item = nullptr;
    Data m_data;
    ItemData m_itemData;
    QQmlContext* m_context;
    VegaTreeObject* next_A = nullptr;
    Node* m_node = nullptr;
};

#endif // VegaTreeObject_H
