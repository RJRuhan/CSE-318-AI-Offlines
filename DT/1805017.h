#include<bits/stdc++.h>
using namespace std;


typedef pair<string,vector<string>> str_vStr_pair;


class Node{
public:
    int attr = -1;
    vector<Node*> children; 
    bool isLeaf = false;
    int output = -1;
};

vector<vector<uint16_t>> dataSet;
vector<uint16_t>  outputs;


vector<str_vStr_pair> attributes;
vector<string> outputClasses;


Node* PLURALITY_VALUE(const vector<uint64_t>& examples){

    const uint16_t number_output_class = outputClasses.size();
    if( number_output_class == 0 )
        return 0;
    
    if( examples.size() == 0 )
        return 0;

    vector<long> outputClassCount(number_output_class,0);

    for( const uint64_t& indx : examples ){
        outputClassCount[outputs[indx]]++;
    }

    int maxIndx = -1;
    long maxVal = -1;
    bool isSame = false;

    for( int j = 0; j < number_output_class;j++ ){
        // cout << outputClassCount[j] << " ";
        if( outputClassCount[j] > maxVal ){
            maxVal = outputClassCount[j];
            maxIndx = j;
            isSame = false;
        }else if( outputClassCount[j] == maxVal ){
            isSame = true;
        }
    }
    // cout << endl;

    Node* newNode = new Node();
    newNode->isLeaf = true;


    if( !isSame ){
        newNode->output = maxIndx;
        // cout << maxIndx << endl;
        return newNode;
    }

    vector<uint16_t> chosenClasses;

    for( int j = maxIndx;j < number_output_class;j++ ){
        if(outputClassCount[j]==maxVal)
            chosenClasses.push_back(j);
    }

    uint16_t randVal = rand() % chosenClasses.size();

    newNode->output = chosenClasses[randVal];
    // cout << newNode->output << endl;

    // cout << "Classes with same plurality value detected" << endl;

    return newNode;

}

bool checkSameClass(const vector<uint64_t>& examples){
    if( examples.size() == 0 )
        return true;
    
    uint16_t val = outputs[examples[0]];
    uint64_t numExamples = examples.size();
    for( uint64_t i = 1;i < numExamples;i++ ){
        if( val != outputs[examples[i]] )
            return false;
    }

    return true;
}

double entropy(const vector<double>& probability){
    double entropy = 0;
    for( const double& p : probability ){
        if( p == 0 )
            continue;
        else if( p < 0 ){
            cout << "error in calculating probability...cant be negative" << endl;
        }
        entropy += p*log2(p);
    }

    return -1*entropy;
}



double informationGain(const uint16_t& attr,const vector<uint64_t>& examples){

    
    uint16_t num_output_class = outputClasses.size();
    vector<uint64_t> outputClassCount_parentNode(num_output_class,0);

    uint16_t num_attr_values = attributes[attr].second.size();
    vector<vector<uint64_t>> outputClassCount_childNode(num_attr_values,vector<uint64_t>(num_output_class,0));
    vector<uint64_t> numExamples_childNode(num_attr_values,0);



    for( const uint64_t& indx : examples ){
        outputClassCount_parentNode[outputs[indx]]++;
        numExamples_childNode[dataSet[indx][attr]]++;
        outputClassCount_childNode[dataSet[indx][attr]][outputs[indx]]++;
    }

    vector<double> probabilities_output_class;
    // cout << "prob_output_class " ;

    for( const uint64_t& p : outputClassCount_parentNode ){
        double prob = (double)p/examples.size();
        probabilities_output_class.push_back(prob);
        // cout << prob << " "; 
    }

    // cout << endl;

    double entropy_current_node = entropy(probabilities_output_class);
    // cout << "entropy parent : " << entropy_current_node << endl;
    double total_entropy_childNodes = 0;

    // cout << "prob_output_class " ;

    for( uint16_t i = 0;i < num_attr_values;i++ ){
        vector<double> probabilities_output_class_childNode;
        for(const uint64_t& p : outputClassCount_childNode[i]){
            double prob = 0;
            if( numExamples_childNode[i] != 0 )
                prob = (double)p/numExamples_childNode[i];
            probabilities_output_class_childNode.push_back(prob);
            //  cout << p << " " << prob << " | ";

        }

        double entropy_childNode = entropy(probabilities_output_class_childNode);
        total_entropy_childNodes += entropy_childNode*( (double) numExamples_childNode[i] / examples.size());
        // cout << " entropy child : " << entropy_childNode << endl;
        

    }
    // cout << endl;

    // cout << "total entropy : " << total_entropy_childNodes << endl;
    double inf_gain = entropy_current_node - total_entropy_childNodes;
    if( inf_gain < 0 ){
        cout << "information gain negative!" << endl;
        return 0;
    }

    return inf_gain;

}


