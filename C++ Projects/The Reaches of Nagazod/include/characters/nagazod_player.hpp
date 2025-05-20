#pragma once

#include "loot/armor/nagazod_armor.hpp"
#include "loot/weapons/nagazod_weapon.hpp"
#include "loot/consumables/nagazod_items.hpp"
#include "dungeon_creation/nagazod_room.hpp"
#include <random>
#include <memory>
#include <algorithm>

/**
 * Structure containing base stats of a Player as integers
 */
struct PLAYER_STATS {
    int HP {15}; 
    int STR {1}, WIS {1}, DEX {1};
    int DAM_DICE {3}, HIT_DICE {1};
    int DODGE {20}, HIT_CHANCE {50};
};

/**
 * Structure containing raw pointers to objects in the player's inventory.
 */
struct PLAYER_EQUIPMENT {
    Helmet* helmet{nullptr};
    Chestpiece* chestpiece{nullptr};
    Leggings* leggings{nullptr};
    Boots* boots{nullptr};
    Gloves* gloves{nullptr};
    Weapon* weapon{nullptr}; //init with &mySword (for instance)

    
    [[nodiscard]] int total_hp() const { //[nodiscard] is to make sure that result is not ignored
        int hp = 0;
        if (helmet) hp += helmet->hp();
        if (chestpiece) hp += chestpiece->hp();
        if (leggings) hp += leggings->hp();
        if (boots) hp += boots->hp();
        if (gloves) hp += gloves->hp();
        return hp;
    }

    [[nodiscard]] int total_str() const {
        int str = 0; 
        if (helmet) str += helmet->strength();
        if (chestpiece) str += chestpiece->strength();
        if (leggings) str += leggings->strength();
        if (boots) str += boots->strength();
        if (gloves) str += gloves->strength();
        if (weapon) str += weapon->strength();
        return str;
    }

    [[nodiscard]] int total_wis() const {
        int wis = 0;
        if (helmet) wis += helmet->wis();
        if (chestpiece) wis += chestpiece->wis();
        if (leggings) wis += leggings->wis();
        if (boots) wis += boots->wis();
        if (gloves) wis += gloves->wis();
        if (weapon) wis += weapon->wis();
        return wis;
    }

    [[nodiscard]] int total_dex() const {
        int dex = 0;
        if(helmet) dex += helmet->dex();
        if (chestpiece) dex += chestpiece->dex();
        if (leggings) dex += leggings->dex();
        if (boots) dex += boots->dex();
        if (gloves) dex += gloves->dex();
        if (weapon) dex += weapon->dex();
        return dex;
    }

    [[nodiscard]] int total_dam() const {
        int dam = 0;
        if (weapon) dam += weapon->dam();
        return dam;
    }

    [[nodiscard]] int total_hit_chance() const {
        int hc = 0;
        if (weapon) hc += weapon->hit_chance();
        return hc;
    }

};

class Player {
    public:
        /* METHODS */
        void add_to_inventory(std::unique_ptr<Object> o);
        void remove_from_inventory(const Object* o);
        void display_inventory() const;
        int roll_attack();
        int roll_damage();
        int roll_dodge();
        void move(DIRECTION d);
        void display_stats();

        /* Getters */
        const PLAYER_STATS& stat_block() { return stat_block_; } //doubt i need this
        int hp() const noexcept { return stat_block_.HP + equipment_.total_hp(); }
        int str() const noexcept { return stat_block_.STR + equipment_.total_str(); }
        int wis() const noexcept { return stat_block_.WIS + equipment_.total_wis(); }
        int dex() const noexcept { return stat_block_.DEX + equipment_.total_dex(); }
        int damage_dice() const noexcept { return stat_block_.DAM_DICE + equipment_.total_dam(); }
        int hit_dice() const noexcept { return stat_block_.HIT_DICE; }
        int dodge() const noexcept { return stat_block_.DODGE; }
        int hit_chance() const noexcept { return stat_block_.HIT_CHANCE + equipment_.total_hit_chance(); }
        Room* currentRoom() const noexcept { return currentRoom_; }
        //Returns a reference (INSTEAD OF A COPY <-- HUGE issue)
        const std::vector<std::unique_ptr<Object>>& inventory() const noexcept { return inventory_; }
        std::mt19937& rng() noexcept { return rng_; } //pulls same random_engine
       
