#include <bits/stdc++.h>

using namespace std;

long long convertVectortoLL(const vector<char> &a)
{
    long long res = 0;
    for (char i : a)
    {
        res = res * 10 + i;
    }
    return res;
}

unordered_map<long long, double> boardScoreMap;
unordered_map<long long, bool> exploredBoards;
typedef pair<char, double> illPair;

class Board
{
public:
    vector<char> board = {4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0};

    char NUMBEROFBINSEACHSIDE = 6;
    char additonalMovesEarned1 = 0;
    char additonalMovesEarned2 = 0;
    char stonesCaptured1 = 0;
    char stonesCaptured2 = 0;

    bool validMove(int bin, int player) const
    {
        bin--;

        if (player == 2)
            bin += NUMBEROFBINSEACHSIDE + 1;

        return board[bin] != 0;
    }

    bool makeMove(int bin, int player)
    {
        // cout << "\nBefore Move : \n";
        // print();
        // cout << "\nPlayer : " << player << "  Move : " << bin << "\n";
        // newBoard.print();

        bin--;
        int myStorage = -1, yourStorage = -1;
        int totalBins = NUMBEROFBINSEACHSIDE * 2 + 2;

        if (player == 1)
        {
            myStorage = NUMBEROFBINSEACHSIDE;
            yourStorage = NUMBEROFBINSEACHSIDE * 2 + 2 - 1;
        }
        else if (player == 2)
        {
            bin += NUMBEROFBINSEACHSIDE + 1;
            myStorage = NUMBEROFBINSEACHSIDE * 2 + 2 - 1;
            yourStorage = NUMBEROFBINSEACHSIDE;
        }

        int stonesInBin = board[bin];
        board[bin] = 0;
        int nextBin = (bin + 1);

        bool gainNextMove = false;

        while (stonesInBin != 0)
        {
            nextBin %= totalBins;
            if (nextBin == yourStorage)
            {
                nextBin++;
                continue;
            }

            if (stonesInBin == 1)
            {
                if (nextBin == myStorage)
                {
                    gainNextMove = true;
                    if (player == 1)
                        additonalMovesEarned1++;
                    else if (player == 2)
                        additonalMovesEarned2++;
                }
                else if ((player == 1 && nextBin >= 0 && nextBin <= 5) || (player == 2 && nextBin >= 7 && nextBin <= 12))
                {
                    if (board[nextBin] == 0)
                    {
                        int oppositeBin = -1;

                        if (player == 1)
                            oppositeBin = NUMBEROFBINSEACHSIDE + 1 + (NUMBEROFBINSEACHSIDE - 1 - nextBin);
                        else if (player == 2)
                            oppositeBin = (NUMBEROFBINSEACHSIDE * 2 - nextBin);

                        if (board[oppositeBin] != 0)
                        {
                            board[myStorage] += board[oppositeBin] + 1;

                            if (player == 1)
                                stonesCaptured1 += board[oppositeBin];
                            else if (player == 2)
                                stonesCaptured2 += board[oppositeBin];

                            board[oppositeBin] = 0;

                            stonesInBin--;
                            nextBin++;

                            continue;
                        }
                    }
                }
            }

            board[nextBin]++;
            stonesInBin--;
            nextBin++;
        }

        if (player == 1)
        {
            int totalStones1 = totalStonesOnEachSide(1);
            int totalStones2 = totalStonesOnEachSide(2);

            // cout << totalStones1 << " " << totalStones2 << endl;

            if (totalStones2 == 0)
            {
                // cout << "\n------it happened-----\n";
                board[myStorage] += totalStones1;
                setEachBin(0, 1);
            }
            else if (totalStones1 == 0)
            {
                board[yourStorage] += totalStones2;
                setEachBin(0, 2);
            }
        }
        else if (player == 2)
        {
            int totalStones1 = totalStonesOnEachSide(1);
            int totalStones2 = totalStonesOnEachSide(2);

            if (totalStones1 == 0)
            {
                // cout << "\n------it happened-----\n";
                board[myStorage] += totalStones2;
                setEachBin(0, 2);
            }

            else if (totalStones2 == 0)
            {
                board[yourStorage] += totalStones1;
                setEachBin(0, 1);
            }
        }

        // print();

        return gainNextMove;
    }

