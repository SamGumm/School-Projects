/*
Start Screen:
|    - 

New Game Screen:
|    - Intro
|      |
|      +------> Character Creation
|                |
|                +------> Options pre-init
|                          |
|                          +------> Gameloop screen

Options Screen:
|    - Changes global values
|       - BGM
|       - color?
|       - 

Continue Screen:
|   |
|   +-----> Choose Save
|            |
|            +------> Gameloop Screen

Quit Screen:
|   - Close

Gameloop Screen:
|    - There will be 6 panels on the stdscr (WxH)
|        - Player Picture (20%x35%)
|        - Map (20%x35%)
|        - Player Stats (50%x30%)
|        - Player Inventory (50%x30%)
|        - Monster (20%x70%)
|            - blank until monster is encountered
|        - Current view (60%x70%)
|            - Views are:
|                - Current Corridor
|                - Current Room



*/

/*
 * Ncurses Cheatsheet
 *
 * Initialization Functions:
 * initscr()            Initializes the terminal in cursor mode. Must be called before any other curses routine.
 * refresh()            Refresh stdscr, refreshing only changed portions.
 * wrefresh(win)        Refresh the given window "win".
 * endwin()             Ends curses mode, returning terminal to normal.
 * raw()                Disable line buffering; control characters passed directly.
 * cbreak()             Disable line buffering; characters available to program immediately.
 * echo()               Turn on echo of input characters.
 * noecho()             Turn off echo of input characters.
 * keypad(stdscr,TRUE)  Enable function keys (e.g., arrows) for stdscr.
 * halfdelay(tenths)    Wait up to tenths*0.1 seconds for input, then return ERR.
 *
 * Miscellaneous Functions:
 * clear()              Clear stdscr.
 * wclear(win)          Clear window "win".
 * move(y,x)            Move cursor to y,x on stdscr.
 * wmove(win,y,x)       Move cursor to y,x on window "win".
 * getmaxyx(win,y,x)    Get dimensions of window (rows in y, cols in x).
 * getyx(win,y,x)       Get current cursor position in window.
 *
 * Output Functions:
 * addch(ch)            Print character at current cursor pos with attributes.
 * mvaddch(y,x,ch)      Move then addch.
 * waddch(win,ch)       Addch into window.
 * mvwaddch(win,y,x,ch) Move then waddch.
 * printw(fmt,...)      Print formatted string at current pos.
 * mvprintw(y,x,fmt,...) Move then printw.
 * wprintw(win,fmt,...) Printw into window.
 * mvwprintw(win,y,x,fmt,...) Move then wprintw.
 * addstr(str)          Print string at current pos.
 * mvaddstr(y,x,str)    Move then addstr.
 * waddstr(win,str)     Addstr into win.
 * mvwaddstr(win,y,x,str) Move then waddstr.
 *
 * Input Functions:
 * getch()              Get character from stdscr.
 * mvgetch(y,x)         Move then getch.
 * wgetch(win)          Getch from window.
 * mvwgetch(win,y,x)    Move then wgetch.
 * scanw(fmt,...)       Input formatted from stdscr.
 * mvscanw(y,x,fmt,...) Move then scanw.
 * wscanw(win,fmt,...)  Scanw from win.
 * mvwscanw(win,y,x,fmt,...) Move then wscanw.
 * getstr(str)          Get string from stdscr.
 * wgetstr(win,str)     Getstr from win.
 * mvwgetstr(win,y,x,str) Move then wgetstr.
 *
 * Attribute Functions:
 * attron(A)            Turn on attribute A.
 * attroff(A)           Turn off attribute A.
 * attrset(A)           Set attributes to A, replacing previous.
 * chgat(n,A,cp,opts)   Change attributes of n chars at current pos.
 * mvchgat(y,x,n,A,cp,opts) ...with move.
 *
 * Common Attributes:
 * A_NORMAL    Normal display.
 * A_STANDOUT  Best highlighting.
 * A_UNDERLINE Underline.
 * A_REVERSE   Reverse video.
 * A_BLINK     Blinking.
 * A_DIM       Half-bright.
 * A_BOLD      Bold.
 * COLOR_PAIR(n) Use color pair n.
 *
 * Color Functions:
 * start_color()        Initialize color mode.
 * has_colors()         Check if terminal supports colors.
 * init_pair(n,fg,bg)   Define color pair n.
 * init_color(c,r,g,b)  Redefine color c.
 * can_change_color()   Check color redefinition capability.
 *
 * Color Constants:
 * COLOR_BLACK, COLOR_RED,...COLOR_WHITE (0-7)
 *
 * Window Functions:
 * newwin(h,w,y,x)      Create new window of size h x w at y,x.
 * box(win,ls,rs)       Draw border around win.
 * wborder(win, ...)    Draw customizable border.
 * delwin(win)          Delete window and free memory.
 *
 */

 #pragma once

 #include <ncurses.h>
 #include <panel.h>
 #include <vector>
 #include <string>
 #include <memory>
 #include <cstdlib>

 //UNIMPLEMENTED, gave up lmao (for right now...)

 // ====================== Screen =========================
 struct Popup {
    WINDOW *win;
    PANEL *pane;
 };

enum struct MENU_OPTIONS {
    CONTINUE, NEW_GAME, OPTIONS, QUIT
};

 class Screen {
    public:
    
    // const char *choices[] = {"Start New Game", "Load Save",
    //         "Options", "Quit"};

    //Getters
        bool popups_empty() { return popups.empty(); }

    //Setters

    //Methods

        void create_menu_panel(int rows, int cols, int y, int x);
        void delete_last();
        void raise_last();
        void init_game_panels();
        void init();
        void create_main_menu();

        ~Screen();
    
    private:
        std::vector<Popup> popups;
        WINDOW* main_panel_win       = nullptr;
        PANEL*  main_panel_p         = nullptr;

        WINDOW* map_panel_win        = nullptr;
        PANEL*  map_panel_p          = nullptr;

        WINDOW* monster_panel_win    = nullptr;
        PANEL*  monster_panel_p      = nullptr;

        WINDOW* player_panel_win     = nullptr;
        PANEL*  player_panel_p       = nullptr;

        WINDOW* inventory_panel_win  = nullptr;
        PANEL*  inventory_panel_p    = nullptr;

        


 };