        /* Equip */
        // returns true on success, false if idx invalid or wrong type
        bool equip_helmet_at(std::size_t idx) noexcept;
        bool equip_chestpiece_at(std::size_t idx) noexcept;
        bool equip_leggings_at(std::size_t idx) noexcept;
        bool equip_boots_at(std::size_t idx) noexcept;
        bool equip_gloves_at(std::size_t idx) noexcept;
        bool equip_weapon_at(std::size_t idx) noexcept;

        /* Setters */
        void set_hp(int newHP) { stat_block_.HP = newHP; }
        /**
         * Used to add/subtract from Player HP
         */
        void alter_hp(int delta) { stat_block_.HP = std::max(0, stat_block_.HP + delta); }

        void set_str(int newSTR) { stat_block_.STR = newSTR; }
        /**
         * Used to add/subtract from Player STR
         */
        void alter_str(int delta) { stat_block_.STR = std::max(0, stat_block_.STR + delta); }

        void set_wis(int newWIS) { stat_block_.WIS = newWIS; }
        /**
         * Used to add/subtract from Player WIS
         */
        void alter_wis(int delta) { stat_block_.WIS = std::max(0, stat_block_.WIS + delta); }    

        void set_dex(int newDEX) { stat_block_.DEX = newDEX; }
        /**
         * Used to add/subtract from Player DEX
         */
        void alter_dex(int delta) { stat_block_.DEX = std::max(0, stat_block_.DEX + delta); }     

        void set_damage_dice(int newDAM_DICE) { stat_block_.DAM_DICE = newDAM_DICE; }
        /**
         * Used to add/subtract from Player DAMAGE_DICE
         */
        void alter_damage_dice(int delta) { stat_block_.DAM_DICE = std::max(0, stat_block_.DAM_DICE + delta); }      

        void set_hit_dice(int newHD) { stat_block_.HIT_DICE = newHD; }
        /**
         * Used to add/subtract from Player HIT_DICE
         */
        void alter_hit_dice(int delta) { stat_block_.HIT_DICE = std::max(0, stat_block_.HIT_DICE + delta); }

        void set_dodge(int newDODGE) { stat_block_.DODGE = newDODGE; }
        /**
         * Used to add/subtract from Player DODGE
         */
        void alter_dodge(int delta) { stat_block_.DODGE = std::max(0, stat_block_.DODGE + delta); }     

        void set_hit_chance(int newHC) { stat_block_.HIT_CHANCE = newHC; } 
        /**
         * Used to add/subtract from Player HIT_CHANCE
         */
        void alter_hit_chance(int delta) { stat_block_.HIT_CHANCE = std::max(0, stat_block_.HIT_CHANCE + delta); }

        void set_current_room(Room* r);

        Player() = default;

        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;



    private:
        /**
         * Contains the player stats as an object.
         */
        PLAYER_STATS stat_block_;
        /**
         * Vector of items that the player is carrying around.
         * Inventory owns the objects, taking care of any dangling pointers.
         */
        std::vector<std::unique_ptr<Object>> inventory_;
        /**
         * What the player currently has equipped.
         * An object of raw pointers to objects within Inventory.
         */
        PLAYER_EQUIPMENT equipment_;
        /**
         * RNG for dice rolling.
         */
        std::mt19937 rng_{std::random_device{}()};
        /**
         * The room the player is currently in.
         */
        Room* currentRoom_ {nullptr};
};  

