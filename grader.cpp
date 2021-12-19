#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>
#include <string>
#include <vector>

const std::string TEST_CASES_FOLDER = "test_cases";
const std::string SOURCE_CODE_FILE_NAME = "example.cpp";
const int TIME_LIMIT = 1;  // seconds

// Change this function to other things if you don't want to use the terminal.
void writeResult(const std::string result) { printf("%s", result.c_str()); }

int run(const std::string command) { return system(command.c_str()); }

bool isFileExist(const std::string fileName) {
  if (FILE *file = fopen(fileName.c_str(), "r")) {
    fclose(file);
    return true;
  } else {
    return false;
  }
}

int main() {
  int systemReturnCode;

  // Compile the source code.
  systemReturnCode =
      run("g++ -std=c++11 " + SOURCE_CODE_FILE_NAME + " -o exec");
  if (systemReturnCode != 0) {
    writeResult("Compilation failed\n");
    return 0;
  }

  int testCaseCounter = 1;
  while (true) {
    // determine test case file name
    std::string input_file_name =
        TEST_CASES_FOLDER + '/' + std::to_string(testCaseCounter) + ".in";
    std::string output_file_name =
        TEST_CASES_FOLDER + '/' + std::to_string(testCaseCounter) + ".out";
    std::string solution_file_name =
        TEST_CASES_FOLDER + '/' + std::to_string(testCaseCounter) + ".sol";
    if (!isFileExist(input_file_name) || !isFileExist(solution_file_name)) {
      break;
    }

    // run command and measure time
    auto start = std::chrono::system_clock::now();
    systemReturnCode = run("timeout -k 1s 10s ./exec < " + input_file_name +
                           " > " + output_file_name);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    // if there is an error, print 'X'
    if (systemReturnCode != 0) {
      writeResult("X");
    } else if (elapsed_seconds.count() > TIME_LIMIT) {
      // if the time is more than 1 second, print 'T'
      writeResult("T");
    } else {
      FILE *out = fopen(output_file_name.c_str(), "r");
      FILE *sol = fopen(solution_file_name.c_str(), "r");

      bool correct = true;
      while (true) {
        char a = fgetc(out), b = fgetc(sol);
        while (a == '\n' || a == ' ') a = fgetc(out);
        while (b == '\n' || b == ' ') b = fgetc(sol);
        if (a == EOF && b == EOF) break;
        if (a != b) {
          correct = false;
          break;
        }
      }
      fclose(out);
      fclose(sol);

      if (correct)
        writeResult("P");
      else
        writeResult("-");
    }

    // Remove the temporary output file.
    systemReturnCode = run("rm -f " + output_file_name);
    if (systemReturnCode != 0) {
      writeResult("\nFailed to remove output file\n");
    }

    ++testCaseCounter;
  }

  // Remove the executable file.
  systemReturnCode = run("rm -f exec");
  if (systemReturnCode != 0) {
    writeResult("Failed to remove executable\n");
  }

  return 0;
}