Node* DecisionTreeLearningAlgorithm(const vector<uint64_t>& examples,const vector<uint16_t>& attr,const vector<uint64_t>& parent_examples){
    if( examples.size() == 0 ){
        return PLURALITY_VALUE(parent_examples);
    }else if(checkSameClass(examples)){
        Node* newNode = new Node();
        newNode->isLeaf = true;
        newNode->output = outputs[examples[0]];
        return newNode;
    }else if( attr.size() == 0 ){
        return PLURALITY_VALUE(examples);
    }else{
        double maxGain = -numeric_limits<double>::max();
        int maxAttrIndx = -1;
        bool isSame = false;
        uint16_t total_attr = attr.size();
        vector<double> gains(total_attr);
        for( uint16_t i = 0; i < total_attr;i++){

            double gain = informationGain(attr[i],examples);
            gains[i] = gain;
            if( gain > maxGain ){
                maxGain = gain;
                maxAttrIndx = i;
                isSame = false;
            }else if ( gain == maxGain ){
                isSame = true;
            }

        }

        if( isSame ){
            // cout << "Same Gain Attributes Found!" << endl;
            vector<double> sameGainAttrIndx;
            for( uint16_t i = maxAttrIndx;i < total_attr;i++ ){
                if( gains[i] == maxGain ){
                    sameGainAttrIndx.push_back(i);
                    // cout << attr[i] << " ";

                }
            }

            // cout << endl;

            uint16_t randIndx = rand() % sameGainAttrIndx.size();

            maxAttrIndx = sameGainAttrIndx[randIndx];
        }

        uint16_t chosenAttr = attr[maxAttrIndx];
        uint16_t attr_values = attributes[chosenAttr].second.size();
        vector<vector<uint64_t>> exs_attr_val(attr_values);

        for( const uint64_t& indx : examples ){
            exs_attr_val[dataSet[indx][chosenAttr]].push_back(indx);
        }

        vector<uint16_t> newAttrSet = attr;
        remove(newAttrSet.begin(),newAttrSet.end(),chosenAttr);

        Node* root = new Node();
        root->attr = chosenAttr;

        // cout << "gain : " << maxGain << " chosen : " << chosenAttr << endl;
        for( uint16_t i = 0;i < attr_values;i++ ){
            
            Node* subtree = DecisionTreeLearningAlgorithm(exs_attr_val[i],newAttrSet,examples);
            root->children.push_back(subtree);
        }

        return root;

        
    }
}




bool testSingleExample(const vector<uint16_t>& example,const uint16_t& output,const Node* tree){

    const Node* node = tree;
    // cout << "\n\nnew Examplee : ";
    while(node != nullptr){
        if( node->isLeaf ){
            return node->output == output;
        }
        //  cout << node->attr << " ";
        // cout << attributes[node->attr].first << " value : " << example[node->attr] << "->";

        node = node->children[example[node->attr]];
       
    }

    

    return false;
}

double testDataSet(const vector<uint64_t>& examples,const Node* tree){
    uint64_t accurateOutput = 0;
    // cout << endl;
    for( uint64_t i : examples ){
        if( testSingleExample(dataSet[i],outputs[i],tree) ){
            accurateOutput++;
        }else{
            // cout << i << "th example is wrong" << endl;
        }
    }
    // cout << "---- " << accurateOutput << " " << examples.size() << endl;
    return (double)accurateOutput/examples.size();
}

void walkTree(Node* root,int level,int& maxLevel,int& minLevel){
    if( !root->isLeaf ){
        cout << attributes[root->attr].first;
        cout << "(";
        uint16_t j = 0;
        for(Node* i : root->children){
            cout << attributes[root->attr].second[j++] << "->";
            walkTree(i,level+1,maxLevel,minLevel);
        }
        cout << ")";
    }else{
        if( level > maxLevel )
            maxLevel = level;
        if( level < minLevel ){
            minLevel = level;
        }
 
        cout << "(" << outputClasses[root->output] << ")";

        // if( root->output < 0 || root->output >= outputClasses.size() )
        //     cout << "suck " << root->output << endl;

        // cout << "(" << root->output << ")";

    }
}

void deleteTree(Node* root){
    if( !root->isLeaf ){
        for(Node* i : root->children){
            deleteTree(i);
        }
    }

    delete root;
    root = nullptr;
}


void printTree(Node* root){
    int maxLevel = numeric_limits<int>::min();
    int minLevel = numeric_limits<int>::max();

    walkTree(root,0,maxLevel,minLevel);

    cout << endl << "maxLevel : " << maxLevel << " minLevel : " << minLevel << endl;
}

unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
// unsigned seed = 10;
auto rng = std::default_random_engine(seed);


void randomTest(vector<uint64_t>& examples,vector<uint16_t>& attr,uint32_t trials,double percentage){
    // for(auto i : outputClasses)
    //     cout << i << endl;
    double avgAccuracy = 0;
    vector<double> accuracies;
    size_t trainingSetSize = examples.size()*percentage;
    // cout << trainingSetSize << endl;
    for( uint32_t i = 0;i < trials;i++ ){
        cout << "trial " << i+1 << ":" << endl;
        std::shuffle(examples.begin(), examples.end(), rng);
        vector<uint64_t> trainingSet(examples.begin(),examples.begin() + trainingSetSize);
        vector<uint64_t> testSet(examples.begin()+ trainingSetSize,examples.end());

        Node* tree = DecisionTreeLearningAlgorithm(trainingSet,attr,vector<uint64_t>());
        printTree(tree);

        double accuracy = testDataSet(testSet,tree);
        avgAccuracy += accuracy;      
        cout << "Accuracy : " << accuracy << endl;
        accuracies.push_back(accuracy);

        deleteTree(tree);
    }

    avgAccuracy /= trials;
    cout << "avgAccuracy : " << avgAccuracy << endl;

    double std_dev = 0;
    for( double i : accuracies ){
        std_dev += (i-avgAccuracy)*(i-avgAccuracy);
    }

    std_dev = sqrt(std_dev/trials);
    cout << "standard Deviation : " << std_dev << endl;

}