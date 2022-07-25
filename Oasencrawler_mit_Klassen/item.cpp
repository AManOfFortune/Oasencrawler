#include "item.h"
#include "entity.h"

//Constructors
item::item(int rarity, char type, const std::string& name, const std::string& description)
{
	item::setRarity(rarity);
	item::setType(type);
	item::setName(name);
	item::setDescription(description);
}

item::item(int rarity, char type, const std::string& name, const std::string& description, void(*effectFunction)(entity* player, entity* enemies[])) : item(rarity, type, name, description)
{
	item::setFunction(effectFunction);
}

//Getters
int item::getRarity()
{
	return _rarity;
}

char item::getType()
{
	return _type;
}

std::string item::getName()
{
	return _name;
}

std::string item::getDescription()
{
	return _description;
}

//Setters
void item::setRarity(int rarity)
{
	_rarity = rarity;
}

void item::setType(char type)
{
	_type = type;
}

void item::setName(std::string name)
{
	_name = name;
}

void item::setDescription(std::string description)
{
	_description = description;
}

void item::setFunction(void(*effectFunction)(entity* player, entity* enemies[]))
{
	_effectFunction = effectFunction;
}

//Public functions
void item::useEffect(entity* player, entity* enemies[])
{
	_effectFunction(player, enemies);
}

item* item::clone()
{
	return new item(*this);
}
