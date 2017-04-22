#include "NN.h"

using namespace std;



NN::NN(double learningRate, int numTrainInputs, int numTestInputs,
       int numOutputs, int mapSize, vector<vector<int> > trainInputs,
       vector<vector<int> > testInputs, vector<int> trainTargets,
       vector<int> testTargets, int maxEpochs):
num_train_inputs(numTrainInputs), num_test_inputs(numTestInputs),
num_outputs(numOutputs), map_size(mapSize),
learning_rate(learningRate), train_inputs(trainInputs),
test_inputs(testInputs), train_targets(trainTargets),
test_targets(testTargets), max_epochs(maxEpochs) {
    
    srand(time(NULL));
    initialize_weights();
}

void NN::initialize_weights() {
    //outputs.reserve(num_outputs);
    for(int i = 0; i < num_outputs; i++) {
        output o;
        outputs.push_back(o);
        for(int j = 0; j < map_size; j++) {
            outputs[i].weights.push_back(0.3*double(rand())/double(RAND_MAX) - 0.15);
            //outputs[i].weights.push_back(0.15);
        }
        outputs[i].weights.push_back(1);
    }
}

//void NN::test() {
//    cout << "in test" << endl;
//}
//
void NN::train() {
    cout << "in train" << endl;
    for(int i = 0; i < max_epochs; i++) {
        for(int input = 0; input < num_train_inputs; input++) { // omg, change this you dip
            double target = train_targets[input];
            for(int output = 0; output < num_outputs; output++) {
                double dot_product = 0;
                for(int weight_index = 0; weight_index <= map_size; weight_index++) {
                    if(weight_index < map_size) {
                        dot_product += train_inputs[input][weight_index]*outputs[output].weights[weight_index];
                    } else {
                        dot_product += outputs[output].weights[weight_index]; // bias node
                    }
                }
                double g = activation_function(dot_product);
                double g_prime = ddx_activation_function(dot_product);
                //cout << dot_product << endl;
                //cout << "g = " << g << endl;
                //cout << "g' = " << g_prime << endl;
                //cout << "g x 10 = " << int(g*10) << endl;
                update_weights(output, input, g, g_prime, target);
            }
        }
        test();
    }
}

void NN::update_weights(int output_index, int input_index, double g, double g_prime, double target) {
    //double output_value;
    if(num_outputs > 1) {
        //output_value = g;
        if(output_index != target) {
            target = 0.0; // if index does not match target digit
        } else {
            target = 1.0; // if index matches target
        }
    } else {
        // no need to set target, as its already set
        //output_value = int ((g*10)); // got rid of + 0.5
        target = target / 10;
    }
    for(int i = 0; i < map_size+1; i++) {
        if(i < map_size) {
            outputs[output_index].weights[i] += learning_rate * (target - g) * g_prime * train_inputs[input_index][i];
        } else {
            outputs[output_index].weights[i] += learning_rate * (target - g) * g_prime; // bias node
        }
    }
}

double spread = 0.01;

double NN::activation_function(double x) {
    return 1/(1+exp(.5-x));
}

double NN::ddx_activation_function(double x) {
    return (1.64872*exp(x))/pow((1.64872+exp(x)),2);
}


// tbh worte this on 4/20, so check for bugs
void NN::test() {
    int num_correct = 0;
    
    // for every test image
    for(int input = 0; input < num_test_inputs; input++) {
        
        int target = test_targets[input];
        
        double answer_value = -1;
        int answer = 0;
        
        // calculate the output at every node
        for(int output = 0; output < num_outputs; output++) {
            double dot_product = 0;
            
            // for every weight connected to the output
            for(int weight_index = 0; weight_index <= map_size; weight_index++) {
                
                // real inputs
                if(weight_index < map_size) {
                    dot_product += test_inputs[input][weight_index]*outputs[output].weights[weight_index];
                }
                //bias node
                else {
                    dot_product += outputs[output].weights[weight_index];
                }
            }
            //cout << dot_product << endl;
            double g = activation_function(dot_product);
            
            if(num_outputs == 1) {
                cout << int(10*g) << endl;
                cout << target << endl << endl;
                if(target == int(10*g)) {
                    num_correct++;
                }
                   
            }
            
            
            else if(g > answer_value) {
                
                answer = output;
                answer_value = g;
            }
        }
        if(num_outputs == 10) {
            cout << "Answer = " << answer << endl;
            cout << "Target = " << target << endl << endl;
            if(answer == target) {
                ++num_correct;
            }
        }
                   
                   
    }
    cout << "number of correct tests " << num_correct << endl;
    //cout << "number of tests " << num_test_inputs << endl;
}
