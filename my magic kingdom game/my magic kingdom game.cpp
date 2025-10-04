#include <SFML/Graphics.hpp>     
#include <iostream>           // MUHAMMAD FAHAD SHOAIB BUTT
#include <string>             // 24I-3148
#include <fstream>            // SE-B
#include <cstring>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cmath>

#define MAX_ITEMS 10
#define MAX_PETS 4
#define MAX_PLAYERS 100
#define ROWS 10
#define COLS 30
#define MAX_FIREBALLS 30

const int CAMP_ROWS = 10;
const int CAMP_COLS = 15;
const int TILE_SIZE = 64;

using namespace std;

struct PetVisual {
    sf::Sprite sprite;
    string name;
};

// Pet class
class pets {
protected:
    string name;
    string id;

public:
    int hp;
    int maxhp;
    int powerattack;
    int level;
    int speed;
    int defence;

    pets(string n, string i, int l, int p, int s, int d, int h, int m)
        : name(n), id(i), level(l), powerattack(p), speed(s), defence(d), hp(h), maxhp(m) {
    }

    void levelup() {
        level++;
        maxhp += 100;
        hp = maxhp;
    }

    void displayStatus() const {
        cout << name << " HP: " << hp << "/" << maxhp << endl;
    }

    void takedamage(int powerattack) {
        cout << "HIT" << endl;
        hp -= powerattack;
        cout << "HP: " << endl;
    }

    virtual void attack(pets* enemy) {
        enemy->hp -= powerattack;
        if (enemy->hp < 0)
            enemy->hp = 0;
        cout << name << " attacks " << enemy->name << " for " << powerattack << " damage!\n";
    }

    void heal() {
        int healAmount = 10;
        hp += healAmount;
        if (hp > maxhp) hp = maxhp;
        cout << name << " heals for " << healAmount << " HP!\n";
    }


    virtual void useAbility(pets* enemy) = 0;

    virtual string getName() const = 0;

    bool isAlive() {
        return hp > 0;
    }

    virtual ~pets() {}
};

class Unicorn : public pets {
public:
    Unicorn(string n, string i, int l, int p, int s, int d, int h, int m)
        : pets(n, i, l, p, s, d, h, m) {
    }

    void useAbility(pets* enemy = nullptr) override {
        // Unicorn's special ability: Healing Aura
        int healingAmount = 30;
        hp += healingAmount;
        if (hp > maxhp) hp = maxhp;
        cout << name << " uses Healing Aura! Restored " << healingAmount << " HP!" << endl;
    }

    string getName() const override {
        return "Unicorn";
    }
};

class Griffin : public pets {
public:
    Griffin(string n, string i, int l, int p, int s, int d, int h, int m)
        : pets(n, i, l, p, s, d, h, m) {
    }

    void useAbility(pets* enemy = nullptr) override {
        // Griffin's special ability: Wind Strike
        int speedBoost = 20;
        speed += speedBoost;
        cout << name << " uses Wind Strike! Speed increased by " << speedBoost << " for 5 seconds!" << endl;
    }

    string getName() const override {
        return "Griffin";
    }
};

class Dragon : public pets {
public:
    Dragon(string n, string i, int l, int p, int s, int d, int h, int m)
        : pets(n, i, l, p, s, d, h, m) {
    }

    void useAbility(pets* enemy) override {
        // Dragon's special ability: Fire Breath
        int fireBreathDamage = 50;
        enemy->takedamage(fireBreathDamage);
        cout << name << " uses Fire Breath! Inflicted " << fireBreathDamage << " damage to " << enemy->getName() << "!" << endl;
    }


    string getName() const override {
        return "Dragon";
    }
};

class Phenix : public pets {
public:
    Phenix(string n, string i, int l, int p, int s, int d, int h, int m)
        : pets(n, i, l, p, s, d, h, m) {
    }

    void useAbility(pets* enemy = nullptr) override {
        // Phenix's special ability: Rebirth Flame
        defence += 15; // Reduce incoming damage by 50% (logic will be handled in the battle loop)
        cout << name << " uses Rebirth Flame! Incoming damage reduced by 50% for 5 seconds!" << endl;
    }

    string getName() const override {
        return "Phenix";
    }
};

// Item classes
class item {
protected:
    string name;
    double price;
    int quantity;

public:
    item(string n, double p, int q) : name(n), price(p), quantity(q) {}

    virtual void use(pets* target) {
        if (quantity > 0) {
            quantity--;
            cout << name << " used. Remaining quantity: " << quantity << endl;
        }
        else {
            cout << "Out of stock!" << endl;
        }
    }

    virtual void display() const {
        cout << name << " - Price: " << price << ", Quantity: " << quantity << endl;
    }

    virtual ~item() {}

    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    string getName() const { return name; }
    void reduceQuantity() { if (quantity > 0) quantity--; }
};

// Specific Item Classes
class Healingportion : public item {
private:
    int heal;

public:
    Healingportion(double p, int q, int h) : item("Healing Portion", p, q), heal(h) {}

    void use(pets* target) {
        if (quantity > 0) {
            quantity--;
            target->hp += heal;
            if (target->hp > target->maxhp) target->hp = target->maxhp;
            cout << "Healing portion used! Restored " << heal << " HP." << endl;
        }
        else {
            cout << "Out of stock!" << endl;
        }
    }

    void display() const override {
        cout << "[Healing Portion] ";
        item::display();
        cout << "Healing: +" << heal << " HP" << endl;
    }
};

class Manastore : public item {
private:
    int mana;
    int speedBoost; // Additional effect: speed boost

public:
    Manastore(double p, int q, int m, int s) : item("Mana Store", p, q), mana(m), speedBoost(s) {}

    void use(pets* target) override {
        if (quantity > 0) {
            quantity--;
            // Recover mana (effect can be visualized or logged)
            cout << "Mana Store used, mana recovered by: " << mana << " MP." << endl;

            // Apply speed boost
            target->speed += speedBoost;
            cout << "Speed increased by: " << speedBoost << " temporarily." << endl;

        }

        else {
            cout << "Out of stock!" << endl;
        }
    }

    void display() const override {
        cout << "[Mana Store] ";
        item::display();
        cout << "Mana Recovery: +" << mana << " MP, Speed Boost: +" << speedBoost << endl;
    }
};

class Buff : public item {
private:
    int buffamount;
    int duration;

public:
    Buff(double p, int q, int b, int d) : item("Buff", p, q), buffamount(b), duration(d) {}

    void use(pets* target) {
        if (quantity > 0) {
            quantity--;
            target->powerattack += buffamount;
            cout << "Buff used! Increased attack power by " << buffamount << " temporarily." << endl;
        }
        else {
            cout << "Out of stock!" << endl;
        }
    }

    void display() const override {
        cout << "[Buff] ";
        item::display();
        cout << "Buff Amount: +" << buffamount << ", Duration: " << duration << " seconds" << endl;
    }
};

class Shield : public item {
private:
    int damageReduction;
    int duration;

public:
    Shield(double p, int q, int d, int t) : item("Shield", p, q), damageReduction(d), duration(t) {}

    void use(pets* target) {
        if (quantity > 0) {
            quantity--;
            target->defence += damageReduction;
            cout << "Shield used! Reduced incoming damage by " << damageReduction << " temporarily." << endl;
        }
        else {
            cout << "Out of stock!" << endl;
        }
    }

    void display() const override {
        cout << "[Shield] ";
        item::display();
        cout << "Damage Reduction: " << damageReduction << "%, Duration: " << duration << " seconds" << endl;
    }
};

// Shop Class
class Shop {
private:
    item** items;
    int itemCount;

public:
    Shop(int maxItems) {
        items = new item * [maxItems];
        itemCount = 0;
    }

    void addItem(item* newItem) {
        items[itemCount++] = newItem;
    }

    item* getItem(int index) const {
        if (index >= 0 && index < itemCount) {
            return items[index];
        }
        return nullptr;
    }

    int getItemCount() const {
        return itemCount;
    }

    void displayItems() const {
        cout << "Shop Items:" << endl;
        for (int i = 0; i < itemCount; ++i) {
            cout << i + 1 << ". ";
            items[i]->display();
        }
    }

    ~Shop() {
        for (int i = 0; i < itemCount; i++) {
            delete items[i];
        }
        delete[] items;
    }
};

// Player class
class Player {
private:
    string name;
    int currency;
    pets* pet;
    item** inventory;
    int inventorySize;
    int inventoryCapacity;
    
public:
    Player(int maxInventory) : currency(10000), pet(nullptr), inventorySize(0), inventoryCapacity(maxInventory) {
        inventory = new item * [maxInventory];
    }

    ~Player() {
        delete pet; // Free the dynamically allocated memory for pet
        for (int i = 0; i < inventorySize; ++i) {
            delete inventory[i];
        }
        delete[] inventory;
    }

    void setName(string n) {
        name = n;
    }
    void setCurrency(int a) {
        currency = a;
    }
    void createNewPlayer() {
        pets* selectedPet;
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);
       
        // Save player to the registry file
        ofstream registryFile("players_registry.txt", ios::app);
        if (registryFile.is_open()) {
  
			registryFile << name << endl;
			registryFile << currency << endl;   

            registryFile.close();
        } else {
            cout << "Error: Unable to open players registry file for writing." << endl;
        }

        choosePet(); // Allow the player to select a pet
    }

    void choosePet()
    {
        int choice;
        while (true) {
            cout << "Choose a pet:\n1. Dragon\n2. Griffin\n3. Unicorn\n4. Phenix" << endl;
            cout << "\nEnter Choice: ";
            cin >> choice;

            // Dynamically allocate memory for the selected pet
            if (choice == 1) {
                pet = new Dragon("L-DRAGO DESTRUCTOR", "D01", 1, 100, 30, 20, 500, 500);

                break;
            } else if (choice == 2) {
                pet = new Griffin("BUCK_BEEK", "G01", 1, 90, 40, 25, 450, 450);

                break;
            } else if (choice == 3) {
                pet = new Unicorn("STRIKER", "S01", 1, 85, 50, 30, 450, 500);

                break;
            } else if (choice == 4) {
                pet = new Phenix("FIRE BLAZE", "P01", 1, 50, 35, 40, 475, 475);

                break;
            } else {
                cout << "Choose a valid option!" << endl;
            }
        }
    }

    void choosepet(pets* pet) {
        if (this->pet) {
            delete this->pet; // Clean up old pet, if any
        }
        this->pet = pet; // Assign the new pet
    }

    

    void displayPlayer() const {
        cout << "Player Name: " << name << endl;
        cout << "Currency: " << currency << endl;
        if (pet) {
            cout << "Pet: " << pet->getName() << endl;
        } else {
            cout << "No pet assigned." << endl;
        }
    }

    void buyItem(Shop& shop, int index) {
        item* selectedItem = shop.getItem(index);
        if (!selectedItem) {
            cout << "Invalid item selection!" << endl;
            return;
        }

        if (currency >= selectedItem->getPrice() && selectedItem->getQuantity() > 0) {
            currency -= selectedItem->getPrice();
            selectedItem->reduceQuantity();

            // Add item to inventory
            if (inventorySize < inventoryCapacity) {
                inventory[inventorySize++] = new item(*selectedItem); // Clone item
                cout << "Purchased " << selectedItem->getName() << "! Remaining currency: " << currency << endl;
            }
            else {
                cout << "Inventory full! Cannot add more items." << endl;
            }
        }
        else {
            cout << "Not enough currency or item out of stock!" << endl;
        }
    }

    void displayBattlePotions(sf::RenderWindow& window, const sf::Font& font) const {
        float xOffset = 50.0f;
        float yOffset = 600.0f;

        for (int i = 0; i < inventorySize; ++i) {
            // Display potion name
            sf::Text potionName;
            potionName.setFont(font);
            potionName.setCharacterSize(18);
            potionName.setFillColor(sf::Color::White);
            potionName.setString(inventory[i]->getName());
            potionName.setPosition(xOffset, yOffset);

            // Draw the text
            window.draw(potionName);

            // Adjust for next item
            yOffset += 30.0f;
        }
    }

    void usePotion(int index, pets* target) {
        if (index >= 0 && index < inventorySize) {
            inventory[index]->use(target);
        }
        else {
            cout << "Invalid potion selection!" << endl;
        }
    }

    void displayInventory() const {
        cout << "Player Inventory:" << endl;
        for (int i = 0; i < inventorySize; ++i) {
            cout << i + 1 << ". ";
            inventory[i]->display();
        }
    }

    // New methods added
    int getInventorySize() const {
        return inventorySize;
    }

    item* getInventoryItem(int index) const {
        if (index >= 0 && index < inventorySize) {
            return inventory[index];
        }
        return nullptr;
    }

    void addMoney(int amount) {
        currency += amount;
        cout << "Currency updated! New balance: " << currency << endl;
    }

    int getCurrency() const {
        return currency;
    }

    pets* getPet() const {
        return pet;
    }

	void SpecialAbility(pets* enemy) {
		if (pet) {
			pet->useAbility(enemy);
		}
	}

    string getname() const {
		return name;
    }

};

