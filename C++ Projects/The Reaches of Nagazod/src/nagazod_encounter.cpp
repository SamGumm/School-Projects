#include "combat/nagazod_encounter.hpp"
#include "characters/nagazod_player.hpp"
#include "characters/nagazod_monster.hpp"

#include <algorithm>

Encounter::Encounter(Player& player, Monster& monster)
  : player_(player), monster_(monster)
{ }

bool Encounter::engage() {
    std::cout << "\n*** A wild " << monster_.description()
              << " appears! ***\n\n";

    // Combat continues until one side has HP ≤ 0
    while (player_.hp() > 0 && monster_.hp() > 0) {
        // ——— Player's Turn ———
        std::cout << "[You] attempt to hit (HitChance: " << player_.hit_chance()
                  << "%) vs [Monster] Dodge (" << monster_.dex() << ")\n";

        int attackRoll = player_.roll_attack();
        int dodgeRoll  = std::uniform_int_distribution<>(1, monster_.dex())(player_.rng());
        if (attackRoll > dodgeRoll) {
            int dmg = player_.roll_damage();
            std::cout << "  You hit for " << dmg << " damage!\n";
            monster_.take_damage(dmg);
        } else {
            std::cout << "  You miss!\n";
        }

        if (monster_.hp() <= 0) {
            std::cout << "\n*** You have slain the " 
                      << monster_.description() << "! ***\n\n";
            monster_.drop_loot();
            return true;
        }

        // ——— Monster's Turn ———
        std::cout << "[Monster] " << monster_.description()
                  << " attacks!\n";

        // monsters always hit for now 
        int mdmg = monster_.attack();
        std::cout << "  You take " << mdmg << " damage.\n";
        player_.alter_hp(-mdmg);
        std::cout << "  (Your HP: " << player_.hp() << ")\n";

        if (player_.hp() <= 0) {
            std::cout << "\n!!! You have been slain by the "
                      << monster_.description() << " !!!\n";
            return false;
        }

        std::cout << "\n-- Next Round --\n\n";
    }

    // Fallback (shouldn’t happen)
    return player_.hp() > 0;
}
