#ifndef __PROBLEM_MAKER_HPP__
#define __PROBLEM_MAKER_HPP__
    
    #include <iostream>
    #include <vector>

    using namespace std;

    typedef struct{
        string name;
        string description;
        vector<string> input_examples;
        vector<string> output_examples;
        float time_limit;
        float memory_limit;
    } Problem;

    class ProblemMaker {
        private:
            void saveProblem(Problem problem) {
                // First, go back to the directory "problems" and create a file with the problem name
                string command = "mkdir -p problems/" + problem.name;
                system(command.c_str());

                // Then, create a file named "description.txt" inside the problem directory and write the problem description
                command = "echo \"" + problem.description + "\" > problems/" + problem.name + "/description.txt";
                system(command.c_str());

                // Then, create a file name "config.txt" inside the problem directory and write the time limit and memory limit
                command = "echo \"TL = " + to_string(problem.time_limit) + "\n" + 
                          "ML = " + to_string(problem.memory_limit) + "\" > problems/" + problem.name + "/config.txt";

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

            void processEditChoice(Problem& problem, int choice, int id = -1) {
                string line;
                switch(choice) {
                    case 1: // Edit description
                        cout << "Enter new description (type \"dexit\" when it's done): " << endl;
                        cin.ignore(); 
                        problem.description.clear();
                        while (getline(cin, line)) {
                            if (line == "dexit") break;
                            problem.description += line + "\n";
                        }
                        line = "echo \"" + problem.description + "\" > problems/" + problem.name + "/description.txt";
                        system(line.c_str());
                        break;

                    case 2: // Edit time limit
                        cout << "Enter new time limit (seconds): ";
                        cin >> problem.time_limit;
                        line = "echo \"TL = " + to_string(problem.time_limit) + "\nML = " + to_string(problem.memory_limit) + "\" > problems/" + problem.name + "/config.txt";
                        system(line.c_str());
                        break;

                    case 3: // Edit memory limit
                        cout << "Enter new memory limit (MB): ";
                        cin >> problem.memory_limit;
                        line = "echo \"TL = " + to_string(problem.time_limit) + "\nML = " + to_string(problem.memory_limit) + "\" > problems/" + problem.name + "/config.txt";
                        system(line.c_str());
                        break;

                    case 4: // Edit input examples
                        if(id < 0 || id >= problem.input_examples.size()) {
                            cout << "Invalid ID for input example." << endl;
                            return;
                        }
                        cout << "Enter new input example (type \"iexit\" when it's done): " << endl;
                        cin.ignore();
                        problem.input_examples[id].clear();
                        while (getline(cin, line)) {
                            if (line == "iexit") break;
                            problem.input_examples[id] += line + "\n";
                        }
                        line = "echo \"" + problem.input_examples[id] + "\" > problems/" + problem.name + "/input_examples/input_" + to_string(id+1) + ".txt";
                        system(line.c_str());
                        break;

                    case 5: // Edit output examples
                        if(id < 0 || id >= problem.output_examples.size()) {
                            cout << "Invalid ID for output example." << endl;
                            return;
                        }
                        cout << "Enter new output example (type \"oexit\" when it's done): " << endl;
                        cin.ignore();
                        problem.output_examples[id].clear();
                        while (getline(cin, line)) {
                            if (line == "oexit") break;
                            problem.output_examples[id] += line + "\n";
                        }
                        line = "echo \"" + problem.output_examples[id] + "\" > problems/" + problem.name + "/output_examples/output_" + to_string(id+1) + ".txt";
                        system(line.c_str());
                        break;
                    default:
                        cout << "Invalid option. Please try again." << endl;
                }

                cout << "Problem updated successfully!" << endl;
            }

        public:
            void createProblem() {
                Problem newProblem;

                cout << "\nEnter problem name: ";
                cin >> newProblem.name;

                cout << "Enter problem description (type \"dexit\" in a single line when it's done): " << endl;
                string line;
                cin.ignore(); // Clear the newline character from the input buffer
                newProblem.description = "";
                while (getline(cin, line)) {
                    if (line == "dexit") break;
                    newProblem.description += line + "\n";
                }

                cout << "Enter time limit (seconds): ";
                cin >> newProblem.time_limit;

                cout << "Enter memory limit (MB): ";
                cin >> newProblem.memory_limit;

                int i = 1;
                bool input_done = false;
                cout << endl;
                while(true){
                    cout << endl << i << " Example of Input (type \"iexit\" in a single line when it's done)" << endl;
                    line.clear();
                    cin.ignore();
                    string input_example = "";
                    while (getline(cin, line)) {
                        if (line == "iexit") {
                            newProblem.input_examples.push_back(input_example);
                            input_example.clear();
                            break;
                        }
                        
                        input_example += line + "\n";
                    }

                    cout << endl << i << " Example of Output (type \"oexit\" in a single line when it's done)" << endl;
                    line.clear();
                    cin.ignore();
                    string output_example = "";
                    while (getline(cin, line)) {
                        if (line == "oexit") {
                            newProblem.output_examples.push_back(output_example);
                            output_example.clear();
                            break;
                        }
                        
                        output_example += line + "\n";
                    }

                    cout << endl << "Do you want to add another example? (y/n): ";
                    cin >> line;
                    if(line == "n" || line == "N") {
                        break;
                    }
                    i++;
                }

                cout << "Creating problem: " << newProblem.name << "..." << endl;

                saveProblem(newProblem);

                cout << "Problem created successfully!" << endl;
            }

            void viewProblems() {
                // List all problems in the "problems" directory
                cout << "Listing all problems..." << endl;
                system("ls -1 problems");

            }

            void editProblem() {
                cout << "Choose a problem to edit: " << endl;
                system("ls -1 problems");
                string problem_name;
                cout << "Enter problem name: ";
                cin >> problem_name;

                Problem to_edit = readProblem(problem_name);
                cout << "Editing problem: " << to_edit.name << endl;
                cout << "Description: " << to_edit.description << endl;
                cout << "Time Limit: " << to_edit.time_limit << " seconds" << endl;
                cout << "Memory Limit: " << to_edit.memory_limit << " MB" << endl;
                cout << "\nInput Examples: " << endl;
                for(size_t i = 0; i < to_edit.input_examples.size(); i++) {
                    cout << "Input Example " << i+1 << ": " << to_edit.input_examples[i] << endl;
                }
                cout << "Output Examples: " << endl;
                for(size_t i = 0; i < to_edit.output_examples.size(); i++) {
                    cout << "Output Example " << i+1 << ": " << to_edit.output_examples[i] << endl;
                }

                cout << "\nChoose an option to edit: " << endl;
                cout << "0. Exit" << endl;
                cout << "1. Edit description" << endl;
                cout << "2. Edit time limit" << endl;
                cout << "3. Edit memory limit" << endl;
                cout << "4. Edit input examples: " << endl;
                cout << "5. Edit output examples: " << endl;
                int choice;
                cin >> choice;

                if(!choice) return;

                if(choice == 4 || choice == 5) {
                    cout << "(ID) to edit: ";
                    int id;
                    cin >> id;
                    processEditChoice(to_edit, choice, id);
                    return;
                }
                else{
                    processEditChoice(to_edit, choice);
                }


            }

            void deleteProblem() {
                cout << "Choose a problem to delete: " << endl;
                system("ls -1 problems");
                string problem_name;
                cout << "Enter problem name: ";
                cin >> problem_name;

                string command = "rm -rf problems/" + problem_name;
                if(system(command.c_str()) == 0) {
                    cout << "Problem deleted successfully!" << endl;
                } else {
                    cout << "Error deleting problem. Please check if the problem exists." << endl;
                }
            }
    };

    
#endif 