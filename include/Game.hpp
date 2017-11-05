#ifndef GAME_HPP
#define GAME_HPP

#include "core.hpp"
#include "Player.hpp"

#include <vector>

class Game {
public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	Game();
	void run();
	bool hasWon(Player& p);

private:
	void init();
	bool processEvents();
	void update(sf::Time deltaTime);
	void render();

	sf::RenderWindow window;
	
	int numPiles;
	std::vector<int> piles;
	int selectedPile;
	int selectedPrime;

	State gameState;

	Player p1;
	Player p2;
	Player* current;

};

#endif // !GAME_HPP
