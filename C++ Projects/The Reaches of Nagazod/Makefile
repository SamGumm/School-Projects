# ---- SRC ------------------
SRCS := src/main.cpp \
        src/nagazod_dungeon.cpp \
        src/nagazod_room.cpp \
		src/nagazod_armor.cpp \
		src/nagazod_weapon.cpp \
		src/nagazod_player.cpp \
		src/nagazod_monster.cpp \
		src/nagazod_object_factory.cpp \
		src/nagazod_encounter.cpp \
        #src/new_file.cpp   # <-- add new .cpp here
# ---------------------------------------------------------------------

OBJS := $(SRCS:.cpp=.o)         # automatic .o list
CXX  := g++
CXXFLAGS := -Wall -Wextra -std=c++20 -Iinclude
LFLAGS := -lncurses -lpanel

# final executable
td: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)

# pattern rule: any .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) td

