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

TEST(ExecutiveTest, NoArguement){

    string filename= "/home/dsaie/Desktop/dsafdsf";
    FileManager manager;
    
    EXPECT_DEATH(manager.isValid("file", filename), "Error Invalid Path: "+filename);

}

TEST(ExecutiveTest, oneArgument){

    string filename= "/home/dsaie/Desktop/dsafdsf";
    FileManager manager;
    
    EXPECT_DEATH(manager.isValid("file", filename), "Error Invalid Path: "+filename);

}
