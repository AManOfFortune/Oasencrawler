#include <iostream>
#include <time.h>
#include <windows.h>
#include <string>

//All defines are inside config.h
#include "config.h"
//Custom entity class
#include "entity.h"
//Custom item class
#include "item.h"

//The characters used for each field type
enum fields {
    EMPTY = ' ',
    DANGER = 'X',
    WELL = '@',
    RELIC = '$'
};

//The characters used to show an enemies' direction
enum enemyDirection {
    UP = '^',
    DOWN = 'v',
    RIGHT = '>',
    LEFT = '<'
};

//Various check functions
namespace check {
    //Checks if a relic exists in the world
    bool relicExists(char world[WORLD_SIZE][WORLD_SIZE])
    {
        bool relicExists = false;
        for (int row = 0; row < WORLD_SIZE; row++) {
            for (int col = 0; col < WORLD_SIZE; col++) {
                if (world[row][col] == '$') {
                    relicExists = true;
                    break;
                }
            }

            if (relicExists)
                break;
        }

        return relicExists;
    }

    //Checks if a given enemy walks into a wall or not
    bool enemyDirectionPossible(char world[WORLD_SIZE][WORLD_SIZE], entity* enemy)
    {
        switch (enemy->getDirection()) {
        case UP:
            if (enemy->getPositionRow() - 1 < 0) {
                return false;
            }
            break;
        case DOWN:
            if (enemy->getPositionRow() + 1 >= WORLD_SIZE) {
                return false;
            }
            break;
        case RIGHT:
            if (enemy->getPositionCol() + 1 >= WORLD_SIZE) {
                return false;
            }
            break;
        case LEFT:
            if (enemy->getPositionCol() - 1 < 0) {
                return false;
            }
            break;
        }

        return true;
    }

    //Checks if an enemy exists at a given position, returns it's index or -1 if it does not exist
    int enemyExistsAt(entity* enemies[], int* numOfEnemies, int row, int col)
    {
        for (int i = 0; i < *numOfEnemies; i++) {
            if (enemies[i]->getPositionCol() == col && enemies[i]->getPositionRow() == row) {
                return i;
            }
        }

        return -1;
    }
}

//All functions concerning the world map
namespace world {
    void buildWorld(char world[WORLD_SIZE][WORLD_SIZE])
    {
        for (int row = 0; row < WORLD_SIZE; row++)
        {
            for (int col = 0; col < WORLD_SIZE; col++)
            {
                //Create a random value from 1 - 100 for each world field
                int randVal = rand() % 100;

                //Empty field with 40% chance
                if (randVal < 40)
                {
                    world[row][col] = EMPTY;
                }
                //Danger field with 40% chance
                else if (randVal < 80)
                {
                    world[row][col] = DANGER;
                }
                //Well field with 10% chance
                else if (randVal < 90)
                {
                    world[row][col] = WELL;
                }
                //Relic field with 10% chance
                else
                {
                    world[row][col] = RELIC;
                }
            }
        }
    }

    void printWorld(char world[WORLD_SIZE][WORLD_SIZE], entity* player, entity* enemies[], int* numOfEnemies)
    {
        //For every row
        for (int row = 0; row < WORLD_SIZE; row++) {

            //Print one gridline
            for (int i = 0; i < WORLD_SIZE; i++) {
                std::cout << "+---";
            }
            std::cout << "+" << std::endl;

            //Print one line with world row values and vertical seperators
            for (int col = 0; col < WORLD_SIZE; col++) {
                std::cout << "| ";

                int existingEnemyIndex = check::enemyExistsAt(enemies, numOfEnemies, row, col);

                if (col == player->getPositionCol() && row == player->getPositionRow()) {
                    std::cout << GREEN_TEXT << "0" << RESET;
                }
                else if (existingEnemyIndex != -1) {
                    std::cout << RED_TEXT << enemies[existingEnemyIndex]->getDirection() << RESET;
                }
                else {
                    if (world[row][col] == RELIC) {
                        std::cout << YELLOW_TEXT << world[row][col] << RESET;
                    }
                    else {
                        std::cout << world[row][col];
                    }
                }

                std::cout << " ";
            }
            std::cout << "|" << std::endl;
        }

        //Print the last closing gridline
        for (int i = 0; i < WORLD_SIZE; i++) {
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;
    }

    void removeField(char world[WORLD_SIZE][WORLD_SIZE], entity* player)
    {
        world[player->getPositionRow()][player->getPositionCol()] = EMPTY;
    }
}

//Functions concerning items
namespace items {
    void sortedInsertOfItem(item* items[], int* itemsCount, item* item)
    {
        //If type is consumeable, simply add the item at the end
        if (item->getType() == 'c') {
            items[*itemsCount] = item;
        }
        //If the item is equipment, shift all consumeables to the right, then add item
        else {
            //Find out where the consumeables start
            int positionToInsert = 0;
            for (int i = 0; i < *itemsCount; i++) {
                if (items[i]->getType() != item->getType()) {
                    positionToInsert = i;
                    break;
                }
            }

            //Shift consumeables
            for (int i = *itemsCount; i > positionToInsert; i--) {
                items[i] = items[i - 1];
            }
            //Insert item
            items[positionToInsert] = item;
        }
    }

