#include <iostream>		// uses std::cout std::cin
#include <stdlib.h>
#include <stdio.h>		// uses exit(0) and fprintf
#include <stack>                // uses std::stack 
#include <vector>
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

class ChessBoard
{
public:
 void PutQueen(int pos);
 void TakeQueen(int pos);
 void GetAvaliablePositions(std::vector<int>&);

 //!constructor
 ChessBoard();
 ~ChessBoard();
private:
 int lastAvaliablePosition;
 enum CellState
 {
    CELL_AVALIABLE ,
    CELL_NOT_AVALIABLE
 };

 CellState cells[64];
 std::stack<int> undo_stack;
};


#endif