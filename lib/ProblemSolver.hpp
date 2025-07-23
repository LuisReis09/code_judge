#ifndef __PROBLEM_SOLVER_HPP__
#define __PROBLEM_SOLVER_HPP__

    #include <iostream>
    #include <vector>
    #include <string>
    #include <sys/resource.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
    #include <cstdlib>
    #include <cstdio>
    
    #include "Problem.hpp"

    using namespace std;

    enum Language {
        C = 1,
        CPP = 2,
        PYTHON = 3,
        JAVA = 4
    };

    class ProblemSolver{
        private:
            float getTimeLimit(const string& problem) {
                ifstream config_file("problems/" + problem + "/config.txt");
                string line;
                float time_limit = 0.0;
                while (getline(config_file, line)) {
                    if (line.find("TL = ") != string::npos) {
                        time_limit = stof(line.substr(5));
                        break;
                    }
                }
                config_file.close();
                return time_limit;
            }

            float getMemoryLimit(const string& problem) {
                ifstream config_file("problems/" + problem + "/config.txt");
                string line;
                float memory_limit = 0.0;
                while (getline(config_file, line)) {
                    if (line.find("ML = ") != string::npos) {
                        memory_limit = stof(line.substr(5));
                        break;
                    }
                }
                config_file.close();
                return memory_limit;
            }

            void menuReceiveUserSolution(string problem) {
                cout << "\n\n===== SOLVE PROBLEM INTERFACE =====" << endl;
                cout << "Problem Name: " << problem << endl;

                cout << "\nSelect an input type option:" << endl;
                cout << "1. By file pathname" << endl;
                cout << "2. By standard input" << endl;
                cout << "Choose an option: ";
                int option;
                cin >> option;

                cout << "\n\nSelect a programming language:" << endl;
                cout << "1. C" << endl;
                cout << "2. C++" << endl;
                cout << "3. Python" << endl;
                cout << "4. Java" << endl;
                cout << "Choose a language option: ";
                int language;
                cin >> language;

                cin.ignore(); // Clear the newline character from the input buffer
                string input_data;
                if (option == 1) {
                    cout << "Enter the input file pathname: ";
                    string pathname;
                    getline(cin, pathname);
                    executeSolutionByFile(problem, pathname, static_cast<Language>(language));
                } else if (option == 2) {
                    cout << "\nEnter the input data (end with Ctrl+D)" << endl 
                        << "(Remember to add \"\\n\" at the end of each line.)" << endl 
                        << "(If you are are coding in Java, you must use \"Solution\" as the class name.)" << "\n\n" << "Your code: " << endl;
                    string line;
                    while (getline(cin, line)) {
                        input_data += line + "\n";
                    }
                    executeSolutionByInput(problem, input_data, static_cast<Language>(language));
                } else {
                    cout << "Invalid option. Returning to main menu." << endl;
                    return;
                }
            }

            /*
                Cria um processo filho para executar a solução, após o tempo limite, 
                o processo filho é encerrado e o resultado é retornado.
                Também limita o uso de memória do processo filho.

            */
            void executeSolutionByFile(string problem, const string& pathname, Language language) {
                // Verifica se o arquivo existe
                ifstream infile(pathname);
                if (!infile.is_open()) {
                    cerr << "Error: Unable to open file: " << pathname << endl;
                    return;
                }

                // Conta os casos de teste
                string command = "ls -p problems/" + problem + "/input_examples/ | grep -v / | wc -l";
                int num_cases = 0;
                FILE* fp = popen(command.c_str(), "r");
                if (fp) {
                    fscanf(fp, "%d", &num_cases);
                    pclose(fp);
                }
                if (num_cases <= 0) {
                    cerr << "No input examples found." << endl;
                    return;
                }

                // Compila se necessário
                switch (language) {
                    case C:
                        system(("gcc " + pathname + " -o solution_c").c_str());
                        break;
                    case CPP:
                        system(("g++ " + pathname + " -o solution_cpp").c_str());
                        break;
                    case JAVA:
                        system(("javac " + pathname).c_str());
                        break;
                }

                // Executa cada caso de teste
                for (int i = 1; i <= num_cases; i++) {
                    pid_t pid = fork();
                    if (pid == 0) {
                        // Redireciona STDIN
                        string input_file = "problems/" + problem + "/input_examples/input_" + to_string(i) + ".txt";
                        int input_fd = open(input_file.c_str(), O_RDONLY);
                        dup2(input_fd, STDIN_FILENO);
                        close(input_fd);

                        // Redireciona STDOUT
                        int output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        dup2(output_fd, STDOUT_FILENO);
                        close(output_fd);

                        // Limita tempo
                        struct rlimit time_limit;
                        time_limit.rlim_cur = getTimeLimit(problem) + 2;
                        time_limit.rlim_max = time_limit.rlim_cur;
                        setrlimit(RLIMIT_CPU, &time_limit);

                        // Limita memória
                        struct rlimit mem_limit;
                        mem_limit.rlim_cur = getMemoryLimit(problem) * 1024 * 1024;
                        mem_limit.rlim_max = mem_limit.rlim_cur;
                        setrlimit(RLIMIT_AS, &mem_limit);

                        // Executa a solução
                        if (language == C)
                            execl("./solution_c", "./solution_c", NULL);
                        else if (language == CPP)
                            execl("./solution_cpp", "./solution_cpp", NULL);
                        else if (language == PYTHON)
                            execlp("python3", "python3", "-u", pathname.c_str(), NULL);
                        else if (language == JAVA) {
                            string class_name = pathname.substr(pathname.find_last_of('/') + 1);
                            class_name = class_name.substr(0, class_name.find_last_of('.'));
                            setenv("LD_LIBRARY_PATH", "/usr/lib/jvm/java-21-openjdk-amd64/lib", 1);
                            execlp("java", "java", "-Xmx400m", class_name.c_str(),NULL);
                        }

                        // Se chegou aqui, falhou
                        cerr << "Execution failed." << endl;
                        exit(1);
                    } else {
                        int status;
                        waitpid(pid, &status, 0);
                        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                            cerr << "Error: Execution failed on case " << i << endl;
                            return;
                        }

                        // Verifica a saída
                        ifstream output_file("output.txt");
                        stringstream ss;
                        ss << output_file.rdbuf();
                        string user_output = ss.str();

                        string expected_file = "problems/" + problem + "/output_examples/output_" + to_string(i) + ".txt";
                        ifstream expected_output(expected_file);
                        stringstream expected_ss;
                        expected_ss << expected_output.rdbuf();
                        string expected = expected_ss.str();

                        if (user_output != expected) {
                            cout << "Wrong answer on case " << i << endl;
                            return;
                        }
                    }
                }

                cout << "\033[1;32mAll test cases passed!\033[0m" << endl;
            }

            void executeSolutionByInput(string problem, const string& input_data, Language language) {
                // Converte o input_data em um arquivo temporário e redireciona a tarefa para a função executeSolutionByFile
                string extension;
                switch (language) {
                    case C:
                        extension = ".c";
                        break;
                    case CPP:
                        extension = ".cpp";
                        break;
                    case PYTHON:
                        extension = ".py";
                        break;
                    case JAVA:
                        extension = ".java";
                        break;
                }
                string temp_input_file = "Solution" + extension;
                ofstream temp_file(temp_input_file);
                if (!temp_file.is_open()) {
                    cerr << "Error: Unable to create temporary input file." << endl;
                    return;
                }
                temp_file << input_data;
                temp_file.close();
                executeSolutionByFile(problem, temp_input_file, language);
            }

        public:
            void listProblems() {
                int quantity = system("ls -l problems | grep ^d | wc -l");
                if (quantity == 0) {
                    cout << "No problems available." << endl;
                    return;
                }
                cout << "Listing all problems..." << endl;
                system("ls -1 problems");
            }

            void viewProblem() {
                cout << "\n===== VIEW PROBLEM INTERFACE =====" << endl;
                system("ls -1 problems");
                string problem_name;
                cout << "Enter problem name: ";
                cin >> problem_name;

                // Verifica se existe uma pasta com o nome do problema
                ifstream problem_dir("problems/" + problem_name);
                if (!problem_dir.is_open()) {
                    cerr << "Problem not found: " << problem_name << endl;
                    return;
                }

                Problem to_view = readProblem(problem_name);
                cout << "\nProblem Name: " << to_view.name << endl;
                cout << "Description: " << to_view.description << endl;
                cout << "Time Limit: " << to_view.time_limit << " seconds" << endl;
                cout << "Memory Limit: " << to_view.memory_limit << " MB" << endl;

                cout << "\nWill you try to solve this problem? (y/n): ";
                string choice;
                cin >> choice;
                if (choice == "y" || choice == "Y") {
                    menuReceiveUserSolution(problem_name);
                } else {
                    cout << "You chose not to solve the problem." << endl;
                }
            }
    };

#endif