    void addItem(item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, entity* player, entity* enemies[])
    {
        system("cls");

        int percentage = rand() % 100;
        int rarity = 0;
        std::string rarityText = "Common";

        if (percentage < 50) {
            rarity = 0;
            rarityText = "Common";
        }
        else if (percentage < 85) {
            rarity = 1;
            rarityText = "Rare";
        }
        else {
            rarity = 2;
            rarityText = "Legendary";
        }

        int randomItemNumber = rand() % NUMBER_OF_ITEMS_PER_RARITY;

        std::string typeText = allItems[rarity][randomItemNumber]->getType() == 'e' ? "Equipment" : "Consumeable";

        std::cout << "You found an Item: " << allItems[rarity][randomItemNumber]->getName() << "\n" << std::endl;
        std::cout << "Rarity: " << rarityText << std::endl;
        std::cout << "Type: " << typeText << "\n" << std::endl;
        std::cout << "\"" << allItems[rarity][randomItemNumber]->getDescription() << "\"\n" << std::endl;
        
        items::sortedInsertOfItem(items, itemsCount, allItems[rarity][randomItemNumber]->clone());

        if (allItems[rarity][randomItemNumber]->getType() == 'e') {
            allItems[rarity][randomItemNumber]->useEffect(player, enemies);
        }
        *itemsCount += 1;

        system("pause");
        system("cls");
    }

    void removeItem(item* items[], int* itemsCount, int indexToRemove)
    {
        *itemsCount -= 1;
        delete items[indexToRemove];

        for (int i = indexToRemove; i < *itemsCount; i++) {
            items[i] = items[i + 1];
        }
    }

    void showItems(item* items[], int* itemsCount, entity* player, entity* enemies[])
    {
        system("cls");

        if (*itemsCount <= 0) {
            std::cout << "You have no items!\n" << std::endl;
            system("pause");
        }
        else {
            std::cout << "Equipment:" << std::endl;
            for (int i = 0; i < *itemsCount; i++) {
                if (items[i]->getType() == 'e') {
                    std::cout << std::to_string(i + 1) << ". " << items[i]->getName() << ": " << items[i]->getDescription() << std::endl;
                }
            }

            bool hasConsumables = false;
            std::cout << "\nConsumables:" << std::endl;
            for (int i = 0; i < *itemsCount; i++) {
                if (items[i]->getType() == 'c') {
                    std::cout << std::to_string(i + 1) << ". " << items[i]->getName() << ": " << items[i]->getDescription() << std::endl;
                    hasConsumables = true;
                }
            }

            if (!hasConsumables) {
                std::cout << "You have no consumables.\n" << std::endl;
                system("pause");
            }
            else {
                char choice = 'x';
                int useNumber = *itemsCount;

                bool exit = false;
                while (!exit) {
                    std::cout << "\n(u) Use Consumable - (x) Exit" << std::endl;
                    std::cin >> choice;

                    switch (choice) {
                    case 'u':
                        do {
                            std::cout << "Use Number: ";
                            std::cin >> useNumber;
                        } while ((useNumber > *itemsCount || useNumber < 1) || items[useNumber - 1]->getType() != 'c');

                        items[useNumber - 1]->useEffect(player, enemies);
                        items::removeItem(items, itemsCount, useNumber - 1);
                        exit = true;
                        break;
                    case 'x':
                        exit = true;
                        break;
                    default:
                        std::cout << "Invalid input" << std::endl;
                        break;
                    }
                }
            }
        }

        system("cls");
    }

    void removeAllItems(item* allItems[][NUMBER_OF_ITEMS_PER_RARITY])
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < NUMBER_OF_ITEMS_PER_RARITY; j++) {
                delete allItems[i][j];
            }
        }
    }

    void removeAllGainedItems(item* items[], int* itemsCount) {
        for (int i = 0; i < *itemsCount; i++) {
            delete items[i];
        }
    }

    void printAllItems(item* allItems[][NUMBER_OF_ITEMS_PER_RARITY])
    {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < NUMBER_OF_ITEMS_PER_RARITY; j++) {
                std::cout << "Item: " << allItems[i][j]->getName() << ", " << std::to_string(allItems[i][j]->getRarity()) << ", '" << allItems[i][j]->getDescription() << "', " << allItems[i][j]->getType() << std::endl;
            }
        }
    }
}

//All functions associated with enemies
namespace enemies {
    //Deletes all enemy classes remaining in array
    void freeAllEnemies(entity* enemies[], int* numOfEnemies)
    {
        for (int i = 0; i < *numOfEnemies; i++) {
            delete enemies[i];
        }
    }

    //Moves all enemies in their specified direction
    void moveEnemies(entity* enemies[], int* numOfEnemies) {
        for (int i = 0; i < *numOfEnemies; i++) {
            switch (enemies[i]->getDirection()) {
            case UP:
                enemies[i]->changePositionBy(-1, 0);
                break;
            case DOWN:
                enemies[i]->changePositionBy(1, 0);
                break;
            case RIGHT:
                enemies[i]->changePositionBy(0, 1);
                break;
            case LEFT:
                enemies[i]->changePositionBy(0, -1);
                break;
            }
        }
    }

    //Checks if a given enemy will collide with the other enemies if everyone moves in their current direction
    bool collisionWithNewEnemy(entity* enemies[], entity* enemy, char world[WORLD_SIZE][WORLD_SIZE], int* numOfEnemies)
    {
        //Creates a copy of the enemies
        entity* enemiesCopy[MAX_NUM_OF_ENEMIES]{ NULL };

        int i = 0;
        for (i = 0; i < *numOfEnemies; i++) {
            entity* enemyCopy = new entity(enemies[i]->getPositionRow(), enemies[i]->getPositionCol(), enemies[i]->getHP(), enemies[i]->getDamage(), enemies[i]->getDirection());
            enemiesCopy[i] = enemyCopy;
        }
        //Adds the new enemy as the last entry in the copy array
        entity* enemyCopy = new entity(enemy->getPositionRow(), enemy->getPositionCol(), enemy->getHP(), enemy->getDamage(), enemy->getDirection());
        enemiesCopy[i] = enemyCopy;

        //The enemies in the copy do their next move (the +1 is necessary because we added the new enemy at the end)
        int numOfEnemiesCopy = *numOfEnemies + 1;
        enemies::moveEnemies(enemiesCopy, &numOfEnemiesCopy);

        //Checks if the new enemy has the same position as an old one, returns true if so
        for (int c = 0; c < *numOfEnemies; c++) {
            if (enemiesCopy[i]->getPositionRow() == enemiesCopy[c]->getPositionRow() && enemiesCopy[i]->getPositionCol() == enemiesCopy[c]->getPositionCol()) {
                enemies::freeAllEnemies(enemiesCopy, &numOfEnemiesCopy);
                return true;
            }
        }

        enemies::freeAllEnemies(enemiesCopy, &numOfEnemiesCopy);
        return false;
    }

