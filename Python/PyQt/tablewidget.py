from copy import deepcopy
#from PyQt5 import QtCore, QtGui, QtWidgets
from panta_rhei.gui.Qt import QtGui, QtCore

#==============================================================================80

#-- editor delegates
class DoubleSpinBoxDelegate(QtGui.QStyledItemDelegate):
    ''' DoubleSpinBox used to edit floats '''

    def __init__(self, parent=None, *args):
        super(DoubleSpinBoxDelegate, self).__init__(parent=parent)

    def createEditor(self, parent, options, index):
        editor = QtGui.QDoubleSpinBox(parent)
        editor.setFrame(False)
        editor.setButtonSymbols(QtGui.QAbstractSpinBox.NoButtons)
        # editor.setSingleStep(1)
        # editor.setMinimum(0)
        # editor.setMaximum(1000)
        # editor.setSuffix(" mA")
        # editor.setAutoFillBackground(True)
        return editor

    def setEditorData(self, editor, index):
        ''' sets the editor data '''
        _data = index.model().data(index)
        value = float(_data)
        editor.setValue(value)

    def setModelData(self, editor, model, index):
        ''' reads the contents of the spin box, and writes it to the model '''
        editor.interpretText()
        value = float(editor.value())
        model.setData(index, value, QtCore.Qt.EditRole)


class SpinBoxDelegate(QtGui.QStyledItemDelegate):
    ''' SpinBox used to edit integers '''

    def __init__(self, parent=None, *args):
        super(SpinBoxDelegate, self).__init__(parent=parent)

    def createEditor(self, parent, options, index):
        editor = QtGui.QSpinBox(parent)
        editor.setFrame(False)
        return editor

    def setEditorData(self, editor, index):
        ''' sets the editor data '''

        _data = index.model().data(index)
        value = int(_data)
        editor.setValue(value)

    def setModelData(self, editor, model, index):
        ''' reads the contents of the spin box, and writes it to the model '''

        editor.interpretText()
        value = int(editor.value())
        model.setData(index, value, QtCore.Qt.EditRole)

#------------------------------------------------------------------------------80

class MyTable(QtGui.QTableWidget):

    def __init__(self, data: dict, *args):
        super(MyTable, self).__init__(*args)

        """ NOTE:
        `data` is assumed to be a dictionary of keys with _simple_ values;
        ie., of integer, float, or bool type. Everything else is basically
        treated as a string.
        """

        self.data = None
        # store default editor delegate for data (re)set
        self.defaultItemDelegate = self.itemDelegate()

        self.setData(data) # insert data into the table
        self.setupUI() # setup table UI

    def setData(self, data):
        ''' sets/resets table data '''

        print("$ set data...")
        self.data = data

        self.clear() # clear all items
        self.setItemDelegate(self.defaultItemDelegate) # reset edit delegate

        #-- set nr of rows and columns
        hHeaders = sorted(self.data.keys()) # horizontal/column headers
        self.setColumnCount(len(hHeaders))
        self.setRowCount(1) # only a single row is allowed

        """ NOTE:
        `setRowCount` or `setColumnCount` will delete the extra `QTableWidgetItems`
        automatically.

        see: <https://stackoverflow.com/a/15849800>
        """

        #-- insert data in each cell
        for col, col_val in enumerate(hHeaders):
            row = 0
            row_val = self.data[col_val]

            #--- assign editor delegate upon type
            dtype = type(row_val) # data type of the column

            print("$ row, col = {}, {} => {} of type {}".format(row, col,
                                                                row_val, dtype.__name__))

            if dtype == int or dtype == float:
                self.setItemDelegateForColumn(col, DoubleSpinBoxDelegate(parent=self))
            else:
                # set editor delegate to its default
                self.setItemDelegateForColumn(col, self.defaultItemDelegate)
            #END if

            #--- create table items
            # add checkboxes for bool type (Note: There is _no_ checkbox delegate)
            if dtype == bool:
                item = QtGui.QTableWidgetItem('')
                # set read-only: no text edit, but checkable
                item.setFlags(QtCore.Qt.ItemIsSelectable | QtCore.Qt.ItemIsEnabled
                              | QtCore.Qt.ItemIsUserCheckable)
                item.setCheckState(QtCore.Qt.Checked if row_val
                                   else QtCore.Qt.Unchecked)

            else:
                item = QtGui.QTableWidgetItem(str(row_val))
            #END if

            self.setItem(row, col, item) # add the item to the table

        #END for col

        #-- add horizontal headers
        self.setHorizontalHeaderLabels(hHeaders)

        #-- resize to contents
        self.resizeColumnsToContents()
        self.resizeRowsToContents()

        return

    def setupUI(self):
        ''' sets up the GUI '''

        self.verticalHeader().setVisible(False) # no vertical/row headers

        # editing start whenever current item changes
        # see: <https://doc.qt.io/qt-5/qabstractitemview.html#EditTrigger-enum>
        self.setEditTriggers(QtGui.QAbstractItemView.CurrentChanged)

        self.setStyleSheet("QTableView {selection-background-color: gray;}")

        # self.setSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        # self.setSelectionBehavior(QtGui.QAbstractItemView.SelectColumns)

        return

    def readData(self):
        ''' reads table data into a dict (copy) '''

        outdata = deepcopy(self.data)

        for col in range(self.columnCount()):
            row = 0

            item = self.item(row, col) # extract table item
            key = self.horizontalHeaderItem(col).text() # extract the key
            dtype = type(self.data[key]) # determine item type

            # extract item value
            value = item.checkState() if dtype == bool else item.text()

            # recast the value into the original type;
            # cast int/float to float
            if dtype == int or dtype == float: dtype = float
            try:
                outdata[key] = dtype(value)
            except ValueError:
                print("invalid value for '%s' of type %s" % (key, dtype.__name__))
        #END for col

        return outdata


#----------------------------------------

def test():

    # initial data
    data = {'col1': 1,
            'col2': 2.1,
            'col2a': True,
            'col2b': False,
            'col3': "Text"}

    app = QtGui.QApplication([""])
    table = MyTable(data)

    #reset data
    data = {'A': 2.9,
            'B': 22,
            'C': False,
            'E': 'Text2'}

    table.setData(data)

    table.show()
    app.exec_()

    print("* data: ", data)
    print("* table data:", table.readData())

if __name__=="__main__":
    test()