// Obstacle Class
class Obstacle {
public:
    sf::Sprite sprite;
    int x, y;

    Obstacle(sf::Texture& texture, int x, int y) : x(x), y(y) {
        sprite.setTexture(texture);
        sprite.setPosition(y * TILE_SIZE, x * TILE_SIZE);
    }
};

// Power-Up Class
class PowerUp {
public:
    sf::Sprite sprite;
    int x, y;

    PowerUp(sf::Texture& texture, int x, int y) : x(x), y(y) {
        sprite.setTexture(texture);
        sprite.setPosition(y * TILE_SIZE, x * TILE_SIZE);
    }
};

// Training Camp Class
class TrainingCamp {
private:
    sf::RenderWindow& window;
    sf::Texture petTexture, obstacleTexture, powerUpTexture, backgroundTexture;
    sf::Sprite petSprite, backgroundSprite;
    Obstacle** obstacles; // Dynamically allocated array of obstacles
    PowerUp** powerUps;   // Dynamically allocated array of power-ups
    int obstacleCount, powerUpCount;

    int playerX, playerY; // Player's position on the grid
    int hpIncrease, attackIncrease, speedIncrease; // Training stats

    // Normalize image sizes
    void normalizeSpriteSize(sf::Sprite& sprite) {
        sf::Vector2u originalSize = sprite.getTexture()->getSize();
        float scaleX = static_cast<float>(TILE_SIZE) / originalSize.x;
        float scaleY = static_cast<float>(TILE_SIZE) / originalSize.y;
        sprite.setScale(scaleX, scaleY);
    }

    void drawMap() {
        // Draw the background
        window.draw(backgroundSprite);

        // Draw pets
        petSprite.setPosition(playerY * TILE_SIZE, playerX * TILE_SIZE);
        window.draw(petSprite);

        // Draw obstacles
        for (int i = 0; i < obstacleCount; i++) {
            window.draw(obstacles[i]->sprite);
        }

        // Draw power-ups
        for (int i = 0; i < powerUpCount; i++) {
            window.draw(powerUps[i]->sprite);
        }
    }

    bool checkCollision() {
        // Check if the player collides with any obstacle
        for (int i = 0; i < obstacleCount; i++) {
            if (playerX == obstacles[i]->x && playerY == obstacles[i]->y) {
                return true;
            }
        }
        return false;
    }

    void checkPowerUpCollection() {
        // Check if the player collects a power-up
        for (int i = 0; i < powerUpCount; i++) {
            if (playerX == powerUps[i]->x && playerY == powerUps[i]->y) {
                // Collect the power-up and increase stats
                hpIncrease += 10;
                attackIncrease += 5;
                speedIncrease += 3;

                // Remove the power-up
                delete powerUps[i];
                for (int j = i; j < powerUpCount - 1; j++) {
                    powerUps[j] = powerUps[j + 1];
                }
                powerUpCount--;
                break;
            }
        }
    }

    void displayFinalStats() {
        // Clear the screen
        window.clear();

        // Display final stats visually
        sf::Font font;
        font.loadFromFile("Revenge of Gods.ttf"); 
        sf::Text statsText;
        statsText.setFont(font);
        statsText.setCharacterSize(24);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(50, 50);

        // Format the stats
        std::string stats = "Training Complete!\n";
        stats += "HP Increase: " + std::to_string(hpIncrease) + "\n";
        stats += "Attack Increase: " + std::to_string(attackIncrease) + "\n";
        stats += "Speed Increase: " + std::to_string(speedIncrease) + "\n";

        statsText.setString(stats);

        // Draw the stats
        window.draw(statsText);
        window.display();

        // Wait for user to proceed
        sf::sleep(sf::seconds(3));
    }

public:
    TrainingCamp(sf::RenderWindow& win, const std::string& petImagePath, const std::string& obstacleImagePath,
        const std::string& powerUpImagePath, const std::string& backgroundImagePath)
        : window(win), playerX(CAMP_ROWS - 1), playerY(CAMP_COLS / 2),
        hpIncrease(0), attackIncrease(0), speedIncrease(0) {
        // Load textures
        petTexture.loadFromFile(petImagePath);
        obstacleTexture.loadFromFile(obstacleImagePath);
        powerUpTexture.loadFromFile(powerUpImagePath);
        backgroundTexture.loadFromFile(backgroundImagePath);

        // Set up pet sprite
        petSprite.setTexture(petTexture);
        normalizeSpriteSize(petSprite);

        // Set up background sprite
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );

        // Initialize obstacles
        obstacleCount = 9;
        obstacles = new Obstacle * [obstacleCount];
        srand(static_cast<unsigned>(time(0)));
        for (int i = 0; i < obstacleCount; i++) {
            int x = rand() % (CAMP_ROWS - 1);
            int y = rand() % CAMP_COLS;
            obstacles[i] = new Obstacle(obstacleTexture, x, y);
            normalizeSpriteSize(obstacles[i]->sprite);
        }

        // Initialize power-ups
        powerUpCount = 7;
        powerUps = new PowerUp * [powerUpCount];
        for (int i = 0; i < powerUpCount; i++) {
            int x = rand() % (CAMP_ROWS - 1);
            int y = rand() % CAMP_COLS;
            powerUps[i] = new PowerUp(powerUpTexture, x, y);
            normalizeSpriteSize(powerUps[i]->sprite);
        }
    }

    ~TrainingCamp() {
        for (int i = 0; i < obstacleCount; i++) {
            delete obstacles[i];
        }
        delete[] obstacles;

        for (int i = 0; i < powerUpCount; i++) {
            delete powerUps[i];
        }
        delete[] powerUps;
    }

    void startTraining() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // Handle player movement
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::W && playerX > 0) playerX--;           // Move up
                    else if (event.key.code == sf::Keyboard::S && playerX < CAMP_ROWS - 1) playerX++; // Move down
                    else if (event.key.code == sf::Keyboard::A && playerY > 0) playerY--;           // Move left
                    else if (event.key.code == sf::Keyboard::D && playerY < CAMP_COLS - 1) playerY++; // Move right
                }
            }

            // Check for collisions
            if (checkCollision()) {
                std::cout << "You hit an obstacle! Training session ended." << std::endl;
                displayFinalStats();
                return;
            }

            // Check for power-up collection
            checkPowerUpCollection();

            // Render everything
            window.clear();
            drawMap();
            window.display();
        }
    }
};

class Guild {
private:
   string attacker;
   string tank;
   string healer;
   static void normalizeSpriteSize(sf::Sprite& sprite, const sf::Vector2f& targetSize) {
       sf::Vector2u originalSize = sprite.getTexture()->getSize();
       float scaleX = targetSize.x / originalSize.x;
       float scaleY = targetSize.y / originalSize.y;
       sprite.setScale(scaleX, scaleY);
   }
public:
    Guild() : attacker(""), tank(""), healer("") {}

    void setAttacker(const string& petName) {
        attacker = petName;
    }

    void setTank(const string& petName) {
        tank = petName;
    }

    void setHealer(const string& petName) {
        healer = petName;
    }

    bool isComplete()  {
        return !attacker.empty() && !tank.empty() && !healer.empty();
    }

    string getAttacker() const { return attacker; }
    string getTank() const { return tank; }
    string getHealer() const { return healer; }

    void displayGuild() const {
        cout << "Your Guild:\n";
        cout << "Attacker: " << (attacker.empty() ? "None" : attacker) << endl;
        cout << "Tank: " << (tank.empty() ? "None" : tank) << endl;
        cout << "Healer: " << (healer.empty() ? "None" : healer) << endl;
    }

    static void startGuildBattle(Player* player,Guild* guild, sf::RenderWindow& window);
};

class Fireball {
private:
    sf::Sprite sprite;
    float speed;
    int damage;

public:
    Fireball(const sf::Texture& texture, float startX, float startY, float fireSpeed, int fireDamage)
        : speed(fireSpeed), damage(fireDamage) {
        sprite.setTexture(texture);
        sprite.setPosition(startX, startY);
    }

    void move() {
        sprite.move(speed, 0); // Move fireball horizontally
    }

    bool isOffScreen(float screenWidth) const {
        return (sprite.getPosition().x > screenWidth || sprite.getPosition().x < 0);
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    int getDamage() const {
        return damage;
    }
};

class Battle {
private:
    static void normalizeSpriteSize(sf::Sprite& sprite, const sf::Vector2f& targetSize) {
        sf::Vector2u originalSize = sprite.getTexture()->getSize();
        float scaleX = targetSize.x / originalSize.x;
        float scaleY = targetSize.y / originalSize.y;
        sprite.setScale(scaleX, scaleY);
    }
public:
    // 1v1 Battle Function
    static void start1v1Battle(Player* player, sf::RenderWindow& window);

    // 2v2 Battle 
    static void start2v2Battle(Player* player, sf::RenderWindow& window);
};

void Battle::start1v1Battle(Player* player, sf::RenderWindow& window) {

    pets* selectedPet = nullptr; // Pointer for the selected pet
    pets* aiPet = nullptr;       // Pointer for the AI pet
    pets* playerPet = nullptr;   // Pointer for the player's pet


    // Load background texture for pet selection
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("pet_selection_background.jpg")) {
        std::cerr << "Error: Could not load pet selection background texture!" << std::endl;
        return;
    }
    sf::Sprite bgSprite(bgTexture);

    // Scale background to fit the pet selection window
    bgSprite.setScale(
        static_cast<float>(window.getSize().x) / bgTexture.getSize().x,
        static_cast<float>(window.getSize().y) / bgTexture.getSize().y
    );

    // Load pet textures
    sf::Texture dragonTexture, unicornTexture, griffinTexture, phenixTexture;
    if (!dragonTexture.loadFromFile("dragon.png") ||
        !unicornTexture.loadFromFile("unicorn.png") ||
        !griffinTexture.loadFromFile("griffin.png") ||
        !phenixTexture.loadFromFile("phoenix.png")) {
        std::cerr << "Error: Could not load pet textures!" << std::endl;
        return;
    }

    // Define the pets to choose from
    PetVisual pets[4];
    pets[0] = { sf::Sprite(dragonTexture), "Dragon" };
    pets[1] = { sf::Sprite(unicornTexture), "Unicorn" };
    pets[2] = { sf::Sprite(griffinTexture), "Griffin" };
    pets[3] = { sf::Sprite(phenixTexture), "Phenix" };

    // Normalize sprite sizes and positions
    const float targetWidth = window.getSize().x * 0.1f; // 10% of screen width
    const float targetHeight = window.getSize().y * 0.2f; // 20% of screen height
    const float spacing = window.getSize().x * 0.05f; // 5% of screen width
    float currentX = spacing;

    for (int i = 0; i < 4; ++i) {
        normalizeSpriteSize(pets[i].sprite, sf::Vector2f(targetWidth, targetHeight));
        pets[i].sprite.setPosition(currentX, (window.getSize().y - targetHeight) / 2.0f);
        currentX += targetWidth + spacing;
    }

