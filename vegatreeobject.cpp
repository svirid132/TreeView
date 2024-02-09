#include "vegatreeobject.h"

#include <QQuickItem>

VegaTreeObject::VegaTreeObject(QModelIndex index, QAbstractItemModel* model, ItemData itemData, Data data)
{
    m_data = data;
    m_index = index;
    m_model = model;
    m_itemData = itemData;

    // root
    if (!index.isValid()) {
        for (int i = 0; i < m_model->rowCount(); ++i) {
            QModelIndex l_index = m_model->index(i, 0);
            VegaTreeObject* treeObject = new VegaTreeObject(l_index, model, itemData, m_data);
            if (i == 0) {
                next_A = treeObject;
            } else {
                VegaTreeObject* treeObject_2;
                for (treeObject_2 = objectChildren[i - 1]; treeObject_2->next_A != nullptr; treeObject_2 = treeObject_2->next_A) { }
                treeObject_2->next_A = treeObject;
            }
            objectChildren.append(treeObject);
        }
        return;
    }

    // node
    m_node = new Node();
    m_node->setToggleCB([this]() {
        this->toggle();
    });
    m_node->setIndex(m_index);
    m_node->setIsToggle(m_model->hasChildren(m_index));
    m_node->setParentIndex(data.parentIndex);

    // context
    m_context = new QQmlContext(m_itemData.m_delegate->creationContext(), m_itemData.m_parentItem);
    auto roles = model->roleNames();
    QHashIterator<int, QByteArray> rolesIterator(roles);
    while (rolesIterator.hasNext()) {
        rolesIterator.next();
        QVariant data = model->data(m_index, rolesIterator.key());
        m_context->setContextProperty(rolesIterator.value(), data);
    }
    m_context->setContextProperty("node", m_node);

    // item
    item = qobject_cast<QQuickItem*>( m_itemData.m_delegate->create(m_context) );
    item->setX(m_itemData.x);
    item->setParentItem(m_itemData.m_parentItem);
}

VegaTreeObject::~VegaTreeObject()
{
    qDeleteAll(objectChildren.begin(), objectChildren.end());
    objectChildren.clear();
    item->deleteLater();
}

QQuickItem *VegaTreeObject::getItem() const
{
    return item;
}

void VegaTreeObject::setupY(int y)
{
    // root
    if (item == nullptr) {
        if (next_A) {
            next_A->setupY(y);
        }
        return;
    }

    item->setY(y);
    if (next_A) {
        next_A->setupY(y + item->implicitHeight());
    }
}

void VegaTreeObject::toggle()
{
    // children
    if (objectChildren.length() == 0) { // на открытие
        VegaTreeObject* save_next_A = next_A;
        Data _aData = m_data;
        _aData.parentIndex = m_index;
        ItemData itemData = m_itemData;
        itemData.x += m_itemData.spacing;
        for (int i = 0; i < m_model->rowCount(m_index); ++i) {
            QModelIndex l_index = m_model->index(i, 0, m_index);
            VegaTreeObject* _a = new VegaTreeObject(l_index, m_model, itemData, _aData);
            if (i == 0) {
                next_A = _a;
            } else {
                VegaTreeObject* _a2;
                for (_a2 = objectChildren[i - 1]; _a2->next_A != nullptr; _a2 = _a2->next_A) { }
                _a2->next_A = _a;
            }
            objectChildren.append(_a);
        }
        if ( !objectChildren.isEmpty() ) {
            objectChildren[ objectChildren.length() - 1 ]->next_A = save_next_A;
        }
    } else { // на закртыие
        VegaTreeObject* a;
        for (a = objectChildren[ objectChildren.length() - 1 ]; a->objectChildren.length() > 0; a = a->objectChildren[ a->objectChildren.length() - 1 ]) { }
        next_A = a->next_A;
        qDeleteAll(objectChildren.begin(), objectChildren.end());
        objectChildren.clear();
    }

    // setupY
    this->setupY( item->y() );

    m_data.afterToggle_cb();
}

int VegaTreeObject::chainMaxRightX()
{
    int maxX = 0;
    for (auto a = next_A; a != nullptr; a = a->next_A) {
        int rightX = a->item->x() + a->item->implicitWidth();
        maxX = rightX > maxX ? rightX : maxX;
    }

    return maxX;
}

int VegaTreeObject::chainHeight()
{
    int height = 0;
    for (auto a = next_A; a != nullptr; a = a->next_A) {
        height += a->item->implicitHeight();
    }

    return height;
}

int VegaTreeObject::chainItemHeight()
{
    int height = 0;
    if (item) {
        height += item->implicitHeight();
    }

    for (int i = 0; i < objectChildren.length(); ++i) {
        height += objectChildren[i]->chainItemHeight();
    }

    return height;
}

