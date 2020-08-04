#include "chessboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>		// uses std::vector

/* macros for the debug purposes */
#define PRINT_VECTOR(vector) for(int i=0;i<vector.size();i++)std::cout<<vector[i]<<" ";

// !constructor
ChessBoard::ChessBoard()
{
  // set all the cells to avaliable
 for(int i=0;i<64;i++)
  cells[i]=CELL_AVALIABLE;

  lastAvaliablePosition=0;
 // we don't need to clear the stack since when the constructor is called it's cleared.
}

// ! destructor 
ChessBoard::~ChessBoard()
{
 // ~ Do nothing here.
}

void ChessBoard::PutQueen(int pos)
{
 int undo_count=0;
 
 // set the pos unavaliable.
 cells[pos-1]=CELL_NOT_AVALIABLE;
 undo_stack.push(pos);
 undo_count++;

 // horizontally,
 int i=pos;
 while(i>8)
  {
   i=i-8;
   if(cells[i-1]!= CELL_NOT_AVALIABLE)
   {
    cells[i-1] = CELL_NOT_AVALIABLE;
    undo_stack.push(i);
    undo_count++;
   }
  }
 i=pos;
 while(i<57)
 {
  i=i+8;
  if(cells[i-1]!= CELL_NOT_AVALIABLE)
  {
   cells[i-1] = CELL_NOT_AVALIABLE;
   undo_stack.push(i);
   undo_count++;
  }
 }
 
 i=pos;
 while(i%8!=0)
 {
   i++;
   if(cells[i-1]!= CELL_NOT_AVALIABLE)
   {
    cells[i-1]=CELL_NOT_AVALIABLE;
    undo_stack.push(i);
    undo_count++;
   }
 }
 i=pos;
 while(i%8!=1)
 {
   i--;
   if(cells[i-1]!=CELL_NOT_AVALIABLE)
   {
    cells[i-1]=CELL_NOT_AVALIABLE;
    undo_stack.push(i);
    undo_count++;
   }
 }
 i=pos;
 while(i%8!=0&&i<57)
 {
  i=i+9;
  if(cells[i-1]!= CELL_NOT_AVALIABLE)
  {
   cells[i-1] = CELL_NOT_AVALIABLE;
   undo_stack.push(i);
   undo_count++;
  }
 }
 i=pos;
 while(i%8!=1&&i>8)
 {
  i=i-9;
   if(cells[i-1]!= CELL_NOT_AVALIABLE)
   {
    cells[i-1] = CELL_NOT_AVALIABLE;
    undo_stack.push(i);
    undo_count++;
   }
 }
 i=pos;
 while(i%8!=1&&i<57)
 {
  i=i+7;
   if(cells[i-1]!= CELL_NOT_AVALIABLE)
   {
    cells[i-1] = CELL_NOT_AVALIABLE;
    undo_stack.push(i);
    undo_count++;
   }  
 }
 
 i=pos;
 while(i%8!=0&&i>8)
 {
   i=i-7;
   if(cells[i-1]!= CELL_NOT_AVALIABLE)
   {
    cells[i-1] = CELL_NOT_AVALIABLE;
    undo_stack.push(i);
    undo_count++;
   }
 }
 // now push the undo_count.
 undo_stack.push(undo_count);
}

void ChessBoard::TakeQueen(int pos)
{
 // get the undo_count
 int undo_count=undo_stack.top();
 undo_stack.pop();
 for(;undo_count>0;undo_count--)
 {
  int undo_pos=undo_stack.top();
  undo_stack.pop();
  cells[undo_pos-1]=CELL_AVALIABLE;
 }
}

void ChessBoard::GetAvaliablePositions(std::vector<int>& _input)
{
 lastAvaliablePosition=-1;
 while(cells[++lastAvaliablePosition] != CELL_AVALIABLE&& lastAvaliablePosition<65);
 while(lastAvaliablePosition<65)
 {
  _input.push_back(lastAvaliablePosition+1);
  while(cells[++lastAvaliablePosition] != CELL_AVALIABLE&& lastAvaliablePosition<65);
 }
 //PRINT_VECTOR(_input);

}

