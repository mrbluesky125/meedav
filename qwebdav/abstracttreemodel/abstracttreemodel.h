#ifndef ABSTRACT_TREE_MODEL_H
#define ABSTRACT_TREE_MODEL_H

#include <QtCore>
#include <QtDeclarative>

#include "abstracttreeitem.h"

///\brief Provides a basic tree model that handles the basic tree structure suitable for a QTreeView.
///The standard implementation of data() and setData() uses the property names that are mapped to the given roles.
///In subclasses it is recommended to define this mapping through setRoleNames() in the constructor. This enables
///also access to the properties from QML views.
///
///\remarks This class is basically intended for just 1 column, more columns
///used in QTreeView requires further considerations :-) (e.g. you can set/get
///particular properties for other columns in the data access functions)
///
///\see AbstractTreeItem
class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    AbstractTreeModel(AbstractTreeItem* rootItem = 0);
    virtual ~AbstractTreeModel();

    virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;

    virtual bool insertRow(int row, AbstractTreeItem* item, const QModelIndex& parent = QModelIndex());
    virtual bool insertRows(int row, const QList<AbstractTreeItem*>& items, const QModelIndex& parent = QModelIndex());
    virtual bool moveRow(int from, int to, const QModelIndex& parent = QModelIndex());
    virtual bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int rows, const QModelIndex& parent = QModelIndex());
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    AbstractTreeItem* getItem(const QModelIndex &index) const;
    QModelIndex getIndex(const AbstractTreeItem* item) const;

protected:
    AbstractTreeItem* m_rootItem;	///<Root item of the model

private:
    Q_DISABLE_COPY(AbstractTreeModel)

};

QML_DECLARE_TYPE(AbstractTreeModel)

#endif
