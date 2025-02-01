#include <bits/stdc++.h>
#include "1805017.h"
using namespace std;

int main()
{

    // srand(time(0));
    int numFiles = 1;
    ofstream outFile("output_construction22.txt");

    auto coutbuf = cout.rdbuf();
    auto cinbuf = cin.rdbuf();

    cout.rdbuf(outFile.rdbuf());

    for (int i = 1; i <= numFiles; i++)
    {
        string file = "./set1/g" + to_string(i) + ".rud";
        ifstream inFile(file);

        cin.rdbuf(inFile.rdbuf());

        cout << "\nGraph 1 : g" << i << ".rud" << endl;  

        input();
        int iter = 30;
        cout << "\nConstructing " << iter << " random solutions" << endl;
        CONSTRUCTION_RANDOM(iter);

        cout << "--------------*************--------------\n"
             << endl;

        cout << "Constructing " << iter << " greedy solutions" << endl;
        CONSTRUCTION_SEMIGREEDY(iter, false, 1);

        cout << "--------------*************--------------\n"
             << endl;

        cout << "Constructing " << iter << " semi-greedy solutions" << endl;
        for (int i = 0; i < 10; i++)
        {
            double alpha = i * 0.1;
            cout << "alpha = " << alpha << endl;
            CONSTRUCTION_SEMIGREEDY(iter, false, alpha);
            cout << "\n";
        }

        cout << "--------------*************--------------\n"
             << endl;

        cout << "Constructing " << iter << " randomized alpha semi-greedy solutions" << endl;
        CONSTRUCTION_SEMIGREEDY(iter, true, 1);

        cout << "--------------*************--------------\n"
             << endl;

        inFile.close();
    }

    cout.rdbuf(coutbuf);
    cin.rdbuf(cinbuf);

    outFile.close();

    return 0;
}