    int totalStonesOnEachSide(int side) const
    {
        int totalStones = 0;
        int i = 0;
        if (side == 2)
            i = NUMBEROFBINSEACHSIDE + 1;
        for (int cnt = 0; cnt < NUMBEROFBINSEACHSIDE; cnt++)
            totalStones += board[i++];

        return totalStones;
    }

    void setEachBin(int val, int side)
    {
        int i = 0;
        if (side == 2)
            i = NUMBEROFBINSEACHSIDE + 1;

        for (int cnt = 0; cnt < NUMBEROFBINSEACHSIDE; cnt++)
            board[i++] = val;
    }

    bool isTerminal() const
    {
        int i = 0;
        int totalStones = totalStonesOnEachSide(1) + totalStonesOnEachSide(2);
        return totalStones == 0;
    }

    void print()
    {

        cout << "\n----------------****-----------------\n";
        cout << "  |    | ";

        int numberOfBins = NUMBEROFBINSEACHSIDE;
        int i = 2 * NUMBEROFBINSEACHSIDE;
        while (numberOfBins--)
        {
            cout << right << setw(2) << (short)board[i--] << " | ";
        }

        cout << "   |\n";
        cout << "  | " << right << setw(2) << (short)board[NUMBEROFBINSEACHSIDE * 2 + 1] << " |";

        numberOfBins = NUMBEROFBINSEACHSIDE;
        while (numberOfBins--)
        {
            if (numberOfBins == 0)
                cout << "----";
            else
                cout << "-----";
        }
        cout << "| " << right << setw(2) << (short)board[NUMBEROFBINSEACHSIDE] << " |\n";

        cout << "  |    | ";

        numberOfBins = NUMBEROFBINSEACHSIDE;
        i = 0;
        while (numberOfBins--)
        {
            cout << right << setw(2) << (short)board[i++] << " | ";
        }

        cout << "   |\n";
        cout << "\n----------------****-----------------\n";
    }

    vector<char> getPossibleMoves(int player) const
    {
        vector<char> possibleMoves;

        for (int i = 1; i <= NUMBEROFBINSEACHSIDE; i++)
        {
            if (validMove(i, player))
                possibleMoves.push_back(i);
        }

        return possibleMoves;
    }
};

typedef pair<Board, bool> boardBoolPair;

int evalHelper1(const Board &board)
{

    int myStorage = board.NUMBEROFBINSEACHSIDE;
    int yourStorage = board.NUMBEROFBINSEACHSIDE * 2 + 2 - 1;

    int score = board.board[myStorage] - board.board[yourStorage];

    return score;
}

int evalHelper2(const Board &board)
{

    int myStones = board.totalStonesOnEachSide(1);
    int yourStones = board.totalStonesOnEachSide(2);

    int score = myStones - yourStones;

    return score;
}

double heuristic1(const Board &board)
{
    // cout << "h1" << endl;
    return evalHelper1(board);
}

void setRandomValue_h1()
{
}

double w1_h2 = 1, w2_h2 = -.9;

double heuristic2(const Board &board)
{
    // cout << "h2" << endl;
    double score = w1_h2 * evalHelper1(board) + w2_h2 * evalHelper2(board);
    return score;
}

void setRandomValue_h2()
{
    w1_h2 = (double)rand() / RAND_MAX;
    w2_h2 = (double)rand() / RAND_MAX;

    cout << "new weights :  " << w1_h2 << " " << w2_h2 << "\n";
}