    //Generates a random position that is not ontop of the player or another enemy
    void randomizePosition(entity* enemies[], int currentNumOfEnemies, entity* player, int enemyPos[])
    {
        int col = 0;
        int row = 0;

        while (true) {
            //Checks if enemy position is on top of starting position of player
            do {
                col = rand() % WORLD_SIZE;
                row = rand() % WORLD_SIZE;
            } while (col == player->getPositionCol() && row == player->getPositionRow());

            //Checks if new Enemy has the same position as an old one
            bool duplicatePosition = false;
            for (int i = 0; i < currentNumOfEnemies; i++) {
                if (enemies[i]->getPositionCol() == col && enemies[i]->getPositionRow() == row) {
                    duplicatePosition = true;
                    break;
                }
            }
            //Repeats loop if duplicate position was found, ends loop if it is not
            if (duplicatePosition)
                continue;
            else
                break;
        }

        enemyPos[0] = col;
        enemyPos[1] = row;
    }

    //Returns a randomized direction
    char randomizeDirection() {
        char direction = UP;

        int dirSwitch = rand() % 4;
        switch (dirSwitch)
        {
        case 0: direction = UP; break;
        case 1: direction = DOWN; break;
        case 2: direction = RIGHT; break;
        case 3: direction = LEFT; break;
        }

        return direction;
    }

    //Initializes all enemies with valid positions and directions
    void initializeEnemies(entity* enemies[], int* numOfEnemies, entity* player, char world[WORLD_SIZE][WORLD_SIZE], int* difficulty)
    {
        for (int i = 0; i < *numOfEnemies; i++) {
            //Randomizes Position of new Enemy (pos[0] = col, pos[1] = row)
            int pos[2]{ 0 };
            enemies::randomizePosition(enemies, i, player, pos);

            //HP is initialized as random value scaling with difficulty
            int HP = (rand() % (*difficulty * 2)) + 1;

            //Direction is initalized randomly
            char direction = enemies::randomizeDirection();

            //Damage scales with difficulty
            int damage = (*difficulty / 2) + 1;

            //Creates new Enemy and appends it to the enemy list
            entity* newEnemy = new entity(pos[1], pos[0], HP, damage, direction);

            //If enemy direction is possible (so does not walk into wall, or another enemy), add enemy, else repeat enemy creation
            if (check::enemyDirectionPossible(world, newEnemy) && enemies::collisionWithNewEnemy(enemies, newEnemy, world, &i) == false) {
                enemies[i] = newEnemy;
            }
            else {
                delete newEnemy;
                i--;
                continue;
            }
        }
    }

    //Randomizes the direction of all enemies
    void changeDirection(entity* enemies[], int* numOfEnemies, char world[WORLD_SIZE][WORLD_SIZE])
    {
        //Safety to stop infinite loop if every direction of enemy movement is impossible
        int upCounter = 0;
        int downCounter = 0;
        int rightCounter = 0;
        int leftCounter = 0;

        for (int i = 0; i < *numOfEnemies; i++) {

            //Direction is initalized randomly
            char direction = enemies::randomizeDirection();

            //Creates new Enemy  with new direction
            entity* newDirectionEnemy = new entity(enemies[i]->getPositionRow(), enemies[i]->getPositionCol(), enemies[i]->getHP(), enemies[i]->getDamage(), direction);

            //If enemy direction is possible (so does not walk into wall, or another enemy), change enemy direction, else repeat direction randomization
            if (check::enemyDirectionPossible(world, newDirectionEnemy) && enemies::collisionWithNewEnemy(enemies, newDirectionEnemy, world, &i) == false) {
                enemies[i]->setDirection(direction);

                upCounter = 0;
                downCounter = 0;
                rightCounter = 0;
                leftCounter = 0;

                delete newDirectionEnemy;
            }
            else {
                //Increases the corresponding counter if direction is impossible
                switch (direction) {
                case UP: upCounter++; break;
                case DOWN: downCounter++; break;
                case RIGHT: rightCounter++; break;
                case LEFT: leftCounter++; break;
                }

                //Changes the direction of enemy and moves on if every direction is impossible. Enemy does collide then, but it is what it is, stopping his movement would be too complicated
                if (upCounter > 0 && downCounter > 0 && rightCounter > 0 && leftCounter > 0) {
                    enemies[i]->setDirection(direction);
                    delete newDirectionEnemy;
                    continue;
                }
                //Repeates same loop otherwise
                else {
                    delete newDirectionEnemy;
                    i--;
                    continue;
                }
            }
        }
    }

    //Removes an enemy at a given index (used if an enemy is killed)
    void removeEnemyAtIndex(entity* enemies[], int* numOfEnemies, int indexToRemove) {
        *numOfEnemies -= 1;
        delete enemies[indexToRemove];

        for (int i = indexToRemove; i < *numOfEnemies; i++) {
            enemies[i] = enemies[i + 1];
        }
    }

