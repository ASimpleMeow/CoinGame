#include "Player.hpp"

#include <math.h>
#include <iostream>

Player::Player(bool isHuman) : human(isHuman),
skill{6} {

}

bool Player::isHuman() {
	return human;
}

void Player::takeTurn(std::vector<int>& piles) {
	int pileAmount{ 0 };
	for (auto pile : piles) pileAmount += pile;
	skill = static_cast<int>(2.04642 + (8.43 - 2.04642) / (1 + powf((pileAmount / 17.86974), 1.15114)));

	std::vector<int> copyPile(piles);
	std::tuple<int, int> move = minimax(copyPile, 0);

	std::cout << "\nAI "<<skill<<": Pile : " << std::get<0>(move) << "   Prime : " << std::get<1>(move) << "\n\n";

	piles[std::get<0>(move)] -= std::get<1>(move);
}

std::tuple<int,int> Player::minimax(std::vector<int>& piles, int depth) {

	const float WIN_SCORE = 100;

	float score;
	float maxScore = -10000;
	int maxPos = -1;
	int finalValue = -1;

	for (int pile = 0; pile < piles.size(); ++pile) {
		for (auto prime : PRIMES) {
			if (piles[pile] <= 0 || piles[pile] - prime < 0) break;

			piles[pile] -= prime;

			auto hasWon = [](std::vector<int>& piles) {
				for (auto p : piles) {
					if (p > 0) return false;
				}
				return true;
			};

			// evaluate result (recursively)
			if (hasWon(piles)) {
				score = WIN_SCORE;
			} else {
				if (depth<skill) {
					score = -std::get<1>(minimax(piles, depth + 1));
				} else {
					score = 0;
				}
			}

			if (score>maxScore) {
				maxScore = score;
				maxPos = pile;
				finalValue = prime;
			}

			//undo move
			piles[pile] += prime;
		}
	}

	return{ maxPos, depth == 0 ? finalValue : maxScore };
}