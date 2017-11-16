#include "Player.hpp"

#include <math.h>

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

	std::tuple<int, int> move = minimax(piles, 0);

	piles[std::get<0>(move)] -= std::get<1>(move);
}

std::tuple<int,int> Player::minimax(std::vector<int>& piles, int depth) {

	const float WIN_SCORE = 100;

	float score;
	float maxScore = -10000;
	int maxPos = -1;
	int finalValue = -1;
	int pilePos{ 0 };

	for (auto& pile : piles) {
		for (auto& prime : PRIMES) {
			if (pile < 1 || pile - prime < 0) break;

			pile -= prime;

			auto hasWon = [](std::vector<int>& piles) {
				for (auto p : piles) {
					if (p > 0) return false;
				}
				return true;
			};

			if (hasWon(piles)) {
				score = WIN_SCORE;
			} else {
				if (depth<skill) {
					score = -std::get<1>(minimax(piles, depth + 1));
				} else {
					score = 0;
				}
			}

			if (abs(score - maxScore)<0.01 && ((double)rand()/RAND_MAX)<0.1) {
				maxScore = score;
				maxPos = pilePos;
				finalValue = prime;

			} else if (score>maxScore) {
				maxScore = score;
				maxPos = pilePos;
				finalValue = prime;
			}

			pile += prime;
		}
		pilePos++;
	}
	return{ maxPos, depth == 0 ? finalValue : maxScore };
}