#include "objects.hpp"

// Utility to turn an ObjectType into a string
std::string Object::equip_slot_name() const {
  switch (slot) {
    case ObjectType::WEAPON:  return "WEAPON";
    case ObjectType::OFFHAND: return "OFFHAND";
    case ObjectType::RANGED:  return "RANGED";
    case ObjectType::ARMOR:   return "ARMOR";
    case ObjectType::HELMET:  return "HELMET";
    case ObjectType::CLOAK:   return "CLOAK";
    case ObjectType::GLOVES:  return "GLOVES";
    case ObjectType::BOOTS:   return "BOOTS";
    case ObjectType::AMULET:  return "AMULET";
    case ObjectType::LIGHT:   return "LIGHT";
    case ObjectType::RING:    return "RING";
    default:                  return "UNKNOWN";
  }
}
