#include <iostream>
#include <vector>
#include <memory>

#include <QApplication>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QTextEdit>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QCollator>


using std::cout;
using std::endl;

class MyTableView : public QTableView
{
    // Q_OBJECT
public:
    MyTableView(QWidget* parent = 0): QTableView(parent)
        {
            // Selection of a cell in the table select the whole line.
            setSelectionBehavior(QAbstractItemView::SelectRows);
            setSelectionMode(QAbstractItemView::ExtendedSelection);
            setSortingEnabled(true);
            sortByColumn(0, Qt::AscendingOrder);
            verticalHeader()->show();
            setFocusPolicy(Qt::StrongFocus);
        }

};

class MyClass {

    int i = 0;
public:
    MyClass() = default;
    MyClass(const int i_) {
        i = i_;
        cout << "MyClass " << i << " init" << endl;
    }

    ~MyClass() {
        cout << "MyClass " << i << " destroyed" << endl;
    }
};


// source <https://stackoverflow.com/a/18652060>
class MyDataItem: public QStandardItem
{
    // static const QRegExp rxp_rem; // regexp for characters to remove

 public:
    const double v1;
    const double v2;

    MyDataItem(const QString& text, const double x, const double y):
        QStandardItem(text), v1{x}, v2{y}
        {};

    bool operator< (const QStandardItem& other) const override
     {
         const MyDataItem* other_p = static_cast<const MyDataItem*>(&other);
         const bool isLess = (this->v1 < other_p->v1)
             || (this->v1 == other_p->v1 && this->v2 < other_p->v2);
         return isLess;
     }
 };

// const QRegExp MyDataItem::rxp_rem {"[()]"};


MyTableView* init_peakTableView(QWidget* parent = 0)
{
    MyTableView* _peak_table = new MyTableView(parent);

    QStandardItemModel* model = new QStandardItemModel(_peak_table);

    // set columns
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, "name");
    model->setHeaderData(1, Qt::Horizontal, "number");

    // Set rows
    model->setRowCount(6);
    model->setHeaderData(0, Qt::Vertical, "record 1");
    model->setHeaderData(1, Qt::Vertical, "record 2");
    model->setHeaderData(2, Qt::Vertical, "record 3");
    model->setHeaderData(3, Qt::Vertical, "record 4");
    model->setHeaderData(4, Qt::Vertical, "record 5");
    model->setHeaderData(5, Qt::Vertical, "record 6");

    // Insert data
    model->setItem(0, 0, new QStandardItem("Zhang 1"));
    model->setItem(0, 1, new MyDataItem("1.28(2)", 1.28, 2));

    model->setItem(1, 0, new QStandardItem("Yu 2 "));
    model->setItem(1, 1, new MyDataItem("2.12(1)", 2.12, 1));

    model->setItem(2, 0, new QStandardItem("Zhu 3"));
    model->setItem(2, 1, new MyDataItem("2.01(2)", 2.01, 2));

    model->setItem(3, 0, new QStandardItem("Fu 4"));
    model->setItem(3, 1, new MyDataItem("1.00(8)", 1.00, 8));

    model->setItem(4, 0, new QStandardItem("Qu 5"));
    model->setItem(4, 1, new MyDataItem("10.00(3)", 10.00, 3));

    model->setItem(5, 0, new QStandardItem("Nu 6"));
    model->setItem(5, 1, new MyDataItem("10.00(1)", 10.00, 1));

    _peak_table->setModel(model);
    _peak_table->resizeColumnsToContents();
    return _peak_table;
}

int main(int argc, char **argv)
{

    QApplication app (argc, argv);

    // TableWidget
    QTableWidget tableWidget{0};

    const std::size_t nRows = 4;

    tableWidget.setColumnCount(5);
    tableWidget.setShowGrid(true);
    tableWidget.setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget.setSelectionBehavior(QAbstractItemView::SelectRows);
    //tableWidget.setHorizontalHeaderLabels(headers);
    tableWidget.horizontalHeader()->setStretchLastSection(true);
    tableWidget.hideColumn(0);

    /* Perform filling QTableWidget records using a loop
     * */
    for(std::size_t i = 0; i < nRows; ++i){
        // Insert row
        tableWidget.insertRow(i);
        /* Set the id column in the first taking it from the result of the SQL-query.
         * This column will be hidden
         * */
        tableWidget.setItem(i,0, new QTableWidgetItem("Item"));

        // Create an element, which will serve as a checkbox
        QTableWidgetItem *item = new QTableWidgetItem();
        item->data(Qt::CheckStateRole);
        /* Check on the status of odd if an odd device,
         * exhibiting state of the checkbox in the Checked, Unchecked otherwise
         * */
        if(true){
            item->setCheckState(Qt::Checked);
        } else {
            item->setCheckState(Qt::Unchecked);
        }
        // Set the checkbox in the second column
        tableWidget.setItem(i,1, item);
        // Next, pick up all the data from a result set in other fields
        QTextEdit* textEdit = new QTextEdit("Long long text1234556677888985990504/9859599595");
        textEdit->setReadOnly(true);
        tableWidget.setItem(i,2, new QTableWidgetItem("Q2"));
        tableWidget.setItem(i,3, new QTableWidgetItem("Q3"));
        tableWidget.setCellWidget(i,4, textEdit);
    }

    tableWidget.resizeColumnsToContents();
    // tableWidget.show();

    //--- MyTableView
    QWidget widget;
    QVBoxLayout* vlay = new QVBoxLayout;
    MyTableView* peaktable = init_peakTableView(&widget);
    vlay->addWidget(peaktable);
    widget.setLayout(vlay);
    widget.show();
    //---END MyTableView

    const int res = app.exec();

    std::cout << "--END" << std::endl;
    return res;
}
