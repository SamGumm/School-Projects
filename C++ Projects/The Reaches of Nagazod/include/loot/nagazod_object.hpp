#pragma once

#include <string>
#include <vector>

/**
 * Base class of all loot found within the Dungeon.
 */
class Object {
    public:
        ~Object() = default;
        virtual std::string displayStatBlock() const = 0;
    
    protected:
        std::string desc_;
        int value_{0};
        static constexpr float baseSTR_{0};
        static constexpr float baseWIS_{0};
        static constexpr float baseDEX_{0};
};
