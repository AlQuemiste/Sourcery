# source <http://www.bogotobogo.com/Qt/Qt5_GridLayout.php>
from panta_rhei.gui.Qt import QtGui, QtCore, QtWidgets

#STRANGE PROBLEM WITH QT PERSISTENCE OF OBJECTS

class save_DACs_button(QtGui.QToolButton):

    def __init__(self, parent=None):
        super().__init__(parent=parent)
    
        PRESLIT = "Preslit"
        POSTSLIT = "Postslit"
        # save_dacs_button = QtGui.QToolButton()
        self.setPopupMode(QtGui.QToolButton.MenuButtonPopup)
        self.setMinimumWidth(100)

        actionSaveDACSSelection = QtGui.QAction('Save DACs')
        actionSaveDACSSelection.triggered.connect(lambda: print("save selected"))

        actionMenu = QtGui.QMenu()
        actionMergeDACS = actionMenu.addAction('Merge DACs')
        actionMergeDACS.triggered.connect(lambda: print("merge"))
        
        actionReportDACS = actionMenu.addAction('Report DACs')
        actionReportDACS.triggered.connect(lambda: print("report DACs"))
        
        # by default saves the selected DACs
        self.setDefaultAction(actionSaveDACSSelection)       
        # actionMenu.addAction(self.actionReportDACS)
        # actionMenu.addAction(self.actionMergeDACS)
        self.setMenu(actionMenu)


def mk_save_DACs_button():
    PRESLIT = "Preslit"
    POSTSLIT = "Postslit"
    save_dacs_button = QtGui.QToolButton()
    save_dacs_button.setPopupMode(QtGui.QToolButton.MenuButtonPopup)
    save_dacs_button.setMinimumWidth(100)

    actionSaveDACSSelection = QtGui.QAction('Save DACs')
    actionSaveDACSSelection.triggered.connect(lambda: print("save selected"))
    actionMergeDACS = QtGui.QAction('Merge DACs')
    actionMergeDACS.triggered.connect(lambda: print("merge"))
    actionReportDACS = QtGui.QAction('Report DACs')
    actionReportDACS.triggered.connect(lambda: print("report DACs"))
    actionSaveDACSPreslit = QtGui.QAction('Save %s' % PRESLIT)
    actionSaveDACSPreslit.triggered.connect(lambda: print("%s-tools: save" % PRESLIT))
    actionSaveDACSPostslit = QtGui.QAction('Save %s' % POSTSLIT)
    actionSaveDACSPostslit.triggered.connect(lambda: print("%s-tools: save" % POSTSLIT))
    # by default saves the selected DACs
    save_dacs_button.setDefaultAction(actionSaveDACSSelection)
    actionMenu = QtGui.QMenu()
    actionMenu.addAction(actionReportDACS)
    actionMenu.addAction(actionMergeDACS)
    actionMenu.addAction(actionSaveDACSPreslit)
    actionMenu.addAction(actionSaveDACSPostslit)
    save_dacs_button.setMenu(actionMenu)

    return save_dacs_button

app = QtGui.QApplication([""])
vlayout = QtWidgets.QVBoxLayout()

#------------------------------
# PRESLIT = "Preslit"
# POSTSLIT = "Postslit"
# save_dacs_button = QtGui.QToolButton()
# save_dacs_button.setPopupMode(QtGui.QToolButton.MenuButtonPopup)
# save_dacs_button.setMinimumWidth(100)

# actionSaveDACSSelection = QtGui.QAction('Save DACs')
# actionSaveDACSSelection.triggered.connect(lambda: print("save selected"))
# actionMergeDACS = QtGui.QAction('Merge DACs')
# actionMergeDACS.triggered.connect(lambda: print("merge"))
# actionReportDACS = QtGui.QAction('Report DACs')
# actionReportDACS.triggered.connect(lambda: print("report DACs"))
# actionSaveDACSPreslit = QtGui.QAction('Save %s' % PRESLIT)
# actionSaveDACSPreslit.triggered.connect(lambda: print("%s-tools: save" % PRESLIT))
# actionSaveDACSPostslit = QtGui.QAction('Save %s' % POSTSLIT)
# actionSaveDACSPostslit.triggered.connect(lambda: print("%s-tools: save" % POSTSLIT))
# # by default saves the selected DACs
# save_dacs_button.setDefaultAction(actionSaveDACSSelection)
# actionMenu = QtGui.QMenu()
# actionMenu.addAction(actionReportDACS)
# actionMenu.addAction(actionMergeDACS)
# actionMenu.addAction(actionSaveDACSPreslit)
# actionMenu.addAction(actionSaveDACSPostslit)
# save_dacs_button.setMenu(actionMenu)

#------------------------------

# Grid layout with 3 buttons
gridLayout = QtWidgets.QGridLayout()
b1 = QtWidgets.QPushButton("A")
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

# 4rd row with 2-column span
# save_dacs_button = mk_save_DACs_button()
save_dacs_button = save_DACs_button()
gridLayout.addWidget(save_dacs_button, 4, 0, 1, 2)
                                         
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