    //Function currently not in use, prints all enemy info for debugging reasons
    void printAllEnemyInfo(entity* enemies[], int* numOfEnemies)
    {
        for (int i = 0; i < *numOfEnemies; i++) {
            std::cout << "Enemy Number " << i + 1 << ": " << enemies[i]->getPositionRow() << "|" << enemies[i]->getPositionCol() << " HP(" << enemies[i]->getHP() << ") Dir(" << enemies[i]->getDirection() << ")" << std::endl;
        }
    }
}

//All field actions and enemy interaction functions
namespace action {
    void fieldEmpty(entity* player, std::string turnLog[MAX_TURNS + 1], int* turnNum)
    {
        std::string message = "Turn " + std::to_string(*turnNum) + ": You stride on in safety.";
        turnLog[*turnNum] = message;
        std::cout << message << std::endl;
    }

    void fieldDanger(entity* player, std::string turnLog[MAX_TURNS + 1], int* turnNum, int* difficulty)
    {
        int randVal = rand() % 100;

        //Player gets hurt with a (difficulty * 10) - (Agility * 3) % chance
        if (randVal < (*difficulty * 10) - (player->getAgility() * 3)) {
            int damage = (*difficulty / 2) + 1;
            std::string message = "Turn " + std::to_string(*turnNum) + ": You got hurt! -" + std::to_string(damage) + "  HP";
            turnLog[*turnNum] = message;
            std::cout << message << std::endl;

            player->loseHP(damage);
        }
        else {
            std::string message = "Turn " + std::to_string(*turnNum) + ": You are lucky and get away unscathed.";
            turnLog[*turnNum] = message;
            std::cout << message << std::endl;
        }
    }

    void fieldWell(entity* player, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, entity* enemies[])
    {
        int chance = rand() % 3;
        if (!chance) {
            items::addItem(items, allItems, itemsCount, player, enemies);
            std::string message = "Turn " + std::to_string(*turnNum) + ": You found an item!";
            turnLog[*turnNum] = message;
            std::cout << message << std::endl;
            *turnNum += 1;
        }

        std::string message = "Turn " + std::to_string(*turnNum) + ": You rest at a well. +1 HP";
        turnLog[*turnNum] = message;
        std::cout << message << std::endl;

        player->gainHP(1);
    }

    void fieldRelic(entity* player, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, entity* enemies[])
    {
        int chance = rand() % 2;
        if (chance) {
            items::addItem(items, allItems, itemsCount, player, enemies);
            std::string message = "Turn " + std::to_string(*turnNum) + ": You found an item!";
            turnLog[*turnNum] = message;
            std::cout << message << std::endl;
            *turnNum += 1;
        }

        std::string message = "Turn " + std::to_string(*turnNum) + ": You found a Relic! It humms with strange Power...";
        turnLog[*turnNum] = message;
        std::cout << message << std::endl;

        player->setRelics(player->getRelics() + 1);
    }

    //Calls the corresponding field functions
    void fieldAction(char world[WORLD_SIZE][WORLD_SIZE], entity* player, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, entity* enemies[], int* difficulty)
    {
        char fieldType = world[player->getPositionRow()][player->getPositionCol()];

        switch (fieldType) {
        case EMPTY:
            fieldEmpty(player, turnLog, turnNum);
            break;
        case DANGER:
            fieldDanger(player, turnLog, turnNum, difficulty);
            break;
        case WELL:
            fieldWell(player, turnLog, turnNum, items, allItems, itemsCount, enemies);
            break;
        case RELIC:
            fieldRelic(player, turnLog, turnNum, items, allItems, itemsCount, enemies);
            break;
        }
    }

    //Enemy-fight function
    void enemyEncounter(entity* player, entity* enemies[], int enemyIndex, int* numOfEnemies, std::string turnLog[MAX_TURNS + 1], int* turnNum)
    {
        //Saves how much damage was done/recieved for the log
        int damageDone = 0;
        int damageRecieved = 0;

        system("cls");

        std::cout << "Enemy Encounter!\n" << std::endl;

        //Loops for each round of the fight
        while (true) {
            int playerGuess = 0;

            //Guess range is a random number between the min and max guess range
            int guessRange = MIN_GUESS_RANGE + (rand() % (MAX_GUESS_RANGE - MIN_GUESS_RANGE + 1));
            //Enemy guess and final number is random
            int enemyGuess = (rand() % guessRange) + 1;
            int randomNumber = (rand() % guessRange) + 1;

            std::string playerHpMessage = player->getHP() > 9 ? "| HP:    " : "| HP:     ";
            std::string enemyHpMessage = enemies[enemyIndex]->getHP() > 9 ? " | HP:    " : " | HP:     ";

            //Prints the HP and the guess range
            std::cout << "|    You    | Range |   Enemy   |" << std::endl;
            std::cout << playerHpMessage << player->getHP() << " | 1 - " << guessRange << enemyHpMessage << enemies[enemyIndex]->getHP() << " | " << std::endl;
            std::cout << "+-----------+-------+-----------+" << std::endl;

            //Asks for a guess as long as guess is not within range
            while (playerGuess < 1 || playerGuess > guessRange) {
                std::cout << "Your guess: ";
                std::cin >> playerGuess;
            }

            int playerOffBy = std::abs(playerGuess - randomNumber);
            int enemyOffBy = std::abs(enemyGuess - randomNumber);

            //Prints the guessed numbers + the actual number + what each guess was off by
            std::cout << "\n|    You    | Value |   Enemy   |" << std::endl;
            std::cout << "|   " << playerGuess << " (" << playerOffBy << ")   |   " << randomNumber << "   |   " << enemyGuess << " (" << enemyOffBy << ")   |" << std::endl;
            std::cout << "+-----------+-------+-----------+" << std::endl;

            //Depending on who was closer, hits the player or the enemy, or does nothing if both were equally close
            if (playerOffBy > enemyOffBy) {
                int damage = enemies[enemyIndex]->getDamage();
                std::cout << "\nYou got hit! -" << damage << " HP\n" << std::endl;
                damageRecieved += damage;
                player->loseHP(damage);
            }
            else if (playerOffBy < enemyOffBy) {
                int damage = player->getDamage();
                std::cout << "\nYou hit! +" << damage << " DMG\n" << std::endl;
                damageDone += damage;
                enemies[enemyIndex]->loseHP(damage);
            }
            else {
                std::cout << "\nEqual!\n" << std::endl;
            }

            //Breaks the loop if enemy or player dies
            if (enemies[enemyIndex]->getHP() <= 0) {
                std::cout << "\nEnemy defeated! Epic victory royale!\n" << std::endl;
                enemies::removeEnemyAtIndex(enemies, numOfEnemies, enemyIndex);
                player->changeLevelBy(1);
                system("pause");
                system("cls");
                break;
            }
            else if (player->getHP() <= 0) {
                system("pause");
                system("cls");
                break;
            }
        }

        std::string message = "Turn " + std::to_string(*turnNum) + ": Fight! You dealt " + std::to_string(damageDone) + " DMG and lost " + std::to_string(damageRecieved) + " HP.";
        turnLog[*turnNum] = message;
        //Turn number needs to be increased, since after the fight the field action will trigger, which counts as a new turn
        *turnNum += 1;
    }
}