    // Prompt message
    sf::Font font;
    if (!font.loadFromFile("Revenge of Gods.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return;
    }

    // Display prompt for pet selection
    sf::Text promptText("Select Your Pet for the Battle!", font, window.getSize().y * 0.05f);
    promptText.setFillColor(sf::Color::White);
    sf::FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setOrigin(promptBounds.left + promptBounds.width / 2, promptBounds.top + promptBounds.height / 2);
    promptText.setPosition(window.getSize().x / 2, window.getSize().y * 0.1f);

    sf::Sprite playerPetSprite;

    while (window.isOpen() && selectedPet == nullptr) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // Handle pet selection
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 4; ++i) {
                    if (pets[i].sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        
                        if (pets[i].name == "Dragon") {
                            selectedPet = new Dragon("Player's Dragon", "D01", 1, 100, 30, 20, 500, 500);
                            playerPetSprite.setTexture(dragonTexture);
                        }
                        else if (pets[i].name == "Unicorn") {
                            selectedPet = new Unicorn("Player's Unicorn", "U01", 1, 85, 50, 30, 450, 500);
                            playerPetSprite.setTexture(unicornTexture);
                        }
                        else if (pets[i].name == "Griffin") {
                            selectedPet = new Griffin("Player's Griffin", "G01", 1, 90, 40, 25, 450, 450);
                            playerPetSprite.setTexture(griffinTexture);
                        }
                        else if (pets[i].name == "Phenix") {
                            selectedPet = new Phenix("Player's Phenix", "P01", 1, 50, 35, 40, 475, 475);
                            playerPetSprite.setTexture(phenixTexture);
                        }
                        player->choosepet(selectedPet);
                        normalizeSpriteSize(playerPetSprite, sf::Vector2f(targetWidth, targetHeight));
                        playerPetSprite.setPosition(150, window.getSize().y / 2 - targetHeight / 2);

                        break;
                    }
                }
            }
        }

        // Render pet selection screen
        window.clear();
        window.draw(bgSprite);
        window.draw(promptText);
        for (int i = 0; i < 4; ++i) {
            window.draw(pets[i].sprite);
        }
        window.display();

    }

    sf::RenderWindow battleWindow(sf::VideoMode(1280, 720), "Battle");
    battleWindow.setFramerateLimit(60);
    // Set up the battle background
    if (!bgTexture.loadFromFile("battle.jpg")) {
        cerr << "Error: Could not load battle background texture!" << endl;
        return;
    }
    bgSprite.setTexture(bgTexture);

    // Set up the player pet sprite

    if (!selectedPet) {
        std::cerr << "Error: No pet selected!" << std::endl;
        return;
    }

    // Retrieve the player's pet
    playerPet = player->getPet();
    if (!playerPet) {
        std::cerr << "Error: Player pet not assigned!" << std::endl;
        return;
    }

    // Create AI pet
    aiPet = new Dragon("AI Dragon", "AI01", 1, 80, 25, 15, 400, 400);

    // Player and AI sprites
    sf::Texture aiPetTexture;
    aiPetTexture.loadFromFile("ai_pet.png");
    if (!aiPetTexture.loadFromFile("ai_pet.png")) {
        cerr << "Error loading AI pet texture!" << endl;
    }

    sf::Sprite aiPetSprite(aiPetTexture);
    normalizeSpriteSize(aiPetSprite, sf::Vector2f(targetWidth, targetHeight));
    aiPetSprite.setPosition(battleWindow.getSize().x - 150 - targetWidth, battleWindow.getSize().y / 2 - targetHeight / 2);

    // Fireball and movement adjustments
    float fireballSpeed = battleWindow.getSize().x * 0.015f; // 0.5% of screen width per frame
    float playerSpeed = battleWindow.getSize().x * 0.008f;   // 0.2% of screen width per frame

    // Health bars
    sf::RectangleShape playerHPBar(sf::Vector2f(200, 20));
    sf::RectangleShape aiHPBar(sf::Vector2f(200, 20));
    playerHPBar.setFillColor(sf::Color::Green);
    aiHPBar.setFillColor(sf::Color::Red);
    playerHPBar.setPosition(50, 50); // Player HP bar position
    aiHPBar.setPosition(600, 50);    // AI HP bar position

    // Timer
    int battleTimer = 60; // 1-minute duration
    sf::Clock battleClock;
    sf::Text timerText("60", sf::Font(), 24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(350, 10);

    // Fireball textures
    sf::Texture playerFireballTexture, enemyFireballTexture;
    if (!playerFireballTexture.loadFromFile("player_fireball.png") ||
        !enemyFireballTexture.loadFromFile("enemy_fireball.png")) {
        std::cerr << "Error: Could not load fireball textures!" << std::endl;
        return;
    }
    // Fireball size adjustment
    const sf::Vector2f fireballTargetSize(window.getSize().x * 0.02f, window.getSize().x * 0.02f);

    // Fireballs
    Fireball* playerFireballs[MAX_FIREBALLS] = { nullptr };
    Fireball* enemyFireballs[MAX_FIREBALLS] = { nullptr };

    sf::Clock enemyFireCooldown;
    bool canEnemyFire = true;
    sf::Clock playerFireCooldown;
    bool canPlayerFire = true;

    // Game loop
    while (battleWindow.isOpen() && battleTimer > 0 && playerPet->hp > 0 && aiPet->hp > 0) {
        sf::Event event;
        while (battleWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                battleWindow.close();
                return;
            }

            // Player movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                playerPetSprite.move(0, -playerSpeed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                playerPetSprite.move(0, playerSpeed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                playerPetSprite.move(-playerSpeed, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                playerPetSprite.move(playerSpeed, 0);
            }

            // Player fires fireball
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::F) && canPlayerFire){
                for (int i = 0; i < MAX_FIREBALLS; ++i) {
                    if (playerFireballs[i] == nullptr) {
                        playerFireballs[i] = new Fireball(
                            playerFireballTexture,
                            playerPetSprite.getPosition().x + playerPetSprite.getGlobalBounds().width,
                            playerPetSprite.getPosition().y + playerPetSprite.getGlobalBounds().height / 2,
                            10.0f, // Fireball speed
                                                   playerPet->powerattack
                        );
                        // Adjust fireball size
                        playerFireballs[i]->getSprite().setScale(
                            fireballTargetSize.x / playerFireballTexture.getSize().x,
                            fireballTargetSize.y / playerFireballTexture.getSize().y
                        );
                        canPlayerFire = false;
                        playerFireCooldown.restart();
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                            player->SpecialAbility(aiPet);
							cout << "Special Ability Used!" << endl;
                        }
                        break;
                    }
                }
            }
        }

        // Cooldown for player fireball
        if (!canPlayerFire && playerFireCooldown.getElapsedTime().asSeconds() > 0.2f) {
            canPlayerFire = true;
        }

        // Enemy fires fireball randomly
        if (canEnemyFire && rand() % 100 < 10) {  // 10% chance to fire each frame
            for (int i = 0; i < MAX_FIREBALLS; ++i) {
                if (enemyFireballs[i] == nullptr) {
                    enemyFireballs[i] = new Fireball(
                        enemyFireballTexture,
                        aiPetSprite.getPosition().x - 50,
                        aiPetSprite.getPosition().y + aiPetSprite.getGlobalBounds().height / 2,
                        -8.0f,  // Fireball speed (negative for leftward movement)
                        aiPet->powerattack
                    );
                    // Adjust fireball size
                    enemyFireballs[i]->getSprite().setScale(
                        fireballTargetSize.x / enemyFireballTexture.getSize().x,
                        fireballTargetSize.y / enemyFireballTexture.getSize().y
                    );
                    canEnemyFire = false;
                    enemyFireCooldown.restart();
                    break;
                }
            }
        }

        // Cooldown for enemy fireball
        if (!canEnemyFire && enemyFireCooldown.getElapsedTime().asSeconds() > 1.0f) {
            canEnemyFire = true;
        }

        // Update fireballs
        for (int i = 0; i < MAX_FIREBALLS; ++i) {
            // Player fireballs
            if (playerFireballs[i]) {
                playerFireballs[i]->move();
                if (playerFireballs[i]->isOffScreen(window.getSize().x) ||
                    playerFireballs[i]->getSprite().getGlobalBounds().intersects(aiPetSprite.getGlobalBounds())) {
                    if (playerFireballs[i]->getSprite().getGlobalBounds().intersects(aiPetSprite.getGlobalBounds())) {
                        aiPet->takedamage(playerFireballs[i]->getDamage());
                    }
                    delete playerFireballs[i];
                    playerFireballs[i] = nullptr;
                }
            }

            // Enemy fireballs
            if (enemyFireballs[i]) {
                enemyFireballs[i]->move();
                if (enemyFireballs[i]->isOffScreen(battleWindow.getSize().x) ||
                    enemyFireballs[i]->getSprite().getGlobalBounds().intersects(playerPetSprite.getGlobalBounds())) {
                    if (enemyFireballs[i]->getSprite().getGlobalBounds().intersects(playerPetSprite.getGlobalBounds())) {
                        playerPet->takedamage(enemyFireballs[i]->getDamage());
                    }
                    delete enemyFireballs[i];
                    enemyFireballs[i] = nullptr;
                }
            }
        }
        // Ensure playerPet and aiPet are valid
        if (!playerPet || !aiPet) {
            cerr << "Error: playerPet or aiPet is null!" << endl;
            return; // Exit the function if either pointer is null
        }

        if (playerPet && aiPet) {
            playerHPBar.setSize(sf::Vector2f(200 * (float(playerPet->hp) / playerPet->maxhp), 20));
            aiHPBar.setSize(sf::Vector2f(200 * (float(aiPet->hp) / aiPet->maxhp), 20));
        }
        else {
            cerr << "Error: playerPet or aiPet is null, cannot update HP bars." << endl;
            return;
        }

        // Rendering Logic
        if (battleWindow.isOpen()) {
            try {
                battleWindow.clear();

                // Debug: Ensure textures are valid before rendering
                if (!bgSprite.getTexture()) {
                    std::cerr << "Error: bgSprite texture is null!" << std::endl;
                    return;
                }
                battleWindow.draw(bgSprite);

                // Draw player and AI sprites
                if (playerPetSprite.getTexture()) {
                    battleWindow.draw(playerPetSprite);
                }
                else {
                    std::cerr << "Error: playerPetSprite texture is null!" << endl;
                }

                if (aiPetSprite.getTexture()) {
                    battleWindow.draw(aiPetSprite);
                }
                else {
                    std::cerr << "Error: aiPetSprite texture is null!" << endl;
                }

                // Draw HP bars
                battleWindow.draw(playerHPBar);
                battleWindow.draw(aiHPBar);

                // Draw fireballs
                for (int i = 0; i < MAX_FIREBALLS; ++i) {
                    if (playerFireballs[i]) {
                        battleWindow.draw(playerFireballs[i]->getSprite());
                    }
                    if (enemyFireballs[i]) {
                        battleWindow.draw(enemyFireballs[i]->getSprite());
                    }
                }

                window.display();
            }
            catch (const std::exception& e) {
                cerr << "Exception occurred during rendering: " << e.what() << endl;
                return;
            }
            catch (...) {
                cerr << "Unknown exception occurred during rendering." << endl;
                return;
            }
        }
        else {
            cerr << "Error: Window is not open. Exiting rendering loop." << endl;
        }

        battleWindow.display();
    }

    if (playerPet->hp > 0) {
        sf::RenderWindow resultWindow(sf::VideoMode(400, 200), "Battle Result");
        sf::Font resultFont;
        if (!resultFont.loadFromFile("Revenge of Gods.ttf")) {
            cerr << "Error loading font!" << endl;
        }
        sf::Text resultText;
        resultText.setFont(resultFont);
        resultText.setCharacterSize(24);
        resultText.setFillColor(sf::Color::White);
        resultText.setPosition(50, 50);
        resultText.setString("You WON!!\n" + playerPet->getName());
        selectedPet->levelup();
        cout << "Level up" << endl;
		player->addMoney(100); // Add money to player after winning
        while (resultWindow.isOpen()) {
            sf::Event event;
            while (resultWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    resultWindow.close();
                }
            }
            resultWindow.clear();
            resultWindow.draw(resultText);
            resultWindow.display();
        }
        // Display victory message
        cout << "Player WON!!" << endl;
    }
    else if (aiPet->hp > 0) {
        sf::RenderWindow resultWindow(sf::VideoMode(400, 200), "Battle Result");
        sf::Font resultFont;
        if (!resultFont.loadFromFile("Revenge of Gods.ttf")) {
            cerr << "Error loading font!" << endl;
        }
        sf::Text resultText;
        resultText.setFont(resultFont);
        resultText.setCharacterSize(24);
        resultText.setFillColor(sf::Color::White);
        resultText.setPosition(50, 50);
        resultText.setString("You LOSE!!\n" + playerPet->getName());
        while (resultWindow.isOpen()) {
            sf::Event event;
            while (resultWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    resultWindow.close();
                }
            }
            resultWindow.clear();
            resultWindow.draw(resultText);
            resultWindow.display();
        }
        cout << "YOU LOSE!!" << endl;
    }

    // Cleanup
    for (int i = 0; i < MAX_FIREBALLS; ++i) {
        delete playerFireballs[i];
        delete enemyFireballs[i];
    }
    delete aiPet;

}

