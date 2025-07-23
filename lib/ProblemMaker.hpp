#ifndef __PROBLEM_MAKER_HPP__
#define __PROBLEM_MAKER_HPP__
    
    #include <iostream>
    #include <vector>
    #include "Problem.hpp"

    using namespace std;

    class ProblemMaker {
        private:

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