double w1_h3 = 1, w2_h3 = -1, w3_h3 = 1.5;
double heuristic3(const Board &board)
{

    int myStones = board.totalStonesOnEachSide(1);
    int yourStones = board.totalStonesOnEachSide(2);

    double score = w1_h3 * evalHelper1(board) + w2_h3 * evalHelper2(board) + w3_h3 * (board.additonalMovesEarned1 - board.additonalMovesEarned2);

    return score;
}

void setRandomValue_h3()
{
    w1_h3 = (double)rand() / RAND_MAX;
    w2_h3 = (double)rand() / RAND_MAX;
    w3_h3 = (double)rand() / RAND_MAX;

    cout << "new weights :  " << w1_h3 << " " << w2_h3 << " " << w3_h3 << "\n";
}

double w1_h4 = 1.5, w2_h4 = -1.3, w3_h4 = 1.3, w4_h4 = .3;
double heuristic4(const Board &board)
{

    int myStones = board.totalStonesOnEachSide(1);
    int yourStones = board.totalStonesOnEachSide(2);

    double score = w1_h4 * evalHelper1(board) + w2_h4 * evalHelper2(board) + w3_h4 * board.additonalMovesEarned1 + w4_h4 * (board.stonesCaptured1 - board.stonesCaptured2);

    return score;
}

void setRandomValue_h4()
{
    w1_h4 = (double)rand() / RAND_MAX;
    w2_h4 = (double)rand() / RAND_MAX;
    w3_h4 = (double)rand() / RAND_MAX;
    w4_h4 = (double)rand() / RAND_MAX;

    cout << "new weights :  " << w1_h4 << " " << w2_h4 << " " << w3_h4 << " " << w4_h4 << "\n";
}

char changePlayer(char currPlayer)
{
    return (currPlayer == 1) ? 2 : 1;
}

struct Info
{
    Board board;
    bool isSamePlayer;
    double score;
    char move;
};

struct CompMax
{
    bool operator()(const Info &a, const Info &b)
    {
        // if( a.score == b.score )
        //     return b.move < a.move;
        // else
        return b.score > a.score;
    }
};

struct CompMin
{
    bool operator()(const Info &a, const Info &b)
    {
        // if( a.score == b.score )
        //     return b.move < a.move;
        // else
        return b.score < a.score;
    }
};

