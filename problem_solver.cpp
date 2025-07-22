#include <bits/stdc++.h>
#include "./lib/ProblemSolver.hpp"
#define endl '\n'

using namespace std;

typedef long long ll;
typedef unsigned long long llu;

int Menu() {
    int choice;
    cout << "\n===== PROBLEM MAKER INTERFACE =====" << endl;
    cout << "0. Return" << endl;
    cout << "1. List Problems" << endl;
    cout << "2. View/Solve a Problem" << endl;
    cout << "Choose an option: ";
    cin >> choice;
    cout << endl;
    return choice;
}

ProblemSolver PS;
void process(int choice){
    switch(choice) {
        case 1:
            PS.listProblems();
            break;
        case 2:
            PS.viewProblem();
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
    }

    cout << "\n\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    system("clear");
}

int main(){ 
    
    int choice;
    while(true){
        choice = Menu();
        choice == 0? exit(0) : process(choice);
    }

    return 0;
}