//Player movement functions
//All of them do more than just move the player, they trigger all field actions and enemy interactions too
namespace move {
    void up(entity* player, char world[WORLD_SIZE][WORLD_SIZE], entity* enemies[], int* numOfEnemies, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, int* difficulty)
    {
        if (player->getPositionRow() - 1 < 0)
            std::cout << "Direction not possible!" << std::endl;
        else {
            //Increase turn number
            *turnNum += 1;
            //Move the player
            player->changePositionBy(-1, 0);
            //Check if the player walked into an enemy facing him
            int enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1 && enemies[enemyIndex]->getDirection() == DOWN) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Move the enemies
            enemies::moveEnemies(enemies, numOfEnemies);
            enemies::changeDirection(enemies, numOfEnemies, world);
            //Check if an enemy walked onto the player
            enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Do the field action
            action::fieldAction(world, player, turnLog, turnNum, items, allItems, itemsCount, enemies, difficulty);
            world::removeField(world, player);
        }
    }

    void down(entity* player, char world[WORLD_SIZE][WORLD_SIZE], entity* enemies[], int* numOfEnemies, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, int* difficulty)
    {
        if (player->getPositionRow() + 1 >= WORLD_SIZE)
            std::cout << "Direction not possible!" << std::endl;
        else {
            //Increase turn number
            *turnNum += 1;
            //Move the player
            player->changePositionBy(1 , 0);
            //Check if the player walked into an enemy facing him
            int enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1 && enemies[enemyIndex]->getDirection() == UP) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Move the enemies
            enemies::moveEnemies(enemies, numOfEnemies);
            enemies::changeDirection(enemies, numOfEnemies, world);
            //Check if an enemy walked onto the player
            enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Do the field action
            action::fieldAction(world, player, turnLog, turnNum, items, allItems, itemsCount, enemies, difficulty);
            world::removeField(world, player);
        }
    }

    void left(entity* player, char world[WORLD_SIZE][WORLD_SIZE], entity* enemies[], int* numOfEnemies, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, int* difficulty)
    {
        if (player->getPositionCol() - 1 < 0)
            std::cout << "Direction not possible!" << std::endl;
        else {
            //Increase turn number
            *turnNum += 1;
            //Move the player
            player->changePositionBy(0, -1);
            //Check if the player walked into an enemy facing him
            int enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1 && enemies[enemyIndex]->getDirection() == RIGHT) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Move the enemies
            enemies::moveEnemies(enemies, numOfEnemies);
            enemies::changeDirection(enemies, numOfEnemies, world);
            //Check if an enemy walked onto the player
            enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Do the field action
            action::fieldAction(world, player, turnLog, turnNum, items, allItems, itemsCount, enemies, difficulty);
            world::removeField(world, player);
        }
    }

    void right(entity* player, char world[WORLD_SIZE][WORLD_SIZE], entity* enemies[], int* numOfEnemies, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, int* difficulty)
    {
        if (player->getPositionCol() + 1 >= WORLD_SIZE)
            std::cout << "Direction not possible!" << std::endl;
        else {
            //Increase turn number
            *turnNum += 1;
            //Move the player
            player->changePositionBy(0, 1);
            //Check if the player walked into an enemy facing him
            int enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1 && enemies[enemyIndex]->getDirection() == LEFT) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Move the enemies
            enemies::moveEnemies(enemies, numOfEnemies);
            enemies::changeDirection(enemies, numOfEnemies, world);
            //Check if an enemy walked onto the player
            enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
            if (enemyIndex != -1) {
                action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
            }
            //Do the field action
            action::fieldAction(world, player, turnLog, turnNum, items, allItems, itemsCount, enemies, difficulty);
            world::removeField(world, player);
        }
    }

    void wait(entity* player, char world[WORLD_SIZE][WORLD_SIZE], entity* enemies[], int* numOfEnemies, std::string turnLog[MAX_TURNS + 1], int* turnNum, item* items[], item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], int* itemsCount, int* difficulty)
    {
        //Move the enemies
        enemies::moveEnemies(enemies, numOfEnemies);
        enemies::changeDirection(enemies, numOfEnemies, world);
        //Check if an enemy walked onto the player
        int enemyIndex = check::enemyExistsAt(enemies, numOfEnemies, player->getPositionRow(), player->getPositionCol());
        if (enemyIndex != -1) {
            action::enemyEncounter(player, enemies, enemyIndex, numOfEnemies, turnLog, turnNum);
        }
    }
}

