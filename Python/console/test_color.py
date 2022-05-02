class bcolors:
    WARNING = '\033[95m'
    ENDC = '\033[0m'

print(bcolors.WARNING + "Warning: No active frommets remain. Continue?" + bcolors.ENDC)

for i in range(0, 128):
    print("\033[%imThis is text!!\t\t( \\033[%im )\033[0m" % (i, i))
#-------------------------------------------------
# https://en.wikipedia.org/wiki/List_of_widget_toolkits
import curses

def main(stdscr):
    from curses.textpad import Textbox, rectangle

    stdscr.addstr(0, 0, "Enter IM message: (hit Ctrl-G to send)")

    editwin = curses.newwin(5,30, 2,1)
    rectangle(stdscr, 1,0, 1+5+1, 1+30+1)
    stdscr.refresh()

    box = Textbox(editwin)

    # Let the user edit until Ctrl-G is struck.
    box.edit()

    # Get resulting contents
    message = box.gather()

stdscr = curses.initscr()
curses.start_color()
curses.init_pair(1, curses.COLOR_RED, curses.COLOR_WHITE)
curses.noecho()
curses.cbreak()
stdscr.keypad(True)
stdscr.addstr(0, 0, "Current mode: Typing mode",
              curses.A_REVERSE)
stdscr.addstr(1, 0, "Current mode: Typing mode",
              curses.A_BLINK)
stdscr.addstr(2, 0, "Current mode: Typing mode",
              curses.A_DIM)
stdscr.addstr(3, 0, "Current mode: Typing mode",
              curses.A_UNDERLINE)
stdscr.addstr(4, 0, "Current mode: Typing mode",
              curses.A_STANDOUT)
stdscr.addstr(5,0, "RED ALERT!", curses.color_pair(1))
stdscr.refresh()
c = stdscr.getch()
main(stdscr)
c = stdscr.getch()
curses.endwin()

#----------------------------------------

