#include<chrono>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<vector>

const std::string TEST_CASES_FOLDER = "test_cases";
const std::string SOURCE_CODE_FILE_NAME = "example.cpp";

void writeResult(const std::string result) {
    printf("%s", result.c_str());
}

int run(const std::string command) {
    return system(command.c_str());
}

bool isFileExist (const std::string fileName) {
    if (FILE *file = fopen(fileName.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

int main() {
    std::string souce_code_file_name = "";
    std::string test_cases_folder = "";
    
    int systemReturnCode;

    systemReturnCode = run("g++ -std=c++11 " + SOURCE_CODE_FILE_NAME +" -o exec");
    if (systemReturnCode != 0) {
        writeResult("Compilation failed\n");
        return 0;
    }

    std::string results = "";
    int testCaseCounter = 1;
    while( true ) {
        // determine test case file name
        std::string input_file_name = TEST_CASES_FOLDER + '/' + std::to_string(testCaseCounter) + ".in";
        std::string output_file_name = TEST_CASES_FOLDER + '/' + std::to_string(testCaseCounter) + ".out";
        std::string solution_file_name = TEST_CASES_FOLDER + '/' + std::to_string(testCaseCounter) + ".sol";
        if( !isFileExist(input_file_name) || !isFileExist(solution_file_name) ) {
            break;
        }

        // run command and measure time
        auto start = std::chrono::system_clock::now();
        systemReturnCode = run("timeout -k 1s 10s ./exec < " + input_file_name + " > " + output_file_name);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        if(systemReturnCode != 0) {
            writeResult("X");
        } else if ( elapsed_seconds.count() > 1 ) {
            writeResult("T");
        } else {
            FILE *out = fopen(output_file_name.c_str(), "r");
            FILE *sol = fopen(solution_file_name.c_str(), "r");

            bool correct = true;
            while(true) {
                char a = fgetc(out), b = fgetc(sol);
                while(a=='\n' || a==' ') a = fgetc(out);
                while(b=='\n' || b==' ') b = fgetc(sol);
                if(a==EOF && b==EOF) break;
                if(a!=b) { correct = false; break; }
            }
            fclose(out); 
            fclose(sol);

            if(correct) writeResult("P");
                   else writeResult("-");
        }
        
        systemReturnCode = run("rm -f " + output_file_name);
        if(systemReturnCode != 0) {
            writeResult("\nFailed to remove output file\n");
        }
        ++testCaseCounter;
    }
    
    systemReturnCode = run("rm -f exec");
    if(systemReturnCode != 0) {
        writeResult("Failed to remove executable\n");
    }
    
    return 0;
}