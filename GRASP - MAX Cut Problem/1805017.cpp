#include <bits/stdc++.h>
#include "1805017.h"
using namespace std;

int main()
{

    // srand(time(0));
    auto coutbuf = cout.rdbuf();
    auto cinbuf = cin.rdbuf();

    int i = 1;

    int maxIterGrasp = 1;
    int maxIterLocalSearch = 1;

    cout << "Enter File No : " << endl;
    cin >> i;

    cout << "Enter max iteration of local Search : " << endl;
    cin >> maxIterLocalSearch;

    cout << "Enter max iteration of GRASP : " << endl;
    cin >> maxIterGrasp;

    int randomAlpha;
    double alpha;
    cout << "do you want set alpha randomly or not?" << endl;
    cin >> randomAlpha;

    if (randomAlpha == 0)
    {
        cout << "Please enter value of alpha : " << endl;
        cin >> alpha;
    }

    string outputFileName = "output.txt";
    ofstream outFile(outputFileName);

    cout.rdbuf(outFile.rdbuf());

    string file = "./set1/g" + to_string(i) + ".rud";
    ifstream inFile(file);

    cin.rdbuf(inFile.rdbuf());

    cout << "\nGraph 1 : g" << i << ".rud" << endl;

    input();

    if (randomAlpha == 1)
    {
        alpha = ((double)rand() / (RAND_MAX));
    }

    cout << "\n\nconstructing solution with semi greedy method with alpha = " << alpha << endl;
    cout << "-------------------***************------------------" << endl;

    cout << "\nMax Iteration of GRASP is " << maxIterGrasp << endl;
    cout << "\nlocal Search : FirstFit(shoulder Fit = false). Max Iteration of local Search is " << maxIterLocalSearch << endl;

    cout << "\n";

    GRASP_MAXCUT(maxIterGrasp, maxIterLocalSearch, semiGreedy, localSearchFirstFit, false, alpha, false);

    cout << "-------------------***************------------------" << endl;


    inFile.close();

    cout.rdbuf(coutbuf);
    cin.rdbuf(cinbuf);

    outFile.close();

    return 0;
}