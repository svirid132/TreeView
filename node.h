#ifndef NODE_H
#define NODE_H

#include <QModelIndex>
#include <QObject>
#include <functional>

class Node : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isToggle READ isToggle WRITE setIsToggle NOTIFY isToggleChanged)
    Q_PROPERTY(QModelIndex index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QModelIndex parentIndex READ parentIndex NOTIFY parentIndexChanged)
public:
    explicit Node(QObject *parent = nullptr);

    bool isToggle() const
    {
        return m_isToggle;
    }
    QModelIndex index() const
    {
        return m_index;
    }
    void setToggleCB(std::function<void()> toggleCB) {
        m_toggleCB = toggleCB;
    }
    Q_INVOKABLE void toggle() {
        m_toggleCB();
    }

    QModelIndex parentIndex() const
    {
        return m_parentIndex;
    }

    void setParentIndex(const QModelIndex &parentIndex);

public slots:
    void setIsToggle(bool isToggle)
    {
        if (m_isToggle == isToggle)
            return;

        m_isToggle = isToggle;
        emit isToggleChanged(m_isToggle);
    }

    void setIndex(QModelIndex index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

signals:
    void isToggleChanged(bool isToggle);
    void indexChanged(QModelIndex index);

    void parentIndexChanged(QModelIndex parentIndex);

private:
    bool m_isToggle;
    QModelIndex m_index;
    std::function<void()> m_toggleCB = [](){ };
    QModelIndex m_parentIndex = QModelIndex();
};

#endif // NODE_H
