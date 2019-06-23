
/*Emily Zastenchik
Assignment 3
March 21, 2017


*/
#include <cstdlib>
#include <iostream>
#include <stack>
#include "outinhelp.h"

using namespace ds_maze;
const int BORDER_INDEX_OFFSET = 2;
//The following constants are possible FloorStatus
const int FLOOR = 0;
const int WALL = 1;
const int PATHWALKED = 2;
const int DEADEND = 3;
bool operator == (const position& , const position&);//overload for position comparison
bool operator != (const position&, const position&);//overload for position comparison
bool canWalk(std::vector<std::vector<int>>& Maze, int row, int col);
bool canWalk(std::vector<std::vector<int>>& Maze, position p);
bool canWalk(std::vector<std::vector<int>>& Maze, position p, int col);
bool canWalk(std::vector<std::vector<int>>& Maze, int row, position p);
void SetPath(std::vector<std::vector<int>>& Maze, int row, int col, int FloorStatus = PATHWALKED);	//sets default parameter as PATHWALKED
void SetPath(std::vector<std::vector<int>>& Maze, position p, int FloorStatus = PATHWALKED);
void SetPath(std::vector<std::vector<int>>& Maze, std::stack<position>& Solution, int FloorStatus = PATHWALKED);
int main(int argc, char* argv[])
{
	std::stack<position>Solution;	//holds positions for solution
	position Entrance = position(1, 1);	//set entrand coordinates
	position Exit;
	std::vector< std::vector<int> > Maze;	//holds binary maze input

	for (int i = 1; i < argc; ++i) //read from command line
	{
		if (readMaze(argv[i], Maze))	
		{
			Solution.empty();	//empty previous solution
			int r = Maze.size() - BORDER_INDEX_OFFSET;	
			int c = Maze[0].size() - BORDER_INDEX_OFFSET;	
			
			outputMaze(Maze);
				
			//solve
			if (canWalk(Maze, Entrance) && canWalk(Maze, r, c))		//if entrance and exit points are not blocked
			{
				Exit.setpos(r, c);	//set row and col exit position with BORDER_OFFSET
				//std::cout << Exit << "Has a value of " << Maze[Exit.row()][Exit.col()] << std::endl;
				Solution.push(Entrance);	//push start position to solution stack
				std::stack<std::stack<position>> SolutionStack;	//holds all possible solutions to maze
				
				SetPath(Maze, Entrance, PATHWALKED);	//marks entrance w/ 2
				while (Solution.size() && Solution.top() != Exit)	//
				{
					if (canWalk(Maze, Solution.top(), 1))  // Maze[Solution.top().row()][ Solution.top().col() + 1] == 0 && Solution.top().row() < Maze.size() && Solution.top().col()  + 1< Maze[0].size()
					{//check EAST direction
						Solution.push(position(Solution.top().row(), Solution.top().col() + 1)); //pushes new position coordinates on stack
						SetPath(Maze, Solution, PATHWALKED);	//marks new position w/ 2 to prevent looping
					}
					else if (canWalk(Maze, Solution.top(), -1)) 
					{//check WEST direction
						Solution.push(position(Solution.top().row(), Solution.top().col() - 1));
						SetPath(Maze, Solution, PATHWALKED);
					}
					else if (canWalk(Maze, 1, Solution.top()))	
					{//check NORTH direction
						Solution.push(position(Solution.top().row() + 1, Solution.top().col()));
						SetPath(Maze, Solution, PATHWALKED);
					}
					else if (canWalk(Maze, -1, Solution.top()))	
					{//check SOUTH direction
						Solution.push(position(Solution.top().row() - 1, Solution.top().col()));
						SetPath(Maze, Solution, PATHWALKED);
					}
					else
					{
						SetPath(Maze, Solution, DEADEND);	//marks position w/ 3
						Solution.pop();	//
					}
					// Shortest Path
					if (!Solution.empty() && Solution.top() == Exit && (SolutionStack.empty() || SolutionStack.top().size() > Solution.size()))	
					{
						SolutionStack.push(Solution);
						SetPath(Maze, Solution, FLOOR);
						Solution.pop();
						SetPath(Maze, Solution, DEADEND);
						Solution.pop();
					}
				}
				if (!SolutionStack.empty())	//at least one solution in solution stack
				{
					while (SolutionStack.top().top().steps() < SolutionStack.top().size() - 1)
					{
						SolutionStack.top().top().stepup();	//counts steps
					}
					OutputPath(&SolutionStack.top());
				}
				else
				{
					std::cout << "This Maze is unsolvable! No solution." << std::endl;	//print if no solution in SolutionStack
				}
			}
			else
			{
				std::cout << "This Maze is unsolvable! Entrance or Exit blocked." << std::endl;	//print if Entrance or Exit position blocked
			}

			Maze.clear();	//removes vector elements and sets size to 0
		}
	}
	return 0;
}
/*
Precondition:passes two positions for equality comparsion
Postcondition: returns true if positions are equal
*/
bool operator == (const position& left, const position& right) //overload for position comparison
{//checks equality of both row and column
	return (left.row() == right.row()) && (left.col() == right.col());
}/*
Precondition:passes two positions for inequality comparison
Postcondition:returns true if positions are not equal
 */
bool operator != (const position& left, const position& right) // overload for position comparison
 
{//checks inequality of both row and column
	return (left.row() != right.row()) || (left.col() != right.col());
}
/*
Precondition: no precondition
Postcondition: returns true if path is 0
*/
bool canWalk(std::vector<std::vector<int>>& Maze, int row, int col)
{
	if (row >= 0 && col >= 0 && row < Maze.size() && col < Maze[0].size()) //checks if within bounds	
			//row and col are positive and within size boundary of maze
	{
		switch (Maze[row][col])
		{
		case WALL:	//1
		case PATHWALKED:	//2
		case DEADEND:	//3
			return false;
		default:
			return true;
		}
	}
	return false;
}

//overloaded canWalk functions: the following three canWalk functions call the first canWalk function
bool canWalk(std::vector<std::vector<int>>& Maze, position p)
{												
	return canWalk(Maze, p.row(), p.col());
}

bool canWalk(std::vector<std::vector<int>>& Maze, position p, int col)
												//position, int column adjustment 
{//checks if west or east directions are blocked
	return canWalk(Maze, p.row(), p.col() + col);
}

bool canWalk(std::vector<std::vector<int>>& Maze, int row, position p)
												//int row adjustment, position 
{//checks if north or south directions are blocked
	return canWalk(Maze, p.row() + row, p.col());
}
/*
Precondition: no precondition
Postcondition: updates position with new integer
*/
void SetPath(std::vector<std::vector<int>>& Maze, int row, int col, int FloorStatus)
{
	if (row >= 0 && col >= 0 && row < Maze.size() && col < Maze[0].size())	//checks boundaries
	{
		Maze[row][col] = FloorStatus;	//sets current Maze position to FloorStatus
	}
}

void SetPath(std::vector<std::vector<int>>& Maze, position p, int FloorStatus)
{
	return SetPath(Maze, p.row(), p.col(), FloorStatus);
}

void SetPath(std::vector<std::vector<int>>& Maze, std::stack<position>& Solution, int FloorStatus)
{
	if (Solution.size())	//at least one position in Solution stack
		return SetPath(Maze, Solution.top(), FloorStatus);		//mark top position in Solution stack
}


