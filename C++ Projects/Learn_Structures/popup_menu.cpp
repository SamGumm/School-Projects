#include "popup_menu.hpp"
#include <cstdlib>

/*–––––––– Menu impl ––––––––*/

void Menu::create_menu(int rows, int cols, int y, int x)
{
    WINDOW *w = newwin(rows, cols, y, x);
    if (!w) { endwin(); std::exit(EXIT_FAILURE); }

    PANEL *p = new_panel(w);
    if (!p) { delwin(w); endwin(); std::exit(EXIT_FAILURE); }

    box(w, 0, 0);               // frame only; caller prints content
    popups.push_back({w, p});

    top_panel(p);
    update_panels(); doupdate();
}

void Menu::delete_last()
{
    if (popups.empty()) return;

    auto pp = popups.back(); popups.pop_back();
    del_panel(pp.pane); delwin(pp.win);

    update_panels(); doupdate();
}

void Menu::raise_last()
{
    if (popups.empty()) return;
    top_panel(popups.back().pane);
    update_panels(); doupdate();
}

bool Menu::popups_empty() const { return popups.empty(); }

WINDOW* Menu::last_win() { return popups.empty() ? nullptr : popups.back().win; }

Menu::~Menu()
{
    for (auto &pp : popups) { del_panel(pp.pane); delwin(pp.win); }
    popups.clear();
    update_panels(); doupdate();
}
