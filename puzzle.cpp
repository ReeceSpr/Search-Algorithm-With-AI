#include "puzzle.h"
#include  <cmath>
#include  <assert.h>

using namespace std;

//////////////////////////////////////////////////////////////
//constructor
//////////////////////////////////////////////////////////////
Puzzle::Puzzle(const Puzzle &p) : path(p.path){
	
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		    board[i][j] = p.board[i][j];
		    goalBoard[i][j] = p.goalBoard[i][j];
		}
	}	
	
	x0 = p.x0;
	y0 = p.y0;
	path = p.path;
	pathLength = p.pathLength;
	hCost = p.hCost;
	fCost = p.fCost;	
	strBoard = toString(); //uses the board contents to generate the string equivalent
	depth = p.depth;
	parentStates = p.parentStates;
    updateFCost();
}

//////////////////////////////////////////////////////////////
//constructor
//inputs:  initial state, goal state
//////////////////////////////////////////////////////////////
Puzzle::Puzzle(string const elements, string const goal){
	
	int n;
	
	n = 0;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		    board[i][j] = elements[n] - '0';
		    if(board[i][j] == 0){
			    x0 = j;
			    y0 = i;
			 }
		    n++;
		} 
	}
		
	///////////////////////
	n = 0;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		    goalBoard[i][j] = goal[n] - '0';
		    n++;
		} 
	}		
	///////////////////////	
	path = "";
	pathLength=0;
	hCost = 0;
	fCost = 0;
	depth = 0;
	strBoard = toString();
    updateFCost();
	parentStates = new vector<string>();
}



void Puzzle::setDepth(int d){
	depth = d;
}

int Puzzle::getDepth(){
	return depth;
}

void Puzzle::updateHCost(heuristicFunction hFunction){
	hCost = h(hFunction);
	updateFCost();
}

void Puzzle::updateFCost(){
	fCost=hCost+pathLength;
}

int Puzzle::getFCost(){
	return fCost;
}

int Puzzle::getHCost(){
	return hCost;
}

int Puzzle::getGCost(){
	return pathLength;
}

//Heuristic function implementation
int Puzzle::h(heuristicFunction hFunction){
	int sum=0;
	int h=0;
	int numOfMisplacedTiles=0;
	
	switch(hFunction){
		case misplacedTiles:			      
				//place your implementation here
			for(int i=0; i < 3; i++){
				for(int j=0; j < 3; j++){
					if (((board[i][j]) == (goalBoard[i][j])) || (board[i][j]==0)){
						//Not a misplaced, not the missing tile
					} else {
						numOfMisplacedTiles++;
					}
				}
			}
			h = numOfMisplacedTiles;
		        break;
		         
		case manhattanDistance:
		        int xone, yone, xtwo, ytwo = 0;
		        int intial, goal;
		        int x,y=0;
		        for(xone=0; xone<3; xone++){
                    for(yone=0; yone<3; yone++){
                        intial=board[xone][yone];
                        for(xtwo=0; xtwo<3; xtwo++){
                            for(ytwo=0; ytwo<3; ytwo++){
                                if(intial==0){
                                    xtwo=3;
                                    ytwo=3;
                                    //We do not want to add 0 to the count
                                } else{
                                    goal=goalBoard[xtwo][ytwo];
                                    if(intial==goal){
                                        //Found the intial and its goal spot
                                        //Do calc
                                        x=abs(xone-xtwo);
                                        y=abs(yone-ytwo);
                                        //Add To Sum
                                        sum=sum+x+y;
                                        //We do not need to check the rest of the list
                                        xtwo=3;
                                        ytwo=3;
                                    }
                                    //If its not 0 and not the intial spot and goal spot do not = then check next goal spot
                                }
                            }
                        }
                    }
		        }
		        h = sum; 					
		        break;         
		           
	};
	
	return h;
	
}


//converts board state into its string representation
string Puzzle::toString(){
  int n;
  string stringPath;
  
  n=0;
  for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){			    
		    stringPath.insert(stringPath.end(), board[i][j] + '0');
		    n++;
		} 
  }
  
//  cout << "toString = " << stringPath << endl;
  
  return stringPath;
}
bool Puzzle::goalMatch(){
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){
			if(board[i][j]!=goalBoard[i][j]){
				return false;
			}
		}
	}
	return true;
}
const string Puzzle::getPath(){
	return path;
}

