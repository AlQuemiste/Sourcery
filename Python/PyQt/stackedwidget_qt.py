import sys
# from PyQt5.QtCore import *
# from PyQt5.QtGui import *
# from PyQt5.QtWidgets import *
from panta_rhei.gui.Qt import QtGui, QtCore, QtWidgets


class stackedExample(QtGui.QWidget):

   def __init__(self):
      super(stackedExample, self).__init__()
      self.leftlist = QtWidgets.QListWidget ()
      self.leftlist.insertItem (0, 'Contact' )
      self.leftlist.insertItem (1, 'Personal' )
      self.leftlist.insertItem (2, 'Educational' )

      self.stack1 = QtGui.QWidget()
      self.stack2 = QtGui.QWidget()
      self.stack3 = QtGui.QWidget()

      #----------------------------------------
      layout = QtGui.QFormLayout()
      layout.addRow("Name",QtGui.QLineEdit())
      layout.addRow("Address",QtGui.QLineEdit())
      #self.setTabText(0,"Contact Details")
      self.stack1.setLayout(layout)
      #----------------------------------------
      layout = QtGui.QFormLayout()
      sex = QtGui.QHBoxLayout()
      sex.addWidget(QtGui.QRadioButton("Male"))
      sex.addWidget(QtGui.QRadioButton("Female"))
      layout.addRow(QtGui.QLabel("Sex"),sex)
      layout.addRow("Date of Birth",QtGui.QLineEdit())
      self.stack2.setLayout(layout)

      #----------------------------------------
      layout = QtGui.QHBoxLayout()
      layout.addWidget(QtGui.QLabel("subjects"))
      layout.addWidget(QtGui.QCheckBox("Physics"))
      layout.addWidget(QtGui.QCheckBox("Maths"))
      self.stack3.setLayout(layout)
      #----------------------------------------

      self.Stack = QtGui.QStackedWidget (self)
      self.Stack.addWidget (self.stack1)
      self.Stack.addWidget (self.stack2)
      self.Stack.addWidget (self.stack3)

      hbox = QtGui.QHBoxLayout(self)
      hbox.addWidget(self.leftlist)
      hbox.addWidget(self.Stack)

      self.setLayout(hbox)
      self.leftlist.currentRowChanged.connect(self.display)
      self.setGeometry(300, 50, 10,10)
      self.setWindowTitle('StackedWidget demo')
      self.show()

   def stack1UI(self):
      layout = QtGui.QFormLayout()
      layout.addRow("Name",QtGui.QLineEdit())
      layout.addRow("Address",QtGui.QLineEdit())
      #self.setTabText(0,"Contact Details")
      self.stack1.setLayout(layout)

   def stack2UI(self):
      layout = QtGui.QFormLayout()
      sex = QtGui.QHBoxLayout()
      sex.addWidget(QtGui.QRadioButton("Male"))
      sex.addWidget(QtGui.QRadioButton("Female"))
      layout.addRow(QtGui.QLabel("Sex"),sex)
      layout.addRow("Date of Birth",QtGui.QLineEdit())

      self.stack2.setLayout(layout)

   def stack3UI(self):
      layout = QtGui.QHBoxLayout()
      layout.addWidget(QtGui.QLabel("subjects"))
      layout.addWidget(QtGui.QCheckBox("Physics"))
      layout.addWidget(QtGui.QCheckBox("Maths"))
      self.stack3.setLayout(layout)

   def display(self,i):
      self.Stack.setCurrentIndex(i)

def main():
   app = QtGui.QApplication(sys.argv)
   ex = stackedExample()
   sys.exit(app.exec_())

if __name__ == '__main__':
   main() 
