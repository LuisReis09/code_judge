#include <bits/stdc++.h>
#include "./lib/ProblemMaker.hpp"
#define endl '\n'

using namespace std;

ProblemMaker PM;

int Menu() {
    int choice;
    cout << "\n===== PROBLEM MAKER INTERFACE =====" << endl;
    cout << "0. Return" << endl;
    cout << "1. Create a new problem" << endl;
    cout << "2. View existing problems" << endl;
    cout << "3. Edit an existing problem" << endl;
    cout << "4. Delete a problem" << endl;
    cout << "Choose an option: ";
    cin >> choice;
    cout << endl;
    return choice;
}

void process(int choice){
    switch(choice) {
        case 1:
            PM.createProblem();
            break;
        case 2:
            PM.viewProblems();
            break;
        case 3:
            PM.editProblem();
            break;
        case 4:
            PM.deleteProblem();
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