void Battle::start2v2Battle(Player* player, sf::RenderWindow& window) {

    pets* selectedPet[2] = { nullptr, nullptr }; // Pointer for the selected pet
    pets* aiPet[2] = { nullptr, nullptr };       // Pointer for the AI pet
    pets* playerPet = nullptr;   // Pointer for the player's pet


    // Load background texture for pet selection
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("pet_selection_background.jpg")) {
        std::cerr << "Error: Could not load pet selection background texture!" << std::endl;
        return;
    }
    sf::Sprite bgSprite(bgTexture);

    // Scale background to fit the pet selection window
    bgSprite.setScale(
        static_cast<float>(window.getSize().x) / bgTexture.getSize().x,
        static_cast<float>(window.getSize().y) / bgTexture.getSize().y
    );

    // Load pet textures
    sf::Texture dragonTexture, unicornTexture, griffinTexture, phenixTexture;
    if (!dragonTexture.loadFromFile("dragon.png") ||
        !unicornTexture.loadFromFile("unicorn.png") ||
        !griffinTexture.loadFromFile("griffin.png") ||
        !phenixTexture.loadFromFile("phoenix.png")) {
        std::cerr << "Error: Could not load pet textures!" << endl;
        return;
    }

    // Define the pets to choose from
    PetVisual pets[4];
    pets[0] = { sf::Sprite(dragonTexture), "Dragon" };
    pets[1] = { sf::Sprite(unicornTexture), "Unicorn" };
    pets[2] = { sf::Sprite(griffinTexture), "Griffin" };
    pets[3] = { sf::Sprite(phenixTexture), "Phenix" };

    // Normalize sprite sizes and positions
    const float targetWidth = window.getSize().x * 0.1f; // 10% of screen width
    const float targetHeight = window.getSize().y * 0.2f; // 20% of screen height
    const float spacing = window.getSize().x * 0.05f; // 5% of screen width
    float currentX = spacing;

    for (int i = 0; i < 4; ++i) {
        normalizeSpriteSize(pets[i].sprite, sf::Vector2f(targetWidth, targetHeight));
        pets[i].sprite.setPosition(currentX, (window.getSize().y - targetHeight) / 2.0f);
        currentX += targetWidth + spacing;
    }

    // Prompt message
    sf::Font font;
    if (!font.loadFromFile("Revenge of Gods.ttf")) {
        std::cerr << "Error: Could not load font!" << endl;
        return;
    }

    // Display prompt for pet selection
    sf::Text promptText("Select Your TWO Pet for the Battle!", font, window.getSize().y * 0.05f);
    promptText.setFillColor(sf::Color::White);
    sf::FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setOrigin(promptBounds.left + promptBounds.width / 2, promptBounds.top + promptBounds.height / 2);
    promptText.setPosition(window.getSize().x / 2, window.getSize().y * 0.1f);

    sf::Sprite playerPetSprite[2];
	int selectedCount = 0; // Count of selected pets    

    while (window.isOpen() && selectedCount < 2) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // Handle pet selection
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 4; ++i) {
                    if (pets[i].sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        // Assign the selected pet
                        bool alreadyselected = false;
                        for (int j = 0; j < selectedCount; ++j) {
                            if (selectedPet[j]->getName() == pets[i].name) {
                                alreadyselected = true;
                                break;
                            }
                        }
                        if (alreadyselected) continue;

                        // Assign the selected pet
                        if (pets[i].name == "Dragon") {
                            selectedPet[selectedCount] = new Dragon("Player's Dragon", "D01", 1, 100, 30, 20, 500, 500);
                            playerPetSprite[selectedCount].setTexture(dragonTexture);
                            cout << "DRAGON SELECTED" << endl;
                        }
                        else if (pets[i].name == "Unicorn") {
                            selectedPet[selectedCount] = new Unicorn("Player's Unicorn", "U01", 1, 85, 50, 30, 450, 500);
                            playerPetSprite[selectedCount].setTexture(unicornTexture);
							cout << "UNICORN SELECTED" << endl; 
                        }
                        else if (pets[i].name == "Griffin") {
                            selectedPet[selectedCount] = new Griffin("Player's Griffin", "G01", 1, 90, 40, 25, 450, 450);
                            playerPetSprite[selectedCount].setTexture(griffinTexture);
							cout << "GRIFFIN SELECTED" << endl;
                        }
                        else if (pets[i].name == "Phenix") {
                            selectedPet[selectedCount] = new Phenix("Player's Phenix", "P01", 1, 50, 35, 40, 475, 475);
                            playerPetSprite[selectedCount].setTexture(phenixTexture);
							cout << "PHENIX SELECTED" << endl;
                        }
                        normalizeSpriteSize(playerPetSprite[selectedCount], sf::Vector2f(targetWidth, targetHeight));
                        playerPetSprite[selectedCount].setPosition(
                            150, (window.getSize().y / 2 - targetHeight / 2) + selectedCount * targetHeight
                        );
                        selectedCount++;
                        break;
                    }
                }
            }
        }

        // Render pet selection screen
        window.clear();
        window.draw(bgSprite);
        window.draw(promptText);
        for (int i = 0; i < 4; ++i) {
            window.draw(pets[i].sprite);
        }
        window.display();

    }

    sf::RenderWindow battleWindow(sf::VideoMode(1280, 720), "Battle");
    battleWindow.setFramerateLimit(60);
    // Set up the battle background
    if (!bgTexture.loadFromFile("battle.jpg")) {
        cerr << "Error: Could not load battle background texture!" << endl;
        return;
    }
    bgSprite.setTexture(bgTexture);

    // Ensure two pets are selected
    if (selectedCount < 2) {
        cerr << "Error: Not enough pets selected!" << endl;
        return;
    }

    // Assign selected pets to the player
    for (int i = 0; i < 2; ++i) {
        player->choosepet(selectedPet[i]);
    }

    // Create AI pet
    aiPet[0] = new Dragon("AI Dragon", "AI01", 1, 80, 25, 15, 400, 400);
	aiPet[1] = new Griffin("AI Griffin", "AI02", 1, 90, 40, 25, 450, 450);

    // Player and AI sprites
    sf::Texture aiPetTexture[2];
    aiPetTexture[0].loadFromFile("ai_pet.png");
    if (!aiPetTexture[0].loadFromFile("ai_pet.png")) {
        cerr << "Error loading AI pet1 texture!" << endl;
    }
    aiPetTexture[1].loadFromFile("ai_pet2.png"); {
		if (!aiPetTexture[1].loadFromFile("ai_pet2.png")) {
			cerr << "Error loading AI pet2 texture!" << endl;
		}
    }
    sf::Sprite aiPetSprite[2];
	aiPetSprite[0].setTexture(aiPetTexture[0]);  
    aiPetSprite[1].setTexture(aiPetTexture[1]);
    for (int i = 0;i < 2;i++) {
		normalizeSpriteSize(aiPetSprite[i], sf::Vector2f(targetWidth, targetHeight));
		aiPetSprite[i].setPosition(battleWindow.getSize().x - 150 - targetWidth, battleWindow.getSize().y / 2 - targetHeight / 2 + i * targetHeight);
    }

        Fireball* playerFireballs[MAX_FIREBALLS] = { nullptr };
        Fireball* enemyFireballs[MAX_FIREBALLS] = { nullptr };
        float fireballSpeed = battleWindow.getSize().x * 0.015f; // Fireball speed
        float playerSpeed = battleWindow.getSize().x * 0.008f;   // Player movement speed
    
        // Health bars
    
        const float HP_BAR_LENGTH = window.getSize().x * 0.15f; // 15% of screen width
        sf::RectangleShape playerHPBars[2];
        sf::RectangleShape aiHPBars[2];
    
        for (int i = 0; i < 2; i++) {
            playerHPBars[i] = sf::RectangleShape(sf::Vector2f(HP_BAR_LENGTH, 20));
            aiHPBars[i] = sf::RectangleShape(sf::Vector2f(HP_BAR_LENGTH, 20));
    
            playerHPBars[i].setFillColor(i == 0 ? sf::Color::Green : sf::Color::Blue);
            aiHPBars[i].setFillColor(i == 0 ? sf::Color::Red : sf::Color::Yellow);
    
            playerHPBars[i].setPosition(50, 50 + i * 50);
            aiHPBars[i].setPosition(window.getSize().x - HP_BAR_LENGTH - 50, 50 + i * 50);
        }
    
        // Timer
        int battleTimer = 60; // 1-minute duration
        sf::Clock battleClock;
        sf::Text timerText("60", sf::Font(), 24);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(350, 10);
    
        // Fireball textures
        sf::Texture playerFireballTexture, enemyFireballTexture;
        if (!playerFireballTexture.loadFromFile("player_fireball.png") ||
            !enemyFireballTexture.loadFromFile("enemy_fireball.png")) {
            std::cerr << "Error: Could not load fireball textures!" << endl;
            return;
        }
        // Fireball size adjustment
        const sf::Vector2f fireballTargetSize(battleWindow.getSize().x * 0.02f, battleWindow.getSize().x * 0.02f);
    
        sf::Clock enemyFireCooldown;
        bool canEnemyFire = true;
        sf::Clock playerFireCooldown;
        bool canPlayerFire = true;
    
        // Game loop
        while (battleWindow.isOpen() && battleTimer > 0 &&
            (selectedPet[0]->hp > 0 || selectedPet[1]->hp > 0) &&
            (aiPet[0]->hp > 0 || aiPet[1]->hp > 0) || aiPet[2]->hp >0) {
            sf::Event event;
            while (battleWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    battleWindow.close();
                    return;
                }
    
                // Player movement
                for (int i = 0; i < 2; i++) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                        playerPetSprite[i].move(0, -playerSpeed);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        playerPetSprite[i].move(0, playerSpeed);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                        playerPetSprite[i].move(-playerSpeed, 0);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                        playerPetSprite[i].move(playerSpeed, 0);
                    }
                }
    
                // Player fires fireball
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::F) && canPlayerFire) {
                    for (int i = 0; i < MAX_FIREBALLS; i++) {
                        if (playerFireballs[i] == nullptr) {
                            for (int j = 0; j <2; j++) { // One fireball for each pet
                                playerFireballs[i] = new Fireball(
                                    playerFireballTexture,
                                    playerPetSprite[j].getPosition().x + playerPetSprite[j].getGlobalBounds().width,
                                    playerPetSprite[j].getPosition().y + playerPetSprite[j].getGlobalBounds().height / 2,
                                    fireballSpeed, selectedPet[j]->powerattack
                                );
                                playerFireballs[i]->getSprite().setScale(
                                    fireballTargetSize.x / playerFireballTexture.getSize().x,
                                    fireballTargetSize.y / playerFireballTexture.getSize().y
                                );
                            }
                            canPlayerFire = false;
                            playerFireCooldown.restart();
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                                player->SpecialAbility(aiPet[0]);
                                cout << "Special Ability Used!" << endl;
                            }
                            break;
                        }
                    }
                }
            }
    
            // Cooldown for player fireball
            if (!canPlayerFire && playerFireCooldown.getElapsedTime().asSeconds() > 0.5f) {
                canPlayerFire = true;
            }
    
            // Enemy fires fireball randomly
            if (canEnemyFire && rand() % 100 < 10) { // 10% chance to fire each frame
                for (int i = 0; i < MAX_FIREBALLS; i++) {
                    if (enemyFireballs[i] == nullptr) {
                        for (int j = 0; j < 2; j++) { // One fireball for each pet
                            enemyFireballs[i] = new Fireball(
                                enemyFireballTexture,
                                aiPetSprite[j].getPosition().x,
                                aiPetSprite[j].getPosition().y + aiPetSprite[j].getGlobalBounds().height / 2,
                                -fireballSpeed, aiPet[j]->powerattack
                            );
                            enemyFireballs[i]->getSprite().setScale(
                                fireballTargetSize.x / enemyFireballTexture.getSize().x,
                                fireballTargetSize.y / enemyFireballTexture.getSize().y
                            );
                        }
                        canEnemyFire = false;
                        enemyFireCooldown.restart();
                        break;
                    }
                }
            }
    
            // Cooldown for enemy fireball
            if (!canEnemyFire && enemyFireCooldown.getElapsedTime().asSeconds() > 1.0f) {
                canEnemyFire = true;
            }
    
            // Update fireballs
            for (int i = 0; i < MAX_FIREBALLS; i++) {
                // Player fireballs
                if (playerFireballs[i]) {
                    playerFireballs[i]->move();
                    bool hit = false;
                    for (int j = 0; j < 2; j++) {
                        if (playerFireballs[i]->getSprite().getGlobalBounds().intersects(aiPetSprite[j].getGlobalBounds()) && aiPet[j]->hp > 0) {
                            aiPet[j]->takedamage(playerFireballs[i]->getDamage());
                            hit = true;
                            break;
                        }
                    }
                    if (hit || playerFireballs[i]->isOffScreen(battleWindow.getSize().x)) {
                        delete playerFireballs[i];
                        playerFireballs[i] = nullptr;
                    }
                }
    
                // Enemy fireballs
                if (enemyFireballs[i]) {
                    enemyFireballs[i]->move();
                    bool hit = false;
                    for (int j = 0; j < 2; j++) {
                        if (enemyFireballs[i]->getSprite().getGlobalBounds().intersects(playerPetSprite[j].getGlobalBounds()) && selectedPet[j]->hp > 0) {
                            selectedPet[j]->takedamage(enemyFireballs[i]->getDamage());
                            hit = true;
                            break;
                        }
                    }
                    if (hit || enemyFireballs[i]->isOffScreen(battleWindow.getSize().x)) {
                        delete enemyFireballs[i];
                        enemyFireballs[i] = nullptr;
                    }
                }
            }
    
            // Update HP bars
            for (int i = 0; i < 2; i++) {
                playerHPBars[i].setSize(sf::Vector2f(200 * (float(selectedPet[i]->hp) / selectedPet[i]->maxhp), 20));
                aiHPBars[i].setSize(sf::Vector2f(200 * (float(aiPet[i]->hp) / aiPet[i]->maxhp), 20));
            }
    
    
            // Rendering Logic
            if (battleWindow.isOpen()) {
                try {
                    battleWindow.clear();
    
                    // Debug: Ensure textures are valid before rendering
                    if (!bgSprite.getTexture()) {
                        std::cerr << "Error: bgSprite texture is null!" << std::endl;
                        return;
                    }
                    battleWindow.draw(bgSprite);
    
                    // Draw player and AI sprites
                    if (playerPetSprite[0].getTexture() && playerPetSprite[1].getTexture()) {
                        battleWindow.draw(playerPetSprite[0]);
                        battleWindow.draw(playerPetSprite[1]);
                    }
                    else {
                        std::cerr << "Error: playerPetSprite1 texture is null!" << std::endl;
                    }
    
                    if (aiPetSprite[0].getTexture() && aiPetSprite[1].getTexture() && aiPetSprite[2].getTexture()){
                        battleWindow.draw(aiPetSprite[0]);
                        battleWindow.draw(aiPetSprite[1]);
    					
                    }
                    else {
                        std::cerr << "Error: aiPetSprite texture is null!" << std::endl;
                    }
    
                    // Draw HP bars
                    battleWindow.draw(playerHPBars[0]);
                    battleWindow.draw(playerHPBars[1]);
    			
                    battleWindow.draw(aiHPBars[0]);
                    battleWindow.draw(aiHPBars[1]);
    				
    
                    // Draw fireballs
                    for (int i = 0; i < MAX_FIREBALLS; i++) {
                        if (playerFireballs[i]) battleWindow.draw(playerFireballs[i]->getSprite());
                        if (enemyFireballs[i]) battleWindow.draw(enemyFireballs[i]->getSprite());
                    }
    
                    //battleWindow.draw(timerText);
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception occurred during rendering: " << e.what() << std::endl;
                    return;
                }
                catch (...) {
                    std::cerr << "Unknown exception occurred during rendering." << std::endl;
                    return;
                }
            }
            else {
                std::cerr << "Error: Window is not open. Exiting rendering loop." << std::endl;
            }
    
            battleWindow.display();
        }
    
        if (selectedPet[0]->hp > aiPet[0]->hp > 0 || selectedPet[0]->hp > aiPet[1]->hp > 0 ||selectedPet[1]->hp > aiPet[0]->hp > 0 || selectedPet[1]->hp > aiPet[1]->hp > 0 ) {
            sf::RenderWindow resultWindow(sf::VideoMode(400, 200), "Battle Result");
            sf::Font resultFont;
            if (!resultFont.loadFromFile("Revenge of Gods.ttf")) {
                cerr << "Error loading font!" << endl;
            }
            sf::Text resultText;
            resultText.setFont(resultFont);
            resultText.setCharacterSize(24);
            resultText.setFillColor(sf::Color::White);
            resultText.setPosition(50, 50);
            resultText.setString("You WON!!\n");
            selectedPet[0]->levelup();
            selectedPet[1]->levelup();
            cout << "Level up" << endl;
            player->addMoney(100); // Add money to player after winning
            while (resultWindow.isOpen()) {
                sf::Event event;
                while (resultWindow.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        resultWindow.close();
                    }
                }
                resultWindow.clear();
                resultWindow.draw(resultText);
                resultWindow.display();
            }
            // Display victory message
            cout << "Player WON!!" << endl;
        }
        else  {
            sf::RenderWindow resultWindow(sf::VideoMode(400, 200), "Battle Result");
            sf::Font resultFont;
            if (!resultFont.loadFromFile("Revenge of Gods.ttf")) {
                cerr << "Error loading font!" << endl;
            }
            sf::Text resultText;
            resultText.setFont(resultFont);
            resultText.setCharacterSize(24);
            resultText.setFillColor(sf::Color::White);
            resultText.setPosition(50, 50);
            resultText.setString("You LOSE!!\n");
            while (resultWindow.isOpen()) {
                sf::Event event;
                while (resultWindow.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        resultWindow.close();
                    }
                }
                resultWindow.clear();
                resultWindow.draw(resultText);
                resultWindow.display();
            }
            cout << "YOU LOSE!!" << endl;
        }
    
        // Cleanup
        for (int i = 0; i < MAX_FIREBALLS; i++) {
            delete playerFireballs[i];
            delete enemyFireballs[i];
        }
        for (int i = 0; i < 2; i++) {
            delete aiPet[i];
        }

}

