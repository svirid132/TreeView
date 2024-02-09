#ifndef VEGATREEVIEW_H
#define VEGATREEVIEW_H

#include "vegatreelines.h"
#include "vegatreeobject.h"

#include <QAbstractItemModel>
#include <QQuickItem>

class VegaTreeView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QObject* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QQmlComponent* delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
public:
    VegaTreeView(QQuickItem *parent = nullptr);

signals:

    void modelChanged();
    void delegateChanged(QQmlComponent* delegate);

    void spacingChanged(int spacing);

public:
    void componentComplete();

    QObject* model() const
    {
        return m_model;
    }

    QQmlComponent* delegate() const
    {
        return m_delegate;
    }

    void init();
    int spacing() const;

public slots:
    void setModel(QObject* model)
    {
        if (m_model == model)
            return;

        m_model = qobject_cast<QAbstractItemModel*>(model);
        emit modelChanged();
    }

    void setDelegate(QQmlComponent* delegate)
    {
        if (m_delegate == delegate)
            return;

        itemData.m_delegate = delegate;
        m_delegate = delegate;
        emit delegateChanged(m_delegate);
    }

    void setSpacing(int spacing);

private:
    QQuickItem* flickableItem;
    VegaTreeLines* wrapperItem;

    // для items дерева
    QAbstractItemModel* m_model = nullptr;
    QQmlComponent* m_delegate;
    VegaTreeObject::Data rootData;
    VegaTreeObject::ItemData itemData;
    VegaTreeObject* rootObject = nullptr;
    int m_spacing = 30;
};

//Q_DECLARE_METATYPE( QAbstractItemModel* )

#endif // VEGATREEVIEW_H