illPair minimaxAlphaBetaPruning(const Board &board, char depth, double alpha, double beta, bool isMaximizer, double (*evaluate)(const Board &))
{

    char player = isMaximizer ? 1 : 2;

    if (board.isTerminal() || depth == 0)
    {
        double score = evaluate(board);
        vector<char> tempBoard = board.board;
        tempBoard.push_back(1);
        boardScoreMap[convertVectortoLL(tempBoard)] = score;
        tempBoard.pop_back();
        tempBoard.push_back(2);
        boardScoreMap[convertVectortoLL(tempBoard)] = score;
        // cout << "--- " << score << endl;
        return illPair(-1, score);
    }

    if (isMaximizer)
    {
        double maxScore = -numeric_limits<double>::max();
        vector<char> possibleMoves = board.getPossibleMoves(player);
        priority_queue<Info, vector<Info>, CompMax> pq;
        char bestMove = -1;

        // cout << "------*******-------\n";
        for (char move : possibleMoves)
        {
            Info info;
            info.board = board;
            char nextPlayer = player;
            info.isSamePlayer = info.board.makeMove(move, player);
            if (!info.isSamePlayer)
                nextPlayer = changePlayer(player);

            info.move = move;

            vector<char> tempBoard = info.board.board;
            tempBoard.push_back(nextPlayer);
            long long key = convertVectortoLL(tempBoard);

            exploredBoards[key] = true;

            if (boardScoreMap.find(key) == boardScoreMap.end())
            {
                // cout << "------it happened------\n";
                double score = -numeric_limits<double>::max();
                info.score = score;
                pq.push(info);
                // cout << score << endl;
            }
            else
            {
                double score = boardScoreMap[key];
                info.score = score;
                pq.push(info);
                // cout << score << endl;
            }
        }
        // cout << "------*******-------\n";

        // cout << "------*******-------\n";

        while (!pq.empty())
        {
            Info info = pq.top();
            pq.pop();
            // cout << "----geg---- " << info.score << endl;
            Board newBoard = info.board;
            bool isSamePlayer = info.isSamePlayer;
            // cout << "Depth : " << depth << endl;
            // cout << "----geg---- " << (short)info.move << "  " << info.score << endl;
            illPair res = minimaxAlphaBetaPruning(newBoard, depth - 1, alpha, beta, isSamePlayer, evaluate);
            double score = res.second;
            if (score > maxScore)
            {
                maxScore = score;
                bestMove = info.move;
            }
            // else if( score == maxScore && info.move < bestMove ){
            //     bestMove = info.move;
            // }
            alpha = max(alpha, score);
            // cout << "alpha " << alpha << " beta " << beta << endl;
            if (alpha >= beta)
            {
                // cout << "------pruning------\n";
                break;
            }
        }
        // cout << "------*******-------\n";
        // cout << "max " << maxScore << endl;

        vector<char> tempBoard = board.board;
        tempBoard.push_back(player);

        boardScoreMap[convertVectortoLL(tempBoard)] = maxScore;
        return illPair(bestMove, maxScore);
    }
    else
    {

        double minScore = numeric_limits<double>::max();
        char bestMove = -1;
        vector<char> possibleMoves = board.getPossibleMoves(player);
        priority_queue<Info, vector<Info>, CompMin> pq;

        for (char move : possibleMoves)
        {
            Info info;
            info.board = board;
            char nextPlayer = player;
            info.isSamePlayer = info.board.makeMove(move, player);

            if (!info.isSamePlayer)
                nextPlayer = changePlayer(player);

            info.move = move;

            vector<char> tempBoard = info.board.board;
            tempBoard.push_back(nextPlayer);

            long long key = convertVectortoLL(tempBoard);

            exploredBoards[key] = true;

            if (boardScoreMap.find(key) == boardScoreMap.end())
            {
                // cout << "------it happened------\n";
                double score = numeric_limits<double>::max();
                info.score = score;
                pq.push(info);
                // cout << score << endl;
            }
            else
            {
                double score = boardScoreMap[key];
                info.score = score;
                pq.push(info);
                // cout << score << endl;
            }
        }
        // cout << "------*******-------\n";

        while (!pq.empty())
        {
            Info info = pq.top();
            pq.pop();
            // cout << "Depth : " << depth << endl;

            // cout << "----geg---- " << info.move << "  " << info.score << endl;
            Board newBoard = info.board;
            bool isSamePlayer = info.isSamePlayer;
            illPair res = minimaxAlphaBetaPruning(newBoard, depth - 1, alpha, beta, !isSamePlayer, evaluate);
            double score = res.second;
            if (score < minScore)
            {
                minScore = score;
                bestMove = info.move;
            }
            // else if( score == minScore && info.move < bestMove ){
            //     bestMove = info.move;
            // }
            beta = min(beta, score);
            // cout << "alpha " << alpha << " beta " << beta << endl;

            if (alpha >= beta)
            {
                // cout << "------pruning------\n";
                break;
            }
        }

        // cout << "------*******-------\n";
        // cout << "min " << minScore << endl;

        vector<char> tempBoard = board.board;
        tempBoard.push_back(player);

        boardScoreMap[convertVectortoLL(tempBoard)] = minScore;
        return illPair(bestMove, minScore);
    }
}

int findBestMove(const Board &board, char maxDepth, char player, double (*heuristic)(const Board &))
{

    bool isMaximizer = false;
    if (player == 1)
        isMaximizer = true;

    char bestMove = -1;

    pair<int, double> res;

    vector<char> tempBoard = board.board;
    tempBoard.push_back(player);
    exploredBoards[convertVectortoLL(tempBoard)] = true;

    for (int depth = 1; depth <= maxDepth; ++depth)
    {
        res = minimaxAlphaBetaPruning(board, depth, -std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), isMaximizer, heuristic);
    }

    // cout << "Best Move : " << res.first << " Best Score : " << res.second << endl;
    bestMove = res.first;

    return bestMove;
}