void Guild::startGuildBattle(Player* player,Guild* guild, sf::RenderWindow& window) {
	
    // Load background texture for pet selection
    pets* selectedPet[3] = { nullptr, nullptr, nullptr }; // Pointer for the selected pet
    pets* aiPet[3] = { nullptr, nullptr, nullptr };       // Pointer for the AI pet
    pets* playerPet = nullptr;   // Pointer for the player's pet

    if (guild->isComplete()) {
		 cout<< "Guild Battle" << endl;
         // Set up the battle background
         sf::Texture bgTexture;
         if (!bgTexture.loadFromFile("battle.jpg")) {
             cerr << "Error: Could not load battle background texture!" << endl;
             return;
         }
         sf::Sprite bgSprite(bgTexture);

         // Normalize sprite sizes and positions
         const float targetWidth = window.getSize().x * 0.1f; // 10% of screen width
         const float targetHeight = window.getSize().y * 0.2f; // 20% of screen height
         const float spacing = window.getSize().x * 0.05f;    // 5% of screen width

         // Create AI pets
         pets* aiPet[3] = {
             new Dragon("AI Dragon", "AI01", 1, 80, 25, 15, 400, 400),
             new Griffin("AI Griffin", "AI02", 1, 90, 40, 25, 450, 450),
             new Phenix("AI Phenix", "AI03", 1, 50, 35, 40, 475, 475)
         };

         sf::Texture aiPetTexture[3];
         sf::Sprite aiPetSprite[3];
         aiPetTexture[0].loadFromFile("ai_pet.png");
         aiPetTexture[1].loadFromFile("ai_pet2.png");
         aiPetTexture[2].loadFromFile("ai_pet3.png");

         for (int i = 0; i < 3; i++) {
             aiPetSprite[i].setTexture(aiPetTexture[i]);
             normalizeSpriteSize(aiPetSprite[i], sf::Vector2f(targetWidth, targetHeight));
             aiPetSprite[i].setPosition(window.getSize().x - spacing - targetWidth, window.getSize().y / 2 - targetHeight / 2 + i * targetHeight);
         }

         sf::Texture playerPetTexture[3];
         sf::Sprite playerPetSprite[3];

         if(guild->getAttacker() == "Dragon") {
			 selectedPet[0] = new Dragon("Player's Dragon", "D01", 1, 100, 30, 20, 500, 500);
			 playerPetTexture[0].loadFromFile("dragon.png");
			 playerPetSprite[0].setTexture(playerPetTexture[0]);
			 normalizeSpriteSize(playerPetSprite[0], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[0].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2);
		 }
		 else if (guild->getAttacker() == "Unicorn") {
			 selectedPet[0] = new Unicorn("Player's Unicorn", "U01", 1, 85, 50, 30, 450, 500);
			 playerPetTexture[0].loadFromFile("unicorn.png");
			 playerPetSprite[0].setTexture(playerPetTexture[0]);
			 normalizeSpriteSize(playerPetSprite[0], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[0].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + targetHeight);
		 }
		 else if (guild->getAttacker() == "Griffin") {
			 selectedPet[0] = new Griffin("Player's Griffin", "G01", 1, 90, 40, 25, 450, 450);
			 playerPetTexture[0].loadFromFile("griffin.png");
			 playerPetSprite[0].setTexture(playerPetTexture[0]);
			 normalizeSpriteSize(playerPetSprite[0], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[0].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 2 * targetHeight);
		 }
         else if (guild->getAttacker() == "Phenix") {
			 selectedPet[0] = new Phenix("Player's Phenix", "P01", 1, 50, 35, 40, 475, 475);
			 playerPetTexture[0].loadFromFile("phenix.png");
			 playerPetSprite[0].setTexture(playerPetTexture[0]);
			 normalizeSpriteSize(playerPetSprite[0], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[0].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 3 * targetHeight);
         }
         else {
             cout << "Error: Invalid attacker!" << endl;
             return;
         }
         guild->getTank();
         if (guild->getTank() == "Dragon") {
			 selectedPet[1] = new Dragon("Player's Dragon", "D01", 1, 100, 30, 20, 500, 500);
			 playerPetTexture[1].loadFromFile("dragon.png");
			 playerPetSprite[1].setTexture(playerPetTexture[1]);
			 normalizeSpriteSize(playerPetSprite[1], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[1].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + targetHeight);
         }
         else if (guild->getTank() == "Unicorn") {
			 selectedPet[1] = new Unicorn("Player's Unicorn", "U01", 1, 85, 50, 30, 450, 500);
			 playerPetTexture[1].loadFromFile("unicorn.png");
			 playerPetSprite[1].setTexture(playerPetTexture[1]);
			 normalizeSpriteSize(playerPetSprite[1], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[1].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 2 * targetHeight);
         }
         else if (guild->getTank() == "Griffin") {
			 selectedPet[1] = new Griffin("Player's Griffin", "G01", 1, 90, 40, 25, 450, 450);
			 playerPetTexture[1].loadFromFile("griffin.png");
			 playerPetSprite[1].setTexture(playerPetTexture[1]);
			 normalizeSpriteSize(playerPetSprite[1], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[1].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 3 * targetHeight);
         }
         else if (guild->getTank() == "Phenix") {
			 selectedPet[1] = new Phenix("Player's Phenix", "P01", 1, 50, 35, 40, 475, 475);
			 playerPetTexture[1].loadFromFile("phenix.png");
			 playerPetSprite[1].setTexture(playerPetTexture[1]);
			 normalizeSpriteSize(playerPetSprite[1], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[1].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 4 * targetHeight);
         }
         else {
             cout << "Error: Invalid tank!" << endl;
             return;
         }
         guild->getHealer();
         if (guild->getHealer() == "Dragon") {
			 selectedPet[2] = new Dragon("Player's Dragon", "D01", 1, 100, 30, 20, 500, 500);
			 playerPetTexture[2].loadFromFile("dragon.png");
			 playerPetSprite[2].setTexture(playerPetTexture[2]);
			 normalizeSpriteSize(playerPetSprite[2], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[2].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 3 * targetHeight);
		 }
		 else if (guild->getHealer() == "Unicorn") {
			 selectedPet[2] = new Unicorn("Player's Unicorn", "U01", 1, 85, 50, 30, 450, 500);
			 playerPetTexture[2].loadFromFile("unicorn.png");
			 playerPetSprite[2].setTexture(playerPetTexture[2]);
			 normalizeSpriteSize(playerPetSprite[2], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[2].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 4 * targetHeight);
		 }
		 else if (guild->getHealer() == "Griffin") {
			 selectedPet[2] = new Griffin("Player's Griffin", "G01", 1, 90, 40, 25, 450, 450);
			 playerPetTexture[2].loadFromFile("griffin.png");
			 playerPetSprite[2].setTexture(playerPetTexture[2]);
			 normalizeSpriteSize(playerPetSprite[2], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[2].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 5 * targetHeight);
         }
         else if (guild->getHealer() == "Phenix") {
			 selectedPet[2] = new Phenix("Player's Phenix", "P01", 1, 50, 35, 40, 475, 475);
			 playerPetTexture[2].loadFromFile("phenix.png");
			 playerPetSprite[2].setTexture(playerPetTexture[2]);
			 normalizeSpriteSize(playerPetSprite[2], sf::Vector2f(targetWidth, targetHeight));
			 playerPetSprite[2].setPosition(spacing, window.getSize().y / 2 - targetHeight / 2 + 6 * targetHeight);
         }
         else {
             cout << "Error: Invalid healer!" << endl;
             return;
         }

         Fireball* playerFireballs[MAX_FIREBALLS] = { nullptr };
                 Fireball* enemyFireballs[MAX_FIREBALLS] = { nullptr };
                 float fireballSpeed = window.getSize().x * 0.015f; // Fireball speed
                 float playerSpeed = window.getSize().x * 0.008f;   // Player movement speed
         
                 // Health bars
         
                 const float HP_BAR_LENGTH = window.getSize().x * 0.15f; // 15% of screen width
                 sf::RectangleShape playerHPBars[3];
                 sf::RectangleShape aiHPBars[3];
         
                 for (int i = 0; i < 3; i++) {
                     playerHPBars[i] = sf::RectangleShape(sf::Vector2f(HP_BAR_LENGTH, 20));
                     aiHPBars[i] = sf::RectangleShape(sf::Vector2f(HP_BAR_LENGTH, 20));
         
                     playerHPBars[i].setFillColor(i == 0 ? sf::Color::Green : i==1? sf::Color::Blue : sf::Color::Cyan);
                     aiHPBars[i].setFillColor(i == 0 ? sf::Color::Red : i==2? sf::Color::Yellow : sf::Color::Magenta);
         
                     playerHPBars[i].setPosition(50, 50 + i * 50);
                     aiHPBars[i].setPosition(window.getSize().x - HP_BAR_LENGTH - 50, 50 + i * 50);
                 }
         
                 // Timer
                 int battleTimer = 60; // 1-minute duration
                 sf::Clock battleClock;
                 sf::Text timerText("60", sf::Font(), 24);
                 timerText.setFillColor(sf::Color::White);
                 timerText.setPosition(350, 10);
         
                 // Fireball textures
                 sf::Texture playerFireballTexture, enemyFireballTexture;
                 if (!playerFireballTexture.loadFromFile("player_fireball.png") ||
                     !enemyFireballTexture.loadFromFile("enemy_fireball.png")) {
                     std::cerr << "Error: Could not load fireball textures!" << std::endl;
                     return;
                 }
                 // Fireball size adjustment
                 const sf::Vector2f fireballTargetSize(window.getSize().x * 0.02f, window.getSize().x * 0.02f);
         
                 sf::Clock enemyFireCooldown;
                 bool canEnemyFire = true;
                 sf::Clock playerFireCooldown;
                 bool canPlayerFire = true;
         
                 // Game loop
                 while (window.isOpen() && battleTimer > 0 &&

                     (aiPet[0]->hp > 0 || aiPet[1]->hp > 0) || aiPet[2]->hp >0) {
                     sf::Event event;
                     while (window.pollEvent(event)) {
                         if (event.type == sf::Event::Closed) {
                             window.close();
                             return;
                         }
         
                         // Player movement
                         for (int i = 0; i < 3; i++) {
                             if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                                 playerPetSprite[i].move(0, -playerSpeed);
                             }
                             if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                                 playerPetSprite[i].move(0, playerSpeed);
                             }
                             if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                                 playerPetSprite[i].move(-playerSpeed, 0);
                             }
                             if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                                 playerPetSprite[i].move(playerSpeed, 0);
                             }
                         }
         
                         // Player fires fireball
                         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::F) || sf::Keyboard::isKeyPressed(sf::Keyboard::G) 
                             || sf::Keyboard::isKeyPressed(sf::Keyboard::H) && canPlayerFire) {
                             for (int i = 0; i < MAX_FIREBALLS; i++) {
                                 if (playerFireballs[i] == nullptr) {
                                     for (int j = 0; j < 3; j++) { // One fireball for each pet
                                         playerFireballs[i] = new Fireball(
                                             playerFireballTexture,
                                             playerPetSprite[j].getPosition().x + playerPetSprite[j].getGlobalBounds().width,
                                             playerPetSprite[j].getPosition().y + playerPetSprite[j].getGlobalBounds().height / 2,
                                             fireballSpeed, selectedPet[j]->powerattack
                                         );
                                         playerFireballs[i]->getSprite().setScale(
                                             fireballTargetSize.x / playerFireballTexture.getSize().x,
                                             fireballTargetSize.y / playerFireballTexture.getSize().y
                                         );
                                     }
                                     canPlayerFire = false;
                                     playerFireCooldown.restart();
                                     if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                                         player->SpecialAbility(aiPet[0]);
                                         cout << "Special Ability Used!" << endl;
                                     }
									 if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
										 player->SpecialAbility(aiPet[1]);
										 cout << "Special Ability Used!" << endl;
									 }
									 if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
										 player->SpecialAbility(aiPet[2]);
										 cout << "Special Ability Used!" << endl;
									 }
                                     break;
                                 }
                             }
                         }
                     }
         
                     // Cooldown for player fireball
                     if (!canPlayerFire && playerFireCooldown.getElapsedTime().asSeconds() > 0.5f) {
                         canPlayerFire = true;
                     }
         
                     // Enemy fires fireball randomly
                     if (canEnemyFire && rand() % 100 < 10) { // 10% chance to fire each frame
                         for (int i = 0; i < MAX_FIREBALLS; i++) {
                             if (enemyFireballs[i] == nullptr) {
                                 for (int j = 0; j < 3; j++) { // One fireball for each pet
                                     enemyFireballs[i] = new Fireball(
                                         enemyFireballTexture,
                                         aiPetSprite[j].getPosition().x,
                                         aiPetSprite[j].getPosition().y + aiPetSprite[j].getGlobalBounds().height / 2,
                                         -fireballSpeed, aiPet[j]->powerattack
                                     );
                                     enemyFireballs[i]->getSprite().setScale(
                                         fireballTargetSize.x / enemyFireballTexture.getSize().x,
                                         fireballTargetSize.y / enemyFireballTexture.getSize().y
                                     );
                                 }
                                 canEnemyFire = false;
                                 enemyFireCooldown.restart();
                                 break;
                             }
                         }
                     }
         
                     // Cooldown for enemy fireball
                     if (!canEnemyFire && enemyFireCooldown.getElapsedTime().asSeconds() > 1.0f) {
                         canEnemyFire = true;
                     }
         
                     // Update fireballs
                     for (int i = 0; i < MAX_FIREBALLS; i++) {
                         // Player fireballs
                         if (playerFireballs[i]) {
                             playerFireballs[i]->move();
                             bool hit = false;
                             for (int j = 0; j < 3; j++) {
                                 if (playerFireballs[i]->getSprite().getGlobalBounds().intersects(aiPetSprite[j].getGlobalBounds()) && aiPet[j]->hp > 0) {
                                     aiPet[j]->takedamage(playerFireballs[i]->getDamage());
                                     hit = true;
                                     break;
                                 }
                             }
                             if (hit || playerFireballs[i]->isOffScreen(window.getSize().x)) {
                                 delete playerFireballs[i];
                                 playerFireballs[i] = nullptr;
                             }
                         }
         
                         // Enemy fireballs
                         if (enemyFireballs[i]) {
                             enemyFireballs[i]->move();
                             bool hit = false;
                             for (int j = 0; j < 3; j++) {
                                 if (enemyFireballs[i]->getSprite().getGlobalBounds().intersects(playerPetSprite[j].getGlobalBounds()) && selectedPet[j]->hp > 0) {
                                     selectedPet[j]->takedamage(enemyFireballs[i]->getDamage());
                                     hit = true;
                                     break;
                                 }
                             }
                             if (hit || enemyFireballs[i]->isOffScreen(window.getSize().x)) {
                                 delete enemyFireballs[i];
                                 enemyFireballs[i] = nullptr;
                             }
                         }
                     }
         
                     // Update HP bars
                     for (int i = 0; i < 3; i++) {
                         playerHPBars[i].setSize(sf::Vector2f(200 * (float(selectedPet[i]->hp) / selectedPet[i]->maxhp), 20));
                         aiHPBars[i].setSize(sf::Vector2f(200 * (float(aiPet[i]->hp) / aiPet[i]->maxhp), 20));
                     }
         
         
                     // Rendering Logic
                     if (window.isOpen()) {
                         try {
                             window.clear();
         
                             // Debug: Ensure textures are valid before rendering
                             if (!bgSprite.getTexture()) {
                                 std::cerr << "Error: bgSprite texture is null!" << std::endl;
                                 return;
                             }
                             window.draw(bgSprite);
         
                             // Draw player and AI sprites
                             if (playerPetSprite[0].getTexture() && playerPetSprite[1].getTexture()) {
                                 window.draw(playerPetSprite[0]);
                                 window.draw(playerPetSprite[1]);
								 window.draw(playerPetSprite[2]);
                             }
                             else {
                                 std::cerr << "Error: playerPetSprite1 texture is null!" << std::endl;
                             }
         
                             if (aiPetSprite[0].getTexture() && aiPetSprite[1].getTexture() && aiPetSprite[2].getTexture()){
                                 window.draw(aiPetSprite[0]);
                                 window.draw(aiPetSprite[1]);
         					    window.draw(aiPetSprite[2]);
                             }
                             else {
                                 std::cerr << "Error: aiPetSprite texture is null!" << std::endl;
                             }
         
                             // Draw HP bars
                             window.draw(playerHPBars[0]);
                             window.draw(playerHPBars[1]);
         				     window.draw(playerHPBars[2]);
                             window.draw(aiHPBars[0]);
                             window.draw(aiHPBars[1]);
         				     window.draw(aiHPBars[2]);
         
                             // Draw fireballs
                             for (int i = 0; i < MAX_FIREBALLS; i++) {
                                 if (playerFireballs[i]) window.draw(playerFireballs[i]->getSprite());
                                 if (enemyFireballs[i]) window.draw(enemyFireballs[i]->getSprite());
                             }
         
                             //battleWindow.draw(timerText);
                         }
                         catch (const std::exception& e) {
                             std::cerr << "Exception occurred during rendering: " << e.what() << std::endl;
                             return;
                         }
                         catch (...) {
                             std::cerr << "Unknown exception occurred during rendering." << std::endl;
                             return;
                         }
                     }
                     else {
                         std::cerr << "Error: Window is not open. Exiting rendering loop." << std::endl;
                     }
         
                     window.display();
                 }
         
                 if (selectedPet[0]->hp > 0 || selectedPet[1]->hp || selectedPet[2]->hp > 0) {
                     sf::RenderWindow resultWindow(sf::VideoMode(400, 200), "Battle Result");
                     sf::Font resultFont;
                     if (!resultFont.loadFromFile("Revenge of Gods.ttf")) {
                         cerr << "Error loading font!" << endl;
                     }
                     sf::Text resultText;
                     resultText.setFont(resultFont);
                     resultText.setCharacterSize(24);
                     resultText.setFillColor(sf::Color::White);
                     resultText.setPosition(50, 50);
                     resultText.setString("You WON!!\n");
                     selectedPet[0]->levelup();
                     selectedPet[1]->levelup();
         		    selectedPet[2]->levelup();
                     cout << "Level up" << endl;
                     player->addMoney(100); // Add money to player after winning
                     while (resultWindow.isOpen()) {
                         sf::Event event;
                         while (resultWindow.pollEvent(event)) {
                             if (event.type == sf::Event::Closed) {
                                 resultWindow.close();
                             }
                         }
                         resultWindow.clear();
                         resultWindow.draw(resultText);
                         resultWindow.display();
                     }
                     // Display victory message
                     cout << "Player WON!!" << endl;
                 }
                 else {
                     sf::RenderWindow resultWindow(sf::VideoMode(400, 200), "Battle Result");
                     sf::Font resultFont;
                     if (!resultFont.loadFromFile("Revenge of Gods.ttf")) {
                         cerr << "Error loading font!" << endl;
                     }
                     sf::Text resultText;
                     resultText.setFont(resultFont);
                     resultText.setCharacterSize(24);
                     resultText.setFillColor(sf::Color::White);
                     resultText.setPosition(50, 50);
                     resultText.setString("You LOSE!!\n");
                     while (resultWindow.isOpen()) {
                         sf::Event event;
                         while (resultWindow.pollEvent(event)) {
                             if (event.type == sf::Event::Closed) {
                                 resultWindow.close();
                             }
                         }
                         resultWindow.clear();
                         resultWindow.draw(resultText);
                         resultWindow.display();
                     }
                     cout << "YOU LOSE!!" << endl;
                 }
         
                 // Cleanup
                 for (int i = 0; i < MAX_FIREBALLS; i++) {
                     delete playerFireballs[i];
                     delete enemyFireballs[i];
                 }
                 for (int i = 0; i < 3; i++) {
                     delete aiPet[i];
                 }
	}
    else {
        cout << "CAN'T START BATTLE GUILD NOT MADE" << endl;
        return;
    }
}  

