# source <http://www.bogotobogo.com/Qt/Qt5_GridLayout.php>
from panta_rhei.gui.Qt import QtGui, QtCore, QtWidgets

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

    def _format_whats_this(self):
        """Generate tooltip with information about the minimum and maximum
        allowed values and the LSB-step.
        """
        decimals = 2
        fieldwidth = decimals + 4
        suffix = 'mA'
        _minimum_single_step = 0.1
        _maximum_value = 12
        _minimum_value = -10
        shortcut = 'S'
        more_button_text = "MORE"
        fmt = "{{: >{}.{}f}}".format(fieldwidth, decimals)
        whatsthis_text = "LSB\t\t: {1}{0}\nminimum\t\t: {2}{0}\nmaximum\t\t: {3}{0}".format(suffix,
                                                                                fmt.format(_minimum_single_step),
                                                                                fmt.format(_minimum_value),
                                                                                fmt.format(_maximum_value))
        whatsthis_text += "\n\n"
        whatsthis_text += "_________________________________________________________"
        whatsthis_text += "\n\n"
        whatsthis_text += WHATS_THIS.format(
            normalize_shortcut=shortcut,
            more_button_label=more_button_text)
        return whatsthis_text

    def event(self, event: QtCore.QEvent):
        if event.type() == QtCore.QEvent.WhatsThis:
            self.setWhatsThis(self._format_whats_this())
        return super().event(event)

    def keyPressEvent(self, event):
        event.accept()
        if event.key() == QtCore.Qt.Key_Question:
            self.showToolTip()
        return super().keyPressEvent(event)



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

# Template for the "What's this" text shown when using the '?' window button.
# Might only be supported on certain platforms.
WHATS_THIS = """\
Wheel or ↑↓ keys:
        no modifiers\t\t: increase/decrease value
\t\t\t   by current step width
\t\t\t   == min_delta*8^(multiplier-1)
        + Alt\t\t: increases/decreases multiplier by +-1,
\t\t\t   corresponding to a change of the
\t\t\t    base step width by a factor of 8^+-1
        + Ctrl\t\t: increases current step width
\t\t\t   by a factor of 8 while pressed
        + Ctrl+Shift\t\t: increases current step width
\t\t\t   by a factor of 64 while pressed

Keys:
        '{normalize_shortcut}'\t\t\t: Normalize element using
\t\t\t   the settings under \"{more_button_label}\"
        'w'\t\t\t: toggle wobbling
        's'\t\t\t: select for wobble
        'z', '0'\t\t: sets channel to zero
        '<', Alt + ←\t\t: sets channel to minimum
        '>', Alt + →\t\t: sets channel to maximum
        Ins, Enter\t\t: sets focus and cursor into line edit
\t\t\t   control(editing mode)
        Esc (while editing)\t: aborts editing mode
\t\t\t   of line edit control,
\t\t\t   changes are discarded.
        Enter (while editing)\t: accepts current content
\t\t\t   as new value if valid.

Mouse buttons + Alt:
        Left\t\t\t: sets channel to minimum
        Center/Wheel\t\t: sets channel to zero
        Right\t\t: sets channel to maximum

Invalid intermediate editing states are allowed.
During those invalid states the text color changes to red.
If the content is not a valid number or is out of range
when Enter is pressed, the previous value is restored.
Allowed characters are all digits and '+-,.'"""

channels= ['A', 'B' ,'C', 'D', 'A']
ws = WobbleChannelSelector(channels)
ws.setToolTip(WHATS_THIS)

ws.set_channel('B')
gridLayout.addWidget(ws, 4, 0, 1, 2)

stbutton = StartStopButton()
gridLayout.addWidget(stbutton, 5, 0, 1, 1)

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
