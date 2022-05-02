from PyQt5 import QtGui, QtCore, QtWidgets
from panta_rhei.gui.qdarkstyle import load_stylesheet

import sys
stylshFile="/home/ammar/Dokumente/panta-rhei/panta_rhei/gui/qdarkstyle/style.qss"

class Widget (QtWidgets.QWidget):
    def __init__(self):
        super(Widget, self).__init__()
        self.gridLayout = QtWidgets.QGridLayout(self)
        self.gridLayout.setVerticalSpacing(2)
        self.gridLayout.setContentsMargins(4, 5, 4, 5)
        self.setLayout(self.gridLayout)
        with open(stylshFile,"r") as fh:
            self.setStyleSheet(fh.read())

        self.setupUi()
        
    def setupUi(self):
        #-- mathematical operations
        mathopr_label = QtWidgets.QLabel("&Mathematical operation:")
        mathopr_label.setToolTip("Perform a mathematical operation on"
            "a set of members to produce new data.\n"
            "Example: log(abs($1)) + $2 + pi - 3.0")
        self.line_edit_mathopr = QtWidgets.QLineEdit()
        mathopr_label.setBuddy(self.line_edit_mathopr)
        self.line_edit_mathopr.setAlignment(QtCore.Qt.AlignRight)
        self.line_edit_mathopr.setStyleSheet("background-color: rgb(139,0,139, 45);")
        
        self.gridLayout.addWidget(mathopr_label, 1, 0)
        self.gridLayout.addWidget(self.line_edit_mathopr, 2, 0, 1, 3)

        self.line_edit_mathopr = QtWidgets.QLineEdit()
        self.line_edit_mathopr.setStyleSheet("background-color: rgb(138,43,226, 50);")
        self.gridLayout.addWidget(self.line_edit_mathopr, 3, 0, 1, 3)


        self.line_edit_mathopr = QtWidgets.QLineEdit()
        self.line_edit_mathopr.setStyleSheet("background-color: #242442;")
        self.gridLayout.addWidget(self.line_edit_mathopr, 4, 0, 1, 3)

        self.spinbox = QtWidgets.QSpinBox()
        self.spinbox.setRange(0, 10)
        self.spinbox.editingFinished.connect(self._on_edit_finished)
        self.gridLayout.addWidget(self.spinbox, 5, 0, 1, 3)
        
        # button
        mathopr_button = QtWidgets.QPushButton('\u25B6', self)
        mathopr_button.clicked.connect(self._on_button_clicked)
        mathopr_button.setStyleSheet(
            "color: greenyellow; background-color: None")
        mathopr_button.setToolTip(
            "Click to confirm action; "
            "the action _cannot_ be undone.")
        self.gridLayout.addWidget(mathopr_button, 4, 3)
        self.show()

    def _on_edit_finished(self):
        print("QSpinBox value => {}".format(self.spinbox.value()))

    def _on_button_clicked(self, checked):
        print(f"PushButton clicked => {checked}")
        self.spinbox.setValue(0)

#----------------------------------------

def main():
    
    app = QtWidgets.QApplication(sys.argv)
    wg = Widget()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
