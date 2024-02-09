#include "vegatreeview.h"

#include <QtQml>

VegaTreeView::VegaTreeView(QQuickItem *parent) : QQuickItem(parent)
{
}

void VegaTreeView::componentComplete()
{
    init();

    // model
    connect(m_model, &QAbstractItemModel::rowsRemoved, [this](const QModelIndex &parent, int first, int last) {
        rootObject->chainRowsRemoved(parent, first, last);
        rootData.afterToggle_cb();
    });
    connect(m_model, &QAbstractItemModel::rowsInserted, [this](const QModelIndex &parent, int first, int last) {
        rootObject->chainRowsInserted(parent, first, last);
        rootData.afterToggle_cb();
    });
    connect(m_model, &QAbstractItemModel::dataChanged, [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        Q_UNUSED(bottomRight)
        Q_UNUSED(roles)

        rootObject->chainUpdate(topLeft);
        QTimer::singleShot(10, this, [this]() { rootData.afterToggle_cb(); }); // из-за rowLayout, который подстраивает width несколько раз
    });
    connect(m_model, &QAbstractItemModel::modelReset, [this]() {
        delete rootObject;
        delete flickableItem;
        delete wrapperItem;
        init();
    });

    QQuickItem::componentComplete();
}

void VegaTreeView::init()
{
    QQmlEngine *engine = qmlEngine(this);

    // ScrollArea
    QQmlComponent flickable(engine, this);
    flickable.setData("import QtQuick 2.15\nimport QtQuick.Controls 2.15\nFlickable { clip: true; width: 200; height: 200; contentWidth: contentItem.childrenRect.width; contentHeight: contentItem.childrenRect.height;\nScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }\n ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AlwaysOn } }", QUrl());
    flickableItem = qobject_cast<QQuickItem*>(flickable.create(engine->rootContext()));
    flickableItem->setParentItem(this);
    flickableItem->setWidth(this->width());
    flickableItem->setHeight(this->height());
    connect(this, &VegaTreeView::widthChanged, [this]() { flickableItem->setWidth(this->width()); });
    connect(this, &VegaTreeView::heightChanged, [this]() { flickableItem->setHeight(this->height()); });

    // root item
    wrapperItem = new VegaTreeLines();
    QQuickItem* scrollAreaContent = flickableItem->property("contentItem").value<QQuickItem*>();
    wrapperItem->setParentItem(scrollAreaContent);

    // items
    itemData.m_parentItem = wrapperItem;
    itemData.m_engine = engine;
    itemData.spacing = m_spacing;
    rootData = VegaTreeObject::Data();
    rootData.afterToggle_cb = [this]() { // обновляет размеры
        int maxRightX = rootObject->chainMaxRightX();
        int height = rootObject->chainHeight();
        wrapperItem->setProperty("width", maxRightX);
        wrapperItem->setProperty("height", height);
        QList<VegaTreeLines::Line> lines;
        rootObject->chainSpaceItem([this, &lines](int x, int y, int height) {
            VegaTreeLines::Line line;
            line.x = x + m_spacing / 2; line.y = y;
            line.height = height;
            lines.append(line);
        });
        wrapperItem->setLines(lines);
    };
    rootObject = new VegaTreeObject(QModelIndex(), m_model, itemData, rootData);
    rootObject->setupY(0);
    rootData.afterToggle_cb();
}

int VegaTreeView::spacing() const
{
    return m_spacing;
}

void VegaTreeView::setSpacing(int spacing)
{
    if (m_spacing == spacing)
        return;

    m_spacing = spacing;
    if (rootObject) {
        VegaTreeObject::ItemData itemData = rootObject->getItemData();
        itemData.spacing = m_spacing;
        rootObject->chainUpdateItem(itemData);
    }
    emit spacingChanged(m_spacing);
}