//All item effect functions
namespace itemEffect {
    void healthPotion(entity* player, entity* enemies[])
    {
        player->gainHP(2);
    }

    void speedPotion(entity* player, entity* enemies[])
    {
        player->changeAgilityBy(2);
    }

    void woodenStick(entity* player, entity* enemies[])
    {
        player->changeDamageBy(1);
    }

    void pieceOfCloth(entity* player, entity* enemies[])
    {
        player->changeAgilityBy(1);
        player->gainHP(1);
    }

    void berries(entity* player, entity* enemies[])
    {
        switch (rand() % 2)
        {
        case 0:
            std::cout << "Yep, poisonous. -1 HP\n" << std::endl;
            player->loseHP(1);
            break;
        case 1:
            std::cout << "Delicious! +1 HP\n" << std::endl;
            player->gainHP(1);
            break;
        }
        std::cout << std::endl;
        system("pause");
    }

    void magicMushroom(entity* player, entity* enemies[])
    {
        switch (rand() % 5)
        {
        case 0:
            std::cout << "I don't feel right Mr. Stark. -5 HP" << std::endl;
            player->loseHP(5);
            break;
        case 1:
            std::cout << "Mario Shroom! +10 HP" << std::endl;
            player->gainHP(10);
            break;
        case 2:
            std::cout << "Is it me or is the world suddenly bigger? -3 DMG" << std::endl;
            player->changeDamageBy(-3);
            break;
        case 3:
            std::cout << "HOLY SHIIIII... +5 Agility" << std::endl;
            player->changeAgilityBy(5);
            break;
        case 4:
            std::cout << "I ate the shroom, the next thing I know I was fighting a naked elf in the clouds! +2 Levels" << std::endl;
            player->changeLevelBy(2);
            break;
        }
        std::cout << std::endl;
        system("pause");
    }

    void redApple(entity* player, entity* enemies[])
    {
        player->gainHP(3);
        player->changeAgilityBy(3);
    }

    void ironSword(entity* player, entity* enemies[])
    {
        player->changeDamageBy(3);
    }

    void ironArmor(entity* player, entity* enemies[])
    {
        player->gainHP(5);
        player->changeAgilityBy(-1);
    }

    void sturdyBoots(entity* player, entity* enemies[])
    {
        player->loseHP(1);
        player->changeAgilityBy(5);
    }

    void magicPotion(entity* player, entity* enemies[])
    {
        player->gainHP(5);
        player->changeAgilityBy(5);
        player->changeDamageBy(5);
    }

    void suspiciousStick(entity* player, entity* enemies[])
    {
        player->changeLevelBy(5);
    }

    void swordOfLegend(entity* player, entity* enemies[])
    {
        player->changeDamageBy(7);
    }

    void darkTalisman(entity* player, entity* enemies[])
    {
        player->changeDamageBy(10);
        player->loseHP(5);
    }

    void supaSuit(entity* player, entity* enemies[])
    {
        player->changeAgilityBy(7);
    }
}

//Various other functions that did not fit anywhere else
namespace various {
    //Prints help messages
    void help() {
        system("cls");
        std::cout << "Help" << std::endl;
        std::cout << "\n1. Movement" << std::endl;
        std::cout << "You move by using the wasd-keys, or wait a turn with 'e'. Just enter a key, then press enter. The player is represented by the green 0 on the map." << std::endl;
        std::cout << "\n2. Enemies" << std::endl;
        std::cout << "Enemies are represented by the red arrows. These arrows indicate in which direction the enemy will move next. Enemies wont walk into walls, or each other." << std::endl;
        std::cout << "\n3. Field Types" << std::endl;
        std::cout << "There are 4 Types of fields you can walk onto:\nThe danger field (X) has a 17% chance to damage you by 1 HP. \nThe well field (@) will heal you by 1 HP. \nThe empy field ( ) does nothing. \nThe relic fields ($) will let you pick up a relic. If no relics are left on the map, you win." << std::endl;
        std::cout << "\n4. Fights" << std::endl;
        std::cout << "You fight an enemy if you and an enemy walk onto the same field, or you and an enemy walk past each other. Each round of the fight you and the enemy guess a number in the random given range. Whichever of you is closer to the number, gets to hit the other." << std::endl;
        std::cout << "\n5. Score" << std::endl;
        std::cout << "The score is determined by (the number of relics you found) + (the number of levels * 3) + ((10 * difficulty) - the number of turns you took). This means that after 10 turns on a map you start losing score, so keep that in mind!" << std::endl;
        std::cout << "\n6. Stats" << std::endl;
        std::cout << "As a player you have a level, HP, Damage and Agility. Each enemy you kill increases your level by 1, which in turn increases all other stats by 1. Your chance to dodge danger increases with your agility." << std::endl;
        std::cout << "\n7. Difficulty" << std::endl;
        std::cout << "Each time you collect all relics on a map, the difficulty increases and you spawn on a new map in the same location. Difficulty determines the strength of the enemies and the chance & damage of danger fields." << std::endl;
        std::cout << "\n8. Items" << std::endl;
        std::cout << "Items can be found at wells and relic fields. There are two types of item: Equipment and Consumeable. If you find equipment, the effects get triggered immediately. If you find a consumeable, you can trigger it's effect from your inventory.\n" << std::endl;

        system("pause");
        system("cls");
    }

