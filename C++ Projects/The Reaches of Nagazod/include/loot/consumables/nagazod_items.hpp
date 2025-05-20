#pragma once

#include "loot/nagazod_object.hpp"

/**
 * All usable items to be found within the Dungeon.
 * Comes in 1 (so far) type of application:
 * - Drinkable
 */
enum struct Item_Application {
    DRINKABLE //Tip one back, these items are safe (sometimes) to drink.
};
class Item : public Object {
    public:
        /* Pre-Virtual Getters */
        virtual int hp() const = 0;

        /* Base methods shared by all weapons */
        std::string displayStatBlock() const {
            std::string StatBlock = desc_ + 
                                    " (value " + std::to_string(value_) +  
                                    ")" +
                                    " Recovered HP: " + std::to_string(hp());
            return StatBlock;
        }        
    
};
class Potion : public Item {
    int hp() const override {return 5;}
};
