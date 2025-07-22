#include <iostream>

using namespace std;

int main() {
    system("g++ problem_maker.cpp -o problem_maker");                  // Compila o arquivo problem_maker.cpp
    system("g++ problem_solver.cpp -o problem_solver");                // Compila o arquivo problem_solver.cpp
    // system("clear");                                                   // Limpa a tela antes de executar o programa

    int escolha;

    do{
        // system("clear");  // Limpa a tela antes de exibir o menu
        cout << "\n===== CODE JUDGE =====" << endl;
        cout << "\nChoose an Interface: " << endl;
        cout << "0. Exit" << endl;
        cout << "1. Problem Maker" << endl;
        cout << "2. Problem Solver" << endl;
        cout << "Enter your choice: ";
        cin >> escolha;

        switch(escolha) {
            case 0:
                break;
            case 1:                   
                system("clear");
                system("./problem_maker");          // Executa o programa problem_maker.cpp
                break;
            case 2:
                system("clear");
                system("./problem_solver");         // Executa o programa problem_solver.cpp
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }

    }while(escolha != 0);

    cout << "\nExiting program. Goodbye!" << endl;
    return 0;
}