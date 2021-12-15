# Grader
This is a simple grader for c++.
## Prerequisites
* [C++11](https://www.cplusplus.com/info/description/)
* [G++](https://gcc.gnu.org/)
* [Ubuntu](https://www.ubuntu.com/)
## Usage
1. To use the grader, first, you need to have a source code file in the same directory as the grader.
1. Set the `TEST_CASES_FOLDER` in `grader.cpp` to the folder where the test cases are.
1. Set the `SOURCE_CODE_FILE_NAME` in `grader.cpp` to the name of the source code file.
1. Build and run the grader.
## Grading
The grader will take the input and output and compare them.
- If the output is correct, the grader will print "P".
- If the output is incorrect, the grader will print "-".
- If the runtime is too long, the grader will print "T".
- If there is an error, the grader will print "E".