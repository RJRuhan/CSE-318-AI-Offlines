#include<iostream>
#include<vector>
#include<map>
#include<cmath>
#include<queue>
#include<stack>
#include<fstream>

using namespace std;

typedef unsigned long long ull;




class Node{

    static ull nextID;

public:

    ull id = -1;
    vector<int> board;
    ull moves = 0;
    Node* prevNode = nullptr;
    pair<int,int> blankBlockPos = pair<int,int>(-1,-1);
    int boardDim;
    ull cost = 0;
    ull hamDist = 0;
    ull manDist = 0;
    ull Dist = 0;



    Node(vector<int>& board,int boardDim,ull moves=0,Node* prevNode=nullptr){

        this->board = board;
        this->boardDim = boardDim;
        this->moves = moves;
        this->prevNode = prevNode;
        setBlankBlockPos();
        this->id = ++nextID;
    }

    void setBlankBlockPos(){
        int boardSize = board.size();
        for(int i = 0;i < boardSize;i++){
            if(board[i]==0){
                blankBlockPos.first = floor(i/boardDim);
                blankBlockPos.second = i % boardDim;
                break;
            }
        }
    }

    void setHammingDistance(vector<int>& goal){
        int boardSize = board.size();
        ull hamDist = 0;
        for( int i = 0;i < boardSize;i++ ) {
            if(board[i] != 0){
                hamDist += (board[i]!=goal[i])?1:0;
            }
        }

        cost = hamDist + moves;
        this->hamDist = hamDist;
        Dist = hamDist;
    }

    void setManhattanDistance(vector<int>& goal){
        int boardSize = board.size();
        ull manDist = 0;

        map<int,pair<int,int>> goalBlockPositions;
        for( int i = 0;i < boardSize;i++ ) {
            goalBlockPositions[goal[i]] = pair<int,int>(floor(i/boardDim),i%boardDim);
            //cout << goalBlockPositions[goal[i]].first << " " << goalBlockPositions[goal[i]].second << endl;
        }
        for( int i = 0;i < boardSize;i++ ) {

            if(board[i] != 0){
                int rowDist = abs( goalBlockPositions[board[i]].first - floor(i/boardDim) );
                int colDist = abs( goalBlockPositions[board[i]].second - i%boardDim );
                //cout << rowDist << " " << colDist << endl;
                manDist += rowDist+colDist;
            }
        }

        cost = manDist + moves;
        this->manDist = manDist;
        Dist = manDist;
    }

    void setDistance(vector<int>& goal,int choice){
        if(choice == 1)
            setHammingDistance(goal);
        else if( choice == 2 )
            setManhattanDistance(goal);
    }
};

void printBoard(const vector<int>& board,int boardDim){

    for( int i = 0;i < boardDim;i++){
        for(int j = 0;j < boardDim;j++){
            cout << board[i*boardDim+j] << " ";
        }
        cout << endl;
    }

}

struct Comp{
    bool operator()(const Node* a, const Node* b){
        return a->cost>b->cost;
    }
};



