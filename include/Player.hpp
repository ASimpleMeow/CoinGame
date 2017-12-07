#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "core.hpp"
#include <vector>
#include <tuple>

class Player {
public:
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	Player(bool isHuman);

	bool isHuman();
	std::tuple<int, int> takeTurn(std::vector<int>& piles);

private:
	bool human;
	int skill;
	std::tuple<int,int> minimax(std::vector<int>& piles, int depth);

};

#endif // !PLAYER_HPP