class GameData {
public:
    static bool loadPlayer(Player& player);
    static void savePlayer(const Player& player);
};

bool GameData::loadPlayer(Player& player) {
    ifstream in("save_data.txt");
    if (!in.is_open()) return false;

    string name, petType;
    int currency, level, power, speed, def, hp, maxhp;

    getline(in, name);
    in >> currency >> petType >> level >> power >> speed >> def >> hp >> maxhp;

    pets* pet = nullptr;
    if (petType == "Dragon") pet = new Dragon("Dragon", "D01", level, power, speed, def, hp, maxhp);
    else if (petType == "Griffin") pet = new Griffin("Griffin", "G01", level, power, speed, def, hp, maxhp);
    else if (petType == "Unicorn") pet = new Unicorn("Unicorn", "U01", level, power, speed, def, hp, maxhp);
    else if (petType == "Phenix") pet = new Phenix("Phenix", "P01", level, power, speed, def, hp, maxhp);

    if (!pet) return false;

    player.choosepet(pet);
    player.setName(name);
    player.setCurrency(currency);

    cout << "Game Loaded Successfully.\n";
    return true;
}

void GameData::savePlayer(const Player& player) {
    ofstream out("save_data.txt");
    if (!out.is_open()) return;

    out << player.getname() << endl;
    out << player.getCurrency() << " ";

    pets* pet = player.getPet();
    out << pet->getName() << " " << pet->level << " " << pet->powerattack << " "
        << pet->speed << " " << pet->defence << " " << pet->hp << " " << pet->maxhp << endl;
    out.close();

    cout << "Game Saved Successfully.\n";
}

