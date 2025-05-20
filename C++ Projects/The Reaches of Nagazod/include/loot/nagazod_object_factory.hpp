#pragma once

#include <memory>
#include "loot/armor/nagazod_armor.hpp"
#include "loot/weapons/nagazod_weapon.hpp"

/**
 * Structure containing all possible objects
 * Used to make unique_ptrs from object factory.
 * Possible objects:
 *  - [CLOTH, LEATHER, IRON, BASIC] Helmet
 *  - [CLOTH, LEATHER, IRON, BASIC] Chestpiece
 *  - [CLOTH, LEATHER, IRON, BASIC] Leggings
 *  - [CLOTH, LEATHER, IRON, BASIC] Boots
 *  - [CLOTH, LEATHER, IRON, BASIC] Gloves
 *  - [IRON] Dagger
 *  - [IRON] Mace
 *  - [IRON] Sword
 */
enum struct Object_ID {
    CLOTH_HELMET, CLOTH_CHESTPIECE, CLOTH_LEGGINGS, CLOTH_BOOTS, CLOTH_GLOVES,
    LEATHER_HELMET, LEATHER_CHESTPIECE, LEATHER_LEGGINGS, LEATHER_BOOTS, LEATHER_GLOVES,
    IRON_HELMET, IRON_CHESTPIECE, IRON_LEGGINGS, IRON_BOOTS, IRON_GLOVES,
    BASIC_HELMET, BASIC_CHESTPIECE, BASIC_LEGGINGS, BASIC_BOOTS, BASIC_GLOVES,
    IRON_DAGGER,
    IRON_MACE,
    IRON_SWORD
};

std::unique_ptr<Object> make_object(Object_ID id);
