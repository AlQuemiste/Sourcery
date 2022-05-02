# source <http://www.bogotobogo.com/Qt/Qt5_GridLayout.php>
from collections import namedtuple
import time

from panta_rhei.gui.Qt import QtGui, QtCore, QtWidgets
#==============================================================================80

class FutureMock:
    """ mocks a future class for testing purposes """
    def __init__(self, result_fn=None, delay=0):
        self._delay = delay
        
        if not result_fn:
            result_fn = lambda: print("*** FutureMock result: None ***")
        self._mock = namedtuple("FutureMock", "result")(result_fn)

    def add_done_callback(self, callback):
        time.sleep(self._delay)
        callback(self._mock)

class WobbleChannelSelector(QtGui.QComboBox):
    def __init__(self, channel_names:'[str]', parent=None):
        super().__init__(parent=parent)
        self.setDuplicatesEnabled(False)
        self.channel_names_list = channel_names
        self.channel_names_ordered = {cnl:n for n, cnl in enumerate(channel_names)}
        self.addItems(channel_names)
        self.current_channel = None
        self.currentIndexChanged.connect(self.on_index_changed)

    def set_channel(self, cnl):
        self.current_channel = cnl
        cnl_idx = self.channel_names_ordered[cnl]
        self.setCurrentIndex(cnl_idx)

    @QtCore.Slot(int)
    def on_index_changed(self, idx):
        cnl = self.channel_names_list[idx]
        self.current_channel = cnl
        print("AN>> WobbleChannelSelector. channel {} selected".format(cnl))

class StartStopButton(QtGui.QPushButton):
    """The widget indicate the wobbling state and to start/stop wobbling."""
    
    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self._isWobbling = False
        self.setAutoDefault(False)
        self.setDefault(False)
        self.setCheckable(True)
        # self.setEnabled(False)
        self.setFocusPolicy(QtCore.Qt.NoFocus)
        self.toggled.connect(self._toggled)
        
        self.update_ui()
        
    def update_ui(self):
        txt = 'Stop' if self._isWobbling else 'Start'
        self.setText(txt + " wobble")
        self.setChecked(self._isWobbling)
        
    def set_status(self, flag:bool):
        """ set the wobbling state """
        self._isWobbling = flag
        self.update_ui()
        
    @QtCore.Slot(bool)
    def _toggled(self, checked):
        print("AN>> StartStopButton: set => {}".format(checked))
        self._isWobbling = checked
        self.update_ui()