void VegaTreeObject::chainSpaceItem(std::function<void (int, int, int)> cb)
{
    int height = chainItemHeight();
    if (item) {
        height -= item->implicitHeight();
        if (height > 0) {
            cb(item->x(), item->y() + item->implicitHeight(), height);
        }
    }

    for (int i = 0; i < objectChildren.length(); ++i) {
        objectChildren[i]->chainSpaceItem(cb);
    }
}

void VegaTreeObject::chainRowsRemoved(const QModelIndex &parent, int first, int last)
{
    int count = last - first + 1;
    VegaTreeObject* parentObj = this;
    for (; parentObj && (parentObj->m_index != parent); parentObj = parentObj->next_A) { }
    if (parentObj == nullptr) {
        return;
    }
    QVector<VegaTreeObject*>* childObjs = &parentObj->objectChildren;
    VegaTreeObject* currentObj = first == 0 ? parentObj : childObjs->at(first - 1);
    for (;first != 0 && (currentObj->objectChildren.length() > 0); currentObj = currentObj->objectChildren[ currentObj->objectChildren.length() - 1 ]) { }
    VegaTreeObject* next;
    for (next = childObjs->at( first + count - 1 ); next->objectChildren.length() > 0; next = next->objectChildren[ next->objectChildren.length() - 1 ]) { }
    next = next->next_A;
    currentObj->next_A = next;
    for (int i = 0; i < count; ++i) {
        VegaTreeObject* child = childObjs->takeAt(first);
        delete child;
    }
    parentObj->update();
    this->setupY(0);
}

void VegaTreeObject::chainRowsInserted(const QModelIndex &parent, int first, int last)
{
    int count = last - first + 1;
    VegaTreeObject* parentObj = this;
    for (; parentObj && (parentObj->m_index != parent); parentObj = parentObj->next_A) { }
    if (parentObj == nullptr) {
        return;
    }
    QVector<VegaTreeObject*>* childObjs = &parentObj->objectChildren;
    if (childObjs->length() > 0) {
        for (int i = 0; i < count; ++i) {
            Data data = childObjs->first()->m_data;
            int row = first + i;
            ItemData itemData = parentObj->m_itemData;
            itemData.x += m_itemData.spacing;
            VegaTreeObject* _Anew = new VegaTreeObject( childObjs->first()->m_index, m_model, itemData, data);
            if (row == 0) {
                parentObj->next_A = _Anew;
            } else {
                childObjs->at( row - 1 )->next_A = _Anew;
            }
            _Anew->next_A = childObjs->at(row);
            childObjs->insert(row, _Anew);
        }
    }
    parentObj->update();
    this->setupY(0);
}

void VegaTreeObject::update()
{
    if (m_node) {
        m_node->setIsToggle( m_model->hasChildren(m_index) );
    }
    if (objectChildren.length() == 0) {
        return;
    }
    for (int i = 0; i < m_model->rowCount( m_index ); ++i) {
        QModelIndex index = m_model->index(i, 0, m_index);
        objectChildren.at(i)->m_index = index;
        objectChildren.at(i)->m_node->setIndex(index);
        objectChildren.at(i)->m_node->setParentIndex( m_index );
        objectChildren.at(i)->m_node->setIsToggle(m_model->hasChildren(index));
        objectChildren.at(i)->update();

        // context
        auto roles = m_model->roleNames();
        QHashIterator<int, QByteArray> rolesIterator(roles);
        while (rolesIterator.hasNext()) {
            rolesIterator.next();
            QVariant data = m_model->data(index, rolesIterator.key());
            objectChildren.at(i)->m_context->setContextProperty(rolesIterator.value(), data);
        }
    }
}

void VegaTreeObject::chainUpdateItem(VegaTreeObject::ItemData itemData)
{
    m_itemData = itemData;
    if (item) {
        item->setX(m_itemData.x);
    }
    for (int i = 0; i < objectChildren.length(); ++i) {
        ItemData itemData = m_itemData;
        itemData.x += m_itemData.spacing;
        objectChildren[i]->chainUpdateItem(itemData);
    }
}

void VegaTreeObject::chainUpdate(QModelIndex index) {
    VegaTreeObject* currentItem = this;
    for (; currentItem && (currentItem->m_index != index); currentItem = currentItem->next_A) {}
    if (currentItem) {
        // context
        auto roles = m_model->roleNames();
        QHashIterator<int, QByteArray> rolesIterator(roles);
        QVector<QQmlContext::PropertyPair> props;
        while (rolesIterator.hasNext()) {
            rolesIterator.next();
            QVariant data = m_model->data(index, rolesIterator.key());
            props.append({rolesIterator.value(), data});
        }
        currentItem->m_context->setContextProperties(props);
    }
}

VegaTreeObject::ItemData VegaTreeObject::getItemData() const
{
    return m_itemData;
}
