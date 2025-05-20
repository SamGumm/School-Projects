#ifndef POPUP_MENU_HPP
#define POPUP_MENU_HPP

#include <ncurses.h>
#include <panel.h>
#include <vector>

class Menu {
public:
    struct Popup { WINDOW *win; PANEL *pane; };

    /* window helpers */
    void   create_menu(int rows, int cols, int y, int x);
    void   delete_last();
    void   raise_last();
    bool   popups_empty() const;
    WINDOW* last_win();          //  ⟵ new helper

    ~Menu();

private:
    std::vector<Popup> popups;
};

#endif
