#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "Board.h"
#include "Constants.h"
class GameLogic {
public:
    Board& getMutablePlayerBoard(); // Acceso mutable
    
    static GameLogic& getInstance();
    void setPlayerBoard(const Board& board);
    void initializeBoards();
    bool placeShip(Board& board, int x, int y, int size, bool isVertical);
    void initializeEnemyBoard();
    bool attack(Board& board, int x, int y);
    bool isGameOver(const Board& board);
    void enemyTurn(); 
    const Board& getPlayerBoard() const;
    const Board& getEnemyBoard() const;

    Board& getMutableEnemyBoard();
private:
    GameLogic();
    GameLogic(const GameLogic&) = delete;
    GameLogic& operator=(const GameLogic&) = delete;
    bool canPlaceShip(const Board& board, int x, int y, int size, bool isVertical);

    Board playerBoard;
    Board enemyBoard;
    bool isCellAttacked(const Board& board, int x, int y) const; // Nueva función
};

#endif // GAME_LOGIC_H