char playMancala(int firstPlayer, int maxDepth, double (*heur1)(const Board &), double (*heur2)(const Board &))
{
    Board board;
    char currPlayer = firstPlayer;
    int MoveNo = 1;
    long long totalExploredBoards = 0;
    while (!board.isTerminal())
    {
        auto start = chrono::high_resolution_clock::now();
        board.print();

        // cout << (short)board.additonalMovesEarned1 << " " << (short)board.additonalMovesEarned2 << endl;
        // cout << (short)board.stonesCaptured1 << " " << (short)board.stonesCaptured2 << endl;
        double (*heuristic)(const Board &);
        if (currPlayer == 1)
            heuristic = heur1;
        else if (currPlayer == 2)
            heuristic = heur2;

        boardScoreMap.clear();
        exploredBoards.clear();

        int move = findBestMove(board, maxDepth, currPlayer, heuristic);
        cout << "\n"
             << MoveNo << ". Player : " << (short)currPlayer << " Move : " << move << "\n";
        bool isSamePlayer = board.makeMove(move, currPlayer);

        if (!isSamePlayer)
            currPlayer = changePlayer(currPlayer);

        MoveNo++;
        totalExploredBoards += exploredBoards.size();

        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        cout << "\nTime taken for each move is : " << fixed
             << time_taken << setprecision(9);
        cout << " sec" << endl;
    }

    exploredBoards.clear();
    boardScoreMap.clear();

    board.print();

    cout << "\nTotal Number of explored boards : " << totalExploredBoards << "\n\n";

    double score = heuristic1(board);
    if (score == 0)
        return 0;
    else if (score > 0)
        return 1;
    else
        return -1;
}

pair<char, char> playTournament1(const int games, int maxDepth, double (*heuristic1)(const Board &), double (*heuristic2)(const Board &), void (*setRandomValue1)(), void (*setRandomValue2)(), bool setRandom)
{

    char win1 = 0, win2 = 0;

    for (int i = 1; i <= games / 2; i++)
    {
        if (setRandom)
        {
            setRandomValue1();
            setRandomValue2();
        }

        auto start = chrono::high_resolution_clock::now();
        char win = playMancala(1, maxDepth, heuristic1, heuristic2);

        cout << "----------------*********----------------\n\n";
        if (win == 0)
        {
            cout << "It's a draw!\n";
        }
        else if (win == 1)
        {
            cout << "Player 1 wins\n";
            win1++;
        }
        else
        {
            cout << "Player 2 wins\n";
            win2++;
        }

        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        cout << "\nTime taken by a game is : " << fixed
             << time_taken << setprecision(9);
        cout << " sec"
             << "\n";

        cout << "----------------*********----------------\n\n";

        start = chrono::high_resolution_clock::now();
        win = playMancala(1, maxDepth, heuristic2, heuristic1);

        cout << "----------------*********----------------\n\n";
        if (win == 0)
        {
            cout << "It's a draw!\n";
        }
        else if (win == 1)
        {
            cout << "Player 1 wins\n";
            win2++;
        }
        else
        {
            cout << "Player 2 wins\n";
            win1++;
        }

        end = chrono::high_resolution_clock::now();
        time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        cout << "\nTime taken by a game is : " << fixed
             << time_taken << setprecision(9);
        cout << " sec"
             << "\n\n";

        cout << "----------------*********----------------\n\n";
    }

    return pair<char, char>(win1, win2);
}

