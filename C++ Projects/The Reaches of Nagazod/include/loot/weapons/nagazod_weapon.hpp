#pragma once

#include "loot/nagazod_object.hpp"

/*======================== WEAPONS ========================*/

/**
 * Weapons to be found within the Dungeon.
 * Comes in 1 (so far) type of material(s)
 * - Iron
 */
enum struct Weapon_Material {
    IRON
};

struct Material_Stats_Weapon {
    int strAdd {0}; 
    int wisAdd {0}; 
    int dexAdd {0}; 
    int damAdd {0}; 
    int hcAdd {0};
    std::string desc {};
    int value {0};
};

class Weapon : public Object {
    public:
        /*Pre-Virtual Getters*/
        virtual int strength() const = 0;
        virtual int wis() const = 0;
        virtual int dex() const = 0;
        virtual int dam() const = 0;
        virtual int hit_chance() const = 0;
        
        /* Base stats shared by all weapons */
        //Creates ONE compile-time constant shared by all weapons
        static constexpr int baseDAM_ = 0;
        static constexpr int baseHC_ = 0;
        
        /* Base methods shared by all weapons */
        std::string displayStatBlock() const;
};


/*======================== Dagger ========================*/

/**
 * Stat Table for Dagger Materials.
 */
inline Material_Stats_Weapon setStatsForDagger(Weapon_Material m);

class Dagger : public Weapon {
    public:
        /* Constructor */

        Dagger(Weapon_Material m);

        
        /* Getters */

        int strength() const override { return baseSTR_ + daggerStats_.strAdd; }
        int wis() const override { return baseWIS_ + daggerStats_.wisAdd; }
        int dex() const override { return baseDEX_ + daggerStats_.dexAdd; }
        int dam() const override { return baseDAM_ + daggerStats_.damAdd; }
        int hit_chance() const override { return baseHC_ + daggerStats_.hcAdd; }

    private:
    /* Per-model bonuses default */
    //Currently, dagger bonuses will never change after construction, thus const
        const Material_Stats_Weapon daggerStats_;

};


/*======================== Mace ========================*/

/**
 * Stat Table for Mace Materials.
 */
inline Material_Stats_Weapon setStatsForMace(Weapon_Material m);

class Mace : public Weapon {
    public:
        /* Constructor */
        
        Mace(Weapon_Material m);

        int strength() const override { return baseSTR_ + maceStats_.strAdd; }
        int wis() const override { return baseWIS_ + maceStats_.wisAdd; }
        int dex() const override { return baseDEX_ + maceStats_.dexAdd; }
        int dam() const override { return baseDAM_ + maceStats_.damAdd; }
        int hit_chance() const override { return baseHC_ + maceStats_.hcAdd; }

    private:
        const Material_Stats_Weapon maceStats_;
};


/*======================== Sword ========================*/

/**
 * Stat Table for Sword Materials.
 */
inline Material_Stats_Weapon setStatsForSword(Weapon_Material m);

class Sword : public Weapon {
    public:
        /* Constructor*/
        Sword(Weapon_Material m);

        int strength() const override { return baseSTR_ + swordStats_.strAdd; }
        int wis() const override { return baseWIS_ + swordStats_.wisAdd; }
        int dex() const override { return baseDEX_ + swordStats_.dexAdd; }
        int dam() const override { return baseDAM_ + swordStats_.damAdd; }
        int hit_chance() const override { return baseHC_ + swordStats_.hcAdd; }

    private:
    /* Per-model bonuses default */
        const Material_Stats_Weapon swordStats_;
};
