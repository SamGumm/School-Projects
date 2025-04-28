#include <ncurses.h>

int main() {
  initscr();
  if (!has_colors()) {
    endwin();
    printf("No color support\n");
    return 1;
  }
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLUE);

  bkgd(COLOR_PAIR(1));     // fill background in blue
  attron(A_BOLD);
  mvprintw(0, 0, "Bright yellow on blue!");
  attroff(A_BOLD);
  refresh();

  getch();
  endwin();
  return 0;
}
