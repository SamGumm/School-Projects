#include <ncurses.h>
#include <vector>
#include <stdlib.h>
#include <panel.h>

//compile with g++ -Wextra -Wall gameloop.cpp -o menu -lncurses -lpanel

class Menu {
public:
    struct Popup {
        WINDOW *win;
        PANEL *pane;
    };

    std::vector<Popup> popups;

    void create_menu(int rows, int cols, int y, int x) {
        WINDOW *w = newwin(rows, cols, y, x);
        PANEL *p = new_panel(w);
        if(!w || !p) {
            endwin();
            std::exit(1);
        }
        box(w, 0, 0);
        mvwprintw(w, 1, 1, "Hello from within win!");

        popups.push_back({w, p});
    }

    /**
     * Remove last pop-up
     */
    void delete_last() {
        if (!popups.empty()) {
            auto last = popups.back();
            del_panel(last.pane);
            delwin(last.win);
            popups.pop_back();
        }
    }

    /**
     * Bring last pop-up to front
     */
    void raise_last() {
        if(!popups.empty()) {
            top_panel(popups.back().pane);
        }
    }

    /**
     * Returns true if popups is empty, false otherwise
     */
    bool popups_empty() {
        return popups.empty();
    }

    ~Menu() {
        for (auto &pp : popups) {
            del_panel(pp.pane);
            delwin(pp.win);
        }
            
    }

};

int main() {
    initscr();
    cbreak(); 
    noecho(); 
    keypad(stdscr, TRUE);

    const char *choices[] = {"Start New Game", "Load Save",
                             "Options", "Quit"};
    int highlight = 0, choice = -1, ch;

    Menu menu;

    while (true) {
        /*Draw stdscr*/
        //werase(stdscr);
        wrefresh(stdscr);
        for (int i = 0; i < 4; ++i) {
            if (i == highlight) attron(A_REVERSE);
            mvprintw(i, 0, "%s", choices[i]);
            if (i == highlight) attroff(A_REVERSE);
        }
        /*Input*/
        ch = getch();
        switch (ch) {
            case KEY_UP:   
                highlight = (highlight + 3) % 4; 
                break;
            case KEY_DOWN: 
                highlight = (highlight + 1) % 4; 
                break;
            case 'f': 
                choice = highlight;                 
                break;
            case 'q':  choice = 3;                         
                break;
        }
        /*Actions*/
        if (choice == 0) {          /* Start New Game */
            if(menu.popups_empty()) {
                menu.create_menu(10,30,5,10);
            } else {
                menu.raise_last();
            }
            choice = -1;
        } else if (choice == 3) {    /* Quit */
            break;
        }
        update_panels();
        doupdate();
    }

    endwin();
    return 0;
}
