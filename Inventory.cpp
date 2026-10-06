#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum class Category {
    WEAPON,
    LOOT,
    FOOD,
    ARMOR,
    POTION,
    MISC
};

string categoryToString(Category category) {
    switch (category) {
        case Category::WEAPON: return "Weapon";
        case Category::LOOT: return "Loot";
        case Category::FOOD: return "Food";
        case Category::ARMOR: return "Armor";
        case Category::POTION: return "Potion";
        case Category::MISC: return "Misc";
    }
    return "Misc";
}

string lowercase(string value) {
    transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(tolower(character));
    });
    return value;
}

bool stringToCategory(const string& value, Category& category) {
    const string normalized = lowercase(value);
    if (normalized == "weapon") category = Category::WEAPON;
    else if (normalized == "loot") category = Category::LOOT;
    else if (normalized == "food") category = Category::FOOD;
    else if (normalized == "armor") category = Category::ARMOR;
    else if (normalized == "potion") category = Category::POTION;
    else if (normalized == "misc") category = Category::MISC;
    else return false;
    return true;
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
    static constexpr int MAX_SLOTS = 20;
    static constexpr int MAX_STACKS = 99;

    Inventory() : nextID_(1) {}

    void run() {
        int choice;
        while (true) {
            printHeader();
            cout << "Slots used: " << items_.size() << " / " << MAX_SLOTS << "\n\n"
                 << "1. View inventory\n"
                 << "2. Add item\n"
                 << "3. Use item\n"
                 << "4. Drop item\n"
                 << "5. Exit\n";

            if (!readInt("Choice: ", 1, 5, choice)) return;
            switch (choice) {
                case 1: printInventory(); break;
                case 2: addItem(); break;
                case 3: changeQuantity("use"); break;
                case 4: changeQuantity("drop"); break;
                case 5:
                    cout << "Leaving the inventory.\n";
                    return;
            }
        }
    }

private:
    vector<Item> items_;
    int nextID_;

    void printHeader() const {
        cout << "\n========================================\n"
             << "        ADVENTURER'S INVENTORY\n"
             << "========================================\n";
    }

    void printInventory() const {
        printHeader();
        cout << "Slots used: " << items_.size() << " / " << MAX_SLOTS << "\n\n";
        if (items_.empty()) {
            cout << "Your inventory is empty.\n\n";
            return;
        }

        cout << left << setw(6) << "ID" << setw(22) << "Name"
             << setw(12) << "Category" << setw(8) << "Qty"
             << "Description\n" << string(72, '-') << '\n';
        for (const Item& item : items_) {
            cout << left << setw(6) << item.id << setw(22) << item.name
                 << setw(12) << categoryToString(item.category)
                 << setw(8) << item.quantity << item.description << '\n';
        }
        cout << '\n';
    }

    bool readLine(const string& prompt, string& value) const {
        cout << prompt;
        return static_cast<bool>(getline(cin, value));
    }

    bool readInt(const string& prompt, int minimum, int maximum, int& value) const {
        string line;
        while (readLine(prompt, line)) {
            istringstream input(line);
            char extra;
            if ((input >> value) && !(input >> extra) && value >= minimum && value <= maximum) {
                return true;
            }
            cout << "Enter a number from " << minimum << " to " << maximum << ".\n";
        }
        return false;
    }

    void addItem() {
        string name;
        string categoryInput;
        string description;
        int quantity;

        cout << "\n---- ADD ITEM ----\n";
        if (!readLine("Item name: ", name)) return;
        if (name.empty()) {
            cout << "Item name cannot be empty.\n";
            return;
        }
        if (!readLine("Category (weapon/loot/food/armor/potion/misc): ", categoryInput)) return;

        Category category;
        if (!stringToCategory(categoryInput, category)) {
            cout << "Unknown category. Item was not added.\n";
            return;
        }
        if (!readInt("Quantity (1-99): ", 1, MAX_STACKS, quantity)) return;
        if (!readLine("Description (optional): ", description)) return;

        int availableInMatchingStacks = 0;
        for (const Item& item : items_) {
            if (item.category == category && lowercase(item.name) == lowercase(name)) {
                availableInMatchingStacks += MAX_STACKS - item.quantity;
            }
        }
        const int remainingAfterStacks = max(0, quantity - availableInMatchingStacks);
        const int newSlotsNeeded = (remainingAfterStacks + MAX_STACKS - 1) / MAX_STACKS;
        if (items_.size() + static_cast<size_t>(newSlotsNeeded) > MAX_SLOTS) {
            cout << "Not enough free slots to add that quantity.\n";
            return;
        }

        int remaining = quantity;
        for (Item& item : items_) {
            if (item.category != category || lowercase(item.name) != lowercase(name)) continue;
            const int added = min(remaining, MAX_STACKS - item.quantity);
            item.quantity += added;
            remaining -= added;
            if (remaining == 0) break;
        }
        while (remaining > 0) {
            const int stackQuantity = min(remaining, MAX_STACKS);
            items_.push_back({nextID_++, name, category, stackQuantity, description});
            remaining -= stackQuantity;
        }
        cout << name << " added to your inventory.\n";
    }

    void changeQuantity(const string& action) {
        if (items_.empty()) {
            cout << "Your inventory is empty.\n";
            return;
        }
        printInventory();

        int id;
        if (!readInt("Item ID (0 to cancel): ", 0, nextID_ - 1, id) || id == 0) return;
        auto item = find_if(items_.begin(), items_.end(), [id](const Item& candidate) {
            return candidate.id == id;
        });
        if (item == items_.end()) {
            cout << "No item has that ID.\n";
            return;
        }

        int quantity;
        if (!readInt("Quantity (1-" + to_string(item->quantity) + "): ",
                     1, item->quantity, quantity)) return;
        item->quantity -= quantity;
        cout << quantity << " " << item->name << (quantity == 1 ? " " : "s ")
             << (action == "use" ? "used." : "dropped.") << '\n';
        if (item->quantity == 0) items_.erase(item);
    }
};

int main() {
    cout << "Ongoing Works!!! thank you.\n";
    cout << "This is a illustration of a simple inventory in a system game.\n";
    Inventory inventory;
    inventory.run();
    return 0;
}