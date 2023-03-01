#include <QApplication>
#include <QSplitter>
#include <QListView>
#include <QTableView>
#include <QStandardItemModel>

#include <stdexcept>
#include <iostream>

typedef int MyType;
typedef int* MyTypePtr;

Q_DECLARE_METATYPE(MyTypePtr);

class Storage {
public:
    static constexpr int n_rows = 5;
    static constexpr int n_cols = 1;

    Storage() {
        for (int i = 0; i < n_rows; ++i) {
            m_data[i] = new int;
            *m_data[i] = i;
        }
    }

    ~Storage()
        {
            for (int i = 0; i < n_rows; ++i)
                delete m_data[i];
        }

    int rowCount() const {
        return n_rows;
    }

    int columnCount() const {
        return n_cols;
    }

    int* getData(const int row, const int col) const {
        if (row < 0 || row >= n_rows || col < 0 || col >= n_cols)
            return nullptr;

        return m_data[row];
    }

private:
    int* m_data[n_rows];
};

class MyList {
public:
    static constexpr int n_rows = 5;

    MyList() {
        for (int i = 0; i < n_rows; ++i) {
            m_data[i] = new int;
            *m_data[i] = i;
        }
    }

    ~MyList()
        {
            for (int i = 0; i < n_rows; ++i)
                delete m_data[i];
        }

    int rowCount() const {
        return n_rows;
    }

    int* getData(const int row) const {
        if (row < 0 || row >= n_rows)
            return nullptr;

        return m_data[row];
    }

private:
    int* m_data[n_rows];
};



// Simple models can be created by subclassing this class and implementing the minimum number of required functions. For example, we could implement a simple read-only QStringList-based model that provides a list of strings to a QListView widget. In such a case, we only need to implement the rowCount() function to return the number of items in the list, and the data() function to retrieve items from the list.


class MyListModel : public QAbstractListModel
{
public:
    explicit MyListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
    }
    // Basic QAbstractTableModel API
    int rowCount(const QModelIndex& parent) const override
    {
        return m_data.rowCount();
    }
    QVariant data(const QModelIndex& index, int role) const override
    {
        if (role != Qt::DisplayRole)
            return {};
        int* intPtr = m_data.getData(index.row(), index.column());
        return *intPtr;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) override
    {
        throw std::runtime_error("bad call to setData");
    }

private:
    Storage m_data;
};


class MyTableModel : public QAbstractTableModel
{
public:
    explicit MyTableModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent)
    {
    }
    // Basic QAbstractTableModel API
    int rowCount(const QModelIndex& parent) const override
    {
        return m_data.rowCount();
    }
    int columnCount(const QModelIndex& parent) const override
    {
        return m_data.columnCount();
    }
    QVariant data(const QModelIndex& index, int role) const override
    {
        if (role != Qt::DisplayRole)
            return {};
        int* intPtr = m_data.getData(index.row(), index.column());
        return *intPtr;
    }
private:
    Storage m_data;
};


class MyStandardItem: public QStandardItem
{
public:
    MyStandardItem(const int i):
    QStandardItem()
    {
        m_item = new MyType;
        *m_item = i;
        std::cout << "MyStandardItem<" << this << ">: item = " << i << std::endl;
    }

    ~MyStandardItem() {
        if (m_item) {
            std::cout << "~MyStandardItem<" << this << ">: item = " << *m_item << std::endl;
            delete m_item;
        }
    }

    void setData(const QVariant &var, int role = Qt::UserRole + 1) override {
        QStandardItem::setData(var);
        m_item = var.value<MyTypePtr>();
    }

    void setData(MyTypePtr itm) = delete;
    //     throw std::runtime_error("bad call to setData");
    //     QVariant var;
    //     var.setValue(itm);
    //     QStandardItem::setData(var);
    //     m_item = itm;
    // }

    QVariant data(int role = Qt::UserRole + 1) const override {
        std::cout << "MyStandardItem<" << this << ">.data - role(" << role << ")";
        // if ( role != Qt::DisplayRole )
        //     return {};

        if (m_item) {
            std::cout << "  => " << QString::number(*m_item).toStdString() << std::endl;
            return {QString::number(*m_item)};
        }

        std::cout << "  => ''" << std::endl;
        return {QString()};
    }

private:
    MyTypePtr m_item;

};

//================================

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    QListView *list = new QListView();
    QTableView *table = new QTableView();

    QSplitter splitter;
    splitter.addWidget( list );
    splitter.addWidget( table );

    // build model
    const int rows = 5, cols = 1;
    QStandardItemModel model0( rows, cols );

    for(int c=0, r=0; r<rows; r++ ) {
        MyStandardItem *item = new MyStandardItem(r+c);
        item->setEditable( false );
        model0.setItem(r, 0, item);
    }

    model0.setHorizontalHeaderItem( 0, new QStandardItem( "FooTable" ) );
    //------------------------------

    MyListModel model;

    list->setModel( &model );
    table->setModel( &model );
    table->setSelectionModel( list->selectionModel() );

    table->setSelectionBehavior( QAbstractItemView::SelectRows );
    table->setSelectionMode( QAbstractItemView::SingleSelection );

    splitter.show();

    return app.exec();
}


/*
 # **Qt project file

 TEMPLATE = app
 TARGET = qListView
 INCLUDEPATH += .

 QT += core widgets gui

 DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

 # Input
 SOURCES += listview_ex.cpp

 */