void solvePuzzle(vector<int>& initBoard,int boardDim,vector<int>& goal,int choice){
    map<vector<int>,Node*> exploredNodes;

    priority_queue<Node*,vector<Node*>,Comp> Queue;

    Node* root = new Node(initBoard,boardDim);
    root->setDistance(goal,choice);


    Queue.push(root);
    Node* target = nullptr;
    int cnt = 0;

    while(!Queue.empty()){
        Node* node = Queue.top();
        Queue.pop();

        const vector<int>& board = node->board;
        exploredNodes[board] = node;


        if( node->Dist == 0){
            target = node;
            break;
        }

        //printBoard(node->board,boardDim);
        //cout << "node Dist: " << node->Dist << endl;
        //cout << "---id: " << node->id << endl;

        int blankRow = node->blankBlockPos.first;
        int blankCol = node->blankBlockPos.second;
        //cout << blankRow << " " << blankCol << endl;

        int temp = blankRow*boardDim + blankCol;

        if( blankRow - 1 >= 0 ){
            vector<int> newBoard = board;
            int t = (blankRow-1)*boardDim + blankCol;
            newBoard[temp] = newBoard[t];
            newBoard[t] = 0;

            if( exploredNodes.find(newBoard) == exploredNodes.end() ){
                Node* newNode = new Node(newBoard,boardDim,node->moves+1,node);
                newNode->setDistance(goal,choice);
                Queue.push(newNode);
                exploredNodes[newNode->board] = newNode;
            }else{
                //cout << "Already explored" << endl;
            }

        }
        if( blankRow + 1 < boardDim ){
            vector<int> newBoard = board;
            int t = (blankRow+1)*boardDim + blankCol;
            newBoard[temp] = newBoard[t];
            newBoard[t] = 0;

            if( exploredNodes.find(newBoard) == exploredNodes.end() ){
                Node* newNode = new Node(newBoard,boardDim,node->moves+1,node);
                newNode->setDistance(goal,choice);
                Queue.push(newNode);
                exploredNodes[newNode->board] = newNode;
            }else{
                //cout << "Already explored" << endl;
            }

        }
        if( blankCol - 1 >= 0 ){
            vector<int> newBoard = board;
            int t = blankRow*boardDim + blankCol-1;
            newBoard[temp] = newBoard[t];
            newBoard[t] = 0;

            if( exploredNodes.find(newBoard) == exploredNodes.end() ){
                Node* newNode = new Node(newBoard,boardDim,node->moves+1,node);
                newNode->setDistance(goal,choice);
                Queue.push(newNode);
                exploredNodes[newNode->board] = newNode;

            }else{
                //cout << "Already explored" << endl;
            }

        }
        if( blankCol + 1 < boardDim ){
            vector<int> newBoard = board;
            int t = blankRow*boardDim + blankCol+1;
            newBoard[temp] = newBoard[t];
            newBoard[t] = 0;

            if( exploredNodes.find(newBoard) == exploredNodes.end() ){
                Node* newNode = new Node(newBoard,boardDim,node->moves+1,node);
                newNode->setDistance(goal,choice);
                Queue.push(newNode);
                exploredNodes[newNode->board] = newNode;

            }else{
                //cout << "Already explored" << endl;
            }

        }

        //cout << exploredNodes.size() << endl;
    }

    if(choice == 1){
        cout << "Using HammingDistance:" << endl;
    }else if( choice == 2 ){
        cout << "Using ManhattanDistance:" << endl;
    }


    cout << "Minimum number of moves = " << target->moves << endl;
    stack<Node*> nodes;
    while(target != nullptr){
        nodes.push(target);
        target = target->prevNode;
    }

    while(!nodes.empty()){
        cout << "-------x-------" << endl;
        printBoard(nodes.top()->board,boardDim);
        nodes.pop();
    }

    cout << "-------x-------" << endl;

    cout << "Number of Visited Nodes = " << exploredNodes.size() << endl;

    cout << "-----------********----------" << endl;

    /*while(!Queue.empty()){
        Node* temp = Queue.top();
        Queue.pop();
        delete temp;
    }*/

    for (auto i = exploredNodes.begin(); i != exploredNodes.end(); i++)
        delete i->second;

}

bool isSolvable(const vector<int>& board,int boardDim){

    if( boardDim%2 == 1 ){
        int boardSize = boardDim*boardDim;
        ull inversionCount = 0;
        for(int i = 0;i < boardSize;i++){
            if(board[i]==0)
                continue;
            for( int j = i+1;j<boardSize;j++ ){
                if( board[j]!=0 && board[i]>board[j])
                    inversionCount++;
            }
        }

        if( inversionCount%2 == 1 ){
            cout << "Unsolvable puzzle" << endl;
            return false;
        }
    }else{
        int boardSize = boardDim*boardDim;
        ull inversionCount = 0;
        int blankRow = -1;
        for(int i = 0;i < boardSize;i++){
            if(board[i]==0){
                blankRow = floor(i/boardDim);
                continue;
            }
            for( int j = i+1;j<boardSize;j++ ){
                if(board[j]!=0 && board[i]>board[j])
                    inversionCount++;
            }
        }

        blankRow = boardDim - blankRow;
        if( (blankRow%2 == 0 && inversionCount%2 != 1) || (blankRow%2 == 1 && inversionCount%2 != 0) ){
            cout << "Unsolvable puzzle" << endl;
            return false;
        }
    }

    return true;

}

ull Node::nextID;


int main(){

    //ifstream in("in13.txt");
    //ofstream out("output.txt");

    //auto cinbuf = cin.rdbuf();
    //cin.rdbuf(in.rdbuf());

    //auto coutbuf = cout.rdbuf();
    //cout.rdbuf(out.rdbuf());

    //int T;
    //cin >> T;

    //while(T--){
        vector<int> board;
        int k = -1;
        vector<int> goal;

        cin >> k;
        int boardSize = k*k;
        int inp;
        for( int i = 0;i < boardSize;i++ ){
            cin >> inp;
            board.push_back(inp);
        }


        for(int i = 0;i < boardSize-1;i++){
            goal.push_back(i+1);
        }


        goal.push_back(0);


        if( isSolvable(board,k) ){
            solvePuzzle(board,k,goal,1);
            solvePuzzle(board,k,goal,2);

        }

    //}



    return 0;
}
