#include "objects.hpp"

// slot characters: weapon='w', offhand='o', etc.
ObjectType slot_to_object_type(char slot) {
  switch (slot) {
    case 'w': return ObjectType::WEAPON;
    case 'o': return ObjectType::OFFHAND;
    case 'r': return ObjectType::RANGED;
    case 'a': return ObjectType::ARMOR;
    case 'h': return ObjectType::HELMET;
    case 'c': return ObjectType::CLOAK;
    case 'g': return ObjectType::GLOVES;
    case 'b': return ObjectType::BOOTS;
    case 'm': return ObjectType::AMULET;
    case 'l': return ObjectType::LIGHT;
    case 'n':                // pick one of the two ring slots a-l
    case 'p':                // second ring slot
      return ObjectType::RING;
    default:
      return ObjectType::UNKNOWN;
  }
}
