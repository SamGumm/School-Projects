# Data-Structure Teaching Console (`ds_demo`)

An ncurses-based mini-console that demonstrates three classic data-structures:

| Demo | What it shows |
|------|---------------|
| **Stack** | Push/peek/pop, balanced-bracket check (`parenMatch`), and infix → postfix conversion. |
| **Binary Tree** | Building a 7-node tree, ASCII visualization, depth-first traversals, and height calculation. |
| **Graph (Dungeon)** | Breadth-first traversal of a small room graph with a text-table of rooms and a rough ASCII mini-map. |

---

## Folder layout

project-root/
│
├─ main.cpp # menu + three demos
├─ popup_menu.hpp/.cpp # tiny wrapper around ncurses panels


├─ generic_stack.hpp # template stack implementation
├─ binarytree.hpp # template binary-tree implementation
├─ nagazod_room.hpp/.cpp # Room class (graph node)
├─ nagazod_dungeon.hpp/.cpp# Dungeon class (graph wrapper)
├─ Makefile # one-line build recipe (optional)
└─ README.md # this file


---

## Prerequisites

* C++17 compiler (tested on **g++ 11+**, **clang 14+**)
* `ncurses` and `panel` development headers  
  *Ubuntu/Debian* → `sudo apt install libncurses5-dev libncursesw5-dev`  
* POSIX environment (Linux, macOS, WSL, MinGW-posix)

---

## Build

```bash
make

---

## Run
./ds_demo

    Use ↑/↓ to move in the left-hand menu.

    f opens a demo; q quits the program.

    Inside a demo, read the output and press any key to return to the menu.

---

## Intention
* I will be using this to help run the final exam review for COMS2280, in order to provide some additional visuals and code.