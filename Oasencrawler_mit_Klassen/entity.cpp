#include "entity.h"
#include <iostream>
#include "config.h"

//Player Constructor
entity::entity(int row, int col, int HP, int relics, int level, int damage, int agility)
{
	entity::setPosition(row, col);
	entity::setHP(HP);
	entity::setRelics(relics);
	entity::setLevel(level);
	entity::setDamage(damage);
	entity::setAgility(agility);
}

//Enemy Constructor
entity::entity(int row, int col, int HP, int damage, char direction)
{
	entity::setPosition(row, col);
	entity::setHP(HP);
	entity::setDamage(damage);
	entity::setDirection(direction);
}

//Getter
int entity::getPositionCol()
{
	return _col;
}

int entity::getPositionRow()
{
	return _row;
}

int entity::getHP()
{
	return _HP;
}

int entity::getRelics()
{
	return _relics;
}

char entity::getDirection()
{
	return _direction;
}

int entity::getLevel()
{
	return _level;
}

int entity::getDamage()
{
	return _damage;
}

int entity::getAgility()
{
	return _agility;
}

//Setter
void entity::setPosition(int row, int col)
{
	if (row > WORLD_SIZE)
		_row = WORLD_SIZE;
	else if (row < 0)
		_row = 0;
	else
		_row = row;

	if (col > WORLD_SIZE)
		_col = WORLD_SIZE;
	else if (col < 0)
		_col = 0;
	else
		_col = col;
}

void entity::setHP(int HP)
{
	if (HP > MAX_HP_PLAYER)
		_HP = MAX_HP_PLAYER;
	else if (HP < 0)
		_HP = 0;
	else
		_HP = HP;
}

void entity::setRelics(int relics)
{
	_relics = relics;
}

void entity::setDirection(char direction)
{
	_direction = direction;
}

void entity::setLevel(int level)
{
	_level = level;
}

void entity::setDamage(int damage)
{
	if (damage > MAX_DMG_PLAYER)
		_damage = MAX_DMG_PLAYER;
	else if (damage < 0)
		_damage = 0;
	else
		_damage = damage;
}

void entity::setAgility(int agility)
{
	if (agility > MAX_AGI_PLAYER)
		_agility = MAX_AGI_PLAYER;
	else if (agility < 0)
		_agility = 0;
	else
		_agility = agility;
}

//Public functions
void entity::loseHP(int lostHP)
{
	setHP(_HP - lostHP);
}

void entity::gainHP(int gainedHP)
{
	setHP(_HP + gainedHP);
}

void entity::changeLevelBy(int changedLevel)
{
	setLevel(_level + changedLevel);
	setHP(_HP + changedLevel);
	setDamage(_damage + changedLevel);
	setAgility(_agility + changedLevel);
}

void entity::changeDamageBy(int changedDamage)
{
	setDamage(_damage + changedDamage);
}

void entity::changeAgilityBy(int changedAgility)
{
	setAgility(_agility + changedAgility);
}

void entity::changePositionBy(int rowAmount, int colAmount)
{
	setPosition(_row + rowAmount, _col + colAmount);
}
