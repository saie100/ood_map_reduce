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
    EXPECT_DEATH(FileManager::isValid(FileManager::FILE, filename), "Error invalid path: "+ filename);
}


