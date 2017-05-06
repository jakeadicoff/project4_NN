#include "main.h"

using namespace std;

int main(int argc, char** argv) {
    
    string trainFile  = argv[1];
    string testFile = argv[2];
    double learningRate = stod(argv[3]);
    int outputNodes = stoi(argv[4]);
    
    srand(420);
    
    int max_epochs = 50;
    
    
    
    string run_command;
    cout << "Run all the tests? (y/n)" << endl;
    cin >> run_command;
    
    if(run_command == "n") {
        Problem trainProb = read_file(trainFile);
        Problem testProb = read_file(testFile);
        
        NN net(learningRate, trainProb, testProb, outputNodes,max_epochs);
        
        vector<double> results = net.train();
        
        cout << "Percent correct: " << endl;
        for(int i = 0; i < max_epochs; ++i) {
            cout << i << ":\t" << results[i] << endl;
        }
    }
    
    
    else {
        string nn_filename;
        ofstream output_file;
        cout << "name output file (with .csv): " << endl;
        cin >> nn_filename;
        
        string output_string = "Neural Networks\nlearning rate,map_size,output_nodes,epoch,correct_tests,run_time\n";
        
        int max_epochs = 50;
        
        vector<double> lr_vector {0.005, 0.01, 0.05, 0.1};
        // set map size
        
        
        for(int i = 1; i < 2; ++i) {
            string train_file, test_file;
            
            if(i == 0) {
                train_file = "optdigits-8x8-int.tra";
                test_file = "optdigits-8x8-int.tes";
            }
            else {
                train_file = "optdigits-32x32.tra";
                test_file = "optdigits-32x32.tes";
            }
            
            for(int j = 0; j < 1; ++j) {
                
                int output_nodes;
                if(j == 0) output_nodes = 4;
                else output_nodes = 10;
                
                for(int lr = 0; lr < lr_vector.size(); ++lr) {
                    
                    double learning_rate = lr_vector[lr];
                    Problem train_prob = read_file(train_file);
                    Problem test_prob = read_file(test_file);
                    
                    
                    NN net(learning_rate, train_prob, test_prob, output_nodes,max_epochs);
                    
                    double start_time = clock();
                    
                    vector<double> results = net.train();
                    
                    double end_time = clock();
                    
                    net.clear();
                    
                    double run_time = (end_time - start_time)/CLOCKS_PER_SEC;
                    
                    //                    for(int epoch = 0; epoch < results.size(); ++ epoch) {
                    //                        output_string += to_string(learning_rate) + "," + to_string(train_prob.map_size) + "," + to_string(output_nodes) + "," + to_string(epoch) + "," + to_string(results[epoch]) + ", " + to_string(run_time) + "\n";
                    //                    }
                    
                    
                    output_string += to_string(learning_rate) + "," + to_string(train_prob.map_size) + "," + to_string(output_nodes) + "," + to_string(49) + "," + to_string(results.back()) + ", "+ to_string(run_time) + "\n";
                    
                    
                    
                    
                }
            }
        }
        
        output_file.open(nn_filename);
        //output_file << output_string;
        output_file.close();
        cout << output_string << endl;
    }
    
    
    
    return 0;
    
}


// reads in sets of digit recognition problems from files
Problem read_file(string problem_file_name) {
    
    
    Problem digit_recognition;
    digit_recognition.map_size = 0;
    digit_recognition.num_inputs = 0;
    vector<int> dummy_inputs;
    digit_recognition.inputs.push_back(dummy_inputs);
    
    string curr_number = "";
    char curr_char;
    
    ifstream problem_stream;
    problem_stream.open(problem_file_name.c_str(), ios::in);
    
    if(!problem_stream.good()) {
        cout << "Error: not able to open file" << endl;
    }
    
    // read the first line and determine map size
    if(problem_stream.peek() == 'B') {
        for(int i = 0; i < 3; i++){
            problem_stream.ignore(999999999,'\n');
        }
        int inputs_index = 0;
        while(problem_stream.peek() != EOF) {
            for(int i = 0; i < 32; i++) {
                for(int j = 0; j < 32; j++) {
                    problem_stream.get(curr_char);
                    curr_number =  curr_char;
                    digit_recognition.inputs[inputs_index].push_back(stoi(curr_number));
                }
                problem_stream.get(curr_char); // get rid of \n char
            }
            // get rid of ' ' and set target
            problem_stream.get(curr_char);
            problem_stream.get(curr_char);
            curr_number = curr_char;
            digit_recognition.targets.push_back(stoi(curr_number));
            problem_stream.get(curr_char);
            inputs_index++;
            vector <int> dummy_vector;
            digit_recognition.inputs.push_back(dummy_vector);
        }
        
        digit_recognition.map_size = 32*32;
        digit_recognition.num_inputs = inputs_index;
        return digit_recognition;
        
        
    } else {
        while(problem_stream.peek() != '\n') {
            problem_stream.get(curr_char);
            
            // if comma, save number and move on
            if(curr_char == ',') {
                digit_recognition.inputs[digit_recognition.num_inputs].push_back(stoi(curr_number));
                curr_number.clear();
                ++digit_recognition.map_size;
            }
            
            // else, add char to curr number string
            else {
                curr_number += curr_char;
            }
        }
        
        while(problem_stream.peek() != EOF) {
            
            problem_stream.get(curr_char);
            
            // if comma, save number and move on
            if(curr_char == ',') {
                digit_recognition.inputs[digit_recognition.num_inputs].push_back(stoi(curr_number));
                curr_number.clear();
            }
            
            // if new line, cap off current digit image and move on to next one
            else if(curr_char == '\n') {
                digit_recognition.targets.push_back(stoi(curr_number));
                curr_number.clear();
                ++digit_recognition.num_inputs;
                //cout << digit_recognition.num_inputs << endl;
                digit_recognition.inputs.push_back(dummy_inputs);
            }
            
            // else, add char to curr number string
            else {
                curr_number += curr_char;
            }
        }
        
        problem_stream.close();
        
        return digit_recognition;
    }
    
}
