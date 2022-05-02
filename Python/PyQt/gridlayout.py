# source <http://www.bogotobogo.com/Qt/Qt5_GridLayout.php>
from panta_rhei.gui.Qt import QtGui, QtCore, QtWidgets

def on_clicked():
    print("Pushbutton clicked")
    

app = QtGui.QApplication([""])
vlayout = QtWidgets.QVBoxLayout()

# Grid layout with 3 buttons
gridLayout = QtWidgets.QGridLayout()
b1 = QtWidgets.QPushButton("A")
b1.clicked.connect(on_clicked)
b2 = QtWidgets.QPushButton("B")
b3 = QtWidgets.QPushButton("C")
b4 = QtWidgets.QPushButton("D")
b5 = QtWidgets.QPushButton("E")
b6 = QtWidgets.QPushButton("F")

# addWidget(row, column, rowspan, colspan)
# 0th row
gridLayout.addWidget(b1,0,0,1,1)
gridLayout.addWidget(b2,0,1,1,1)
gridLayout.addWidget(b3,0,2,1,1)

# 1st row
gridLayout.addWidget(b4,1,0,1,1)

# 2nd row with 2-column span
gridLayout.addWidget(b5,2,0,1,2)

# 3rd row with 3-column span
gridLayout.addWidget(b6,3,0,1,3)

#----------------------------------------

title = "HLINE"

def _hline():
    h_line = QtGui.QFrame()
    h_line.setFrameShape(QtGui.QFrame.HLine)
    h_line.setFrameShadow(QtGui.QFrame.Sunken)
    return h_line

#h_layout.addWidget(QtGui.QFrame(flags=QtGui.QFrame.Sunken | QtGui.QFrame.HLine))
title = QtGui.QLabel(title)
title.setContentsMargins(0, 0, 0, 0)
title.setAlignment(QtCore.Qt.AlignCenter)

#h_layout.addWidget(QtGui.QFrame(flags=QtGui.QFrame.Sunken | QtGui.QFrame.HLine))

# h_line = QtGui.QFrame()
# h_line.setFrameShape(QtGui.QFrame.HLine)
# h_line.setFrameShadow(QtGui.QFrame.Sunken)

h_layout = QtGui.QHBoxLayout()
h_layout.setContentsMargins(0, 0, 0, 0)
h_layout.addWidget(_hline())
h_layout.addWidget(title)
h_layout.addWidget(_hline())

hrule = QtGui.QWidget()
# h_layout.addWidget(h_line)

hrule.setLayout(h_layout)


class QHLine(QtGui.QWidget):
    
    def __init__(self, title:str=None):
        super(QHLine, self).__init__()

        # make the layout
        _hLayout = QtGui.QHBoxLayout()
        _hLayout.setContentsMargins(0, 0, 0, 0)

        # add a horizontal line
        _hLayout.addWidget(self.hline())

        # add a title and a second h-line, if needed
        if isinstance(title, str):
            _hLayout.addWidget(title)
            _hLayout.addWidget(self.hline())
            
        self.setLayout(_hLayout)

    def title(self, title_txt:str)
        _title = QtGui.QLabel(title_txt)
        _title.setContentsMargins(0, 0, 0, 0)
        _title.setAlignment(QtCore.Qt.AlignCenter)
        return _title

    def hline(self):
        _hline = QtGui.QFrame()
        _hline.setFrameShape(QtGui.QFrame.HLine)
        _hline.setFrameShadow(QtGui.QFrame.Sunken)
        return _hline

#----------------------------------------

gridLayout1 = QtWidgets.QGridLayout()
b1 = QtWidgets.QPushButton("A1")
b2 = QtWidgets.QPushButton("B1")
b3 = QtWidgets.QPushButton("C1")

# addWidget(row, column, rowspan, colspan)
# 0th row
gridLayout1.addWidget(b1,0,0,1,1)
gridLayout1.addWidget(b2,0,1,1,1)
gridLayout1.addWidget(b3,0,2,1,1)
#----------------------------------------

vlayout.addLayout(gridLayout)
vlayout.addWidget(hrule)
vlayout.addLayout(gridLayout1)

# Create a widget
w = QtWidgets.QWidget()

# Set the grid layout as a main layout
w.setLayout(vlayout)


# Window title
w.setWindowTitle("Grid Layouts (3x4)")

# Display
w.show()

app.exec()
