#include <bits/stdc++.h>
#include "1805017.h"
using namespace std;

int main()
{

    // srand(time(0));
    auto coutbuf = cout.rdbuf();
    auto cinbuf = cin.rdbuf();

    ifstream inputFile("input.txt");
    cin.rdbuf(inputFile.rdbuf());

    int i = 1;
    int numFiles = 1;

    int maxIterGrasp = 1;
    int maxIterLocalSearch = 1;

    cout << "Enter initial File No : " << endl;
    cin >> i;
    cout << "Enter last File No : " << endl;
    cin >> numFiles;

    cout << "Enter output File No : " << endl;
    int outputFileNo;
    cin >> outputFileNo;

    int randomAlpha;
    double alpha;
    cout << "do you want set alpha randomly or not?" << endl;
    cin >> randomAlpha;

    if (randomAlpha == 0)
    {
        cout << "Please enter value of alpha : " << endl;
        cin >> alpha;
    }

    string outputFileName = "output_GRASP" + to_string(outputFileNo) + ".txt";
    ofstream outFile(outputFileName, ios_base::app);

    cout.rdbuf(outFile.rdbuf());
    inputFile.close();

    for (; i <= numFiles; i++)
    {
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

        // maxIterGrasp = 5;
        // maxIterLocalSearch = 50;
        // cout << "\nMax Iteration of GRASP is " << maxIterGrasp << endl;
        // cout << "local Search : BestFit(shoulder Fit = true). Max Iteration of local Search is " << maxIterLocalSearch << endl;

        // cout << "\n";

        // GRASP_MAXCUT(maxIterGrasp, maxIterLocalSearch, semiGreedy, localSearchBestFit, false, alpha, true);

        // cout << "-------------------***************------------------" << endl;

        maxIterGrasp = 10;
        maxIterLocalSearch = 250;
        cout << "\nMax Iteration of GRASP is " << maxIterGrasp << endl;
        cout << "\nlocal Search : FirstFit(shoulder Fit = false). Max Iteration of local Search is " << maxIterLocalSearch << endl;

        cout << "\n";

        GRASP_MAXCUT(maxIterGrasp, maxIterLocalSearch, semiGreedy, localSearchFirstFit, false, alpha, false);

        cout << "-------------------***************------------------" << endl;

        // maxIterGrasp = 1;
        // maxIterLocalSearch = 300;
        // cout << "\nMax Iteration of GRASP is " << maxIterGrasp << endl;
        // cout << "\nlocal Search : ExploreFirst(shoulder Fit = true). Max Iteration of local Search is " << maxIterLocalSearch << endl;

        // cout << "\n";

        // // GRASP_MAXCUT(maxIterGrasp, maxIterLocalSearch, semiGreedy, localSearchExplore, false, alpha, true);

        // cout << "-------------------***************------------------" << endl;

        inFile.close();
    }

    cout.rdbuf(coutbuf);
    cin.rdbuf(cinbuf);

    outFile.close();

    return 0;
}