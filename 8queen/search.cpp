
/* Author : M Sandun Dhammika Perera.(sandundhammikaperera@yahoo.com PS:-communication dev ideas only).
 * License: GNU GPL
 * purpose: To demonstrate 8-queen problem.
 * :NOTE: This program will generate all the permutations of the 8th queen problem but not the combinations.
 */

#include <iostream>		//uses std:;cin std::cout
#include <stdio.h>
#include <stdlib.h>
#include "chessboard.h"
#include <vector>			// uses std::vector

void search(int(&)[8],ChessBoard& ,int);
void print_queen_locations(int(&)[8]);
int queens[8];

int main(int argc,char argv[])
{
 ChessBoard chess_board;
 search(queens,chess_board,1);
 return 0;
}

void search(int(&queens)[8],ChessBoard& chess_board,int queen)
{
 if(queen==8)
 {
  std::vector<int> avaliable_positions;
  avaliable_positions.clear();
  chess_board.GetAvaliablePositions(avaliable_positions);
  for(int i=0;i<avaliable_positions.size();i++)
  {
    queens[7]=avaliable_positions[i];
    print_queen_locations(queens);
  }
 }else
 {
  std::vector<int> avaliable_positions;
  avaliable_positions.clear();
  chess_board.GetAvaliablePositions(avaliable_positions);
  for (int i=0;i<avaliable_positions.size();i++)
  {
    int pos=avaliable_positions[i];
    chess_board.PutQueen(pos);
    queens[queen-1]=pos;
    search(queens,chess_board,queen+1);
    chess_board.TakeQueen(pos);
  }
 }
}

void print_queen_locations(int(& queen)[8])
{
 std::cout<<'<';
 for(int i=0;i<8;i++)
   std::cout<<queen[i]<<" ";
 std::cout<<'>'<<'\n';
}


