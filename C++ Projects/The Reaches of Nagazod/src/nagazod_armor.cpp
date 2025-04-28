#include "loot/armor/nagazod_armor.hpp"

/*======================== Armor ========================*/
std::string Armor::displayStatBlock() const {
    std::string StatBlock = desc_ + 
    " (value " + std::to_string(value_) +  
    ")" +
    " HP: " + std::to_string(hp()) +
    " STR: " + std::to_string(strength()) +
    " WIS: " + std::to_string(wis()) +
    " DEX: " + std::to_string(dex());
    return StatBlock;
}

/*======================== Helmet ========================*/

/**
 * Stat Table for Helmet Materials
 */
Material_Stats_Armor setStatsForHelmet(Armor_Material m) {
    switch (m) {                                /* hp,str,wis,dex,desc,value */
        case Armor_Material::CLOTH: return {2, 0, 2, 0, "A comfortable cloth bandanna", 2};
        case Armor_Material::LEATHER: return {3, 0, 0, 2, "A supple leather cap", 4};
        case Armor_Material::IRON: return {4, 2, 0, 0, "A sturdy iron helmet", 6};
        default: return {0, 0, 0, 0, "A burnt, unidentifiable helmet", 0};
    }
}

Helmet::Helmet(Armor_Material m) : helmetStats_{setStatsForHelmet(m)} {
    desc_ = helmetStats_.desc;
    value_ = helmetStats_.value;
}

/*======================== Chestpiece ========================*/

/**
 * Stat Table for Chestpiece Materials
 */
Material_Stats_Armor setStatsForChestpiece(Armor_Material m) {
    switch (m) {                                /* hp,str,wis,dex,desc,value */
        case Armor_Material::CLOTH: return {2, 0, 2, 0, "A comfortable cloth shirt", 2};
        case Armor_Material::LEATHER: return {3, 0, 0, 2, "A supple leather gambison", 4};
        case Armor_Material::IRON: return {6, 2, 0, 0, "A sturdy iron chestplate", 6};
        default: return {0, 0, 0, 0, "A broken, unidentifiable chestpiece", 0};
    }
}

Chestpiece::Chestpiece(Armor_Material m) : chestpieceStats_{setStatsForChestpiece(m)} {
    desc_ = chestpieceStats_.desc;
    value_ = chestpieceStats_.value;
}

/*======================== Leggings ========================*/

/**
 * Stat Table for Leggings Materials
 */
Material_Stats_Armor setStatsForLeggings(Armor_Material m) {
    switch (m) {                                /* hp,str,wis,dex,desc,value */
        case Armor_Material::CLOTH: return {2, 0, 2, 0, "A comfortable pair of cloth pants", 2};
        case Armor_Material::LEATHER: return {3, 0, 0, 2, "A pair of supple leather leggings", 4};
        case Armor_Material::IRON: return {4, 2, 0, 0, "A sturdy pair of iron greaves", 6};
        default: return {0, 0, 0, 0, "A tattered, unidentifiable pair of pants", 0};
    }
}

Leggings::Leggings(Armor_Material m) : leggingsStats_{setStatsForLeggings(m)} {
    desc_ = leggingsStats_.desc;
    value_ = leggingsStats_.value;
}

/*======================== Gloves ========================*/

/**
 * Stat Table for Gloves Materials
 */
Material_Stats_Armor setStatsForGloves(Armor_Material m) {
    switch (m) {                                /* hp,str,wis,dex,desc,value */
        case Armor_Material::CLOTH: return {2, 0, 2, 0, "A pair of fingerless cloth gloves", 2};
        case Armor_Material::LEATHER: return {3, 0, 0, 2, "A pair of supple leather gloves", 4};
        case Armor_Material::IRON: return {4, 2, 0, 0, "A sturdy pair of iron gauntlets", 6};
        default: return {0, 0, 0, 0, "A shredded, unidentifiable pair of gloves", 0};
    }
}

Gloves::Gloves(Armor_Material m) : glovesStats_{setStatsForGloves(m)} {
    desc_ = glovesStats_.desc;
    value_ = glovesStats_.value;
}

/*======================== Boots ========================*/

/**
 * Stat Table for Boots Materials
 */
Material_Stats_Armor setStatsForBoots(Armor_Material m) {
    switch (m) {                                /* hp,str,wis,dex,desc,value */
        case Armor_Material::CLOTH: return {2, 0, 2, 0, "A worn pair of boots", 2};
        case Armor_Material::LEATHER: return {3, 0, 0, 2, "A pair of supple leather boots", 4};
        case Armor_Material::IRON: return {4, 2, 0, 0, "A sturdy pair of iron-plated boots", 6};
        default: return {0, 0, 0, 0, "A ripped, unidentifiable pair of boots", 0};
    }
}

Boots::Boots(Armor_Material m) : bootsStats_{setStatsForBoots(m)} {
    desc_ = bootsStats_.desc;
    value_ = bootsStats_.value;
}