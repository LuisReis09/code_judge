```markdown
# Code Judge

## Project Overview

The Code Judge repository is a C++ application designed to facilitate the creation, submission, and evaluation of programming problems. It provides a structured environment for users to generate problems and evaluate solutions.
This mini-project was casually developed as a means to study and explore concepts of concurrent programming.

## Attention!
Make sure to have gcc, g++, the JDK, and python3 installed to run user-submitted code properly. Additionally, this project must be executed on a POSIX-compliant system.

## Directory Structure

- **.vscode/**: Configuration files for Visual Studio Code.
  
- **lib/**: Contains libraries or helper functions used in the project.

- **problems/**: Directory where programming problems are defined.

- **main.cpp**: The main C++ source file that serves as the entry point for the application.

- **output.txt**: A text file that may contain output results from the program.

- **problem_maker.cpp**: C++ source file responsible for generating programming problems.

- **problem_solver.cpp**: C++ source file that handles the evaluation of submitted solutions.

- **.gitignore**: Specifies files and directories that should be ignored by Git.

## Installation Instructions

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/LuisReis09/code_judge.git
   ```

2. **Navigate to the Project Directory**:
   ```bash
   cd code_judge
   ```

3. **Compile the C++ Files**:
   ```bash
   g++ main.cpp problem_maker.cpp problem_solver.cpp -o code_judge
   ```

4. **Run the Application**:
   ```bash
   ./code_judge
   ```

## Usage

- The application allows users to create programming problems and submit solutions for evaluation.
- Users can generate problems using the `problem_maker` component.
- Solutions can be evaluated using the `problem_solver` component.

## Contact

- For any inquiries, please reach out to the repository owner via GitHub.
```
