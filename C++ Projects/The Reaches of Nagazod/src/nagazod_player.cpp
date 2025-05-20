#include "characters/nagazod_player.hpp"
#include "loot/nagazod_object_factory.hpp"
#include <iostream>
#include <iomanip> //std::setw (padding)

void Player::add_to_inventory(std::unique_ptr<Object> o) {
    if(o) inventory_.push_back(std::move(o));
}

void Player::remove_from_inventory(const Object* o)
{
    if (!o) return;

    auto item = std::find_if(inventory_.begin(), inventory_.end(),
                           [o](const std::unique_ptr<Object>& up) {
                               return up.get() == o;
                           });

    if (item != inventory_.end()) inventory_.erase(item);          // unique_ptr destructor deletes the item
}

void Player::display_inventory() const
{
    if (inventory_.empty()) {
        std::cout << "\n— Inventory is empty —\n";
        return;
    }

    std::cout << "\n=====  Inventory  =====\n";
    std::size_t idx = 0;
    for (const auto& up : inventory_) {               // up is const unique_ptr&
        std::cout << '[' << std::setw(2) << idx++ << "] "
                  << up->displayStatBlock() << '\n';  // raw ptr via operator->
    }
    std::cout << "=========================\n";
}


void Player::display_stats() {
    std::cout << "HP: " << hp() << "\n"
                << "STR: " << str() << "\n"
                << "WIS: " << wis() << "\n"
                << "DEX: " << dex() << "\n"
                << "DAMAGE: " << hit_dice() << "d" << damage_dice() << "\n"
                << "DODGE: " << dodge() << "\n"
                << "HIT_CHANCE: " << hit_chance() << std::endl;
};

// void Player::move(DIRECTION d)
// {
//     if (!currentRoom_) {
//         std::cout << "Player is not placed in the dungeon!\n";
//         return;
//     }

//     Room* next = currentRoom_->neighbor(d);

//     if (!next) {
//         std::cout << "You bump into a wall.\n";
//         return;
//     }

//     /* 1. leave the current room */
//     currentRoom_->contains_player(false);

//     /* 2. enter the new room */
//     currentRoom_ = next;
//     currentRoom_->contains_player(true);
//     currentRoom_->has_been_visited(true);   // mark for BFS etc.

//     // /* 3. feedback */
//     // std::cout << currentRoom_->description() << '\n';

//     // /* 4. initiate room events */
//     // switch(currentRoom_->roomtype()) {
//     //     case RoomType::START_ROOM : break;
//     //     case RoomType::CORRIDOR : break;
//     //     case RoomType::G_LOOT_ROOM : break;
//     //     case RoomType::R_LOOT_ROOM : break;
//     //     case RoomType::MONSTER_ROOM : break;
//     //     case RoomType::BOSS_ROOM : break;
//     //     case RoomType::EMPTY : break;
//     //     case RoomType::END_ROOM : break;
//     //     default: std::cout << "something has gone terribly wrong T_T" << std::endl;
//     // }
//     switch (currentRoom_->roomtype()) {
      
//         case RoomType::START_ROOM: {
//         //   std::cout << "You find yourself at the very entrance of the dungeon.\n";
//             std::cout << currentRoom_->description() << std::endl;
//             currentRoom_->print_room();
//         } break;
  
//         case RoomType::CORRIDOR: {
//           //std::cout << "A silent, torchlit corridor stretches before you.\n";
//             std::cout << currentRoom_->description() << std::endl;
//         } break;
  
//         case RoomType::G_LOOT_ROOM: {
//             std::cout << currentRoom_->description() << std::endl;
//             if(currentRoom_->has_been_visited()) {
//                 std::cout << "You've been here before already..." << std::endl;
//             } else {
//                 // pick one of a few high‐value items at random
//                 std::vector<Object_ID> chest = {
//                     Object_ID::IRON_SWORD,
//                     Object_ID::IRON_HELMET,
//                     //Object_ID::HEALTH_POTION
//                 };
//                 std::uniform_int_distribution<size_t> d(0, chest.size()-1);
//                 Object_ID found = chest[d(rng_)];
//                 auto item = make_object(found);
//                 std::cout << "You found: " << item->displayStatBlock() << "\n";
//                 add_to_inventory(std::move(item));
//             }
//             //std::cout << "You step into a glittering treasure chamber!\n";
            
            
//         } break;
  
