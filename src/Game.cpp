#include "Game.hpp"

#include <iostream>
#include <functional>

Game::Game() :	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Coin Game"),
				numPiles(NUM_PILES),
				p1(PLAYER_1_HUMAN),
				p2(PLAYER_2_HUMAN)
{

	auto initializeText = [](const sf::Font& font, int charSize, const std::string str) {
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(charSize);
		text.setString(str);
		return text;
	};

	srand(static_cast<int>(time(NULL)));

	//Loading fonts and setting them to all the text
	if (font.loadFromFile("media/font/retganon.ttf")) {
		for (int i = 0; i < 3; ++i) pileText.push_back(initializeText(font, 30, ""));
		for (auto prime : PRIMES) primesText.push_back(initializeText(font, 30, std::to_string(prime)));
		winText = initializeText(font, 50, "");
		playerInfoText = initializeText(font, 35, "");
		undoButtonText = initializeText(font, 35, "Undo");
	} else {
		std::cout << "Error reading font from media/font/ - needs trs-million.ttf font\n";
	}

	//Loading texture
	if (!coinTexture.loadFromFile("media/texture/goldCoin.png")) {
		std::cout << "Error reading texture from media/texture/ - needs goldCoin.png\n";
	}

	if (!backgroundTexture.loadFromFile("media/texture/background.jpg")) {
		std::cout << "Error reading texture from media/texture/ - needs background.jpg\n";
	}

	undoButtonText.setPosition({WINDOW_WIDTH*0.9f, 0});

	init();
	render();
}

void Game::init() {
	piles.clear();
	for (int i = 0; i < numPiles; ++i) piles.push_back((rand() % COINS_PER_PILE) + 1);
	current = (rand()/RAND_MAX >= 0.5) ? &p1 : &p2;

	selectedPile = -1;
	selectedPrime = -1;

	pileText[0].setPosition(150, WINDOW_HEIGHT / 3);
	pileText[1].setPosition(390, WINDOW_HEIGHT / 3);
	pileText[2].setPosition(640, WINDOW_HEIGHT / 3);

	float xPos{ WINDOW_WIDTH / 5 };
	for (auto& prime : primesText) {
		prime.setPosition({ xPos, WINDOW_HEIGHT / 1.75f });
		xPos += WINDOW_WIDTH / 15;
	}

	background.setTexture(backgroundTexture);
	float scaleX = (float)window.getSize().x / backgroundTexture.getSize().x;
	float scaleY = (float)window.getSize().y / backgroundTexture.getSize().y;
	background.setScale({ scaleX, scaleY });
	background.setColor(sf::Color(255, 255, 255, 100));
}

void Game::run() {
	sf::Clock clock;

	while (window.isOpen()) {
		if (!processEvents()) break;
		update(clock.restart());
		render();
	}
}

bool Game::gameWon() {
	for (auto pile : piles) {
		if (pile > 0) return false;
	}
	return true;
}


bool Game::processEvents() {

	auto isIntersectingText = [](sf::Vector2f &mouse, sf::Text &t, float delta, float width, float height) {
		sf::FloatRect textRect;
		t.getTransform().transformRect(textRect);
		textRect.top = t.getPosition().y;
		textRect.left = t.getPosition().x - delta;
		textRect.width = width;
		textRect.height = height;
		return textRect.contains(mouse);
	};

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			break;
		}

		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::U) {
			undoMove();
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f mouse{ sf::Mouse::getPosition(window) };
			if (isIntersectingText(mouse, undoButtonText, 20, 100, 50)) {
				std::cout << "Undo \n";
				undoMove();
			}

		}

		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
			init();
			return true;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) return false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2f mouse{ sf::Mouse::getPosition(window) };

		for (int pile = 0; pile < pileText.size(); ++pile) {
			if (isIntersectingText(mouse, pileText[pile], 10, 40, 40)) {
				selectedPile = pile;
				pileText[pile].setFillColor(piles[pile] == 0 ? sf::Color::Black : sf::Color::Green);
				for (int pile = 0; pile < pileText.size(); ++pile) {
					if (selectedPile != pile) pileText[pile].setFillColor(sf::Color::White);
				}
				break;
			}
		}

		if (selectedPile == -1) return true;
		for (auto& text : primesText) text.setFillColor(sf::Color(0,0,0,0));
		for (int prime = 0; prime < primesText.size(); ++prime) {
			int primeValue = *std::next(PRIMES.begin(), prime);
			if (piles[selectedPile] - primeValue < 0) break;
			primesText[prime].setFillColor(sf::Color::White);
			if (isIntersectingText(mouse, primesText[prime], 10, 40, 40)) {
				primesText[prime].setFillColor(sf::Color::Green);
				selectedPrime = primeValue;
			}
		}
	}
	return true;
}

