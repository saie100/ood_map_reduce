#include <gtest/gtest.h>
#include "../src/headers/Executive.h"
#include "../src/headers/FileManager.h"

#include <array>
#include <iostream>
#include <string>
#include <vector>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


TEST(ExecutiveTest, NoArgument){

    // No Argements
    int arguementCount = 0;
    char * command[arguementCount*2 + 1];
    string main = "./main";
    command[0] = main.data();
    char ** pointer = command;

    EXPECT_DEATH(Executive commandline(arguementCount*2 +1, pointer), "--inputDir keyword argument required");           
}

TEST(ExecutiveTest, OneArgument){
    
    // One Argement
    int arguementCount = 1;
    char * command[arguementCount*2 + 1];
    string main = "./main";
    string kwarg1 = "--inputDir";
    string arg1 = "../tests/";
    command[0] = main.data();
    command[1] = kwarg1.data();
    command[2] = arg1.data();
    
    char ** pointer = command;
    EXPECT_DEATH(Executive commandline(arguementCount*2 +1, pointer), "--outputDir keyword argument required");
}

TEST(ExecutiveTest, TwoArgument){
    
    // Two Argements
    int arguementCount = 2;
    char * command[arguementCount*2 + 1];    
    string main = "./main";
    string kwarg1 = "--inputDir";
    string arg1 = "../test_data/input_data/";
    string kwarg2 = "--outputDir";
    string arg2 = "../test_data/output_data/";
    
    command[0] = main.data();
    command[1] = kwarg1.data();
    command[2] = arg1.data();
    command[3] = kwarg2.data();
    command[4] = arg2.data();
    
    char ** pointer = command;
    
    EXPECT_DEATH(Executive commandline(arguementCount*2 +1, pointer), "--dllDir keyword argument required");

}


TEST(ExecutiveTest, ThreeArgument){
    
    // Three Argements
    int arguementCount = 3;
    char * command[arguementCount*2 + 1];

    string main = "./main";
    string kwarg1 = "--inputDir";
    string arg1 = "../test_data/input_data/";
    string kwarg2 = "--outputDir";
    string arg2 = "../test_data/output_data/";
    string kwarg3 = "--dllDir";
    string arg3 = "../test_data/";
    
    command[0] = main.data();
    command[1] = kwarg1.data();
    command[2] = arg1.data();
    command[3] = kwarg2.data();
    command[4] = arg2.data();
    command[5] = kwarg3.data();
    command[6] = arg3.data();

    char ** pointer = command;
    Executive commandline(arguementCount*2 +1, pointer);
    string input = commandline.getInputDir();
    string output = commandline.getOutputDir();
    string temp = commandline.getTempDir();
    string dll = commandline.getReduceDLL();
    
    EXPECT_EQ(input, "../test_data/input_data/");
    EXPECT_EQ(output, "../test_data/output_data/");
    EXPECT_EQ(temp, "../test_data/temp_data/");
    EXPECT_EQ(dll, "../test_data/");
}


TEST(ExecutiveTest, InvalidPath){
    
    // InvalidPath
    int arguementCount = 3;
    char * command[arguementCount*2 + 1];

    string main = "./main";
    string kwarg1 = "--inputDir";
    string arg1 = "../test_data/input_data/random_path/";
    string kwarg2 = "--outputDir";
    string arg2 = "../test_data/output_data/";
    string kwarg3 = "--dllDir";
    string arg3 = "../test_data/";
    
    command[0] = main.data();
    command[1] = kwarg1.data();
    command[2] = arg1.data();
    command[3] = kwarg2.data();
    command[4] = arg2.data();
    command[5] = kwarg3.data();
    command[6] = arg3.data();

    char ** pointer = command;
    EXPECT_DEATH(Executive commandline(arguementCount*2 +1, pointer), "Error invalid path: ");
}
