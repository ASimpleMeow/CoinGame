#ifndef CORE_HPP
#define CORE_HPP

#include <SFML/Graphics.hpp>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr int SCALE{ 1 };
constexpr int WINDOW_WIDTH{ 800 * SCALE }, WINDOW_HEIGHT{ 600 * SCALE };
const std::set<int> PRIMES{ 1,2,3,5,7,11,13,17,23 };

constexpr int NUM_PILES{ 3 };
constexpr int COINS_PER_PILE{ 28 };

constexpr bool PLAYER_1_HUMAN{ true };
constexpr bool PLAYER_2_HUMAN{ true };

#endif // !CORE_HPP