    //Prints the player HP and relic count
    void printStats(entity* player, int* score) {
        std::cout << "\nScore: " << *score << std::endl;
        std::cout << "\nLevel: " << player->getLevel() << std::endl;

        std::cout << "HP:  ";
        for (int i = 0; i < MAX_HP_PLAYER; i++) {
            if (i < player->getHP()) {
                std::cout << "(" << RED_TEXT << "O" << RESET << ")";
            }
            else {
                std::cout << "( )";
            }
        }

        std::cout << "\nDMG: ";
        for (int i = 0; i < MAX_DMG_PLAYER; i++) {
            if (i < player->getDamage()) {
                std::cout << "(" << YELLOW_TEXT << "O" << RESET << ")";
            }
            else {
                std::cout << "( )";
            }
        }

        std::cout << "\nAGI: ";
        for (int i = 0; i < MAX_AGI_PLAYER; i++) {
            if (i < player->getAgility()) {
                std::cout << "(" << GREEN_TEXT << "O" << RESET << ")";
            }
            else {
                std::cout << "( )";
            }
        }

        std::cout << "\n\nRelics: " << player->getRelics() << "\n" << std::endl;
    }

    //Gives a final summary if the player wins, dies or has no turns left
    void printFinalSummary(std::string turnLog[MAX_TURNS + 1], int* turnNum, entity* player, int* score) {

        //Prints all turns
        for (int i = 0; i <= *turnNum; i++) {
            std::cout << turnLog[i] << std::endl;
        }

        //Prints the summary table, with 1 space different lines if number has 2 digits
        std::cout << "\n+-------------+" << std::endl;
        if (*turnNum > 9)
            std::cout << "| Turns:   " << *turnNum << " |" << std::endl;
        else
            std::cout << "| Turns:    " << *turnNum << " |" << std::endl;

        if (player->getRelics() > 9)
            std::cout << "| Relics:  " << player->getRelics() << " |" << std::endl;
        else
            std::cout << "| Relics:   " << player->getRelics() << " |" << std::endl;

        if (player->getLevel() > 9)
            std::cout << "| Level:   " << player->getLevel() << " |" << std::endl;
        else
            std::cout << "| Level:    " << player->getLevel() << " |" << std::endl;

        std::cout << "+-------------+" << std::endl;

        if (*score > 9 || *score < 0)
            std::cout << "| Score:   " << *score << " |" << std::endl;
        else
            std::cout << "| Score:    " << *score << " |" << std::endl;

        std::cout << "+-------------+" << std::endl;
    }

    void deleteEverything(entity* player, entity* enemies[], int* numOfEnemies, item* allItems[][NUMBER_OF_ITEMS_PER_RARITY], item* items[], int* itemsCount)
    {
        delete player;
        enemies::freeAllEnemies(enemies, numOfEnemies);
        items::removeAllGainedItems(items, itemsCount);
        items::removeAllItems(allItems);
    }
}

