#include<bits/stdc++.h>
#include"1805017.h"

using namespace std;

unordered_map<string,uint16_t> outputClassNameMap;
vector<unordered_map<string,uint16_t>> attributesValueNameMap;

void inputConf(){
    string numAttributes;
    cout << "\nEnter Number of Attributes : ";
    getline(cin,numAttributes);
    int n = stoi(numAttributes);
    for( uint16_t i = 0;i < n;i++ ){
        cout << "\nEnter Attribute Name : ";

        string attrName;
        vector<string> attrValues;
        unordered_map<string,uint16_t> attrValNameMap;
        getline(cin,attrName);
        cout << "\nEnter Attribute values (separated by comma + press Enter) : ";
        string line;
        getline(cin,line);
        // cout << line << endl;

        stringstream obj_ss(line);
        string temp;
        uint16_t j = 0;
        // while loop function to go through source string until it is finished.
        while (getline(obj_ss, temp, ',')) {
            // cout << endl << temp;
            attrValues.push_back(temp);
            attrValNameMap[temp] = j++;
        }

        attributes.push_back(str_vStr_pair(attrName,attrValues));
        attributesValueNameMap.push_back(attrValNameMap);
    }

    cout << "\nEnter name of output Classes (separated by comma + press Enter) : ";
    
    string line;
    getline(cin,line);

    stringstream obj_ss(line);
    string outputClassName;
    uint16_t j = 0;
    // while loop function to go through source string until it is finished.
    while (getline(obj_ss, outputClassName, ',')) {
        // cout << endl << outputClassName;
        outputClassNameMap[outputClassName] = j++;
        outputClasses.push_back(outputClassName);
    }


    cout << endl;
    
}

void inputData(){

    string line;
    uint64_t indx = 0;
    while(getline(cin,line)){

        vector<uint16_t> example(attributes.size());
        // cout << line << endl;
        stringstream obj_ss(line);
        string temp;
        // while loop function to go through source string until it is finished.
        for (uint16_t j = 0;j < attributes.size();j++ ) {
            if( getline(obj_ss, temp, ',') ){
                try{
                    example[j] = attributesValueNameMap[j].at(temp);
                }catch(std::out_of_range e){
                    cout << "Error in " << indx << "th example...ignoring the example";
                    continue;
                }
            }else{
                cout << "Error in " << indx << "th example...ignoring the example";
                continue;
            }

            // cout << temp << " ";
        }

        getline(obj_ss, temp, '\n');
        try{
            outputs.push_back(outputClassNameMap.at(temp));
        }catch(std::out_of_range e){
            cout << "Error in " << indx << "th example...ignoring the example";
            continue;
        }
        // cout << temp << endl;

        dataSet.push_back(example);

        indx++;

    }
    
}


int main(){

    auto coutbuf = cout.rdbuf();
    auto cinbuf = cin.rdbuf();
    srand(time(0));

    ofstream outFile("output.txt");
    ifstream inFile("input.txt");

    // cout.rdbuf(outFile.rdbuf());
    cin.rdbuf(inFile.rdbuf());

    inputConf();

    string dataFileName;
    cout << "\nEnter data FileName : ";
    getline(cin,dataFileName);

    cout << endl;

    inFile.close();
    inFile.open(dataFileName);
    cin.rdbuf(inFile.rdbuf());

    inputData();

    inFile.close();

    cin.rdbuf(cinbuf);
    cout.rdbuf(outFile.rdbuf());

    vector<uint64_t> examples(dataSet.size()) ; 
    iota (examples.begin(), examples.end(), 0); // Fill with 0, 1, ...,.

    vector<uint16_t> attr(attributes.size()) ; 
    iota (attr.begin(), attr.end(), 0); // Fill with 0, 1, ...,.


    randomTest(examples,attr,20,0.8);

    // Node* tree = DecisionTreeLearningAlgorithm(examples,attr,vector<uint64_t>());
    // printTree(tree);

    // double accuracy = testDataSet(examples,tree);
    // // avgAccuracy += accuracy;      
    // cout << "Accuracy : " << accuracy << endl;


    
    cout.rdbuf(coutbuf);

    outFile.close();

    return 0;
}