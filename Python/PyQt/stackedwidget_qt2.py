import sys
# from PyQt5.QtCore import *
# from PyQt5.QtGui import *
# from PyQt5.QtWidgets import *
from panta_rhei.gui.Qt import QtGui, QtCore, QtWidgets


class stackedExample(QtGui.QWidget):

   def __init__(self):
      super(stackedExample, self).__init__()

      v_layout = QtGui.QVBoxLayout()
      param1 = QtGui.QHBoxLayout()

      #-- exposure time in ms
      param1.addWidget(QtGui.QLabel('exposure time:'))
      self.exposure_time_widget = QtGui.QDoubleSpinBox()
      self.exposure_time_widget.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignTrailing | QtCore.Qt.AlignVCenter)
      self.exposure_time_widget.setDecimals(0)
      self.exposure_time_widget.setMinimum(100)
      self.exposure_time_widget.setMaximum(1000)
      self.exposure_time_widget.setSingleStep(10)
      self.exposure_time_widget.setSuffix(" ms")
      self.exposure_time_widget.valueChanged.connect(self._on_changed)

      param1.addWidget(self.exposure_time_widget)
      param1.addStretch()
        #self.exposure_time_widget.setMaximumWidth(45)

      #-- binning
      param1.addWidget(QtGui.QLabel('binning:'))
      self.binning_widget = QtGui.QComboBox()
      self.binning_widget.addItems(['1', '2', '4', '8'])
      self.binning_widget.currentIndexChanged.connect(self._on_changed)
      param1.addWidget(self.binning_widget)
      param1.addStretch()
      
      v_layout.addLayout(param1)
        
      param2 = QtGui.QHBoxLayout()

      #-- start HT
      param2.addWidget(QtGui.QLabel('start:'))
      self.start_HT_widget = QtGui.QDoubleSpinBox()
      # self.start_HT_widget.setKeyboardTracking(False)
      self.start_HT_widget.setRange(-999,+999)
      self.start_HT_widget.setSingleStep(1)
      self.start_HT_widget.setDecimals(1)
      self.start_HT_widget.setSuffix(' eV')
      self.start_HT_widget.valueChanged.connect(self._on_changed)
      param2.addWidget(self.start_HT_widget)
      param2.addStretch()

      #-- stop HT
      param2.addWidget(QtGui.QLabel('stop:'))
      self.stop_HT_widget = QtGui.QDoubleSpinBox()
      # self.stop_HT_widget.setKeyboardTracking(False)
      self.stop_HT_widget.setRange(-999,+999)
      self.stop_HT_widget.setSingleStep(1)
      self.stop_HT_widget.setDecimals(1)
      self.stop_HT_widget.setSuffix(' eV')
      self.stop_HT_widget.valueChanged.connect(self._on_changed)
      param2.addWidget(self.stop_HT_widget)
      param2.addStretch()

      #-- steps
      param2.addWidget(QtGui.QLabel('steps:'))
      self.steps_HT_widget = QtGui.QSpinBox()
      self.steps_HT_widget.setRange(1,99)
      self.steps_HT_widget.setSingleStep(1)
      self.steps_HT_widget.valueChanged.connect(self._on_changed)
      param2.addWidget(self.steps_HT_widget)
      param2.addStretch()
      
      v_layout.addLayout(param2)

      param3 = QtGui.QHBoxLayout()
      #-- mask combo box
      aperture_label = QtGui.QLabel('aperture:')
      param3.addWidget(aperture_label)
      self.aperture_widget = QtGui.QComboBox()
      self.aperture_widget.addItems(['2mm','5mm'])
      self.aperture_widget.currentIndexChanged.connect(self._on_changed)
      param3.addWidget(self.aperture_widget)
      
      v_layout.addLayout(param3)

      # separator line
      h_layout = QtGui.QHBoxLayout()

      h_line = QtGui.QFrame()
      h_line.setFrameShape(QtGui.QFrame.HLine)
      h_line.setFrameShadow(QtGui.QFrame.Sunken)        
      h_layout.addWidget(h_line)

      h_layout.addWidget(QtGui.QLabel("Corrections"))

      h_line = QtGui.QFrame()
      h_line.setFrameShape(QtGui.QFrame.HLine)
      h_line.setFrameShadow(QtGui.QFrame.Sunken)
      h_layout.addWidget(h_line)
      
      v_layout.addLayout(h_layout)

      # correction buttons
      cor_btns = QtGui.QHBoxLayout()

      # high-res | low-res combobox
      self.hilores_widget = QtGui.QComboBox()
      self.hilores_widget.addItems(['High Res', 'Low Res'])

      # #----------------------------------------
        
      _stack_lo_layout = QtGui.QHBoxLayout()
      _stack_lo_layout.addWidget(QtGui.QLabel("LO1"))
      _stack_lo_layout.addWidget(QtGui.QLabel("LO2"))

      _stack_hi_layout = QtGui.QHBoxLayout()
      _stack_hi_layout.addWidget(QtGui.QLabel("HI1"))
      _stack_hi_layout.addWidget(QtGui.QLabel("HI2"))

      _stack_hi = QtGui.QWidget()
      _stack_hi.setLayout(_stack_hi_layout)
      
      _stack_lo = QtGui.QWidget()
      _stack_lo.setLayout(_stack_lo_layout)

      self._stack = QtGui.QStackedWidget()        
      self._stack.addWidget(_stack_lo)
      self._stack.addWidget(_stack_hi)

      self.hilores_widget.currentIndexChanged.connect(self.displayHiLo)
      #----------------------------------------

      cor_btns.addWidget(self.hilores_widget)
      cor_btns.addWidget(self._stack)


      # # param1.addWidget(self.hilores_widget)
      # # param1.addStretch()
        
      v_layout.addLayout(cor_btns)
      #----------------------------------------

      root_h_layout = QtGui.QHBoxLayout()
      root_h_layout.addLayout(v_layout)
      self.setLayout(root_h_layout)

      self.show()

   def _on_changed(self):
      print("CHANGED!")

   def displayHiLo(self, i):
      self._stack.setCurrentIndex(i)

      
def main():
   app = QtGui.QApplication(sys.argv)
   ex = stackedExample()
   sys.exit(app.exec_())

if __name__ == '__main__':
   main() 
