#ifndef NAGAZOD_ARMOR_HPP
#define NAGAZOD_AMOR_HPP

#include "loot/nagazod_object.hpp"

/* ARMOR */
/**
 * All wearable armor and clothes to be found within the Dungeon.
 * Comes in 3 (so far) types of material(s):
 * - Cloth (HP+, WIS+)
 * - Leather (HP+, DEX+)
 * - Iron (HP+, STR+)
 */
enum struct Armor_Material {
    CLOTH, //cheapest, most easily found. Light and breathable, increases WIL
    LEATHER, //Middle-of-the-road pricing, can be found sometimes. Sleek and well-kept, increases DEX
    IRON //Most expensive, hard to find (something must be eating it...). Durable and hardy, increases STR
};

/**
 * Stat block for the materials armor is made out of:
 * - HP
 * - STR
 * - WIS
 * - DEX
 */
struct Material_Stats_Armor {
    int hpAdd, strAdd, wisAdd, dexAdd;
    std::string desc;
    int value;
};

class Armor : public Object {
    public:
        /* Pre-Virtual Getters */

        virtual int hp() const = 0;
        virtual int strength() const = 0;
        virtual int wis() const = 0;
        virtual int dex() const = 0;


        /* Base stats shared by all armor */

        static constexpr int baseHP_ = 0;

        
        /* Base methods shared by all armor */

        std::string displayStatBlock() const;
};

/* Helmet */
inline Material_Stats_Armor setStatsForHelmet(Armor_Material m);

class Helmet : public Armor {
    public:
        Helmet(Armor_Material m);

        int hp() const override { return baseHP_ + helmetStats_.hpAdd; }
        int strength() const override { return baseSTR_ + helmetStats_.strAdd; }
        int wis() const override { return baseWIS_ + helmetStats_.wisAdd; }
        int dex() const override { return baseDEX_ + helmetStats_.dexAdd; }

    private:
        const Material_Stats_Armor helmetStats_;
};

/* Chestpiece */
inline Material_Stats_Armor setStatsForChestpiece(Armor_Material m);

class Chestpiece : public Armor {
    public:
        Chestpiece(Armor_Material m);

        int hp() const override { return baseHP_ + chestpieceStats_.hpAdd; }
        int strength() const override { return baseSTR_ + chestpieceStats_.strAdd; }
        int wis() const override { return baseWIS_ + chestpieceStats_.wisAdd; }
        int dex() const override { return baseDEX_ + chestpieceStats_.dexAdd; }

    private:
        const Material_Stats_Armor chestpieceStats_;
};

/* Leggings */
inline Material_Stats_Armor setStatsForLeggings(Armor_Material m);

class Leggings : public Armor {
    public:
        Leggings(Armor_Material m);

        int hp() const override { return baseHP_ + leggingsStats_.hpAdd; }
        int strength() const override { return baseSTR_ + leggingsStats_.strAdd; }
        int wis() const override { return baseWIS_ + leggingsStats_.wisAdd; }
        int dex() const override { return baseDEX_ + leggingsStats_.dexAdd; }

    private:
        const Material_Stats_Armor leggingsStats_;
};

/* Gloves */
inline Material_Stats_Armor setStatsForGloves(Armor_Material m);

class Gloves : public Armor {
    public:
        Gloves(Armor_Material m);

        int hp() const override { return baseHP_ + glovesStats_.hpAdd; }
        int strength() const override { return baseSTR_ + glovesStats_.strAdd; }
        int wis() const override { return baseWIS_ + glovesStats_.wisAdd; }
        int dex() const override { return baseDEX_ + glovesStats_.dexAdd; }

    private:
        const Material_Stats_Armor glovesStats_;
};

/* Boots */
inline Material_Stats_Armor setStatsForBoots(Armor_Material m);

class Boots : public Armor {
    public:
        Boots(Armor_Material m);

        int hp() const override { return baseHP_ + bootsStats_.hpAdd; }
        int strength() const override { return baseSTR_ + bootsStats_.strAdd; }
        int wis() const override { return baseWIS_ + bootsStats_.wisAdd; }
        int dex() const override { return baseDEX_ + bootsStats_.dexAdd; }

    private:
        const Material_Stats_Armor bootsStats_;
};

#endif