class ValueSpinBox(QtGui.QDoubleSpinBox):
    """ DoubleSpinBox which corrects ',' for '.' as the decimal point.
    The SpinBox always displays the total value of a (single axis of) channel
    in the current calibration and in the corresponding SI order-of-magnitude.
    """

    TEMP_COLOR = QtCore.Qt.darkCyan
    TEMP_BACKGROUND = QtCore.Qt.cyan
    INVALID_COLOR = QtCore.Qt.red
    INVALID_BACKGROUND = QtGui.QColor("#ff7373")
    PENDING_COLOR = QtCore.Qt.blue
    VALID_COLOR = QtCore.Qt.black
    VALID_BACKGROUND = QtCore.Qt.white
    
    def __init__(self, name:str='ValueSpinBox', parent=None):
        super().__init__(parent)
        self.setLocale(QtCore.QLocale.c()) # English/US Locale
        self.setAccelerated(True)  # speed up when holding down
        self.setKeyboardTracking(False)  # wait for editing to finish
        # self.setButtonSymbols(QtGui.QAbstractSpinBox.PlusMinus)
        # self.setMinimumWidth(90)
        self.setMinimumHeight(20)
        self.name = name
        self._delay = 0.1
        self.setStyleSheet("""
        border-width: 2px;
        border-color: red;
        font: regular;
        """)
        # DEBUG
        ValueSpinBox.VALID_COLOR = self.lineEdit().palette().color(QtGui.QPalette.Text)
        ValueSpinBox.VALID_BACKGROUND = self.lineEdit().palette().color(QtGui.QPalette.Base)

        self._lineEdit_palette = self.lineEdit().palette()
        ValueSpinBox.VALID_COLOR = self._lineEdit_palette.color(QtGui.QPalette.Text)
        ValueSpinBox.VALID_BACKGROUND = self._lineEdit_palette.color(QtGui.QPalette.Base)

        self._text_color_role = QtGui.QPalette.Text      
        self._line_edit_palette = self.lineEdit().palette()      
        self._valid_text_brush = self._line_edit_palette.brush(QtGui.QPalette.Text)
        
        self._temp_text_brush = QtGui.QBrush(QtCore.Qt.darkCyan)
        self._invalid_text_brush = QtGui.QBrush(QtCore.Qt.red)
        self._pending_text_brush = QtGui.QBrush(QtCore.Qt.blue)

        # palette = self.lineEdit().palette()
        # base_color = palette.color(QtGui.QPalette.Base)
        # palette.setColor(QtGui.QPalette.Base, QtCore.Qt.black)
        # palette.setColor(QtGui.QPalette.Text, QtCore.Qt.green)
        # self.lineEdit().setPalette(palette)
        
        
        #----------
        self._normal_txt_style = self.lineEdit().styleSheet()
        self.last_valid_value = 0

        # SingleShot QTimer will emit once the timeout() signal
        # after an interval; used for indicating erroneous values
        # self._timeout = 300  # time interval for showing error (ms)
        # self._errorshowTimer = QtCore.QTimer()
        # self._errorshowTimer.setSingleShot(True)
        # self._errorshowTimer.timeout.connect(self.fallback_to_valid)

        self.editingFinished.connect(self.apply_value)
        self.update_ui()

    def update_ui(self):
        self.setSuffix(' mA')
        self.setDecimals(2)
        self.setRange(-10, 10)
        self.set_valid_value(5)
        self.setSingleStep(0.1)

    def set_valid_value(self, value_SI):
        """ set a value and store it as the last good value;
        assumes that the value is valid. """

        print("AN>> {}.last_valid_value => {}".format(self.name, value_SI))
        self.last_valid_value = value_SI
        self._show_valid_state()
        self.setValue(value_SI)

    def validate(self, text, pos):
        # Q? what are s, t, p? are they (QValidator.State, QString input, int pos)
        # what is s == 0, s == 1

        # QValidator.Invalid = 0
        # QValidator.Intermediate = 1
        # QValidator.Acceptable = 2
        
        print("AN>> {}.validate: text = {}, pos = {}".format(self.name, text, pos))       
        text_cor = text.replace(',', '.', 1)
        return super().validate(text_cor, pos)

    def is_temp_value(self):
        """ checks if channel has a temporary offset """
        t1 = self.minimum() * 0.6
        t2 = self.maximum() * 0.6
        val = self.value()
        temp_status = val < t1 or val > t2
        return temp_status

    def change_cnl(self, delta):
        print("AN>> {}.change_cnl (mock) by {}...".format(self.name, delta))
        return FutureMock(delay=self._delay)

    def set_cnl(self, value):
        print("AN>> {}.set_cnl (mock) to {}...".format(self.name, value))
        return FutureMock(delay=self._delay)

    def stepBy(self, step):
        # function that is called whenever the user triggers a step

        delta_bit = step * self.singleStep()
        value_SI = delta_bit + self.value()
        self.setValue(value_SI)

        self._show_pending_state()
        change_cnl_future = self.change_cnl(delta_bit)
        change_cnl_future.add_done_callback(self.on_set_cnl_done)

    @QtCore.Slot()
    def apply_value(self, value_SI=None):
        """ send a request to change the channel value if it has changed """

        print("{}.apply_value {}".format(self.name, value_SI))

        # take the current value, if no value given
        if value_SI is None:
            value_SI = self.value()

        if value_SI == self.last_valid_value: return
        
        t1 = self.minimum()
        t2 = self.maximum()
        print("value: {}, range: ({}, {}), last-value: {}".format(value_SI, t1, t2, self.last_valid_value))
        
        # if not (self.minimum() <= value_SI <= self.maximum()):
        #     return
        if value_SI == 0:
            self._show_invalid_state()
            self.fallback_to_valid()
            return

        # if value changed significantly
        self._show_pending_state()
        print("AN>> {}: set value to {} [SI]".format(self.name, value_SI))
        self.set_valid_value(value_SI)
        set_cnl_future = self.set_cnl(value_SI)
        set_cnl_future.add_done_callback(self.on_set_cnl_done)

    def on_set_cnl_done(self, set_cnl_future):
        try:
            set_cnl_future.result()
            # if successful, Optics will update the value, and finally,
            # the value of the SpinBox will be updated accordingly
            self._show_valid_state()

        except Exception as error:
            self._show_invalid_state()
            print("Failed to set {}: {}".format(self.name, error))
            # ResultWindow.error("Failed to set %s: %s", self.channel.name, error)
            self.fallback_to_valid()

    def _set_text_brush(self, brush: QtGui.QBrush):
        self._line_edit_palette.setBrush(self._text_color_role, brush)
        self.lineEdit().setPalette(self._line_edit_palette)
        #QtGui.QGuiApplication.processEvents()
  
    def _show_pending_state(self):
        self._set_palette(ValueSpinBox.TEMP_BACKGROUND, ValueSpinBox.TEMP_COLOR)
        print("AN>> {}.pending state".format(self.name))

    def _show_pending_state_000(self):
        self.lineEdit().setStyleSheet("QLineEdit {color: darkgray;}")
        QtGui.QApplication.processEvents()
        # self._set_text_brush(self._pending_text_brush)
        # self.repaint()
        print("AN>> {}.pending state".format(self.name))

    def _show_invalid_state(self):
        self._set_palette(ValueSpinBox.INVALID_BACKGROUND, ValueSpinBox.INVALID_COLOR)
        print("AN>> {}.invalid state".format(self.name))
        time.sleep(self._delay)

    def _show_invalid_state_000(self):
        print("AN>> {}.invalid state".format(self.name))
        # paint red
        # prev_palette = self.lineEdit().palette()
        self._line_edit_palette.setBrush(self._text_color_role, self._invalid_text_brush)
        self.lineEdit().setPalette(self._line_edit_palette)
        QtGui.QApplication.processEvents()
        time.sleep(self._delay)
        # self.lineEdit().setPalette(prev_palette)

    def _set_palette(self, base_color, text_color):
        self._lineEdit_palette.setColor(QtGui.QPalette.Text,
                                        text_color)
        self._lineEdit_palette.setColor(QtGui.QPalette.Base,
                                        base_color)
        self.lineEdit().setPalette(self._lineEdit_palette)
        self.lineEdit().repaint()

    def _show_temp_state(self):
        self._set_palette(QtGui.QColor("#b6d9e1"), ValueSpinBox.TEMP_COLOR)
        print("AN>> {}.temporary state".format(self.name))

    def _show_valid_state(self):
        # self.lineEdit().setStyleSheet("")
        # valid value can be persistent or temporary
        if self.is_temp_value():
            self._show_temp_state()
        else:
            self._set_palette(ValueSpinBox.VALID_BACKGROUND, ValueSpinBox.VALID_COLOR)
            print("AN>> {}.valid state".format(self.name))

    def fallback_to_valid(self):
        """ resets value to the last (correct) value in case of problems """
        print("AN>> {}.fall-back to normal value...".format(self.name))
        self.setValue(self.last_valid_value)
        self._show_valid_state()


#------------------------------
app = QtGui.QApplication([""])

vlayout = QtWidgets.QVBoxLayout()

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
channels= ['A', 'B' ,'C', 'D', 'A']
ws = WobbleChannelSelector(channels)
ws.set_channel('B')
gridLayout.addWidget(ws, 4, 0, 1, 2)

value_spinbox = ValueSpinBox()
gridLayout.addWidget(value_spinbox, 5, 0, 1, 1)

stbutton = StartStopButton()
gridLayout.addWidget(stbutton, 5, 1, 1, 1)

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