bool Puzzle::canMoveLeft(){
    return (x0 > 0);

}
bool Puzzle::canMoveRight(){

   return (x0 < 2);
	
}
bool Puzzle::canMoveUp(){

   return (y0 > 0);
	
}
bool Puzzle::canMoveDown(){

   return (y0 < 2);
	
}

///////////////////////////////////////////////
//these functions will be useful for Progressive Deepening Search 

bool Puzzle::canMoveLeft(int maxDepth){
  	if(maxDepth>depth){
        return (x0 > 0);
  	} else {
        return false;
  	}
}
bool Puzzle::canMoveRight(int maxDepth){
    if(maxDepth>depth){
        return (x0 < 2);
    } else {
        return false;
    }
}
bool Puzzle::canMoveUp(int maxDepth){
    if(maxDepth>depth){
		return (y0 > 0);
    } else {
        return false;
    }
}
bool Puzzle::canMoveDown(int maxDepth){
    if(maxDepth>depth){
		return (y0 < 2);
    } else {
        return false;
    }
}

///////////////////////////////////////////////

Puzzle *Puzzle::moveLeft(){
	
	Puzzle *p = new Puzzle(*this);
	
	
   if(x0 > 0){
		
		p->board[y0][x0] = p->board[y0][x0-1];
		p->board[y0][x0-1] = 0;
		
		p->x0--;
		
		p->path = path + "L";
		p->pathLength = pathLength + 1;  
		p->depth = depth + 1; 
		
		
	}
   p->parentStates->push_back(toString());
   p->strBoard = p->toString();
   return p;
	
}
Puzzle *Puzzle::moveRight(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(x0 < 2){
		
		p->board[y0][x0] = p->board[y0][x0+1];
		p->board[y0][x0+1] = 0;
		
		p->x0++;
		
		p->path = path + "R";
		p->pathLength = pathLength + 1; 
     	
		p->depth = depth + 1;
		
	}
	p->parentStates->push_back(toString());
	p->strBoard = p->toString();
	return p;
	
}
Puzzle *Puzzle::moveUp(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(y0 > 0){
		
		p->board[y0][x0] = p->board[y0-1][x0];
		p->board[y0-1][x0] = 0;
		
		p->y0--;
		
		p->path = path + "U";
		p->pathLength = pathLength + 1;  
	
		p->depth = depth + 1;
		
	}
	p->parentStates->push_back(toString());
	p->strBoard = p->toString();
	return p;
	
}
Puzzle *Puzzle::moveDown(){
	
   Puzzle *p = new Puzzle(*this);
	
	
   if(y0 < 2){
		
		p->board[y0][x0] = p->board[y0+1][x0];
		p->board[y0+1][x0] = 0;
		
		p->y0++;
		
		p->path = path + "D";
		p->pathLength = pathLength + 1;  
		
		p->depth = depth + 1;
		
	}
	p->parentStates->push_back(toString());
	p->strBoard = p->toString();
	return p;
	
}

/////////////////////////////////////////////////////


void Puzzle::printBoard(){
	cout << "board: "<< endl;
	/*for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){	
		  cout << endl << "board[" << i << "][" << j << "] = " << board[i][j];
		}
	}
	cout << endl;*/
	cout<<board[0][0]<<"\t"<<board[0][1]<<"\t"<<board[0][2]<<endl;
	cout<<board[1][0]<<"\t"<<board[1][1]<<"\t"<<board[1][2]<<endl;
	cout<<board[2][0]<<"\t"<<board[2][1]<<"\t"<<board[2][2]<<endl;
}
int Puzzle::getPathLength() const {
	return pathLength;
}
//Checks if state is in vector
bool Puzzle::contains(vector<string> l, string goal) {
		vector<string>::iterator itr;
		for (itr = l.begin(); itr<l.end(); itr++){
			if(*itr==goal){
				return true;
			}
		}
		return false;
}
//Checks if the current state is not in parents states
bool Puzzle::isNewState(){
	return !contains(*parentStates, getString());
}
//Overidden for Uniform and A*
bool Puzzle::operator<(const Puzzle& c) const
{
	return (this->fCost>c.fCost);
}


