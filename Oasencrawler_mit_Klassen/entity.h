#pragma once

class entity
{
public:
	//Constructor & Destructor
	entity(int row, int col, int HP, int relics, int level, int damage, int agility);
	entity(int row, int col, int HP, int damage, char direction);

	//Getter & Setter
	int getPositionCol();
	int getPositionRow();
	int getHP();
	int getRelics();
	char getDirection();
	int getLevel();
	int getDamage();
	int getAgility();

	void setPosition(int row, int col);
	void setHP(int HP);
	void setRelics(int relics);
	void setDirection(char direction);
	void setLevel(int level);
	void setDamage(int damage);
	void setAgility(int agility);

	//Public functions
	void loseHP(int lostHP);
	void gainHP(int gainedHP);
	void changeLevelBy(int changedLevel);
	void changeDamageBy(int changedDamage);
	void changeAgilityBy(int changedAgility);
	void changePositionBy(int rowAmount, int colAmount);

private:
	int _row;
	int _col;
	char _direction;

	int _HP;
	int _relics;

	int _level;
	int _damage;
	int _agility;
};

