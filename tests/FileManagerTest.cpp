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

    string filename= "/home/saie/Desktop/";    
    EXPECT_EQ(FileManager::isValid(FileManager::FILE, filename), false);
}

TEST(FileManagerTest, InvalidTYPE2){

    string dirName= "/home/saie/Desktop/gradenote.txt";    
    EXPECT_EQ(FileManager::isValid(FileManager::DIR, dirName), false);
}

TEST(FileManagerTest, InvalidPath){

    string dirName= "/home/saie/Desktop/dsafdsf";    
    EXPECT_DEATH(FileManager::isValid(FileManager::FILE, dirName), "Error invalid path: ");
}

TEST(FileManagerTest, ValidFilePath){

    string filePath= "/home/saie/Desktop/gradenote.txt";    
    EXPECT_EQ(FileManager::isValid(FileManager::FILE, filePath), true);
}

/*
TEST(FileManagerTest, ReadFromDir){

    system("ls > ./temp/list.temp");
    system("pwd > location.temp" );
    
    array<string, 2> file_list = FileManager::readFile("file_list.temp");
    array<string, 2> dirPath = FileManager::readFile("location.temp");

    EXPECT_EQ(FileManager::getFilesFromDir(dirPath), true);
}

*/


