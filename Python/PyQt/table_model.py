# source: https://stackoverflow.com/q/11736560
# https://stackoverflow.com/q/17697352
# https://doc.qt.io/qt-5/modelview.html

import sys
import numpy as np
from collections import OrderedDict as odict
#from PyQt5 import QtGui, QtCore
from panta_rhei.gui.Qt import QtGui, QtCore, QtWidgets

# données à représenter
my_array = np.arange(4*4).reshape(4, 4)

# my_array = [['00','01','02'],
#             ['10','11','12'],
#             ['20','21','22']]

DA_elements = odict(
        D4x=odict(
	    amplitude= 0.01,
	    aberrations= ["X00","Y00"],
	    at_zero= False,
            mask_size= 7
        ),
	
        D4y=odict(
	    amplitude= 0.02,
	    aberrations= ["X00","Y00"],
	    at_zero= False,
	    mask_size= 7
	),

        D5x=odict(
	    amplitude= 0.01,
	    aberrations= ["X00","Y00"],
	    at_zero= True,
	    mask_size= 7
	),
    
        S5= odict(
	amplitude= 0.050,
  	aberrations= ["X00","Y00","X10","X01","Y10","Y01","X20","Y11","X02"],
	at_zero= True,
	mask_size= 7
	),

)

def main():
    app = QtGui.QApplication(sys.argv)
    to_perform = [True] * len(DA_elements)
    w = MyWindow( datain=(to_perform, DA_elements) )
    w.show()
    app.exec_()
    print("* final:")
    print(DA_elements)
    print("* to perform:")
    print(to_perform)

#----------------------------------------
# création de la vue et du conteneur
class MyWindow(QtWidgets.QWidget):
    def __init__(self, datain, *args):
        QtGui.QWidget.__init__(self, *args)

        tablemodel = MyTableModel(datain, self)
        tableview = QtGui.QTableView()
        tableview.setModel(tablemodel)

        layout = QtGui.QHBoxLayout(self)
        layout.addWidget(tableview)
        self.setLayout(layout)

# Table Model
class MyTableModel(QtCore.QAbstractTableModel):
    def __init__(self, datain, parent=None, *args):
        #QtCore.QAbstractTableModel.__init__(self, parent, *args)
        super(MyTableModel, self).__init__(parent, *args)

        # table data
        self.to_perform = datain[0]  # flags to perform a calibration (will be modified)
        self.table_data = datain[1]  # shallow copy of input data (will be modified)

        # headers / column indices and names
        self.PERFORM = 0
        self.AMPLITUDE = 1
        self.ABERRATIONS = 2
        self.AT_ZERO = 3
        self.MASK_SIZE = 4

        self.headers = odict()
        self.headers["perform"] = "perform"  # perform calibration flag
        self.headers["amplitude"] = "amplitude"
        self.headers["aberrations"] = "aberrations"
        self.headers["at_zero"] = "at HT-offset=0"
        self.headers["mask_size"] = "mask size"

        self.headers_list = list(self.headers.values())

        self.rows = list(self.table_data.keys())
        self.rows_nr = len(self.rows)
        self.columns = list(self.headers.keys())
        self.columns_nr = len(self.columns)
        
    # nr of rows
    def rowCount(self, parent):
        return self.rows_nr

    # nr of columns
    def columnCount(self, parent):
        return self.columns_nr

    # row and column headers
    def headerData(self, section, orientation, role):
        if role == QtCore.Qt.DisplayRole:
            if orientation == QtCore.Qt.Horizontal:
                return self.headers_list[section]
            elif orientation == QtCore.Qt.Vertical:
                return self.rows[section]
            #END if
        #ELSE:
        return QtCore.QVariant()

    # display table contents
    def data(self, index, role=QtCore.Qt.DisplayRole):
        r_ = index.row()
        c_ = index.column()
        
        if role == QtCore.Qt.DisplayRole:
            if c_ == self.AT_ZERO or c_ == self.PERFORM:
                return None
            else:
                return '{}'.format(self.table_data[self.rows[r_]][self.columns[c_]])

        # font
        # elif role == QtCore.Qt.FontRole:
        #     if index.row() == 0:
        #         boldFont = QtGui.QFont()
        #         boldFont.setBold(True)
        #         return boldFont
        
        # background
        # elif role == QtCore.Qt.BackgroundRole:
        #     if index.row() == 1 and index.column() == 1:
        #         redBackground = QtGui.QBrush(QtCore.Qt.red);
        #         return redBackground

        # text alignment in cells
        elif role == QtCore.Qt.TextAlignmentRole:
            return QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter

        # checkboxes
        elif role == QtCore.Qt.CheckStateRole:
            
            if c_ == self.AT_ZERO:
                check_state = self.table_data[self.rows[r_]][self.columns[c_]]
                return QtCore.Qt.Checked if check_state else QtCore.Qt.Unchecked
                        
            elif c_ == self.PERFORM:
                check_state = self.to_perform[r_]
                return QtCore.Qt.Checked if check_state else QtCore.Qt.Unchecked

            #END if
            
        #END if
        
        return QtCore.QVariant()

    # set data
    def setData(self, index, value, role):
        r_ = index.row()
        c_ = index.column()

        # editable fields
        if role == QtCore.Qt.EditRole:
            pass
            # self.table_data[index.row(), index.column()] = value

        # editable checkboxes
        if role == QtCore.Qt.CheckStateRole:
            if c_ == self.AT_ZERO:
                self.table_data[self.rows[r_]][self.columns[c_]] = \
                    not self.table_data[self.rows[r_]][self.columns[c_]]

            elif c_ == self.PERFORM:
                self.to_perform[r_] = not self.to_perform[r_]

            #END if
            
        return True

    # edit flags
    def flags(self, index):
        r_ = index.row()
        c_ = index.column()
        
        if c_ == self.AT_ZERO or c_ == self.PERFORM:
            return QtCore.Qt.ItemIsUserCheckable | QtCore.Qt.ItemIsEnabled
        
        # if (c_==0):
        #     return QtCore.Qt.ItemIsEditable | QtCore.Qt.ItemIsEnabled
        
        #ELSE:
        return QtCore.Qt.ItemIsEnabled
        
if __name__ == "__main__":
    main() 
