#include "algorithm.h"
#include <queue>
#include <list>
#include <algorithm>
#include <stack>
using namespace std;
/*
 * In each function I have a commented out a timer exit.
 * Algorithms may lock themselves in a permanent loop.
 * Most algorithms exit correctly when the Q is empty in this case, but because of the nature of breadth first, sometimes this does not occur.
 */
/*
 * Helper Functions
 */

//Search function to clean up iterators in C++
//Returns bool, true if in false if not.
bool contains(vector<string> l, string goal){
    vector<string>::iterator itr;
    for (itr = l.begin(); itr<l.end(); itr++){
        if(*itr==goal){
            return true;
        }
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Breadth-First Search 
//
// Move Generator:  Pick first element of Q; Add path extensions to END of Q.
//
////////////////////////////////////////////////////////////////////////////////////////////
string breadthFirstSearch(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
    string path;
	clock_t startTime = clock();
	Puzzle *puzzle = new Puzzle(initialState, goalState);
	auto *inQ = new queue<Puzzle>();
	inQ->push(*puzzle);
	Puzzle *temp;
    numOfStateExpansions=0;
    maxQLength=0;
    actualRunningTime=0.0;
	try {
        while (!inQ->empty()) {
            puzzle = &inQ->front();
            if (puzzle->goalMatch()) {
                break;
            }
            /*actualRunningTime=((float)(clock() - startTime)/CLOCKS_PER_SEC);
            if(actualRunningTime>3600){
                cout << "\n\n*---- Search Timed Out. (Searched For 1 Hour) ----*" << endl;
                return "";
            } */
            if (puzzle->canMoveUp()) {
                temp = puzzle->moveUp();
                if (temp->isNewState()) {
                    inQ->push(*temp);
                }
                delete temp;
            }
            numOfStateExpansions++;
            if (puzzle->canMoveRight()) {
                temp = puzzle->moveRight();
                if (temp->isNewState()) {
                    inQ->push(*temp);
                }
                delete temp;
            }
            if (puzzle->canMoveDown()) {
                temp = puzzle->moveDown();
                if (temp->isNewState()) {
                    inQ->push(*temp);
                }
                delete temp;
            }
            if (puzzle->canMoveLeft()) {
                temp = puzzle->moveLeft();
                if (temp->isNewState()) {
                    inQ->push(*temp);
                }
                delete temp;
            }
            if (inQ->size() > maxQLength) {
                maxQLength = inQ->size();
            }
            inQ->pop();
        }


    }
    catch(bad_alloc& ba){
        cout<<"Out of Memory";
        return "";
    }
    if(inQ->empty()){
        return "";
    }
    actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
    return puzzle->getPath();

}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  Breadth-First Search with VisitedList
//
// Move Generator: Pick first element of Q; Add path extensions to END of Q if they are not in visted list, add element to visted list.
//
////////////////////////////////////////////////////////////////////////////////////////////
string breadthFirstSearch_with_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime){
    string path;
    clock_t startTime= clock();
    Puzzle *puzzle = new Puzzle(initialState, goalState);
    auto *inQ = new queue<Puzzle>();
    inQ->push(*puzzle);
    Puzzle *temp;
    numOfStateExpansions=0;
    maxQLength=0;
    actualRunningTime=0.0;
    vector<string> *vlist = new vector<string>();

    try {
        while (!inQ->empty()) {
            puzzle = &inQ->front();

            if (puzzle->goalMatch()) {
                break;

            }
            /*actualRunningTime=((float)(clock() - startTime)/CLOCKS_PER_SEC);
            if(actualRunningTime>1200){
                cout << "\n\n*---- Search Timed Out. (Searched For 1 Hour) ----*" << endl;
                return "";
            }*/
            if (puzzle->canMoveUp()) {
                temp = puzzle->moveUp();
                if(!contains(*vlist,temp->toString())) {
                    inQ->push(*temp);
                    vlist->push_back(temp->toString());
                    delete temp;
                } else {
                    delete temp;
                }
            }
            numOfStateExpansions++;
            if (puzzle->canMoveRight()) {
                temp = puzzle->moveRight();
                if(!contains(*vlist,temp->toString())) {
                    inQ->push(*temp);
                    vlist->push_back(temp->toString());
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveDown()) {
                temp = puzzle->moveDown();
                if(!contains(*vlist,temp->toString())) {
                    inQ->push(*temp);
                    vlist->push_back(temp->toString());
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveLeft()) {
                temp = puzzle->moveLeft();
                if(!contains(*vlist,temp->toString())) {
                    inQ->push(*temp);
                    vlist->push_back(temp->toString());
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (inQ->size() > maxQLength) {
                maxQLength = inQ->size();
            }
            inQ->pop();
        }
        if(inQ->empty()){
            return "";
        }
    }
    catch(bad_alloc& ba){
        cout<<"Out of Memory";
        return "";
    }

    actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
    return puzzle->getPath();
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm: Depth First Search until ultimate max depth.
//
//
// Move Generator:  Pick first element of Q, if its depth is equal to maxDepth, pick the next element; Add path extensions to the bottom of the Q; If Q is empty increase maxDepth; If maxDepth equals ultimateDepth exit.
//
////////////////////////////////////////////////////////////////////////////////////////////
string progressiveDeepeningSearch_No_VisitedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, float &actualRunningTime, int ultimateMaxDepth){
    string path;
    clock_t startTime= clock();
    Puzzle *puzzle;
    auto *inQ = new stack<Puzzle>();
    Puzzle *temp;
    numOfStateExpansions=0;
    maxQLength=0;
    int maxDepth = 1;
    bool found=false;
    actualRunningTime=0.0;
    try {
        while(maxDepth<ultimateMaxDepth&&!found) {
            puzzle = new Puzzle(initialState, goalState);
            inQ->push(*puzzle);
            while (!inQ->empty()) {
                actualRunningTime=((float)(clock() - startTime)/CLOCKS_PER_SEC);
                /*if(actualRunningTime>1200){
                    cout << "\n\n*---- Search Timed Out. (Searched For 1 Hour) ----*" << endl;
                    return "";
                }*/
                puzzle = new Puzzle(inQ->top());
                inQ->pop();
                if (puzzle->getDepth() > maxDepth) {
                    puzzle = new Puzzle(inQ->top());
                    inQ->pop();

                }
                if (puzzle->goalMatch()) {
                    found=true;
                    break;
                }
                numOfStateExpansions++;
                if (puzzle->canMoveLeft(maxDepth)) {
                    temp = puzzle->moveLeft();
                    if (temp->isNewState()) {
                        inQ->push(*temp);
                    }
                    delete temp;
                }
                if (puzzle->canMoveDown(maxDepth)) {
                    temp = puzzle->moveDown();
                    if (temp->isNewState()) {
                        inQ->push(*temp);
                    }
                    delete temp;
                }
                if (puzzle->canMoveRight(maxDepth)) {
                    temp = puzzle->moveRight();
                    if (temp->isNewState()) {
                        inQ->push(*temp);
                    }
                    delete temp;
                }
                if (puzzle->canMoveUp(maxDepth)) {
                    temp = puzzle->moveUp();
                    if (temp->isNewState()) {
                        inQ->push(*temp);
                    }
                    delete temp;
                }


                if (inQ->size() > maxQLength) {
                    maxQLength = inQ->size();
                }
                    delete puzzle;
            }
            maxDepth++;
        }
    }
    catch(bad_alloc& ba){
        cout<<"Out of Memory";
        return "";
    }
    if(inQ->empty()){
        return "";
    }
    actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
    return puzzle->getPath();
}
	



///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm: Uniform Cost Expanded List
//
// Move Generator:  Pick first element of Q; Add path extensions to the correct position on Q in reguard to FCost(Without Hueristic).
//
////////////////////////////////////////////////////////////////////////////////////////////
    string uniformCost_ExpandedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength,
                               float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions){

    //Firstly start with normal search and add puzzles to a vector
    //Expand only the puzzle with the shortest path
    //ENDIF Q empty, OOM, Goal State
   string path;
   clock_t startTime= clock();;
   actualRunningTime=0.0;
   numOfDeletionsFromMiddleOfHeap=0;
   numOfLocalLoopsAvoided=0;
   numOfAttemptedNodeReExpansions=0;


    auto *inQ = new priority_queue<Puzzle>();

    Puzzle *puzzle = new Puzzle(initialState, goalState);
    puzzle->updateFCost();
    inQ->push(*puzzle);
    Puzzle *temp;
    numOfStateExpansions=0;
    maxQLength=0;
    auto *expandedList = new vector<string>();

    try {
        while (!inQ->empty()) {
            actualRunningTime=((float)(clock() - startTime)/CLOCKS_PER_SEC);
            /*if(actualRunningTime>1200){
                cout << "\n\n*---- Search Timed Out. (Searched For 1 Hour) ----*" << endl;
                return "";
            }*/
            puzzle = new Puzzle(inQ->top());
            expandedList->push_back(puzzle->toString());
            if (puzzle->goalMatch()) {
                break;

            }
            numOfStateExpansions++;
            if (puzzle->canMoveUp()) {
                temp = puzzle->moveUp();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveRight()) {
                temp = puzzle->moveRight();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveDown()) {
                temp = puzzle->moveDown();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveLeft()) {
                temp = puzzle->moveLeft();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (inQ->size() > maxQLength) {
                maxQLength = inQ->size();
            }
            inQ->pop();
            delete puzzle;
        }
    }
    catch(bad_alloc& ba){
        cout<<"Out of Memory";
        return "";
    }

    if(inQ->empty()){
        return "";
    }

	actualRunningTime=0.0;
	actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);
	             
	return puzzle->getPath();
		
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Search Algorithm:  
//
// Move Generator:  Pick first element of Q; Add path extensions to the correct position on Q in reguard to FCost(With Hueristic).
//
////////////////////////////////////////////////////////////////////////////////////////////
string aStar_ExpandedList(string const initialState, string const goalState, int &numOfStateExpansions, int& maxQLength, 
                               float &actualRunningTime, int &numOfDeletionsFromMiddleOfHeap, int &numOfLocalLoopsAvoided, int &numOfAttemptedNodeReExpansions, heuristicFunction heuristic){
											 
   string path;
   clock_t startTime= clock();
   actualRunningTime=0.0;
   numOfDeletionsFromMiddleOfHeap=0;
   numOfLocalLoopsAvoided=0;
   numOfAttemptedNodeReExpansions=0;

    numOfDeletionsFromMiddleOfHeap=0;
    numOfLocalLoopsAvoided=0;
    numOfAttemptedNodeReExpansions=0;


    auto *inQ = new priority_queue<Puzzle>();

    Puzzle *puzzle = new Puzzle(initialState, goalState);
    puzzle->updateHCost(heuristic);
    puzzle->updateFCost();
    inQ->push(*puzzle);
    Puzzle *temp;
    numOfStateExpansions=0;
    maxQLength=0;
    auto *expandedList = new vector<string>();


    try {
        while (!inQ->empty()) {
            puzzle = new Puzzle(inQ->top());
            expandedList->push_back(puzzle->toString());
            if (puzzle->goalMatch()) {
                break;

            }/*
            actualRunningTime=((float)(clock() - startTime)/CLOCKS_PER_SEC);
            if(actualRunningTime>1200){
                cout << "\n\n*---- Search Timed Out. (Searched For 1 Hour) ----*" << endl;
                return "";
            }*/
            numOfStateExpansions++;
            if (puzzle->canMoveUp()) {
                temp = puzzle->moveUp();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateHCost(heuristic);
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveRight()) {
                temp = puzzle->moveRight();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateHCost(heuristic);
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveDown()) {
                temp = puzzle->moveDown();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateHCost(heuristic);
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (puzzle->canMoveLeft()) {
                temp = puzzle->moveLeft();
                if(!contains(*expandedList,temp->toString())) {
                    temp->updateHCost(heuristic);
                    temp->updateFCost();
                    inQ->push(*temp);
                    delete temp;
                } else {
                    delete temp;
                }
            }
            if (inQ->size() > maxQLength) {
                maxQLength = inQ->size();
            }
            inQ->pop();
            delete puzzle;
        }
    }
    catch(bad_alloc& ba){
        cout<<"Out of Memory";
        return "";
    }

    if(inQ->empty()){
        return "";
    }


//***********************************************************************************************************
    actualRunningTime = ((float)(clock() - startTime)/CLOCKS_PER_SEC);

    return puzzle->getPath();

}
