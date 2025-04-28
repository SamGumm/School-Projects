#include <fstream>
#include <sstream>
#include <unordered_map>
#include "monster_description.hpp"
#include "generateDungeon.hpp"   // COLOR_*
#include <ncurses.h>
#include <iostream>
#include "parser.hpp"

/* ---------- helper: color word → curses number (BLACK -> WHITE) -------- */
static int color_num(const std::string& word)
{
    static const std::unordered_map<std::string,int> tbl = {
        {"RED",COLOR_RED},   {"GREEN",COLOR_GREEN}, {"YELLOW",COLOR_YELLOW},
        {"BLUE",COLOR_BLUE}, {"MAGENTA",COLOR_MAGENTA},
        {"CYAN",COLOR_CYAN}, {"WHITE",COLOR_WHITE}, {"BLACK",COLOR_WHITE}
    };
    auto it = tbl.find(word);
    return (it == tbl.end() ? COLOR_WHITE : it->second);
}

/* -----------------------------------------------------------------------
 *  readMonsterDescriptions  — factory‑ready descriptions only
 * ----------------------------------------------------------------------*/
std::vector<MonsterDescription>
Parser::readMonsterDescriptions(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) { std::cerr << "Cannot open " << filename << '\n'; return {}; }

    /* Skip the RLG header line */
    std::string line;
    std::getline(in,line);        // "RLG327 MONSTER DESCRIPTION 1"

    std::vector<MonsterDescription> list;
    MonsterDescription cur;
    bool in_block = false;

    while (std::getline(in,line))
    {
        if (line == "BEGIN MONSTER") { cur = {}; in_block = true; continue; }
        if (line == "END")           { list.push_back(cur); in_block=false; continue; }
        if (!in_block)               continue;

        std::istringstream iss(line);
        std::string key; iss >> key;

        /* Multi‑line DESC handled specially */
        if (key == "DESC") {
            cur.desc.clear();
            while (std::getline(in,line) && line != ".")
                cur.desc += line + ' ';
            continue;
        }

        /* read the rest of the line (may be empty) */
        std::string rest;
        std::getline(iss,rest);
        if (!rest.empty() && rest[0]==' ') rest.erase(0,1);

        if (key == "NAME")   cur.name = rest;
        else if (key == "SYMB") cur.symbol = rest[0];
        else if (key == "COLOR") {
            std::istringstream cs(rest); std::string c;
            while (cs >> c) cur.colors.push_back(color_num(c));
        }
        else if (key == "SPEED")  cur.speedDice  = rest;
        else if (key == "HP")     cur.hpDice     = rest;
        else if (key == "DAM")    cur.damageDice = rest;
        else if (key == "ABIL") {
            std::istringstream as(rest); std::string a;
            while (as >> a) cur.abilities.push_back(a);
        }
        else if (key == "RRTY")   cur.rarity = std::stoi(rest);
        else if (key == "UNIQ")   cur.is_unique = true;
    }
    return list;
}

std::vector<ObjectDescription>
Parser::readObjectDescriptions(const std::string& fn)
{
    std::ifstream in(fn);
    if (!in) { std::cerr<<"Cannot open "<<fn<<"\n"; return {}; }

    std::string line;
    std::getline(in,line);   // RLG327 OBJECT DESCRIPTION 1

    std::vector<ObjectDescription> list;
    ObjectDescription cur;
    bool in_block=false;

    while (std::getline(in,line)) {
        if (line=="BEGIN OBJECT") { cur={}; in_block=true; continue; }
        if (line=="END")          { list.push_back(cur); in_block=false; continue; }
        if (!in_block) continue;

        std::istringstream iss(line); std::string key; iss>>key;
        if (key=="DESC") {
            cur.desc.clear();
            while (std::getline(in,line) && line!=".") cur.desc+=line+' ';
            continue;
        }
        std::string rest; std::getline(iss,rest);
        if (!rest.empty() && rest[0]==' ') rest.erase(0,1);

        if (key=="NAME")     cur.name = rest;
        else if (key=="SYMB")cur.symbol = rest[0];
        else if (key=="COLOR") {
            std::istringstream cs(rest); std::string c;
            while (cs>>c) cur.colors.push_back(color_num(c));
        }
        else if (key=="WEIGHT")     cur.weightDice = rest;
        else if (key=="HIT")        cur.hitDice = rest;
        else if (key=="DAM")        cur.damageDice = rest;
        else if (key=="DODGE")      cur.dodgeDice = rest;
        else if (key=="DEF")        cur.defenseDice = rest;
        else if (key=="ATTR")       cur.attributeDice = rest;
        else if (key=="VAL")        cur.valueDice = rest;
        else if (key=="SPEED")      cur.speedDice = rest;
        else if (key=="RRTY")       cur.rarity = std::stoi(rest);
        else if (key=="ART")        cur.is_artifact = true;
    }
    return list;
}

