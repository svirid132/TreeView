#include "node.h"

Node::Node(QObject *parent) : QObject(parent)
{

}

void Node::setParentIndex(const QModelIndex &parentIndex)
{
    m_parentIndex = parentIndex;
}