pair<char, char> playTournament2(const int games, int maxDepth, double (*heuristic1)(const Board &), double (*heuristic2)(const Board &))
{

    char win1 = 0, win2 = 0;

    for (int i = 1; i <= games; i++)
    {
        int randomDepth = (rand() % maxDepth) + 1;
        cout << "Random Depth : " << randomDepth << "\n";

        auto start = chrono::high_resolution_clock::now();
        char win = playMancala(1, randomDepth, heuristic1, heuristic2);

        cout << "----------------*********----------------\n\n";
        if (win == 0)
        {
            cout << "It's a draw!\n";
        }
        else if (win == 1)
        {
            cout << "Player 1 wins\n";
            win1++;
        }
        else
        {
            cout << "Player 2 wins\n";
            win2++;
        }

        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        cout << "\nTime taken by a game is : " << fixed
             << time_taken << setprecision(9);
        cout << " sec"
             << "\n";

        cout << "----------------*********----------------\n\n";
    }

    return pair<char, char>(win1, win2);
}

void play1()
{
    int maxDepth, games, rand;
    cout << "Please Enter maximum depth : ";
    cin >> maxDepth;
    cout << "Please Enter number of games per tournament : ";
    cin >> games;
    cout << "Set Random Values : 1.Yes 2.No\n";
    cin >> rand;

    bool setRand = (rand == 1) ? true : false;

    // ifstream in("in07.txt");
    ofstream out("output.txt");

    // auto cinbuf = cin.rdbuf();
    // cin.rdbuf(in.rdbuf());

    auto coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    // srand(time(0));
    cout << "heuristic-1: (stones_in_my_storage - stones_in_opponents_storage)\n";
    cout << "heuristic-2: W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_on_my_side - stones_on_opponents_side)\n";
    cout << "heuristic-3: W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_on_my_side - stones_on_opponents_side) + W3 * (additional_move_earned)\n";
    cout << "heuristic-4: W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_on_my_side - stones_on_opponents_side) + W3 * (additional_move_earned) + W4 * (stones_captured)\n";

    double (*heuristic[4])(const Board &) = {heuristic1, heuristic2, heuristic3, heuristic4};
    void (*setRandomValueFunctions[4])() = {setRandomValue_h1, setRandomValue_h2, setRandomValue_h3, setRandomValue_h4};

    char tournamentWins[4] = {0, 0, 0, 0};
    char tournamentDraws[4] = {0, 0, 0, 0};

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < 3; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            cout << "\n----------------*********----------------\n";
            cout << "\nTournament between heuristic " << i + 1 << " and "
                 << "heuristic " << j + 1 << " Begins : \n\n";
            pair<char, char> win = playTournament1(games, maxDepth, heuristic[i], heuristic[j], setRandomValueFunctions[i], setRandomValueFunctions[j], setRand);
            cout << "\nTournament Result : \n";
            cout << "heuristic " << i + 1 << " wins : " << (short)win.first << "\n";
            cout << "heuristic " << j + 1 << " wins : " << (short)win.second << "\n";
            cout << "draws : " << (short)(games - win.first - win.second) << "\n";
            if (win.first == win.second)
            {
                tournamentDraws[i]++;
                tournamentDraws[j]++;
            }
            else if (win.first > win.second)
            {
                tournamentWins[i]++;
            }
            else if (win.first < win.second)
            {
                tournamentWins[j]++;
            }
        }
    }

    cout << "\n----------------*******************-----------------\n\n";
    cout << "Overall Tournament Result : \n\n";

    const int totalTourPerHeuristic = 3;

    for (int i = 0; i < 4; i++)
    {
        cout << "heuristic " << i + 1 << " :   wins : " << (short)tournamentWins[i] << " draws : " << (short)tournamentDraws[i] << " losses : " << totalTourPerHeuristic - (short)tournamentWins[i] - (short)tournamentDraws[i] << "\n";
    }

    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    time_taken *= 1e-9;

    cout << "\nTime taken by the program is : " << fixed
         << time_taken << setprecision(9);
    cout << " sec"
         << "\n\n\n";

    cout << "for each game random weights were chosen for each heuristic\n";
    cout << "games per Tournament : " << games << "\n";

    cout.rdbuf(coutbuf);
    out.close();
}