//         case RoomType::R_LOOT_ROOM: {
//             //std::cout << "This side‐room holds some modest loot.\n";
//             currentRoom_->description();
//             if(currentRoom_->has_been_visited()) {
//                 std::cout << "Already been here" << std::endl;
//             } else {
//                 std::vector<Object_ID> stash = {
//                     Object_ID::CLOTH_BOOTS,
//                     Object_ID::CLOTH_LEGGINGS,
//                     //Object_ID::SMALL_MANA_POTION
//                 };
//                 std::uniform_int_distribution<size_t> d(0, stash.size()-1);
//                 Object_ID found = stash[d(rng_)];
//                 auto item = make_object(found);
//                 std::cout << "You pick up: " << item->displayStatBlock() << "\n";
//                 add_to_inventory(std::move(item));
//             }
//         } break;
  
//         case RoomType::MONSTER_ROOM: {
//             //std::cout << "A foul creature snarls and attacks!\n";
//             currentRoom_->description();

//             // placeholder for your combat routine
//             // e.g.   Combat c(*this, *currentRoom_->monster());
//             //        c.start();
//         } break;
  
//         case RoomType::BOSS_ROOM: {
//             std::cout << "The air trembles as the dungeon’s boss emerges!\n";
//             currentRoom_->description();
//             // e.g. startBossFight(currentRoom_->boss());
//         } break;
  
//         case RoomType::EMPTY: {
//           //std::cout << "An empty chamber—nothing of interest here.\n";
//             currentRoom_->description();
//         } break;
  
//         case RoomType::END_ROOM: {
//             //std::cout << "You see the stairs leading down to the next level.\n";
//             // e.g. dungeon_.goToNextFloor();
//             currentRoom_->description();
//         } break;
  
//         default: {
//             std::cout << "Something has gone terribly wrong T_T\n";
//         } break;
//       }
// }
void Player::move(DIRECTION d)
{
    if (!currentRoom_) {
        std::cout << "Player is not placed in the dungeon!\n";
        return;
    }

    // 1) Find the next room
    Room* next = currentRoom_->neighbor(d);
    if (!next) {
        std::cout << "You bump into a wall.\n";
        return;
    }

    // 2) Leave old room
    currentRoom_->contains_player(false);

    // 3) Enter new room
    currentRoom_ = next;
    currentRoom_->contains_player(true);
    
    // Mark visited the *first* time we arrive:
    bool firstTime = ! currentRoom_->has_been_visited();
    //currentRoom_->has_been_visited(true);

    // 4) Describe & handle the room
    switch (currentRoom_->roomtype()) {

      case RoomType::START_ROOM: {
        std::cout << currentRoom_->description() << "\n";
      } break;

      case RoomType::CORRIDOR: {
        std::cout << currentRoom_->description() << "\n";
      } break;

      case RoomType::G_LOOT_ROOM: {
        std::cout << currentRoom_->description() << "\n";
        if (!firstTime) {
          std::cout << "You've been here before already...\n";
        } else {
          // high-value loot
          std::vector<Object_ID> chest = {
            Object_ID::IRON_SWORD,
            Object_ID::IRON_HELMET,
            // Object_ID::HEALTH_POTION
          };
          std::uniform_int_distribution<size_t> dist(0, chest.size() - 1);
          Object_ID found = chest[dist(rng_)];
          auto item = make_object(found);
          std::cout << "You found: " << item->displayStatBlock() << "\n";
          add_to_inventory(std::move(item));
        }
      } break;

      case RoomType::R_LOOT_ROOM: {
        std::cout << currentRoom_->description() << "\n";
        if (!firstTime) {
          std::cout << "Already been here.\n";
        } else {
          std::vector<Object_ID> stash = {
            Object_ID::CLOTH_BOOTS,
            Object_ID::CLOTH_LEGGINGS,
            // Object_ID::SMALL_MANA_POTION
          };
          std::uniform_int_distribution<size_t> dist(0, stash.size() - 1);
          Object_ID found = stash[dist(rng_)];
          auto item = make_object(found);
          std::cout << "You pick up: " << item->displayStatBlock() << "\n";
          add_to_inventory(std::move(item));
        }
      } break;

      case RoomType::MONSTER_ROOM: {
        std::cout << currentRoom_->description() << "\n";
        // run your combat routine once:
        if (firstTime) {
          std::cout << "A foul creature snarls and attacks!\n";
          // Combat c(*this, *currentRoom_->monster());
          // c.start();
        } else {
          std::cout << "The corpses of your previous foe still lie scattered.\n";
        }
      } break;

      case RoomType::BOSS_ROOM: {
        std::cout << currentRoom_->description() << "\n";
        if (firstTime) {
          std::cout << "The dungeon’s boss emerges, roaring!\n";
          // startBossFight(...)
        } else {
          std::cout << "The silence of the fallen boss room hangs heavy.\n";
        }
      } break;

      case RoomType::EMPTY: {
        std::cout << currentRoom_->description() << "\n";
      } break;

      case RoomType::END_ROOM: {
        std::cout << currentRoom_->description() << "\n";
        std::cout << "Press '>' to descend the stairs.\n";
      } break;

      default: {
        std::cout << "Something has gone terribly wrong T_T\n";
      } break;
    }
}



