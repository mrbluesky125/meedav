#include "abstracttreeitem.h"
#include "abstracttreemodel.h"

AbstractTreeModel::AbstractTreeModel(AbstractTreeItem* rootItem) : QAbstractItemModel()
  ,m_rootItem(rootItem == 0 ? new AbstractTreeItem() : rootItem)
{

}

AbstractTreeModel::~AbstractTreeModel() 
{
    delete m_rootItem;
}


///\brief Returns the item at the given model index
///\param index The index of the requested item
///\return The requested item or a the root item if the index is invalid
AbstractTreeItem* AbstractTreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return m_rootItem;
}

///\brief Returns the index of the given item
///\param index The index of the requested item
///\return The requested index or an invalid index if the item is not found
QModelIndex AbstractTreeModel::getIndex(const AbstractTreeItem* item) const {
    if (item != 0 && item->parentItem() != 0) {
        unsigned int row = item->childNumber();
        return createIndex(row, 0, const_cast<AbstractTreeItem*>(item));
    }
    return QModelIndex();
}

///\brief returns an index at a specific row and column of a parent index
///\param row The row number of the item
///\param column The column number of the item (currently ignored)
///\param parent The parent index
///\remarks The column is currently ignored
///\return The requested index
QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const {
    //Is parent a valid model index?
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    //Retrieve the item pointer
    AbstractTreeItem* parentItem = getItem(parent);

    AbstractTreeItem* childItem = parentItem->child(row);
    if (childItem != 0)
        return createIndex(row, column, static_cast<void*>(childItem));
    else
        return QModelIndex();
}

///\brief Inserts a item at the given row of a given parent index
///\param row The row at wich the item is inserted
///\param item The item that is inserted
///\param parent The parent index in at which the item is inserted
///return returns true if the item was inserted, otherwise false
bool AbstractTreeModel::insertRow(int row, AbstractTreeItem* item, const QModelIndex& parent) 
{
    AbstractTreeItem* parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, row, row);
    success = parentItem->insertChild(row, item);
    endInsertRows();

    return success;
}

///\brief Inserts a list of items at the given row of a given parent index
///\param row The row at wich the items are inserted
///\param items The item list that is inserted
///\param parent The parent index in at which the items are inserted
///return returns true if the items were inserted, otherwise false
bool AbstractTreeModel::insertRows(int row, const QList<AbstractTreeItem*>& items, const QModelIndex& parent) 
{
    AbstractTreeItem* parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, row, row + items.count() - 1);
    success = parentItem->insertChildren(row, items);
    endInsertRows();

    return success;
}

///\brief Moves the item at row 'from' of item 'parent' to row 'to' of item 'parent'.
///\return True if the row was successfully moved
bool AbstractTreeModel::moveRow(int from, int to, const QModelIndex& parent)
{
    AbstractTreeItem* parentItem = getItem(parent);
    bool success;

    if(from < 0 || to < 0 || from > parentItem->childNumber() || to > parentItem->childNumber())
        return false;

    beginMoveRows(parent, from, from, parent, to);
    success = parentItem->moveChild(from, to);
    endMoveRows();

    return success;
}

///\brief Returns the parent index of a given index
///\return The parent index of the given index
QModelIndex AbstractTreeModel::parent(const QModelIndex &index) const 
{
    if (!index.isValid())
        return QModelIndex();

    AbstractTreeItem* childItem = getItem(index);
    AbstractTreeItem* parentItem = childItem->parentItem();

    if (parentItem == 0 || parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

///\brief Removes a row from a given parent index
///\param row The specific row that is removed
///\param parent The parent index from wich the row is removed
///\return true if the row could be removed, otherwise false
bool AbstractTreeModel::removeRow(int row, const QModelIndex &parent) 
{
    AbstractTreeItem* parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, row, row);
    success = parentItem->removeChildren(row, 1);
    endRemoveRows();

    return success;
}

///\brief Removes multiple row from a given parent index
///\param row The specific row from which the romoving starts
///\param rows The number of rows that are removed
///\param parent The parent index from wich the row is removed
///\return true if the rows could be removed, otherwise false
bool AbstractTreeModel::removeRows(int row, int rows, const QModelIndex &parent) 
{
    AbstractTreeItem* parentItem = getItem(parent);
    if(parentItem == 0)
        return false;

    beginRemoveRows(parent, row, row + rows - 1);
    bool success = parentItem->removeChildren(row, rows);
    endRemoveRows();

    return success;
}

///\brief Returns the total number of rows of the given parent index
///\param parent The index on which the counting is performed
///\return The row count
int AbstractTreeModel::rowCount(const QModelIndex &parent) const 
{
    AbstractTreeItem* parentItem = getItem(parent);
    if(parentItem == 0)
        return -1;

    return parentItem->childCount();
}

///\brief Standard implementation
int AbstractTreeModel::columnCount(const QModelIndex& parent) const 
{
    return m_rootItem->columnCount();
}

///\brief Standard implementation
QVariant AbstractTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());
    QHash<int, QByteArray> roleNames = this->roleNames();

    if(!roleNames.contains(role))
        return QVariant();

    return item->property(roleNames.value(role));
}

///\brief Standard implementation
bool AbstractTreeModel::setData(const QModelIndex & index, const QVariant & value, int role/*=Qt::EditRole*/)
{
    if(!index.isValid())
        return false;

    AbstractTreeItem *item = static_cast<AbstractTreeItem*>(index.internalPointer());
    QHash<int, QByteArray> roleNames = this->roleNames();

    if(!roleNames.contains(role))
        return false;
    
    return item->setProperty(roleNames.value(role), value);
}

///\brief Standard implementation
Qt::ItemFlags AbstractTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    AbstractTreeItem *item = static_cast<AbstractTreeItem*>(index.internalPointer());
    return item->flags();
}

///\brief Standard implementation
QVariant AbstractTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
