#include "loot/weapons/nagazod_weapon.hpp"

/*======================== Weapon ========================*/
std::string Weapon::displayStatBlock() const {
    std::string StatBlock = desc_ + 
    " (value " + std::to_string(value_) +  
    ")" +
    " STR: " + std::to_string(strength()) +
    " WIL: " + std::to_string(wis()) +
    " DEX: " + std::to_string(dex()) +
    " DMG: " + std::to_string(dam()) +
    " Hit%: " + std::to_string(hit_chance());
    return StatBlock;
}

/*======================== Dagger ========================*/

/**
 * Stat Table for Dagger Materials
 */
Material_Stats_Weapon setStatsForDagger(Weapon_Material m) {
    switch (m) {
        case Weapon_Material::IRON: return {0, 0, 2, 1, 25, "Simple Iron Dagger", 5};
        default: return {0, 0, 0, 0, 0, "A broken, unidentifiable dagger", 0};
    }
}

/**
 * Explicit Convenience Constructor for Dagger
 */
Dagger::Dagger(Weapon_Material m) : daggerStats_{setStatsForDagger(m)} {
    desc_ = daggerStats_.desc;
    value_ = daggerStats_.value;
}


/*======================== Mace ========================*/

/**
 * Stat Table for Mace Materials
 */
Material_Stats_Weapon setStatsForMace(Weapon_Material m) {
    switch (m) {
        case Weapon_Material::IRON: return {0, 3, 0, 1, 15, "Simple Iron Mace", 6};
        default: return {0, 0, 0, 0, 0, "A broken, unidentifiable mace", 0};
    }
}

/**
 * Explicit Convenience Constructor for Mace
 */
Mace::Mace(Weapon_Material m) : maceStats_{setStatsForMace(m)} {
    desc_ = maceStats_.desc;
    value_ = maceStats_.value;
}



/*======================== Sword ======================== */

/**
 * Stat Table for Sword Materials
 */
Material_Stats_Weapon setStatsForSword(Weapon_Material m) {
    switch (m) {
        case Weapon_Material::IRON: return {1, 0, 0, 3, 20, "Simple Iron Sword", 8};
        default: return {0, 0, 0, 0, 0, "A broken, unidentifiable sword", 0};
    }
}

/**
 * Explicit Convenience Constructor for Sword
 */
Sword::Sword(Weapon_Material m) : swordStats_{setStatsForSword(m)} {
    desc_ = swordStats_.desc;
    value_ = swordStats_.value;
}