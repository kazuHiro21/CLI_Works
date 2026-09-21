#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <sstream>
using namespace std;

enum class Category {
    WEAPON,
    LOOT,
    FOOD,
    ARMOR,
    POTION,
    MISC
};

string categoryToString(Category cat) {
    switch(cat) {
        case Category::WEAPON: return "Weapon";
        case Category::FOOD:   return "Food";
        case Category::LOOT:   return "Loot";
        case Category::ARMOR:  return "Armor";
        case Category::POTION: return "Potion";
        case Category::MISC:   return "Misc";
    }
}

Category stringToCategory(const string& s) {
    string lower = s;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "weapon") return Category::WEAPON;
    if (lower == "food") return Category::FOOD;
    if (lower == "loot") return Category::LOOT;
    if (lower == "armor") return Category::ARMOR;
    if (lower == "potion") return Category::POTION;
    return Category::MISC;
}

struct Item {
    int id;
    string name;
    Category category;
    int quantity;
    string description;
};

class Inventory {
    public: 
        static const int MAX_SLOTS = 20;
        static const int MAX_STACKS = 99;

        Inventory() : nextID(1) {}

        void Inventory::printHeader() const {
            cout << "\n";
            cout << "  ╔══════════════════════════════════════════════════════════════╗\n";
            cout << "  ║              ⚔  ADVENTURER'S INVENTORY  ⚔                  ║\n";
            cout << "  ╚══════════════════════════════════════════════════════════════╝\n";
        }
};

void Inventory::printInventory() const {
    printHeader();
    cout << " Slots used: " << items.size() << " / " << MAX_SLOTS << "\n\n";

    if (items.empty()) {
        cout << " [ Your inventory is empty, nothing to grab. ]\n\n";
        return;
    }
    cout << " " << std::left
        << std::setw(4) << "ID"
        << std::setw(20) << "Name"
        << std::setw(10) << "Category"
        << std::setw(6) << "Qty"
        << "Description\n";
    cout << "  " << string(62, '-') << "\n";

    for (const auto& item : items) {
        cout << " " <<  std::left
            << std::setw(4) << item.id
            << std::setw(20) << item.name
            << std::setw(10) << categoryToString(item.category)
            << std::setw(6) << item.quantity
            << item.description << "\n";
    }
    cout << "\n";
}

bool storeItem() {
    if ((int)items.size() >= MAX_SLOTS) {
        cout << "\n Invetory is currently full storage! Please Dispose or Disarm something first \n";
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max()), '\n';

    std::string name, desc, catInput;
    int qty;

    cout << "\n----ADD ITEM-----------------------------\n";
    cout << "Item name         : "getline(cin, name);
    if(name.empty()) {cout << " Name cannot be empty!\n"; return false; }

    cout << " Category\n"
         << " (weapon/food/loot/armor/misc/potion) : ";
    getline(cin, catInput);

    std::cout << "  Quantity (1-" << MAX_STACK << ")  : ";
        std::cin >> qty;
        if (std::cin.fail() || qty < 1 || qty > MAX_STACK) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  ✗ Invalid quantity.\n";
            return false;

}

}
int main() {
    cout << "Ongoing Works!!! thank you." << endl;
    return 0;
}