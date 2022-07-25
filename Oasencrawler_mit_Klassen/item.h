#pragma once
#include <string>
#include "entity.h"

class item
{
public:
	//Constructor & Destructor
	item(int rarity, char type, const std::string& name, const std::string& description);
	item(int rarity, char type, const std::string& name, const std::string& description, void(*effectFunction)(entity* player, entity* enemies[]));

	//Getter & Setter
	int getRarity();
	char getType();
	std::string getName();
	std::string getDescription();

	void setRarity(int rarity);
	void setType(char type);
	void setName(std::string name);
	void setDescription(std::string description);
	void setFunction(void(*effectFunction)(entity* player, entity* enemies[]));

	//Public functions
	void useEffect(entity* player, entity* enemies[]);
	item* clone();

private:
	int _rarity;
	char _type;
	std::string _name;
	std::string _description;

	void(*_effectFunction)(entity* player, entity* enemies[]);
};