int main() {

    Player* player = new Player(10);
    char choice;

    cout << "\n\tWelcome to Magical Kingdom" << endl;
    top:
    cout << "Do you want to load a saved game? (y,n): ";
    cin >> choice;
    if (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
        cout << "Enter Valid Input" << endl;
        goto top;
    }
    if (choice == 'y' || choice == 'Y') {
        if (!GameData::loadPlayer(*player)) {
            cout << "Starting a new game...\n";
            player->createNewPlayer();
            GameData::savePlayer(*player);
        }
    }
    else {
        player->createNewPlayer();
        GameData::savePlayer(*player);
    }
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML Multi-Page Game");

    // Load the first background texture
    sf::Texture backgroundTexture1;
    if (!backgroundTexture1.loadFromFile("background.jpg")) {
        std::cerr << "Error: Could not load first background image!" << endl;
        return -1;
    }

    // Load the second background texture
    sf::Texture backgroundTexture2;
    if (!backgroundTexture2.loadFromFile("background2.jpg")) {
        cerr << "Error: Could not load second background image!" << endl;
        return -1;
    }

    // Load the third background texture
    sf::Texture backgroundTexture3;
    if (!backgroundTexture3.loadFromFile("background3.jpg")) {
        cerr << "Error: Could not load third background image!" << endl;
        return -1;
    }

    sf::Texture backgroundTexture4;
    if (!backgroundTexture4.loadFromFile("battle_background.jpg")) {
        cerr << "Error: Could not load fourth background image!" << endl;
        return -1;
    }

    sf::Texture trainBackgroundTexture;
    if (!trainBackgroundTexture.loadFromFile("train_background.jpg")) {
        cerr << "Error: Could not training background image!" << endl;
        return -1;
    }

    sf::Texture shopbackgroundTexture;
    if (!shopbackgroundTexture.loadFromFile("shop_background.jpg")) {
        cerr << "Error: Could not load shop background!" << endl;
        return -1;
    }

    sf::Texture guildBackgroundTexture;
    if (!guildBackgroundTexture.loadFromFile("guild background.jpg")){
        cerr<< "Error: Could not training background image!"  << endl;
        return -1;
    } 

    // Create background sprite
    sf::Sprite backgroundSprite1, backgroundSprite2, backgroundSprite3, backgroundSprite4, trainBackgroundSprite, shopbackgroundSprite, guildBackgroundSprite;
    backgroundSprite1.setTexture(backgroundTexture1);
    backgroundSprite1.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture1.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture1.getSize().y
    );
    backgroundSprite2.setTexture(backgroundTexture2);
    backgroundSprite2.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture2.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture2.getSize().y
    );
    backgroundSprite3.setTexture(backgroundTexture3);
    backgroundSprite3.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture3.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture3.getSize().y
    );
    backgroundSprite4.setTexture(backgroundTexture4);
    backgroundSprite4.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture4.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture4.getSize().y
    );
    trainBackgroundSprite.setTexture(trainBackgroundTexture);
    trainBackgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / trainBackgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / trainBackgroundTexture.getSize().y
    );
    shopbackgroundSprite.setTexture(shopbackgroundTexture);
    shopbackgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / shopbackgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / shopbackgroundTexture.getSize().y
        );
    guildBackgroundSprite.setTexture(guildBackgroundTexture);
    guildBackgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / guildBackgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / guildBackgroundTexture.getSize().y
    );

    // Create a button rectangle for the first page
    sf::RectangleShape button(sf::Vector2f(200, 80)); // Width = 200, Height = 80
    button.setFillColor(sf::Color(200, 200, 200, 255)); // Light gray color
    button.setOutlineThickness(3);
    button.setOutlineColor(sf::Color::Black);
    button.setPosition(540, 320); // Centered in the window

    // Load font for the button text
    sf::Font font;
    if (!font.loadFromFile("Blood Spots.otf")) {
        cerr << "Error: Could not load font!" << endl;
        return -1;
    }
   
    // Player currency
    int playerMoney = player->getCurrency();
    sf::Text currencyText;
    currencyText.setFont(font);
    currencyText.setCharacterSize(24);
    currencyText.setFillColor(sf::Color::White);
    currencyText.setPosition(1050, 20);
    currencyText.setString("Money: " + to_string(playerMoney));

    sf::Text shopHeading;
    shopHeading.setFont(font);
    shopHeading.setCharacterSize(36);
    shopHeading.setFillColor(sf::Color::White);
    shopHeading.setString("Shop");
    shopHeading.setPosition(600, 20);

    // Initialize shop with DMA and player
    const int maxItems = 4;
    Shop shop (maxItems);
    shop.addItem(new Healingportion(150, 10, 50));
    shop.addItem(new Manastore(150, 10, 50,30));
    shop.addItem(new Buff(250, 5, 20, 10));
    shop.addItem(new Shield(200, 8, 30, 15));

    sf::Texture potionTextures[maxItems];
    if (!potionTextures[0].loadFromFile("heal_potion.png") ||
        !potionTextures[1].loadFromFile("mana_potion.png") ||
        !potionTextures[2].loadFromFile("buff_potion.png") ||
        !potionTextures[3].loadFromFile("shield_potion.png")) {
        cerr << "Error: Could not load potion images!" << endl;
        return -1;
    }

    // Create the button text
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("Click Me");
    buttonText.setCharacterSize(30); // Font size
    buttonText.setFillColor(sf::Color::Black);
    // Center text on the button
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    buttonText.setPosition(button.getPosition().x + button.getSize().x / 2, button.getPosition().y + button.getSize().y / 2);

    // Textbox for entering name
    sf::RectangleShape inputBox(sf::Vector2f(400, 50));
    inputBox.setFillColor(sf::Color(255, 255, 255, 255)); // White
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setPosition(440, 300); // Centered

    // Create the message above the text box
    sf::Text promptText;
    promptText.setFont(font);
    promptText.setString("Please enter your name:");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    sf::FloatRect promptBounds = promptText.getLocalBounds();
    promptText.setOrigin(promptBounds.left + promptBounds.width / 2, promptBounds.top + promptBounds.height / 2);
    promptText.setPosition(inputBox.getPosition().x + inputBox.getSize().x / 2, inputBox.getPosition().y - 40);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 10);

    std::string userInput = "";
    bool inSecondPage = false;
    bool inThirdPage = false;
    bool inBattlePage = false;
    bool inTrainPage = false;
    bool inShopPage = false;
    bool inGuildPage = false;

    // Third page buttons
    const int menuButtonCount = 6;
    sf::RectangleShape menuButtons[menuButtonCount];
    sf::Text menuTexts[menuButtonCount];
    const std::string buttonLabels[menuButtonCount] = { "To Battle", "Train", "Shop", "Guild", "Exit", "Load Game"};
    for (int i = 0; i < menuButtonCount; ++i) {
        menuButtons[i].setSize(sf::Vector2f(200, 80));
        menuButtons[i].setFillColor(sf::Color(200, 200, 200, 255));
        menuButtons[i].setOutlineThickness(3);
        menuButtons[i].setOutlineColor(sf::Color::Black);
        menuButtons[i].setPosition(540, 150 + i * 100);

        menuTexts[i].setFont(font);
        menuTexts[i].setString(buttonLabels[i]);
        menuTexts[i].setCharacterSize(24);
        menuTexts[i].setFillColor(sf::Color::Black);
        sf::FloatRect menuTextBounds = menuTexts[i].getLocalBounds();
        menuTexts[i].setOrigin(menuTextBounds.left + menuTextBounds.width / 2, menuTextBounds.top + menuTextBounds.height / 2);
        menuTexts[i].setPosition(menuButtons[i].getPosition().x + menuButtons[i].getSize().x / 2, menuButtons[i].getPosition().y + menuButtons[i].getSize().y / 2);
    }

    // Battle options buttons
    Battle battle;
    const int battleButtonCount = 4; // Including "Back to Home" button
    sf::RectangleShape battleButtons[battleButtonCount];
    sf::Text battleTexts[battleButtonCount];
    const std::string battleLabels[battleButtonCount] = { "One vs One", "Two vs Two", "Guild vs Guild", "Back to Home" };

    for (int i = 0; i < battleButtonCount; ++i) {
        battleButtons[i].setSize(sf::Vector2f(250, 80));
        battleButtons[i].setFillColor(sf::Color(200, 200, 200, 255));
        battleButtons[i].setOutlineThickness(3);
        battleButtons[i].setOutlineColor(sf::Color::Black);

        if (i == battleButtonCount - 1) {
            battleButtons[i].setPosition(515, 200 + i * 100 + 50); // Position for "Back to Home"
        }
        else {
            battleButtons[i].setPosition(515, 200 + i * 100); // Normal buttons
        }

        battleTexts[i].setFont(font);
        battleTexts[i].setString(battleLabels[i]);
        battleTexts[i].setCharacterSize(24);
        battleTexts[i].setFillColor(sf::Color::Black);
        sf::FloatRect battleTextBounds = battleTexts[i].getLocalBounds();
        battleTexts[i].setOrigin(battleTextBounds.left + battleTextBounds.width / 2, battleTextBounds.top + battleTextBounds.height / 2);
        battleTexts[i].setPosition(battleButtons[i].getPosition().x + battleButtons[i].getSize().x / 2, battleButtons[i].getPosition().y + battleButtons[i].getSize().y / 2);
    }
    // Train page pets
    sf::Texture dragonTexture, unicornTexture, griffinTexture, phenixTexture;
    if (!dragonTexture.loadFromFile("dragon.png") ||
        !unicornTexture.loadFromFile("unicorn.png") ||
        !griffinTexture.loadFromFile("griffin.png") ||
        !phenixTexture.loadFromFile("phoenix.png")) {
        cerr << "Error: Could not load pet images!" << endl;
        return -1;
    }

    PetVisual pets[4];
    pets[0] = { sf::Sprite(dragonTexture), "Dragon" };
    pets[1] = { sf::Sprite(unicornTexture), "Unicorn" };
    pets[2] = { sf::Sprite(griffinTexture), "Griffin" };
    pets[3] = { sf::Sprite(phenixTexture), "Phenix" };

    const float pettargetWidth = 150.0f;  // Desired width for all pet sprites
    const float pettargetHeight = 150.0f; // Desired height for all pet sprites

    // Position pets on the training page
    const float windowWidth = 1580.f; 
    const float spacing = 50.0f;    

    float currentX = spacing;
    for (int i = 0; i < 4; i++) {

        sf::Vector2u originalSize = pets[i].sprite.getTexture()->getSize();

        // Calculate the scale factors for width and height
        float petscaleX = pettargetWidth / originalSize.x;
        float petscaleY = pettargetHeight / originalSize.y;

        // Apply scaling to normalize the size of the sprites
        pets[i].sprite.setScale(petscaleX, petscaleY);

        // Set the position of each pet sprite
        pets[i].sprite.setPosition(currentX,
            (720.f - pettargetHeight) / 2.f); 

        // Adjust the X position for the next pet, including spacing
        currentX += pettargetWidth + spacing;
    }

    // Create "Back to Home" button for training page
    sf::RectangleShape trainBackButton(sf::Vector2f(250, 80));
    trainBackButton.setFillColor(sf::Color(200, 200, 200, 255));
    trainBackButton.setOutlineThickness(3);
    trainBackButton.setOutlineColor(sf::Color::Black);
    trainBackButton.setPosition(515, 600);

    sf::Text trainBackText;
    trainBackText.setFont(font);
    trainBackText.setString("Back to Home");
    trainBackText.setCharacterSize(24);
    trainBackText.setFillColor(sf::Color::Black);
    sf::FloatRect trainBackTextBounds = trainBackText.getLocalBounds();
    trainBackText.setOrigin(trainBackTextBounds.left + trainBackTextBounds.width / 2, trainBackTextBounds.top + trainBackTextBounds.height / 2);
    trainBackText.setPosition(trainBackButton.getPosition().x + trainBackButton.getSize().x / 2, trainBackButton.getPosition().y + trainBackButton.getSize().y / 2);

    // Create item UI elements using DMA
    sf::Sprite potionSprites[maxItems];
    sf::RectangleShape buyButtons[maxItems];
    sf::Text buyButtonTexts[maxItems];
    sf::Text itemNames[maxItems];
    sf::Text itemPrices[maxItems];

    // Define the target width and height for all potion sprites
    float xOffset = 150.0f; // Starting X position for the first item
    float yOffset = 150.0f; // Starting Y position for the items
    float spacingX = 300.0f; // Horizontal spacing between items
    float spacingY = 50.0f;  // Vertical spacing between item elements

    const float targetWidth = 100.0f;  // Desired width for all potion images
    const float targetHeight = 100.0f; // Desired height for all potion images

    for (int i = 0; i < shop.getItemCount(); ++i) {
        // Get the original size of the texture
        sf::Vector2u originalSize = potionTextures[i].getSize();

        // Calculate the scale factors for width and height
        float scaleX = targetWidth / originalSize.x;
        float scaleY = targetHeight / originalSize.y;

        // Apply the scale to the sprite
        potionSprites[i].setTexture(potionTextures[i]);
        potionSprites[i].setScale(scaleX, scaleY);

        // Align the sprite and other elements (e.g., name, price, buy button)
        potionSprites[i].setPosition(xOffset, yOffset);

        itemNames[i].setFont(font);
        itemNames[i].setCharacterSize(20);
        itemNames[i].setFillColor(sf::Color::White);
        itemNames[i].setString(shop.getItem(i)->getName());
        itemNames[i].setPosition(
            xOffset + (targetWidth - itemNames[i].getLocalBounds().width) / 2,
            yOffset + targetHeight + 10
        );

        itemPrices[i].setFont(font);
        itemPrices[i].setCharacterSize(18);
        itemPrices[i].setFillColor(sf::Color::White);
        itemPrices[i].setString("Price: " + to_string(static_cast<int>(shop.getItem(i)->getPrice())));
        itemPrices[i].setPosition(
            xOffset + (targetWidth - itemPrices[i].getLocalBounds().width) / 2,
            yOffset + targetHeight + 40
        );

        buyButtons[i].setSize(sf::Vector2f(100, 40));
        buyButtons[i].setFillColor(sf::Color(200, 200, 200));
        buyButtons[i].setPosition(
            xOffset + (targetWidth - buyButtons[i].getSize().x) / 2,
            yOffset + targetHeight + 80
        );

        buyButtonTexts[i].setFont(font);
        buyButtonTexts[i].setCharacterSize(18);
        buyButtonTexts[i].setFillColor(sf::Color::Black);
        buyButtonTexts[i].setString("Buy");
        sf::FloatRect textBounds = buyButtonTexts[i].getLocalBounds();
        buyButtonTexts[i].setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
        buyButtonTexts[i].setPosition(
            buyButtons[i].getPosition().x + buyButtons[i].getSize().x / 2,
            buyButtons[i].getPosition().y + buyButtons[i].getSize().y / 2
        );

        // Move to the next position
        xOffset += spacingX;
        if ((i + 1) % 3 == 0) { // Move to the next row after 3 items
            xOffset = 150.0f;
            yOffset += targetHeight + spacingY + 100;
        }
    }

    // Create "Back to Home" button for shop page
    sf::RectangleShape shopBackButton(sf::Vector2f(250, 80));
    shopBackButton.setFillColor(sf::Color(200, 200, 200, 255));
    shopBackButton.setOutlineThickness(3);
    shopBackButton.setOutlineColor(sf::Color::Black);
    shopBackButton.setPosition(515, 600);

    sf::Text shopBackText;
    shopBackText.setFont(font);
    shopBackText.setString("Back to Home");
    shopBackText.setCharacterSize(24);
    shopBackText.setFillColor(sf::Color::Black);
    sf::FloatRect shopBackTextBounds = shopBackText.getLocalBounds();
    shopBackText.setOrigin(shopBackTextBounds.left + shopBackTextBounds.width / 2,
        shopBackTextBounds.top + shopBackTextBounds.height / 2);
    shopBackText.setPosition(shopBackButton.getPosition().x + shopBackButton.getSize().x / 2,
        shopBackButton.getPosition().y + shopBackButton.getSize().y / 2);

    // Guild page
    Guild guild;
    bool isSelectingTank = false;
    bool isSelectingHealer = false;
    bool guildComplete = false;
    sf::RectangleShape guildBackButton(sf::Vector2f(250, 80));
    guildBackButton.setFillColor(sf::Color(200, 200, 200, 255));
    guildBackButton.setOutlineThickness(3);
    guildBackButton.setOutlineColor(sf::Color::Black);
    guildBackButton.setPosition(515, 600);

    sf::Text guildBackText;
    guildBackText.setFont(font);
    guildBackText.setString("Back to Home");
    guildBackText.setCharacterSize(24);
    guildBackText.setFillColor(sf::Color::Black);
    sf::FloatRect guildBackTextBounds = guildBackText.getLocalBounds();
    guildBackText.setOrigin(guildBackTextBounds.left + guildBackTextBounds.width / 2, guildBackTextBounds.top + guildBackTextBounds.height / 2);
    guildBackText.setPosition(guildBackButton.getPosition().x + guildBackButton.getSize().x / 2, guildBackButton.getPosition().y + guildBackButton.getSize().y / 2);

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!inSecondPage && !inThirdPage && !inBattlePage && !inTrainPage && !inShopPage && !inGuildPage) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (button.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        inSecondPage = true;
                    }
                }
            }
            else if (inSecondPage && !inThirdPage && !inBattlePage && !inTrainPage && !inShopPage &&!inGuildPage) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b' && !userInput.empty()) {
                        userInput.pop_back();
                    }
                    else if (event.text.unicode > 31 && event.text.unicode < 128) {
                        userInput += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(userInput);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && !userInput.empty()) {
                    inSecondPage = false;
                    inThirdPage = true;
                }
            }
            else if (inThirdPage && !inBattlePage && !inTrainPage && !inShopPage && !inGuildPage) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (int i = 0; i < menuButtonCount; ++i) {
                        if (menuButtons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            if (buttonLabels[i] == "To Battle") {
                                inThirdPage = false;
                                inBattlePage = true;
                            }
                            else if (buttonLabels[i] == "Train") {
                                inThirdPage = false;
                                inTrainPage = true;
                            }
                            else if (buttonLabels[i] == "Shop") {
                                inThirdPage = false;
                                inShopPage = true;
                            }
                            else if (buttonLabels[i] == "Guild") {
                                inThirdPage = false;
                                inGuildPage = true;
                                cout << "Navigated to Guild Page: inGuildPage = " << inGuildPage << endl;
                            }
                            else if (buttonLabels[i] == "Exit") {
                                window.close();
                            }
                            else {
								cout << "Load Game" << endl;
                                GameData::loadPlayer(*player);
                                cout << "Player txt file loaded" << endl;
                            }
                        }
                    }
                }
            }
            else if (inBattlePage) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (int i = 0; i < battleButtonCount; ++i) {
                        if (battleButtons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            if (battleLabels[i] == "Back to Home") {
                                inBattlePage = false;
                                inThirdPage = true;
                            }
                            else if (battleLabels[i] == "One vs One") {
                                battle.start1v1Battle(player, window);
                            }
                            else if (battleLabels[i] == "Two vs Two") {
                                battle.start2v2Battle(player, window);
                            }
                            else if (battleLabels[i] == "Guild vs Guild") {
                                guild.startGuildBattle(player,&guild, window);
                            }
                        }
                    }
                }
            }
            else if (inTrainPage) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Check if "Back to Home" button was clicked
                    if (trainBackButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        inTrainPage = false;
                        inThirdPage = true;
                    }

                    for (int i = 0; i < 4; ++i) {
                        if (pets[i].sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            cout << "You selected: " << pets[i].name << " for training!" << endl;

                            // Create a dynamically allocated TrainingCamp object
                            TrainingCamp* train = new TrainingCamp(
                                window,                           // Pass the SFML window reference
                                pets[i].sprite.getTexture()->copyToImage().saveToFile("temp_pet_image.png") ? "temp_pet_image.png" : "default_pet.png", // Pet image
                                "obstacle.png",                   // Obstacle image
                                "powerup.png",                    // Power-up image
                                "training_ground.jpeg"             // Training ground background
                            );

                            // Start training
                            train->startTraining();

                            // Clean up the TrainingCamp object after training
                            delete train;

                            // Exit the training page and return to the main menu
                            inTrainPage = false;
                            inThirdPage = true;
                            break;
                        }
                    }

                }
            }
            else if (inShopPage) {
             
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Check if the "Back to Home" button was clicked
                    if (shopBackButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        inShopPage = false;
                        inThirdPage = true;
                    }

                    // Check if any "Buy" button was clicked
                    for (int i = 0; i < shop.getItemCount(); ++i) {
                        if (buyButtons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            item* currentItem = shop.getItem(i);

                            // Check if the player can afford the item and if it's in stock
                            if (playerMoney >= currentItem->getPrice() && currentItem->getQuantity() > 0) {
                                // Deduct the price from the player's money
                                playerMoney -= currentItem->getPrice();

                                // Reduce the item's quantity
                                currentItem->reduceQuantity();

                                // Update the SFML UI elements
                                currencyText.setString("Money: " + to_string(playerMoney));
                                itemPrices[i].setString("Price: " + to_string(static_cast<int>(currentItem->getPrice())) +
                                    " | Stock: " + to_string(currentItem->getQuantity()));

                                // Log the updated values to the console
                                cout << "Bought " << currentItem->getName() << endl;
                                cout << "Remaining Money: " << playerMoney << endl;
                                cout << currentItem->getName() << " Stock: " << currentItem->getQuantity() << endl;
                            }
                            else {
                                // Log insufficient funds or out-of-stock message to the console
                                if (playerMoney < currentItem->getPrice()) {
                                    cout << "Not enough money to buy " << currentItem->getName() << "!" << endl;
                                }
                                if (currentItem->getQuantity() <= 0) {
                                    cout << currentItem->getName() << " is out of stock!" << endl;
                                }
                            }
                        }
                    }
                }
                
            }
            else if (inGuildPage) {
                 
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Check if "Back to Home" button was clicked
                    if (guildBackButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        inGuildPage = false;
                        inThirdPage = true;
                    }

                    // Handle pet selection for each role
                    for (int i = 0; i < 4; ++i) {
                        if (pets[i].sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            if (!isSelectingTank && !isSelectingHealer) {
                                // Attacker selection
                                guild.setAttacker(pets[i].name);
                                std::cout << "Selected Attacker: " << pets[i].name << std::endl;
                                isSelectingTank = true; // Move to tank selection
                            }
                            else if (isSelectingTank && !isSelectingHealer) {
                                // Tank selection
                                guild.setTank(pets[i].name);
                                std::cout << "Selected Tank: " << pets[i].name << std::endl;
                                isSelectingHealer = true; // Move to healer selection
                            }
                            else if (isSelectingHealer) {
                                // Healer selection
                                guild.setHealer(pets[i].name);
                                std::cout << "Selected Healer: " << pets[i].name << std::endl;

                                // Guild is complete
                                isSelectingTank = false;
                                isSelectingHealer = false;
                                guildComplete = true;
                                guild.displayGuild();
                              
                                
                            }
                        }
                    }
                }
                }
            
        }

        window.clear();

        if (!inSecondPage && !inThirdPage && !inBattlePage && !inTrainPage && !inShopPage && !inGuildPage) {
            // First page rendering
           
            window.draw(backgroundSprite1);
            window.draw(button);
            window.draw(buttonText);
        }
        else if (inSecondPage) {
            // Second page rendering
           
            window.draw(backgroundSprite2);
            window.draw(promptText);
            window.draw(inputBox);
            window.draw(inputText);
        }
        else if (inThirdPage) {
            // Third page rendering
           
            window.draw(backgroundSprite3);
            for (int i = 0; i < menuButtonCount; ++i) {
                window.draw(menuButtons[i]);
                window.draw(menuTexts[i]);
            }
        }
        else if (inBattlePage) {
            // Battle page rendering
          
            window.draw(backgroundSprite4);
            for (int i = 0; i < battleButtonCount; ++i) {
                window.draw(battleButtons[i]);
                window.draw(battleTexts[i]);
            }
        }
        else if (inTrainPage) {
            // Train page rendering
          
            window.draw(trainBackgroundSprite);
            for (int i = 0; i < 4; ++i) {
                window.draw(pets[i].sprite);
            }
            window.draw(trainBackButton);
            window.draw(trainBackText);
        }
        else if (inShopPage) {
            // Shop page rendering
            
            window.draw(shopbackgroundSprite);
            window.draw(shopHeading);
            window.draw(currencyText);
            window.draw(shopBackButton);
            window.draw(shopBackText);

            for (int i = 0; i < shop.getItemCount(); ++i) {
                window.draw(potionSprites[i]);
                window.draw(itemNames[i]);
                window.draw(itemPrices[i]);
                window.draw(buyButtons[i]);
                window.draw(buyButtonTexts[i]);
            }
        }
        else if (inGuildPage) {

            // Clear the window to ensure no artifacts remain
            window.clear(sf::Color::Black);
            // Render the guild page background
            window.draw(guildBackgroundSprite);

            // Render the message
            sf::Text messageText;
            messageText.setFont(font);
            messageText.setCharacterSize(36);
            messageText.setFillColor(sf::Color::White);
            if (!isSelectingTank && !isSelectingHealer) {
                messageText.setString("Build Your Guild: Select Attacker");
            }
            else if (isSelectingTank && !isSelectingHealer) {
                messageText.setString("Build Your Guild: Select Tank");
            }
            else if (isSelectingHealer) {
                messageText.setString("Build Your Guild: Select Healer");
            }
            sf::FloatRect messageBounds = messageText.getLocalBounds();
            messageText.setOrigin(messageBounds.left + messageBounds.width / 2, messageBounds.top + messageBounds.height / 2);
            messageText.setPosition(window.getSize().x / 2, 50);
            window.draw(messageText);

            // Render available pets
            for (int i = 0; i < 4; ++i) {
                window.draw(pets[i].sprite);
            }

            // Render "Back to Home" button
            window.draw(guildBackButton);
            window.draw(guildBackText);

            // Render guild lineup if complete
            if (guildComplete) {
                sf::Text guildText;
                guildText.setFont(font);
                guildText.setCharacterSize(24);
                guildText.setFillColor(sf::Color::White);
                guildText.setPosition(100, 100);

                std::string guildLineup = "Your Guild:\n";
                guildLineup += "Attacker: " + guild.getAttacker() + "\n";
                guildLineup += "Tank: " + guild.getTank() + "\n";
                guildLineup += "Healer: " + guild.getHealer() + "\n";

                guildText.setString(guildLineup);
                window.draw(guildText);
            }
        }

        window.display();
    }

    return 0;
    // THE END THANKS FOR PLAYING 
    // GAME OVER!!
}
