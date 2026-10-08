#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Monster {
	std::string name;
	std::string title;
	std::string description;
};

int readChoice(int minimum, int maximum) {
	int choice;
	while (true) {
		std::cout << "> ";
		if (std::cin >> choice && choice >= minimum && choice <= maximum) {
			return choice;
		}

		if (std::cin.eof()) {
			return -1;
		}

		std::cout << "Please enter a number from " << minimum << " to " << maximum << ".\n";
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}
}

void printHealthBar(const std::string& name, int health) {
	const int barWidth = 20;
	const int filled = health * barWidth / 100;

	std::cout << name << " [";
	for (int i = 0; i < barWidth; ++i) {
		std::cout << (i < filled ? '#' : '-');
	}
	std::cout << "] " << health << "%\n";
}

void hunt(const Monster& monster, std::mt19937& random) {
	std::uniform_int_distribution<int> damage(10, 20);
	int hunterHealth = 100;
	int monsterHealth = 100;

	std::cout << "\nYou arrive in the field. " << monster.name << " appears!\n";
	std::cout << monster.description << "\n\n";

	while (hunterHealth > 0 && monsterHealth > 0) {
		printHealthBar("Hunter", hunterHealth);
		printHealthBar(monster.name, monsterHealth);
		std::cout << "\n1. Attack\n2. Retreat\n";

		const int choice = readChoice(1, 2);
		if (choice == -1) {
			std::cout << "\nLeaving the hunt board. Happy hunting!\n";
			std::exit(0);
		}
		if (choice == 2) {
			std::cout << "You retreat from the field. Quest abandoned.\n";
			return;
		}

		const int hunterDamage = damage(random);
		monsterHealth = std::max(0, monsterHealth - hunterDamage);
		std::cout << "You strike " << monster.name << " for " << hunterDamage << "% damage.\n";

		if (monsterHealth == 0) {
			break;
		}

		const int monsterDamage = damage(random);
		hunterHealth = std::max(0, hunterHealth - monsterDamage);
		std::cout << monster.name << " counterattacks for " << monsterDamage << "% damage!\n\n";
	}

	if (monsterHealth == 0) {
		std::cout << "\nQuest complete! You hunted " << monster.name << ".\n";
	} else {
		std::cout << "\nQuest failed. You have been defeated by " << monster.name << ".\n";
		std::cout << "Your next quest will begin with full health.\n";
	}
}

int main() {
	const std::vector<Monster> monsters = {
		{"Rathian", "Queen of the Land",
		 "A ground-focused wyvern that attacks with sweeping charges and a venomous tail."},
		{"Rathalos", "King of the Skies",
		 "A flying wyvern that pressures hunters from above with fiery breath and poisoned talons."},
		{"Diablos", "Horned Tyrant of the Sandy Plains",
		 "A powerful desert-dweller that burrows underground before erupting into heavy charges."},
		{"Anjanath", "The Brute Wyvern",
		 "A territorial brute wyvern known for explosive charges and scorching blasts of breath."},
		{"Great Jagras", "The Pack Leader",
		 "A large fanged wyvern that fights with lunges and swipes, often backed by smaller Jagras."}
	};
	std::mt19937 random(std::random_device{}());

	std::cout << "=================================\n"
			  << "      MONSTER HUNTER: CLI        \n"
			  << "=================================\n";

	while (true) {
		std::cout << "\nQUEST BOARD\n";
		for (std::size_t i = 0; i < monsters.size(); ++i) {
			std::cout << i + 1 << ". " << monsters[i].name << " - " << monsters[i].title << "\n";
		}
		std::cout << "0. Leave the guild\nChoose a quest to view its details.\n";

		const int choice = readChoice(0, static_cast<int>(monsters.size()));
		if (choice == -1 || choice == 0) {
			std::cout << "\nThanks for playing. Happy hunting!\n";
			break;
		}

		const Monster& monster = monsters[choice - 1];
		std::cout << "\nQUEST: Hunt " << monster.name << "\n"
				  << "Target: " << monster.title << "\n"
				  << "Field report: " << monster.description << "\n"
				  << "Accept this quest?\n1. Hunt\n2. Decline\n";

		const int accept = readChoice(1, 2);
		if (accept == -1) {
			std::cout << "\nThanks for playing. Happy hunting!\n";
			break;
		}
		if (accept == 1) {
			hunt(monster, random);
		} else {
			std::cout << "Quest declined. Returning to the board.\n";
		}
	}

	return 0;
}
