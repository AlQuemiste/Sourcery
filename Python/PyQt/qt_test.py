#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
import sys
from PyQt5.QtGui import *
from PyQt5 import QtWidgets

from pympler import asizeof, summary, muppy
# import sip

def mk_widget():
    # Create an PyQT4 application object.
    a = QtWidgets.QApplication(sys.argv)

    # The QWidget widget is the base class of all user interface objects in PyQt4.
    w = QtWidgets.QMainWindow()

    # Set window size. 
    w.resize(320, 240)

    # Set window title  
    w.setWindowTitle("Hello World!") 

    # Create main menu
    mainMenu = w.menuBar()
    mainMenu.setNativeMenuBar(False)
    fileMenu = mainMenu.addMenu('&File')

    # Add exit button
    exitButton = QtWidgets.QAction(QIcon('exit24.png'), 'Exit', w)
    exitButton.setShortcut('Ctrl+Q')
    exitButton.setStatusTip('Exit application')
    exitButton.triggered.connect(w.close)
    fileMenu.addAction(exitButton)

    print("w: {} bytes".format(asizeof.asizeof(w)))
    print("ExitButton: {} bytes".format(asizeof.asizeof(exitButton)))

    # Show window
    w.show() 

    a.exec_()

    sum1 = summary.summarize(muppy.get_objects())
    summary.print_(sum1)

if __name__ == "__main__":
    mk_widget()
    # print(sip.isdeleted(exitButton))
