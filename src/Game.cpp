#include "Game.hpp"

#include <iostream>
#include <functional>

Game::Game() :	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Coin Game"),
				numPiles(NUM_PILES),
				gameState{State::PLAYER_1_TURN},
				p1(PLAYER_1_HUMAN),
				p2(PLAYER_2_HUMAN)
{
	srand(static_cast<int>(time(NULL)));
	init();
}

void Game::init() {
	for (int i = 0; i < numPiles; ++i) piles.push_back((rand() % COINS_PER_PILE) + 1);
	current = &p1;
}

void Game::run() {
	sf::Clock clock;

	while (window.isOpen()) {
		if (!processEvents()) break;
		//update(clock.restart());
		render();
	}
}

bool Game::hasWon(Player& p) {
	if (current == &p) {
		for (auto pile : piles) {
			if (pile > 0) return false;
		}
		return true;
	}
	return false;
}


bool Game::processEvents() {

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			break;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) return false;
	return true;
	if (current->isHuman()) {
		std::cout << "Select a pile : ";
		std::cin >> selectedPile;
		std::cout << "Select a prime : ";
		std::cin >> selectedPrime;
	}
	return true;
}

void Game::update(sf::Time deltaTime) {
	if (!current->isHuman()) {
		current->takeTurn(piles);
		current = current == &p1 ? &p2 : &p1;
		return;
	}
	if (selectedPile < 0 || selectedPile >= numPiles) return;
	if (PRIMES.count(selectedPrime) == 0) return;

	int diff = piles[selectedPile] - selectedPrime;
	if (diff >= 0) {
		piles[selectedPile] = diff;
		current = current == &p1 ? &p2 : &p1;
		return;
	}
	std::cout << "You entered something wrong!" << std::endl;
}

void Game::render() {
	window.clear(sf::Color::Black);

	std::function<void(sf::Vector2f, int, int)> drawPile;
	drawPile = [this, &drawPile](sf::Vector2f startLocation, int amount, int row) {
		if (amount <= 0) return;
		sf::RectangleShape coinShape({ WINDOW_WIDTH / 29, WINDOW_HEIGHT / 60 });
		coinShape.setFillColor(sf::Color::Yellow);
		for (int r = 0; r < row; ++r) {
			if (amount <= 0) return;
			coinShape.setPosition({ startLocation.x + (coinShape.getSize().x * 1.2f)*r, startLocation.y });
			window.draw(coinShape);
			--amount;
		}
		drawPile({ startLocation.x + coinShape.getSize().x/1.5f, startLocation.y - coinShape.getSize().y*1.5f }, amount, --row);
	};

	int pileNum = 0;
	for (auto p : piles) {
		drawPile({(pileNum++ * 120.0f) + 50, WINDOW_WIDTH/5 }, p, 7);
		pileNum++;
	}

	window.display();

}

