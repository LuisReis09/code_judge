#ifndef __PROBLEM_HPP__
#define __PROBLEM_HPP__

    #include <iostream>
    #include <vector>
    #include <fstream>
    using namespace std;

    typedef struct{
        string name;
        string description;
        vector<string> input_examples;
        vector<string> output_examples;
        float time_limit;
        float memory_limit;
    } Problem;

    void saveProblem(Problem problem) {
        // First, go back to the directory "problems" and create a file with the problem name
        string command = "mkdir -p problems/" + problem.name;
        system(command.c_str());

        // Then, create a file named "description.txt" inside the problem directory and write the problem description
        command = "echo \"" + problem.description + "\" > problems/" + problem.name + "/description.txt";
        system(command.c_str());

        // Then, create a file name "config.txt" inside the problem directory and write the time limit and memory limit
        command = "echo \"TL = " + to_string(problem.time_limit) + "\" > problems/" + problem.name + "/config.txt";
        command += " && echo \"ML = " + to_string(problem.memory_limit + 200) + "\" >> problems/" + problem.name + "/config.txt"; // Adding 200 MB to the memory limit to avoid issues with the system

        system(command.c_str());

        // Now, create the directorys for input and output examples
        command = "mkdir -p problems/" + problem.name + "/input_examples";
        command += " && mkdir -p problems/" + problem.name + "/output_examples";
        system(command.c_str());

        // Finally, create the input and output example files, each named "input_{ID}.txt" and "output_{ID}.txt" respectively, where ID is the index of the example
        for(size_t i = 0; i < problem.input_examples.size(); i++) {
            command = "echo \"" + problem.input_examples[i] + "\" > problems/" + problem.name + "/input_examples/input_" + to_string(i+1) + ".txt";
            system(command.c_str());

            command = "echo \"" + problem.output_examples[i] + "\" > problems/" + problem.name + "/output_examples/output_" + to_string(i+1) + ".txt";
            system(command.c_str());
        }
    }

    Problem readProblem(string pathname){
        Problem problem;
        problem.name = pathname;

        // Read the problem description from the "description.txt" file in the "problems/{problem_name}" directory
        ifstream desc_file("problems/" + pathname + "/description.txt");
        if (desc_file.is_open()) {
            string line;
            while (getline(desc_file, line)) {
                problem.description += line + "\n";
            }
            desc_file.close();
        } else {
            cerr << "Unable to open description file for problem: " << pathname << endl;
        }

        // Read the time limit and memory limit from the "config.txt" file in the "problems/{problem_name}" directory
        ifstream config_file("problems/" + pathname + "/config.txt");
        if (config_file.is_open()) {
            string line;
            while (getline(config_file, line)) {
                if (line.find("TL = ") == 0) {
                    problem.time_limit = stof(line.substr(5));
                } else if (line.find("ML = ") == 0) {
                    problem.memory_limit = stof(line.substr(5));
                }
            }
            config_file.close();
        } else {
            cerr << "Unable to open config file for problem: " << pathname << endl;
        }

        // Accumulate inputs and outputs from the respective directories
        string input_path = "problems/" + pathname + "/input_examples/";
        string output_path = "problems/" + pathname + "/output_examples/";

        for (size_t i = 1; ; i++) {
            string input_file = input_path + "input_" + to_string(i) + ".txt";
            ifstream infile(input_file);
            if (!infile.is_open()) break; // No more input files

            string input_example;
            string line;
            while (getline(infile, line)) {
                input_example += line + "\n";
            }
            problem.input_examples.push_back(input_example);
            infile.close();
        }

        for (size_t i = 1; ; i++) {
            string output_file = output_path + "output_" + to_string(i) + ".txt";
            ifstream outfile(output_file);
            if (!outfile.is_open()) break; // No more output files

            string output_example;
            string line;
            while (getline(outfile, line)) {
                output_example += line + "\n";
            }
            problem.output_examples.push_back(output_example);
            outfile.close();
        }

        return problem;
    }

#endif