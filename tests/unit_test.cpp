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

TEST(FileManagerTest, InvalidPath){

    string filename= "/home/dsaie/Desktop/dsafdsf";
    FileManager manager;
    
    EXPECT_DEATH(manager.isValid(FileManager::FILE, filename), "Error invalid path: "+ filename);

}

TEST(ExecutiveTest, NoArgument){

    char ** pointer;
    int arguementCount =0;
    arguementCount = 0;
    char * command1[arguementCount + 1] = {"./main"};
    pointer = command1;
    Executive commandline1(arguementCount +1, pointer);
    EXPECT_DEATH(commandline1.getArguments(), "Error: No arguments entered, need 3\nHelp: main [input dir] [temp dir] [output dir]");

    // One Argement
    arguementCount = 1;
    char * command2[arguementCount + 1] = {"./main", "/home/saie/Desktop"};
    pointer = command2;
    Executive commandline2(arguementCount +1, pointer);
    EXPECT_DEATH(commandline2.getArguments(), "Error: 1 argument entered, need 3\nHelp: main [input dir] [temp dir] [output dir]");

    // Two Argements
    arguementCount = 2;
    char * command3[arguementCount + 1] = {"./main", "/home/saie/Desktop/", "/home/saie/Desktop/" };
    pointer = command3;
    Executive commandline3(arguementCount +1, pointer);
    EXPECT_DEATH(commandline3.getArguments(), "Error: 2 arguments entered, need 3\nHelp: main [input dir] [temp dir] [output dir]");

    // Three Argements
    arguementCount = 3;
    char * command4[arguementCount + 1] = {"./main", "/home/saie/Desktop/", "/home/saie/Desktop/", "/home/saie/Desktop/OODesign/" };
    pointer = command4;
    Executive commandline4(arguementCount +1, pointer);
    vector<string> expected_value{pointer[1], pointer[2], pointer[3]};
    EXPECT_EQ(commandline3.getArguments(), expected_value);

}


