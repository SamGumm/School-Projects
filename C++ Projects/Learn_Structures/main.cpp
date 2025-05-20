/*────────────────────────  main.cpp  ────────────────────────*/
#include "popup_menu.hpp"
#include "generic_stack.hpp"
#include "binarytree.hpp"
#include "nagazod_dungeon.hpp"

#include <ncurses.h>
#include <panel.h>
#include <thread>
#include <chrono>
#include <unistd.h>   // dup, dup2, close
#include <cstdio>
#include <cstring>
#include <iostream>

/* little helper so students can follow the animation */
static void pause_ms(int ms = 600)
{ std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

/*────────────────────  Stack demo  ────────────────────*/
void demo_stack(Menu& menu)
{
    constexpr int ROWS = 26, COLS = 70;
    menu.create_menu(ROWS, COLS, 1, 5);
    WINDOW* w = menu.last_win();
    if (!w) return;

    /* ── 1. Show the source code ─────────────────── */
    const char* code[] = {
        "Stack<char> s;",
        "s.push('a');",
        "s.push('b');",
        "char top = s.peek();",
        "char x   = s.pop();",
        "bool e   = s.isEmpty();",
        "",
        "char braces[] = \"{[()]}\";",
        "s.parenMatch(braces);",
        "",
        "char infix[] = \"(A+B/C*(D+E)-F)\";",
        "s.infixToPostfix(infix);"
    };
    int y = 1;
    wattron(w, A_BOLD); mvwprintw(w, y++, 2, "Example code:"); wattroff(w, A_BOLD);
    for (auto line : code) mvwprintw(w, y++, 4, "%s", line);
    pause_ms();

    /* ── 2. Run it ── */
    FILE* tmp = tmpfile(); int save = dup(STDOUT_FILENO); dup2(fileno(tmp), STDOUT_FILENO);

    /* actual execution */
    Stack<char> s;
    s.push('a');
    s.push('b');
    char top = s.peek();
    char x   = s.pop();
    bool e   = s.isEmpty();
    std::cout << "peek()  = " << top << "\n";
    std::cout << "pop()   = " << x   << "\n";
    std::cout << "empty() = " << std::boolalpha << e << "\n";

    char braces[] = {'{','[','(',')',']','}','\0'};
    std::cout << "parenMatch(\"{[()]}\") -> "
              << (s.parenMatch(braces) ? "true" : "false") << "\n";

    char infix[]  = {'(', 'A','+','B','/','C','*','(', 'D','+','E',')','-','F',')','\0'};
    std::cout << "infixToPostfix(...)   -> ";
    s.infixToPostfix(infix);             // prints its own postfix
    std::cout << "\n";

    /* restore cout */
    fflush(stdout); dup2(save, STDOUT_FILENO); close(save);
    rewind(tmp);

    /* ── 3. Display the captured output ────────────────────────── */
    char line[128];
    wattron(w, A_BOLD); mvwprintw(w, y+1, 2, "Program output:"); wattroff(w, A_BOLD);
    int outY = y + 3;
    while (fgets(line, sizeof line, tmp) && outY < ROWS-3) {
        size_t L = strlen(line); if (L && line[L-1]=='\n') line[L-1]=0;
        mvwprintw(w, outY++, 4, "%s", line);
    }
    fclose(tmp);

    mvwprintw(w, ROWS-2, 2, "Press any key to return");
    wrefresh(w);
    wgetch(w);
    menu.delete_last();
}


/*────────────────────  Tree demo  ────────────────────*/
void demo_tree(Menu& menu)
{
    /* Roomy panel so the picture never wraps */
    constexpr int ROWS = 58, COLS = 78;
    menu.create_menu(ROWS, COLS, 0, 1);
    WINDOW* w = menu.last_win();
    if (!w) return;

    /* ── 1.  Code listing (shorter 7-node example, easy to picture) ───────── */
    const char* code[] = {
        "BinaryTree<int> bt(5);",
        "auto r  = bt.root();",
        "auto n3 = bt.addChild(r, 3, LEFT);",
        "auto n8 = bt.addChild(r, 8, RIGHT);",
        "bt.addChild(n3, 1, LEFT);",
        "bt.addChild(n3, 4, RIGHT);",
        "bt.addChild(n8, 7, LEFT);",
        "bt.addChild(n8, 9, RIGHT);"
    };
    int y = 1;
    wattron(w, A_BOLD); mvwprintw(w, y++, 2, "Code to build the tree:"); wattroff(w, A_BOLD);
    for (auto line : code) mvwprintw(w, y++, 4, "%s", line);
    pause_ms();

    /* ── 2.  Build the tree exactly as above ─────────────────────────────── */
    BinaryTree<int> bt(5);
    auto r  = bt.root();
    auto n3 = bt.addChild(r, 3, CHILD::LEFT);
    auto n8 = bt.addChild(r, 8, CHILD::RIGHT);
    bt.addChild(n3, 1, CHILD::LEFT);
    bt.addChild(n3, 4, CHILD::RIGHT);
    bt.addChild(n8, 7, CHILD::LEFT);
    bt.addChild(n8, 9, CHILD::RIGHT);

    /* ── 3.  Show a centered ASCII picture of the tree ───────────────────── */
    const char* art[] = {
        "               5               ",
        "            /     \\            ",
        "          3         8          ",
        "        /  \\       /  \\        ",
        "       1    4     7    9       "
    };
    wattron(w, A_BOLD); mvwprintw(w, y+1, 2, "Visual layout:"); wattroff(w, A_BOLD);
    for (auto line : art) mvwprintw(w, y += 3, 10, "%s", line);
    pause_ms();

    /* ── 4.  Capture traversals + height into a tmp file ─────────────────── */
    FILE* tmp = tmpfile();
    int   save = dup(STDOUT_FILENO);
    dup2(fileno(tmp), STDOUT_FILENO);

    std::cout << "Pre-order : ";  bt.preOrder(bt.root());  std::cout << '\n';
    std::cout << "In-order  : ";  bt.inOrder(bt.root());   std::cout << '\n';
    std::cout << "Post-order: ";  bt.postOrder(bt.root()); std::cout << '\n';
    std::cout << "Height    : "  << bt.height(bt.root())  << '\n';

    fflush(stdout);
    dup2(save, STDOUT_FILENO); close(save);
    rewind(tmp);

    /* ── 5.  Replay captured output inside the panel ─────────────────────── */
    char buf[160];  int outY = y + 6;
    wattron(w, A_BOLD); mvwprintw(w, outY++, 2, "Output:"); wattroff(w, A_BOLD);
    while (fgets(buf, sizeof buf, tmp) && outY < ROWS - 3) {
        size_t L = strlen(buf); if (L && buf[L-1]=='\n') buf[L-1]='\0';
        mvwprintw(w, outY++, 4, "%s", buf);
    }
    fclose(tmp);

    /* ── 6.  Wait for the learner to finish reading ──────────────────────── */
    mvwprintw(w, ROWS-2, 2, "Press any key to return");
    wrefresh(w);
    wgetch(w);
    menu.delete_last();      // close the pop-up
}




/*────────────────────  Graph / dungeon demo  ────────────────────*/
void demo_graph(Menu& menu)
{
    constexpr int ROWS = 28, COLS = 78;
    menu.create_menu(ROWS, COLS, 0, 1);
    WINDOW* w = menu.last_win();
    if (!w) return;

    /* 1.  Build the canned dungeon */
    Dungeon d;  
    d.create_test_level_1();

    /* 2.  BFS */
    std::vector<Room*> order;
    std::queue<Room*>  q;
    std::unordered_set<int> seen;

    Room* start = d.firstRoom();
    if (start) { q.push(start); seen.insert(start->id()); }

    while (!q.empty()) {
        Room* cur = q.front(); q.pop();
        order.push_back(cur);

        for (DIRECTION dir : {DIRECTION::NORTH, DIRECTION::EAST,
                              DIRECTION::SOUTH, DIRECTION::WEST})
        {
            Room* nb = cur->neighbor(dir);
            if (nb && !seen.count(nb->id())) {
                seen.insert(nb->id());
                q.push(nb);
            }
        }
    }

    /* 3.  Display a table of the rooms visited */
    wattron(w, A_BOLD);
    mvwprintw(w, 1, 2, "Breadth-first traversal order:");
    wattroff(w, A_BOLD);

    mvwprintw(w, 3, 4, "%-6s %-12s  Description", "ID", "Type");
    mvwhline(w, 4, 4, ACS_HLINE, 64);

    int y = 5;
    for (Room* r : order) {
        if (y >= ROWS - 4) break;              // keep inside panel
        mvwprintw(w, y++, 4, "%-6d %-12s",
                  r->id(),
                  r->roomtype_string().c_str());
    }

    /* 4.  Simple ASCII mini-map */
    const char* map[] = {
        "+---+   +---+   +---+",
        "| 0 |---| 1 |---| 2 |",
        "+---+   +---+   +---+",
        "            |        ",
        "          +---+      ",
        "          | 3 |      ",
        "          +---+      "
    };
    wattron(w, A_BOLD);
    mvwprintw(w, y + 1, 2, "Rough layout:");
    wattroff(w, A_BOLD);
    for (int i = 0; i < 7 && y + 3 + i < ROWS - 2; ++i)
        mvwprintw(w, y + 3 + i, 8, "%s", map[i]);

    /* 5.  Wait for user to read, then close pop-up */
    mvwprintw(w, ROWS - 2, 2, "Press any key to return");
    wrefresh(w);
    wgetch(w);
    menu.delete_last();
}


/*────────────────────  Program entry  ────────────────────*/
int main()
{
    initscr();
    cbreak(); noecho();
    keypad(stdscr, TRUE);
    curs_set(0);                                   // hide cursor
    std::atexit([]{ echo(); nocbreak(); endwin(); });

    const char* choices[] = { "Stack", "Trees", "Graphs", "Quit" };
    int highlight = 0, choice = -1;
    Menu menu;

    while (true) {

        /* draw background menu */
        werase(stdscr);
        for (int i = 0; i < 4; ++i) {
            if (i == highlight) attron(A_REVERSE);
            mvprintw(i, 0, "%s", choices[i]);
            if (i == highlight) attroff(A_REVERSE);
        }
        update_panels(); doupdate();

        int ch = getch();
        switch (ch) {
            case KEY_UP:   highlight = (highlight + 3) % 4; break;
            case KEY_DOWN: highlight = (highlight + 1) % 4; break;
            case 'f':     choice = highlight;              break;
            case 'q':      choice = 3;                      break;
            default:       continue;
        }

        switch (choice) {
            case 0: demo_stack(menu);  break;
            case 1: demo_tree(menu);   break;
            case 2: demo_graph(menu);  break;
            case 3: return 0;
        }
        choice = -1;  /* loop back to menu */
    }
}
