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
    char * command[arguementCount + 1];
    string main = "./main";
    command[0] = main.data();
    char ** pointer = command;

    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer);
        commandline.getArguments();
        }, "Error: No arguments entered, need 3");           
}

TEST(ExecutiveTest, OneArgument){
    
    // One Argement
    int arguementCount = 1;
    char * command[arguementCount + 1];
    string main = "main";
    string arg1 = "";
    command[0] = main.data();
    command[1] = arg1.data();

    char ** pointer = command;
    
    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer); 
        commandline.getArguments();
        }, "Error: 1 argument entered, need 3");

}

TEST(ExecutiveTest, TwoArgument){
    
    // Two Argements
    int arguementCount = 2;
    char * command[arguementCount + 1];
    string main = "./main";
    string arg1 = "";
    string arg2 = "/home/saie/Desktop";
    
    command[0] = main.data();
    command[1] = arg1.data();
    command[2] = arg1.data();

    char ** pointer = command;
    
    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer); 
        commandline.getArguments();
        }, "Error: 2 arguments entered, need 3");

}


TEST(ExecutiveTest, ThreeArgument){
    
    // Three Argements
    int arguementCount = 3;
    char * command[arguementCount + 1];
    string main = "./main";
    string arg1 = "/home/saie/Desktop";
    string arg2 = "/home/saie/Desktop/OODesign/";
    
    command[0] = main.data();
    command[1] = arg1.data();
    command[2] = arg1.data();

    char ** pointer = command;
    vector<string> expected_value{pointer[1], pointer[2], pointer[3]};
    
    Executive commandline(arguementCount +1, pointer);        
    EXPECT_EQ(commandline.getArguments(), expected_value);

}


TEST(ExecutiveTest, InvalidPath){
    
    // InvalidPath
    int arguementCount = 3;
    char * command[arguementCount + 1];
    string main = "./main";
    string arg1 = "/home/saie/Desktas/";
    string arg2 = "/home/saie/Desktop/OODesign/";
    
    command[0] = main.data();
    command[1] = arg1.data();
    command[2] = arg1.data();
    char ** pointer = command;
    
    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer); 
        commandline.getArguments();
        }, "Error invalid path: ");
}
