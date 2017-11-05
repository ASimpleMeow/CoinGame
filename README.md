# SFML Coin Game
---
A simple coin game made with SFML.
Two players play this game, and it also features a minimax AI player.


## Rules
- There are 3 piles of coins, each pile has a random amount of coins between 1 and 28.
- A player picks a pile where there are more than 0 coins left, and chooses a prime number no greater than the amount of coins in the pile.
- The chosen prime will be subtracted from the remaining coins in that pile.
- The next player does the same.
- The game continues until there are no more coins left in any pile.
- The winner is the player who took the last coin.

## Features
- MiniMax AI (with a small element of randomness so it's not too predictable)
- Numbers below each pile to show how much coins are in the pile
- Coins in each pile are rendered as a yellow rectangle for now, they're arranged in a pyramid
