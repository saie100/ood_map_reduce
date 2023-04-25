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
    char * command[arguementCount + 1] = {"./main"};
    char ** pointer = command;

    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer);
        commandline.getArguments();
        }, "Error: No arguments entered, need 3");           
}

TEST(ExecutiveTest, OneArgument){
    
    // One Argement
    int arguementCount = 1;
    char * command[arguementCount + 1] = {"./main", "/home/saie/Desktop"};
    char ** pointer = command;
    
    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer); 
        commandline.getArguments();
        }, "Error: 1 argument entered, need 3");

}

TEST(ExecutiveTest, TwoArgument){
    
    // Two Argements
    int arguementCount = 2;
    char * command[arguementCount + 1] = {"./main", "/home/saie/Desktop/", "/home/saie/Desktop/"};
    char ** pointer = command;
    
    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer); 
        commandline.getArguments();
        }, "Error: 2 arguments entered, need 3");

}


TEST(ExecutiveTest, ThreeArgument){
    
    // Three Argements
    int arguementCount = 3;
    char * command[arguementCount + 1] = {"./main", "/home/saie/Desktop/", "/home/saie/Desktop/", "/home/saie/Desktop/OODesign/" };
    char ** pointer = command;
    vector<string> expected_value{pointer[1], pointer[2], pointer[3]};
    
    Executive commandline(arguementCount +1, pointer);        
    EXPECT_EQ(commandline.getArguments(), expected_value);

}

TEST(ExecutiveTest, InvalidPath){
    
    // InvalidPath
    int arguementCount = 3;
    char * command[arguementCount + 1] = {"./main", "/home/saie/Desktop/", "/home/saie/Desktas/", "/home/saie/Desktop/OODesign/" };
    char ** pointer = command;
    
    EXPECT_DEATH({
        Executive commandline(arguementCount +1, pointer); 
        commandline.getArguments();
        }, "Error invalid path: ");
}