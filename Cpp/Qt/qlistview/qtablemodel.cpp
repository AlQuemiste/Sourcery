// from <https://www.qtcentre.org/threads/3977-QAbstractItemModel-for-dummies>

#include <QtCore/QDebug>
#include <QtCore/QStringList>

// Note: ModelIndex holds the column and row coordinates relative to the parent of any given item
// For root items, the model index is the model index of the tree/view/widget itself, which is a model index with the values 0,0.

// Data models in Qt are implemented by QAbstractItemModel subclasses. Application developers can either choose one of the ready-to-use item-based models coming with Qt (like QStringListModel or QStandardItemModel), or can develop custom model classes.

// Note that this model is not storing the data; it’s acting as an adaptor between the real data storage (represented by the Storage class) and the views.

class ModelItem
{

}

class ArticleModel: public QAbstractItemModel
{

    ArticleModel( const QStringList &data, QObject *parent ) :
        QAbstractItemModel( parent )
        {
            QList<QString> rootData;
            rootData << "From" << "Subject" << "Date" << "Message-ID"<< "Lines";
            rootItem = new ModelItem( rootData );
            for ( int i = 0; i < data.count(); i += 5 ) {
                ModelItem *item = new ModelItem( QStringList() << data[ i ] << data[ i + 1 ] << data[ i + 2 ] << data[ i + 3 ] << data[ i + 4 ], rootItem );
                rootItem->appendChild( item );
            }
        }

    ~ArticleModel()
        {
            delete rootItem;
        }

    QModelIndex index( int row, int column, const QModelIndex &parent ) const
        {
            ModelItem * parentItem;

            if ( !parent.isValid() )
                parentItem = rootItem;
            else
                parentItem = static_cast<ModelItem*>( parent.internalPointer() );

            ModelItem *childItem = parentItem->child( row );
            if ( childItem )
                return createIndex( row, column, childItem );
            else
                return QModelIndex();
        }

    QModelIndex parent( const QModelIndex &index ) const
        {
            if ( !index.isValid() )
                return QModelIndex();

            ModelItem *childItem = static_cast<ModelItem*>( index.internalPointer() );
            ModelItem *parentItem = childItem->parent();

            if ( parentItem == rootItem )
                return QModelIndex();

            return createIndex( parentItem->row(), 0, parentItem );
        }

    int rowCount( const QModelIndex &parent ) const
        {
            ModelItem * parentItem;

            if ( !parent.isValid() )
                parentItem = rootItem;
            else
                parentItem = static_cast<ModelItem*>( parent.internalPointer() );

            return parentItem->childCount();
        }

    int columnCount( const QModelIndex &parent ) const
        {
            if ( parent.isValid() )
                return static_cast<ModelItem*>( parent.internalPointer() ) ->columnCount();
            else
                return rootItem->columnCount();
        }

    QVariant data( const QModelIndex &index, int role ) const
        {
            if ( !index.isValid() )
                return QVariant();

            if ( role != Qt::DisplayRole )
                return QVariant();

            ModelItem *item = static_cast<ModelItem*>( index.internalPointer() );

            return item->data( index.column() );
        }

    Qt::ItemFlags flags( const QModelIndex &index ) const
        {
            if ( !index.isValid() )
                return Qt::ItemIsEnabled;

            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        }

    QVariant headerData( int section, Qt::Orientation orientation, int role ) const
        {
            if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
                return rootItem->data( section );

            return QVariant();
        }

};
//========================================



ArticleModel( const QStringList &data, QObject *parent ) : QAbstractItemModel( parent )
{
    QList<QString> rootData;
    rootData << "From" << "Subject" << "Date" << "Message-ID"<< "Lines";
    rootItem = new ModelItem( rootData );
    for ( int i = 0; i < data.count(); i += 5 ) {
        ModelItem *item = new ModelItem( QStringList() << data[ i ] << data[ i + 1 ] << data[ i + 2 ] << data[ i + 3 ] << data[ i + 4 ], rootItem );
        rootItem->appendChild( item );
    }
}

~ArticleModel()
{
    delete rootItem;
}

QModelIndex index( int row, int column, const QModelIndex &parent ) const
{
    ModelItem * parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelItem*>( parent.internalPointer() );

    ModelItem *childItem = parentItem->child( row );
    if ( childItem )
        return createIndex( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    ModelItem *childItem = static_cast<ModelItem*>( index.internalPointer() );
    ModelItem *parentItem = childItem->parent();

    if ( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

int rowCount( const QModelIndex &parent ) const
{
    ModelItem * parentItem;

    if ( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelItem*>( parent.internalPointer() );

    return parentItem->childCount();
}

int columnCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return static_cast<ModelItem*>( parent.internalPointer() ) ->columnCount();
    else
        return rootItem->columnCount();
}

QVariant data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( role != Qt::DisplayRole )
        return QVariant();

    ModelItem *item = static_cast<ModelItem*>( index.internalPointer() );

    return item->data( index.column() );
}

Qt::ItemFlags flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}
