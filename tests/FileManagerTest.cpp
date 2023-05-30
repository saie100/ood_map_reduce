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


TEST(FileManagerTest, CreateDirTest){

    string test_dir = "./temp_test";

    // create a temporary directory for testing
    FileManager::createDir("temp_test");
    
    EXPECT_EQ(FileManager::isValid(FileManager::DIR, test_dir), true);
    
    // delete the temporary directory
    system("rm -R temp_test");
}

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

TEST(FileManagerTest, WriteFile){

    // create a temporary directory for testing
    system("mkdir temp_test");

    // create three files
    FileManager::writeFile(FileManager::CREATE, "./temp_test/file1.txt", "hello world 1");
    FileManager::writeFile(FileManager::CREATE, "./temp_test/file2.txt", "hello world 2");
    FileManager::writeFile(FileManager::CREATE, "./temp_test/file3.txt", "hello world 3");
    
    // verify files were created
    vector<string> list = {"./temp_test/file1.txt","./temp_test/file2.txt","./temp_test/file3.txt"};
    EXPECT_EQ(FileManager::getFilesFromDir("./temp_test/"), list);
    
    // delete the temporary directory
    system("rm -R temp_test");

}



TEST(FileManagerTest, DeleteFiles){

    // create a temporary directory for testing
    system("mkdir temp_test");

    // create three files
    FileManager::writeFile(FileManager::CREATE, "./temp_test/file1.txt", "hello world 1");
    FileManager::writeFile(FileManager::CREATE, "./temp_test/file2.txt", "hello world 2");
    FileManager::writeFile(FileManager::CREATE, "./temp_test/file3.txt", "hello world 3");
    
    // verify files were created
    vector<string> list = {"./temp_test/file1.txt","./temp_test/file2.txt","./temp_test/file3.txt"};
    EXPECT_EQ(FileManager::getFilesFromDir("./temp_test/"), list);
    
    // delete all files from directory
    FileManager::deleteFilesFromDir("./temp_test/");
    
    // verify files were created
    EXPECT_EQ(FileManager::getFilesFromDir("./temp_test/").empty(), true);

    // delete the temporary directory
    system("rm -R temp_test");
}