int main()
{
    //Create a random seed once
    srand(time(NULL));

    //Initializes and fills the available items array
    //Rarity: [0] Common - [1] Rare - [2] Legendary
    //Type: 'c' Consumeable - 'e' Equipment
    item* allItems[3][NUMBER_OF_ITEMS_PER_RARITY] = { {
        new item(0, 'c', "Health Potion", "All time classic. Heals 2 HP on use.", &itemEffect::healthPotion),
        new item(0, 'c', "Speed Potion", "I am speed. Increases Agility by 2 on use.", &itemEffect::speedPotion),
        new item(0, 'c', "Berries", "Might be bad, might be good. Increases or decreases HP by 1.", &itemEffect::berries),
        new item(0, 'e', "Wooden Stick", "Another one for my stick collection, nice! Damage +1.", &itemEffect::woodenStick),
        new item(0, 'e', "Piece of Cloth", "Very versitile, from footpad to toiletpaper. Agility +1, HP +1.", &itemEffect::pieceOfCloth)
    },{
        new item(1, 'c', "Magic Mushroom", "I'm not sure about this. Random effect on use.", &itemEffect::magicMushroom),
        new item(1, 'c', "Red Apple", "An apple a day keeps the doctor away. HP +3, Agility +3 on use.", &itemEffect::redApple),
        new item(1, 'e', "Iron Sword", "Sharp as your wit. Could be better, in other words. Damage +3", &itemEffect::ironSword),
        new item(1, 'e', "Iron Armor", "It's two sizes to small. You feel like a sardine. HP +5, Agility -1", &itemEffect::ironArmor),
        new item(1, 'e', "Sturdy Boots", "Finally! You are so excited, you fall on your head tying your shoelaces. Agility +5, HP -1", &itemEffect::sturdyBoots)
    },{
        new item(2, 'c', "Magic Potion", "Obelix is looking for you. Damage +5, HP +5, Agility +5", &itemEffect::magicPotion),
        new item(2, 'c', "Suspicious Stick", "Wait, why is this a consumable?. Questionable use, increases level by 5.", &itemEffect::suspiciousStick),
        new item(2, 'e', "Sword of Legend", "It's dangerous to go alone. Take this. Damage +7", &itemEffect::swordOfLegend),
        new item(2, 'e', "Dark Talisman", "Putting this on might have been a mistake... Damage +10, HP -5", &itemEffect::darkTalisman),
        new item(2, 'e', "Supa Suit", "Sorry Mr. Incredible. Agility +7", &itemEffect::supaSuit)
    } };

    //Loops only more than once if player chooses to reset
    bool gameLoop = true;
    while (gameLoop) {
        gameLoop = false;

        //Initialize the player with a start position of 0|0, 5 HP, 0 Relics, 1 Level, 1 DMG and 1 AGI
        entity* player = new entity(0 , 0, 5, 0, 1, 1, 1);

        //Initializes the found items array
        item* items[MAX_ITEMS]{ NULL };
        int itemsCount = 0;

        //Initializes the turn log and turn counter
        std::string turnLog[MAX_TURNS + 1];
        int turnNum = 0;

        //Difficulty is a number that increases by 1 for each "completed" map
        //Enemy Damage scales (difficulty / 2) + 1
        //Enemy HP scales (rand() % (difficulty * 2))
        //Danger field hit chance scales (difficulty * 10) - (Agility * 3)
        //Danger field hit damage scales (difficulty / 2) + 1
        int difficulty = 0;

        //Score calculates like follows:
        //relics +
        //3 * level +
        //(difficulty * 10) - turns (this means that after more than 10 turns on a map the score starts going down)
        int score = player->getRelics() + (3 * player->getLevel()) + ((difficulty * 10) - turnNum);

        //Difficulty loop, each loop increases the difficulty
        bool aliveAndWell = true;
        while (aliveAndWell) {
            aliveAndWell = false;
            difficulty++;

            //Initialize and build the world[row][col], remove the current field of the player
            char world[WORLD_SIZE][WORLD_SIZE];
            world::buildWorld(world);
            world::removeField(world, player);

            //Build the world again as long as no relic exists
            while (check::relicExists(world) == false) {
                world::buildWorld(world);
                world::removeField(world, player);
            }

            //Initializes random number of enemies with random positions
            int numOfEnemies = (rand() % MAX_NUM_OF_ENEMIES) + 1;
            entity* enemies[MAX_NUM_OF_ENEMIES]{ NULL };
            enemies::initializeEnemies(enemies, &numOfEnemies, player, world, &difficulty);
            //enemies::printAllEnemyInfo(enemies, &numOfEnemies);

            //Main game loop
            char input = ' ';
            while (true) {
                //Updates the score
                score = player->getRelics() + (3 * player->getLevel()) + ((difficulty * 10) - turnNum);
                //Prints the world
                world::printWorld(world, player, enemies, &numOfEnemies);
                //Prints the player stats
                various::printStats(player, &score);

                //Invalid input initialized as true so while loop triggers once
                bool invalidInput = true;

                //Asks for input as long as input is invalid
                while (invalidInput) {
                    std::cout << "(w) Up - (a) Left - (s) Down - (d) Right - (e) Wait - (i) Inventory   |   (h) Help - (r) Reset - (x) Exit" << std::endl;
                    std::cin >> input;

                    system("cls");

                    //Moves the player on valid input
                    //Actions and interactions are called in the movement functions
                    invalidInput = false;
                    switch (input) {
                    case 'w':
                        move::up(player, world, enemies, &numOfEnemies, turnLog, &turnNum, items, allItems, &itemsCount, &difficulty);
                        break;
                    case 'a':
                        move::left(player, world, enemies, &numOfEnemies, turnLog, &turnNum, items, allItems, &itemsCount, &difficulty);
                        break;
                    case 's':
                        move::down(player, world, enemies, &numOfEnemies, turnLog, &turnNum, items, allItems, &itemsCount, &difficulty);
                        break;
                    case 'd':
                        move::right(player, world, enemies, &numOfEnemies, turnLog, &turnNum, items, allItems, &itemsCount, &difficulty);
                        break;
                    case 'e':
                        move::wait(player, world, enemies, &numOfEnemies, turnLog, &turnNum, items, allItems, &itemsCount, &difficulty);
                        break;
                    case 'i':
                        items::showItems(items, &itemsCount, player, enemies);
                        break;
                    case 'h':
                        various::help();
                        break;
                    case 'r':
                        break;
                    case 'x':
                        break;
                    default:
                        //Prints the world
                        world::printWorld(world, player, enemies, &numOfEnemies);
                        //Prints the player stats
                        various::printStats(player, &score);

                        std::cout << "Invalid input! Try again.\n" << std::endl;
                        invalidInput = true;
                    }
                }

                //Break condition if player HP reaces 0 or runs out of turns, if input is 'r' or 'x', or if no relics are left on the map
                if (player->getHP() <= 0) {
                    system("cls");
                    std::cout << "You died. Good luck next time." << std::endl;
                    various::printFinalSummary(turnLog, &turnNum, player, &score);
                    various::deleteEverything(player, enemies, &numOfEnemies, allItems, items, &itemsCount);
                    break;
                }
                else if (turnNum >= MAX_TURNS) {
                    system("cls");
                    std::cout << "You exeeded the maxium number of allowed turns!" << std::endl;
                    various::printFinalSummary(turnLog, &turnNum, player, &score);
                    various::deleteEverything(player, enemies, &numOfEnemies, allItems, items, &itemsCount);
                    break;
                }
                else if (input == 'x') {
                    system("cls");
                    std::cout << "Thank you for playing! Goodbye." << std::endl;
                    various::printFinalSummary(turnLog, &turnNum, player, &score);
                    various::deleteEverything(player, enemies, &numOfEnemies, allItems, items, &itemsCount);
                    break;
                }
                else if (input == 'r') {
                    gameLoop = true;
                    delete player;
                    enemies::freeAllEnemies(enemies, &numOfEnemies);
                    items::removeAllGainedItems(items, &itemsCount);
                    break;
                }
                else if (check::relicExists(world) == false) {
                    aliveAndWell = true;
                    system("cls");
                    break;
                }
            }
        }
    }

    return 0;
}