void Game::update(sf::Time deltaTime) {
	if (gameWon()) return;

	for (auto i = 0; i < piles.size(); ++i) if (piles[i] == 0) pileText[i].setFillColor(sf::Color::Black);

	if (!current->isHuman()) {
		std::tuple<int,int> aiMove = current->takeTurn(piles);
		undoStack.push(aiMove);
		current = current == &p1 ? &p2 : &p1;
		return;
	}
	if (selectedPile < 0 || selectedPile >= numPiles) return;
	if (PRIMES.count(selectedPrime) == 0) return;

	int diff = piles[selectedPile] - selectedPrime;
	if (diff >= 0) {
		piles[selectedPile] = diff;
		std::tuple<int, int> humanMove = { selectedPile, selectedPrime };
		undoStack.push(humanMove);
		current = current == &p1 ? &p2 : &p1;
		selectedPile = -1;
		selectedPrime = -1;
		for (auto& text : pileText) text.setFillColor(sf::Color::White);
		for (auto& text : primesText) text.setFillColor(sf::Color(0, 0, 0, 0));
		return;
	}
}

void Game::render() {
	window.clear(sf::Color::Black);
	window.draw(background);

	if (gameWon()) {
		std::string winner = current == &p1 ? "2" : "1";
		winText.setString("Player " + winner + " Has Won!");
		winText.setPosition({WINDOW_WIDTH/3, WINDOW_HEIGHT/2});
		window.draw(winText);
		window.display();
		return;
	}

	std::function<void(sf::Vector2f, int, int)> drawPile;
	drawPile = [this, &drawPile](sf::Vector2f startLocation, int amount, int row) {
		if (amount <= 0) return;
		sf::RectangleShape coinShape({ WINDOW_WIDTH / 29, WINDOW_HEIGHT / 60 });
		coinShape.setTexture(&coinTexture);
		coinShape.setFillColor(sf::Color::Yellow);
		for (int r = 0; r < row; ++r) {
			if (amount <= 0) return;
			coinShape.setPosition({ startLocation.x + (coinShape.getSize().x * 1.2f)*r, startLocation.y });
			window.draw(coinShape);
			--amount;
		}
		drawPile({ startLocation.x + coinShape.getSize().x/1.5f, startLocation.y - coinShape.getSize().y*1.5f }, amount, --row);
	};

	auto leadingSpace{ 0 };
	auto pileNum{ 0 };
	for (auto& p : piles) {
		drawPile({(leadingSpace++ * 120.0f) + 50, WINDOW_WIDTH/5 }, p, 7);
		leadingSpace++;

		sf::CircleShape circle(20);
		circle.setPosition({ pileText[pileNum].getPosition().x - 12, pileText[pileNum].getPosition().y - 2 });
		circle.setFillColor(piles[pileNum] == 0 ? sf::Color(100,100,100,175) : sf::Color(0, 255, 0, 100));
		window.draw(circle);

		pileText[pileNum].setString(std::to_string(p));
		window.draw(pileText[pileNum]);
		pileNum++;
	}

	for (auto& prime : primesText) {
		if (prime.getFillColor() != sf::Color(0, 0, 0, 0)) {
			sf::CircleShape circle(20);
			circle.setPosition({ prime.getPosition().x - 12, prime.getPosition().y - 2 });
			circle.setFillColor(sf::Color(0,255,0,100));
			window.draw(circle);
		}
		window.draw(prime);
	}

	if (!current->isHuman()) {
		sf::RectangleShape rect;
		rect.setPosition({0,0});
		rect.setSize({ (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
		rect.setFillColor(sf::Color(0,0,0,150));
		window.draw(rect);

		sf::Text screenText;
		screenText.setFont(font);
		screenText.setString("AI IS THINKING...");
		screenText.setCharacterSize(40);
		screenText.setPosition({ (float)WINDOW_WIDTH / 2 - 100, (float)WINDOW_HEIGHT / 2 - 50});
		window.draw(screenText);
	}

	std::string playerNum = current == &p1 ? "1" : "2";
	playerInfoText.setString("Turn : Player " + playerNum);
	window.draw(playerInfoText);
	
	//Undo button
	sf::RectangleShape buttonShape;
	buttonShape.setPosition({ undoButtonText.getPosition().x - 25, undoButtonText.getPosition().y });
	buttonShape.setSize({ 100, 50 });
	buttonShape.setFillColor(sf::Color::Blue);
	window.draw(buttonShape);
	window.draw(undoButtonText);

	window.display();

}

void Game::undoMove() {
	if (undoStack.empty() || undoStack.size() < 2) return;

	for (auto amount = 2; amount > 0; --amount) {
		std::tuple<int, int> lastMove = undoStack.top();
		undoStack.pop();
		piles[std::get<0>(lastMove)] += std::get<1>(lastMove);
	}
}

