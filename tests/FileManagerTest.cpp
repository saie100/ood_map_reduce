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

TEST(FileManagerTest, InvalidTYPE1){

    string filename= "../test_data/";    
    EXPECT_EQ(FileManager::isValid(FileManager::FILE, filename), false);
}

TEST(FileManagerTest, InvalidTYPE2){

    string dirName= "../test_data/input_data/input1.txt";  
    EXPECT_EQ(FileManager::isValid(FileManager::DIR, dirName), false);
}

TEST(FileManagerTest, InvalidPath){

    string dirName= "../test_data/randompath/";    
    EXPECT_DEATH(FileManager::isValid(FileManager::FILE, dirName), "Error invalid path: ");
}

TEST(FileManagerTest, ValidFilePath){

    string filePath= "../test_data/input_data/input1.txt";   
    EXPECT_EQ(FileManager::isValid(FileManager::FILE, filePath), true);
}


TEST(FileManagerTest, ReadFromFile){

    array<string,2> file_list = {"sample.txt", "Hello world!"};
    EXPECT_EQ(FileManager::readFile("../test_data/input_data/sample.txt"), file_list);
}