void play2()
{
    int maxDepth, games;
    cout << "Please Enter maximum depth : ";
    cin >> maxDepth;
    cout << "Please Enter number of games per tournament : ";
    cin >> games;

    // ifstream in("in07.txt");
    ofstream out("output.txt");

    // auto cinbuf = cin.rdbuf();
    // cin.rdbuf(in.rdbuf());

    auto coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    // srand(time(0));
    cout << "heuristic-1: (stones_in_my_storage - stones_in_opponents_storage)\n";
    cout << "heuristic-2: W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_on_my_side - stones_on_opponents_side)\n";
    cout << "heuristic-3: W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_on_my_side - stones_on_opponents_side) + W3 * (additional_move_earned)\n";
    cout << "heuristic-4: W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_on_my_side - stones_on_opponents_side) + W3 * (additional_move_earned) + W4 * (stones_captured)\n";

    double (*heuristic[4])(const Board &) = {heuristic1, heuristic2, heuristic3, heuristic4};

    char tournamentWins[4] = {0, 0, 0, 0};
    char tournamentDraws[4] = {0, 0, 0, 0};

    pair<int, int> res[4][4] = {pair<int, int>(0, 0)};

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
                continue;
            cout << "\n----------------*********----------------\n";
            cout << "\nTournament between heuristic " << i + 1 << " and "
                 << "heuristic " << j + 1 << " Begins : \n\n";
            pair<char, char> win = playTournament2(games, maxDepth, heuristic[i], heuristic[j]);
            cout << "\nTournament Result : \n";
            cout << "heuristic " << i + 1 << " wins : " << (short)win.first << "\n";
            cout << "heuristic " << j + 1 << " wins : " << (short)win.second << "\n";
            cout << "draws : " << (short)(games - win.first - win.second) << "\n";
            res[i][j] = pair<int, int>(win.first, win.second);
            if (win.first == win.second)
            {
                tournamentDraws[i]++;
                tournamentDraws[j]++;
            }
            else if (win.first > win.second)
            {
                tournamentWins[i]++;
            }
            else if (win.first < win.second)
            {
                tournamentWins[j]++;
            }
        }
    }

    cout << "\n----------------*******************-----------------\n\n";
    cout << "Overall Tournament Result : \n\n";

    const int totalTourPerHeuristic = 6;

    for (int i = 0; i < 4; i++)
    {
        cout << "heuristic " << i + 1 << " :   wins : " << (short)tournamentWins[i] << " draws : " << (short)tournamentDraws[i] << " losses : " << totalTourPerHeuristic - (short)tournamentWins[i] - (short)tournamentDraws[i] << "\n";
    }

    cout << "\n----------------*******************-----------------\n\n";
    cout << "     h1    h2    h3    h4   \n\n";
    for (int i = 0; i < 4; i++)
    {
        cout << "h" << i+1 << " ";
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                cout << "      ";
                continue;
            }

            cout << right << setw(2) << res[i][j].first << ":" << left << setw(2) << res[i][j].second << " ";
        }

        cout << "\n";
    }

    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    time_taken *= 1e-9;

    cout << "\nTime taken by the program is : " << fixed
         << time_taken << setprecision(9);
    cout << " sec"
         << "\n\n\n";

    cout << "depth for each game is randomly chosen between 1 and " << maxDepth << "\n";
    cout << "games per Tournament : " << games << "\n";

    cout.rdbuf(coutbuf);
    out.close();
}

int main(void)
{
    int input;
    cout << "1.Random depth for each game with fixed heuristics\n2.Random weights chosen for each heuristics for each game with fixed depth\n";
    cout << "Please Enter your choice : ";
    cin >> input;

    if (input == 1)
        play2();
    else
        play1();

    return 0;
}