/**
 * Spawn or teleport the player.
 */
void Player::set_current_room(Room* r) {
    if (!r) return;

    if(currentRoom_) currentRoom_->contains_player(false); //leaving old room

    /* Move to new room */
    currentRoom_ = r;

    /* Update graph */
    currentRoom_->contains_player(true);
    //currentRoom_->has_been_visited(true);
}

template<typename T>
static T* as_type(const std::vector<std::unique_ptr<Object>>& inv, std::size_t idx) {
    if (idx >= inv.size()) return nullptr;
    return dynamic_cast<T*>(inv[idx].get());
}

bool Player::equip_helmet_at(std::size_t idx) noexcept {
    if (Helmet* h = as_type<Helmet>(inventory_, idx)) {
        equipment_.helmet = h;
        return true;
    }
    return false;
}

bool Player::equip_chestpiece_at(std::size_t idx) noexcept {
    if (Chestpiece* c = as_type<Chestpiece>(inventory_, idx)) {
        equipment_.chestpiece = c;
        return true;
    }
    return false;
}

bool Player::equip_leggings_at(std::size_t idx) noexcept {
    if (Leggings* l = as_type<Leggings>(inventory_, idx)) {
        equipment_.leggings = l;
        return true;
    }
    return false;
}

bool Player::equip_boots_at(std::size_t idx) noexcept {
    if (Boots* b = as_type<Boots>(inventory_, idx)) {
        equipment_.boots = b;
        return true;
    }
    return false;
}

bool Player::equip_gloves_at(std::size_t idx) noexcept {
    if (Gloves* g = as_type<Gloves>(inventory_, idx)) {
        equipment_.gloves = g;
        return true;
    }
    return false;
}

bool Player::equip_weapon_at(std::size_t idx) noexcept {
    if (Weapon* w = as_type<Weapon>(inventory_, idx)) {
        equipment_.weapon = w;
        return true;
    }
    return false;
}

int Player::roll_attack() {
  // Roll from 1 up to  hit chance
  std::uniform_int_distribution<int> dist(1, hit_chance());
  return dist(rng());
}

int Player::roll_damage() {
  int total = 0;
  std::uniform_int_distribution<int> die(1, damage_dice());
  for (int i = 0; i < hit_dice(); ++i) {
      total += die(rng());
  }
  return total;
}

int Player::roll_dodge() {
  std::uniform_int_distribution<int> dist(1, dodge());
  return dist(rng());
}