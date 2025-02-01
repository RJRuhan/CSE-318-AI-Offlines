#include <bits/stdc++.h>

using namespace std;

inline size_t key(int i, int j) { return (size_t)i << 32 | (unsigned int)j; }

unordered_map<size_t, int> graph;

uint16_t totalNodes = -1;

auto rng = std::default_random_engine{};
vector<uint16_t> nodeVec;

class uint16_vec_hasher
{
public:
    std::size_t operator()(const vector<bool> &vec) const
    {
        std::size_t seed = vec.size();
        for (bool i : vec)
        {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

typedef unordered_map<vector<bool>, bool, uint16_vec_hasher> vecMap;

bool existsInMap(const vecMap& exploredSolutions,const vector<bool>& solution)
{
    bool found = false;
    try
    {
        found = exploredSolutions.at(solution);
    }
    catch (const out_of_range &e)
    {
    }

    return found;
}

class Edge
{

public:
    uint16_t u = -1;
    uint16_t v = -1;
    int weight = -1;

    Edge(uint16_t u, uint16_t v, int weight)
    {

        this->u = u;
        this->v = v;
        this->weight = weight;
    }

    bool operator<(const Edge &e)
    {
        return weight < e.weight;
    }
};

class Node
{

public:
    uint16_t node = -1;
    long sigma_X = -1;
    long sigma_Y = -1;

    Node(uint16_t node, long sigma_X, long sigma_Y)
    {

        this->node = node;
        this->sigma_X = sigma_X;
        this->sigma_Y = sigma_Y;
    }

    bool operator<(const Node &n)
    {
        // if( max(sigma_X, sigma_Y) == max(n.sigma_X, n.sigma_Y) ){
        //     return node < n.node;
        // }
        return max(sigma_X, sigma_Y) < max(n.sigma_X, n.sigma_Y);
    }
};

vector<Edge> edges;

long calculateCut(const vector<bool> &solution)
{

    vector<uint16_t> S;
    vector<uint16_t> T;

    for (int i = 1; i <= totalNodes; i++)
    {
        if (solution[i])
            S.push_back(i);
        else
            T.push_back(i);
    }

    long cut = 0;

    for (uint16_t u : S)
    {
        for (uint16_t v : T)
        {
            int n1 = u, n2 = v;

            if (n1 > n2)
                swap(n1, n2);

            try
            {
                cut += graph.at(key(n1, n2));
            }
            catch (const out_of_range &e)
            {
            }
        }
    }

    return cut;
}

void printSolution(const vector<bool> &solution)
{
    cout << "S : ";
    for (int i = 1; i <= totalNodes; i++)
    {
        if (solution[i])
            cout << i << " ";
    }

    cout << endl
         << "T : ";
    for (int i = 1; i <= totalNodes; i++)
    {
        if (!solution[i])
            cout << i << " ";
    }

    cout << endl;
}

vector<bool> randomConstruction()
{

    vector<bool> solution(totalNodes + 1, false);

    for (int i = 1; i <= totalNodes; i++)
    {
        int choice = rand() % 2;

        if (choice == 0)
            solution[i] = true;
    }

    // cout << "max cut : " << calculateCut(solution);

    // printSolution(solution);

    return solution;
}

vector<bool> semiGreedy(double alpha, bool randomAlpha)
{

    if (randomAlpha)
    {
        alpha = ((double)rand() / (RAND_MAX));
    }

    int maxWeight = edges.back().weight;
    int minWeight = edges.front().weight;

    double miu = minWeight + (maxWeight - minWeight) * alpha;

    if (miu > maxWeight)
    {
        cout << "happened 1" << endl;
        miu = maxWeight;
    }

    uint16_t lowerBoundIndex = lower_bound(edges.begin(), edges.end(), Edge(-1, -1, miu)) - edges.begin();

    if (lowerBoundIndex == edges.size())
    {
        cout << "happened 2" << endl;
        exit(1);
    }

    uint16_t randomIndex = rand() % (edges.size() - lowerBoundIndex) + lowerBoundIndex;

    // cout << "random : " << edges[randomIndex].u << " " << edges[randomIndex].v << endl;

    vector<bool> X(totalNodes + 1, false);
    vector<bool> groundSet(totalNodes + 1, true);

    X[edges[randomIndex].u] = true;

    groundSet[edges[randomIndex].u] = false;
    groundSet[edges[randomIndex].v] = false;

    vector<Node> nodes;
    long minSigma = numeric_limits<long>::max();
    long maxSigma = numeric_limits<long>::min();

    uint16_t totalNodes_groundset = totalNodes - 2;

    for (int i = 1; i <= totalNodes; i++)
    {
        if (!groundSet[i])
            continue;

        long sigma_X = 0, sigma_Y = 0;

        int n1 = i, n2 = edges[randomIndex].v;

        if (n1 > n2)
            swap(n1, n2);

        try
        {
            sigma_X += graph.at(key(n1, n2));
        }
        catch (const out_of_range &e)
        {
        }

        n1 = i;
        n2 = edges[randomIndex].u;

        if (n1 > n2)
            swap(n1, n2);

        try
        {
            sigma_Y += graph.at(key(n1, n2));
        }
        catch (const out_of_range &e)
        {
        }

        minSigma = min(minSigma, min(sigma_X, sigma_Y));
        maxSigma = max(maxSigma, max(sigma_X, sigma_Y));

        nodes.push_back(Node(i, sigma_X, sigma_Y));
    }

    while (totalNodes_groundset != 0)
    {

        // cout << "here 2 " << nodes.size() << endl;
        std::sort(nodes.begin(), nodes.end());

        if (randomAlpha)
        {
            alpha = ((double)rand() / (RAND_MAX));
        }

        miu = minSigma + (maxSigma - minSigma) * alpha;

        if (miu > maxSigma)
        {
            cout << "happened 3" << endl;
            miu = maxSigma;
        }

        lowerBoundIndex = lower_bound(nodes.begin(), nodes.end(), Node(-1, miu, miu)) - nodes.begin();

        if (lowerBoundIndex == nodes.size())
        {
            cout << "happened 4" << endl;
            exit(1);
        }

        uint16_t randomIndex = rand() % (nodes.size() - lowerBoundIndex) + lowerBoundIndex;

        Node selectedNode = nodes[randomIndex];

        // cout << "random : " << selectedNode.node << endl;
        // cout << "fghjsaf " << miu << " " << max(selectedNode.sigma_X, selectedNode.sigma_Y) << endl;
        // cout << "groundSet Size : " << groundSet.size() << endl;

        groundSet[selectedNode.node] = false;
        nodes.erase(nodes.begin() + randomIndex);

        minSigma = numeric_limits<long>::max();
        maxSigma = numeric_limits<long>::min();

        if (selectedNode.sigma_X > selectedNode.sigma_Y)
        {
            X[selectedNode.node] = true;
            for (Node &n : nodes)
            {
                int n1 = selectedNode.node, n2 = n.node;

                if (n1 > n2)
                    swap(n1, n2);

                try
                {
                    n.sigma_Y += graph.at(key(n1, n2));
                }
                catch (const out_of_range &e)
                {
                }

                minSigma = min(minSigma, min(n.sigma_X, n.sigma_Y));
                maxSigma = max(maxSigma, max(n.sigma_X, n.sigma_Y));
            }
        }

        else
        {

            for (Node &n : nodes)
            {
                int n1 = selectedNode.node, n2 = n.node;

                if (n1 > n2)
                    swap(n1, n2);

                try
                {
                    n.sigma_X += graph.at(key(n1, n2));
                }
                catch (const out_of_range &e)
                {
                }

                minSigma = min(minSigma, min(n.sigma_X, n.sigma_Y));
                maxSigma = max(maxSigma, max(n.sigma_X, n.sigma_Y));
            }
        }

        totalNodes_groundset--;
    }

    return X;
}

uint16_t localSearchExplore(vector<bool> &solution, bool shoulderFit, uint16_t maxIterations)
{

    long globalCut = calculateCut(solution);

    vector<bool> currSolution = solution;
    vecMap exploredSolutions;


    for (int k = 0; k < maxIterations; k++)
    {
        long bestScore = numeric_limits<long>::min();

        if(shoulderFit)
            exploredSolutions[currSolution] = true;

        vector<uint16_t> currX;
        vector<uint16_t> currY;
        vector<bool> bestLocalSolution;


        for( int i = 1;i <= totalNodes;i++ ){
            if(currSolution[i])
                currX.push_back(i);
            else
                currY.push_back(i);
        }

        for (uint16_t i : nodeVec)
        {

            if (currSolution[i])
            {
                long sigma_X = 0, sigma_Y = 0;
                for (uint16_t u : currY)
                {
                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_X += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                for (uint16_t u : currX)
                {
                    if (u == i)
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_Y += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                long score = sigma_Y - sigma_X;

                if (shoulderFit)
                {
                    if (score >= bestScore)
                    {
                        vector<bool> temp = currSolution;
                        temp[i] = false;
                        if( !existsInMap(exploredSolutions,temp) ){
                            bestScore = score;
                            bestLocalSolution = temp;
                            // cout << i << endl;
                        }
                    }
                }
                else
                {
                    if (score > bestScore)
                    {
                        bestLocalSolution = currSolution;
                        bestLocalSolution[i] = false;
                        bestScore = score;
                        // cout << "fsf " << bestScore << endl;
                        // printSolution(pset(bestX,bestY));
                    }
                }
            }
            else
            {
                long sigma_X = 0, sigma_Y = 0;
                for (uint16_t u : currY)
                {
                    if (u == i)
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_X += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                for (uint16_t u : currX)
                {

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_Y += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                long score = sigma_X - sigma_Y;

                if (shoulderFit)
                {
                    if (score >= bestScore)
                    {
                        vector<bool> temp = currSolution;
                        temp[i] = true;
                        if( !existsInMap(exploredSolutions,temp) ){
                            bestScore = score;
                            bestLocalSolution = temp;
                            // cout << i << endl;
                        }
                    }
                }
                else
                {
                    if (score > bestScore)
                    {
                        bestLocalSolution = currSolution;
                        bestLocalSolution[i] = true;
                        bestScore = score;
                        // cout << "fsf " << bestScore << endl;
                        // printSolution(pset(bestX,bestY));
                    }
                }
            }
        }

        long localCut = calculateCut(bestLocalSolution);
        if (localCut > globalCut)
        {
            // cout << "local : " << localCut << " gloval : " << globalCut << endl;
            globalCut = localCut;
            solution = bestLocalSolution;
        }

        // printSolution(pset(X,Y));

        currSolution = bestLocalSolution;
    }

    return maxIterations;
}

uint16_t localSearchBestFit(vector<bool> &solution, bool shoulderFit, uint16_t maxIterations)
{
    bool change = true;
    uint16_t iteration = 0;

    vector<bool> bestSolution;
    long bestScore;
    vecMap exploredSolutions;

    while (change)
    {
        if (iteration == maxIterations)
            break;

        if(shoulderFit)
            exploredSolutions[solution] = true;

        vector<uint16_t> X;
        vector<uint16_t> Y;

        for( int i = 1;i <= totalNodes;i++ ){
            if(solution[i])
                X.push_back(i);
            else
                Y.push_back(i);
        }

        // cout << "local Searching" << endl;
        change = false;
        bestScore = 0;

        for (uint16_t i : nodeVec)
        {

            if (solution[i])
            {
                long sigma_X = 0, sigma_Y = 0;
                for (uint16_t u : Y)
                {
                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_X += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                for (uint16_t u : X)
                {
                    if (u == i)
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_Y += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                long score = sigma_Y - sigma_X;

                if (shoulderFit)
                {
                    if (score >= bestScore)
                    {
                        vector<bool> temp = solution;
                        temp[i] = false;

                        if( !existsInMap(exploredSolutions,temp) ){
                            change = true;
                            bestScore = score;
                            bestSolution = temp;
                            // cout << i << endl;
                        }
                    }
                }
                else
                {
                    if (score > bestScore)
                    {
                        bestSolution = solution;
                        bestSolution[i] = false;
                        change = true;
                        bestScore = score;
                        // cout << "fsf " << bestScore << endl;
                        // printSolution(pset(bestX,bestY));
                    }
                }
            }
            else
            {
                long sigma_X = 0, sigma_Y = 0;
                for (uint16_t u : Y)
                {
                    if (u == i)
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_X += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                for (uint16_t u : X)
                {

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_Y += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                long score = sigma_X - sigma_Y;

                if (shoulderFit)
                {
                    if (score >= bestScore)
                    {
                        vector<bool> temp = solution;
                        temp[i] = true;

                        if( !existsInMap(exploredSolutions,temp) ){
                            change = true;
                            bestScore = score;
                            bestSolution = temp;
                            // cout << i << endl;
                        }
                    }
                }
                else
                {
                    if (score > bestScore)
                    {
                        bestSolution = solution;
                        bestSolution[i] = true;
                        change = true;
                        bestScore = score;
                    }
                }
            }
        }


        if (change)
        {
            solution = bestSolution;
            // cout << selectedNode << endl;
        }

        // printSolution(pset(X,Y));

        iteration++;
    }

    return iteration;
}

uint16_t localSearchFirstFit(vector<bool> &solution, bool shoulderFit, uint16_t maxIterations)
{
    bool change = true;
    uint16_t iteration = 0;

    vecMap exploredSolutions;

    while (change)
    {
        if (iteration == maxIterations)
            break;
        // printSolution(solution);

        change = false;
        if (shoulderFit)
            exploredSolutions[solution] = true;
        
        // shuffle(nodeVec.begin(), nodeVec.end(), rng);

        for (uint16_t i : nodeVec)
        {
            // cout << i << " ";
            if (change)
                break;

            if (solution[i])
            {
                long sigma_X = 0, sigma_Y = 0;
                for (uint16_t u = 1; u <= totalNodes; u++)
                {
                    if (solution[u])
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_X += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                for (uint16_t u = 1; u <= totalNodes; u++)
                {
                    if (!solution[u])
                        continue;

                    if (u == i)
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_Y += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                if (shoulderFit)
                {
                    if (sigma_Y >= sigma_X)
                    {
                        solution[i] = false;
                        if( !existsInMap(exploredSolutions,solution) ){
                            change = true;
                            // cout << i << endl;
                        }else{
                            solution[i] = true;
                        }
                    }
                }
                else
                {
                    if (sigma_Y > sigma_X)
                    {
                        solution[i] = false;
                        change = true;
                    }
                }
            }
            else
            {
                long sigma_X = 0, sigma_Y = 0;
                for (uint16_t u = 1; u <= totalNodes; u++)
                {
                    if (solution[u])
                        continue;

                    if (u == i)
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_X += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                for (uint16_t u = 1; u <= totalNodes; u++)
                {
                    if (!solution[u])
                        continue;

                    int n1 = i, n2 = u;

                    if (n1 > n2)
                        swap(n1, n2);

                    try
                    {
                        sigma_Y += graph.at(key(n1, n2));
                    }
                    catch (const out_of_range &e)
                    {
                    }
                }

                if (shoulderFit)
                {
                    if (sigma_X >= sigma_Y)
                    {
                        solution[i] = true;
                        if( !existsInMap(exploredSolutions,solution) ){
                            change = true;
                            // cout << i << endl;
                        }else{
                            solution[i] = false;
                        }
                    }
                }
                else
                {
                    if (sigma_X > sigma_Y)
                    {
                        solution[i] = true;
                        change = true;
                    }
                }
            }
        }
        // cout << "size " << exploredSolutions.size() << endl;

        // cout << endl;
        iteration++;
    }


    return iteration;
}

long GRASP_MAXCUT(uint32_t maxIterations, uint16_t maxIterations_localSearch, vector<bool> (*solutionFunc)(double, bool), uint16_t (*localSearch)(vector<bool> &, bool, uint16_t), bool randomAlpha, double alpha, bool shoulderFit)
{
    vector<bool> bestSolution;
    long bestCut = std::numeric_limits<long>::min();

    double totalLocalSearchIterations = 0;
    double totalCut = 0;

    auto start = chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < maxIterations; i++)
    {
        // auto start = chrono::high_resolution_clock::now();

        vector<bool> solution = solutionFunc(alpha,randomAlpha);
        // vector<bool> solution = randomConstruction();


        // auto end = chrono::high_resolution_clock::now();
        // double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        // time_taken *= 1e-9;

        // cout << "\nTime taken for constructing greedy solution is : " << fixed
        //      << time_taken << setprecision(9);
        // cout << " sec"
        //      << "\n";

        // printSolution(solution);
        // cout << "Construction maxCut : " << calculateCut(solution) << endl;

        // start = chrono::high_resolution_clock::now();

        totalLocalSearchIterations += localSearch(solution, shoulderFit, maxIterations_localSearch);
        // end = chrono::high_resolution_clock::now();
        // time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        // time_taken *= 1e-9;

        // cout << "\nTime taken by local search is : " << fixed
        //      << time_taken << setprecision(9);
        // cout << " sec"
        //      << "\n";

        // printSolution(solution);

        long newCut = calculateCut(solution);
        // cout << "newCut : " << newCut << endl;

        totalCut += newCut;

        if (newCut > bestCut)
        {
            bestSolution = solution;
            bestCut = newCut;
        }
    }

    // cout << "best solution : " << endl;
    // printSolution(bestSolution);

    cout << "Local Search : avg iteration : " << totalLocalSearchIterations / maxIterations << " avg value : " << totalCut / maxIterations << endl;
    cout << "GRASP : total Iteration : " << maxIterations << "     Best value : " << bestCut << endl;

    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;

    cout << "\nTime taken for running GRASP is : " << fixed
         << time_taken << setprecision(9);
    cout << " sec"
         << "\n";

    return bestCut;
}

void CONSTRUCTION_SEMIGREEDY(uint32_t iterations, bool randomAlpha, double alpha)
{
    if (iterations <= 0)
    {
        cout << "Invalid Input" << endl;
        return;
    }

    long bestCut = std::numeric_limits<long>::min();
    double avgCut = 0;

    auto start = chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < iterations; i++)
    {
        vector<bool> solution = semiGreedy(alpha, randomAlpha);

        long newCut = calculateCut(solution);

        avgCut += newCut;

        if (newCut > bestCut)
        {
            bestCut = newCut;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;

    if (randomAlpha)
    {
        cout << "\nTime taken for constructing " << iterations << " randomized alpha semi-greedy solutions is : " << fixed
             << time_taken << setprecision(9);
        cout << " sec"
             << "\n";
    }
    else if (alpha == 1)
    {
        cout << "\nTime taken for constructing " << iterations << " greedy solutions is : " << fixed
             << time_taken << setprecision(9);
        cout << " sec"
             << "\n";
    }
    else
    {
        cout << "\nTime taken for constructing " << iterations << " semi-greedy solutions is : " << fixed
             << time_taken << setprecision(9);
        cout << " sec"
             << "\n";
    }

    avgCut /= iterations;

    cout << "best value : " << bestCut << endl;
    cout << "avg value : " << avgCut << endl;
}

void CONSTRUCTION_RANDOM(uint32_t iterations)
{
    if (iterations <= 0)
    {
        cout << "Invalid Input" << endl;
        return;
    }

    long bestCut = std::numeric_limits<long>::min();
    double avgCut = 0;

    auto start = chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < iterations; i++)
    {
        vector<bool> solution = randomConstruction();
        long newCut = calculateCut(solution);

        avgCut += newCut;

        if (newCut > bestCut)
        {
            bestCut = newCut;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;

    cout << "\nTime taken for constructing " << iterations << " random solutions is : " << fixed
         << time_taken << setprecision(9);
    cout << " sec"
         << "\n";

    avgCut /= iterations;

    cout << "best value : " << bestCut << endl;
    cout << "avg value : " << avgCut << endl;
}

void input()
{

    graph.clear();
    edges.clear();

    int numEdges;
    cin >> totalNodes >> numEdges;

    cout << "nodes : " << totalNodes << " edges : " << numEdges << endl;

    int u, v, weight;

    for (int i = 0; i < numEdges; i++)
    {
        cin >> u >> v >> weight;
        graph[key(u, v)] = weight;

        edges.push_back(Edge(u, v, weight));
    }

    for (int i = 1; i <= totalNodes; i++)
        nodeVec.push_back(i);

    std::sort(edges.begin(), edges.end());
}