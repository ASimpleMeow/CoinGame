#ifndef GAME_HPP
#define GAME_HPP

#include "core.hpp"
#include "Player.hpp"

#include <vector>
#include <stack>
#include <tuple>

class Game {
public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	Game();
	void run();
	bool gameWon();

private:
	void init();
	bool processEvents();
	void update(sf::Time deltaTime);
	void render();
	void undoMove();

	std::stack<std::tuple<int, int>> undoStack;

	sf::RenderWindow window;
	sf::Font font;
	std::vector<sf::Text> pileText;
	std::vector<sf::Text> primesText;
	sf::Text winText;
	sf::Text playerInfoText;
	sf::Text undoButtonText;

	sf::Texture coinTexture;
	sf::Texture backgroundTexture;
	sf::Sprite background;

	int numPiles;
	std::vector<int> piles;
	int selectedPile;
	int selectedPrime;

	Player p1;
	Player p2;
	Player* current;

};

#endif // !GAME_HPP
