/**
 * There will be an Object base class
 *  - With derived classes:
 *      - Weapon:
 *          - Sword
 *          - Dagger
 *          - Mace
 *      - Armor
 *          - Helmet:
 *              - Bandanna
 *              - Leather Helmet
 *              - Iron Helmet
 *          - Chestpiece
 *              - Shirt
 *              - Leather Chestpiece
 *              - Iron Chestpiece 
 *          - Leggings
 *              - Pants
 *              - Leather Pants
 *              - Iron Grieves
 *          - Gloves
 *              - Fingerless Gloves
 *              - Leather Gloves
 *              - Iron Gauntlets
 *          - Boots
 *              - Shoes
 *              - Leather Boots
 *              - Iron Boots  
 *      - Item 
 *          - Potion
 *          - ...
 */

#ifndef NAGAZOD_OBJECT_HPP
#define NAGAZOD_OBJECT_HPP

#include <string>
#include <vector>

/**
 * Base class of all loot found within the Dungeon.
 */
class Object {
    public:
        ~Object() = default;

    protected:
        std::string desc_;
        int value_{0};
        static constexpr int baseSTR_{0};
        static constexpr int baseWIS_{0};
        static constexpr int baseDEX_{0};

};

#endif