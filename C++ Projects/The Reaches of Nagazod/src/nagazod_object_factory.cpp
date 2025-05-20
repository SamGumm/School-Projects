#include "loot/nagazod_object_factory.hpp"

std::unique_ptr<Object> make_object(Object_ID id) {
    switch(id) {
        //ARMOR

        //BASIC
        case Object_ID::BASIC_HELMET: return std::make_unique<Helmet>(Armor_Material::BASIC);
        case Object_ID::BASIC_CHESTPIECE: return std::make_unique<Chestpiece>(Armor_Material::BASIC); 
        case Object_ID::BASIC_LEGGINGS: return std::make_unique<Leggings>(Armor_Material::BASIC); 
        case Object_ID::BASIC_BOOTS: return std::make_unique<Boots>(Armor_Material::BASIC); 
        case Object_ID::BASIC_GLOVES: return std::make_unique<Gloves>(Armor_Material::BASIC); 
        //CLOTH
        case Object_ID::CLOTH_HELMET: return std::make_unique<Helmet>(Armor_Material::CLOTH); 
        case Object_ID::CLOTH_CHESTPIECE: return std::make_unique<Chestpiece>(Armor_Material::CLOTH); 
        case Object_ID::CLOTH_LEGGINGS: return std::make_unique<Leggings>(Armor_Material::CLOTH); 
        case Object_ID::CLOTH_BOOTS: return std::make_unique<Boots>(Armor_Material::CLOTH); 
        case Object_ID::CLOTH_GLOVES: return std::make_unique<Gloves>(Armor_Material::CLOTH); 
        //LEATHER
        case Object_ID::LEATHER_HELMET: return std::make_unique<Helmet>(Armor_Material::LEATHER); 
        case Object_ID::LEATHER_CHESTPIECE: return std::make_unique<Chestpiece>(Armor_Material::LEATHER); 
        case Object_ID::LEATHER_LEGGINGS: return std::make_unique<Leggings>(Armor_Material::LEATHER); 
        case Object_ID::LEATHER_BOOTS: return std::make_unique<Boots>(Armor_Material::LEATHER);  
        case Object_ID::LEATHER_GLOVES: return std::make_unique<Gloves>(Armor_Material::LEATHER); 
        //IRON
        case Object_ID::IRON_HELMET: return std::make_unique<Helmet>(Armor_Material::IRON); 
        case Object_ID::IRON_CHESTPIECE: return std::make_unique<Chestpiece>(Armor_Material::IRON); 
        case Object_ID::IRON_LEGGINGS: return std::make_unique<Leggings>(Armor_Material::IRON); 
        case Object_ID::IRON_BOOTS: return std::make_unique<Boots>(Armor_Material::IRON); 
        case Object_ID::IRON_GLOVES: return std::make_unique<Gloves>(Armor_Material::IRON); 
        

        //WEAPONS

        //IRON
        case Object_ID::IRON_DAGGER: return std::make_unique<Dagger>(Weapon_Material::IRON); 
        case Object_ID::IRON_MACE: return std::make_unique<Mace>(Weapon_Material::IRON); 
        case Object_ID::IRON_SWORD: return std::make_unique<Sword>(Weapon_Material::IRON); 

        //DEFAULT
        default: return {nullptr}; 
    }; 
};