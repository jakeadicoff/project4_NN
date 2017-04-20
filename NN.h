#ifndef __NN_h
#define __NN_h

#include <vector>

using namespace std;

struct output {
    vector<double> weights;
};


class NN {
public:
    NN(double learningRate, int numTrainInputs, int numTestInputs,
       int numOutputs, int mapSize, vector<vector<int> > trainInputs,
       vector<vector<int> > testInputs, vector<int> trainTargets,
       vector<int> testTargets, int maxEpochs);
    void test();
    void train();

private:
    int num_train_inputs;
    int num_test_inputs;
    int num_outputs;
    int map_size;

    double learning_rate;
    vector<vector<int>> train_inputs;
    vector<vector<int>> test_inputs;
    vector<int> train_targets;
    vector<int> test_targets;
    vector<output> outputs;
    int max_epochs;

};

#endif
