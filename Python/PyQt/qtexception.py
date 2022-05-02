# https://github.com/pytest-dev/pytest-qt/issues/25#issuecomment-61141918
from PyQt5 import QtGui, QtCore, QtWidgets
import sys
import traceback
#------------------------------------------------------------------------------80

def something(QtApp, exctype, excvalue, exctraceback):
    print("!!! Qt Error Occured !!!")
    """
    tb = "".join(traceback.format_exception(exctype, excvalue, exctraceback))
    print("Exception: type = {}\nvalue = {}\ntraceback = {}"
          .format(exctype, excvalue, tb)) 
    """
    QtApp.exit(-1)
    sys.excepthook = sys._excepthook
    print("--- Qt Event Loop ended ---")


class ExceptionHandler:
    def __init__(self, QtApp):
        # super().__init__()
        self.QtApp = QtApp

    def handler(self, exctype, value, traceback):
        self.on_exception(exctype, value, traceback)
        sys.__excepthook__(exctype, value, traceback)
        sys.excepthook = sys.__excepthook__
        print("exception handling finished.")

    def on_exception(self, exctype, excvalue, exctraceback):
        print("ExceptionHandler: Qt Error Occured!")
        """
        tb = "".join(traceback.format_exception(exctype, excvalue, exctraceback))
        print("Exception: type = {}\nvalue = {}\ntraceback = {}"
        .format(exctype, excvalue, tb)) 
        """
        qt_thr = QtCore.QThread.currentThread()
        self.QtApp.exit(-1)
        print("--- Qt Event-Loop ended {}---".format(qt_thr))


class TestWidget(QtWidgets.QPushButton):

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setText("hello")
        self.clicked.connect(self.doException)

    def doException(self):
        raise Exception("!CRITICAL EXCEPTION!")


def problematicCode():
    print("* Problematic Code Executed *")
    raise Exception("!CRITICAL EXCEPTION!")


def main():
    app = QtWidgets.QApplication(sys.argv)
    widget = TestWidget()

    exceptionHandler = ExceptionHandler(app)
    # sys._excepthook = sys.excepthook
    sys.excepthook = exceptionHandler.handler

    #exceptionHandler.errorSignal.connect(lambda *args: something(app, *args))

    QtCore.QTimer.singleShot(1000, problematicCode)
    # widget.show()
    
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
