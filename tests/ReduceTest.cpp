#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/headers/Reduce.h"
#include "../src/headers/FileManager.h"
#include <string>
#include <vector>
#include <fstream>

using std::string;
using std::ifstream;
using testing::Return;
using testing::Mock;
using std::array;

class MockFileManager: public FileManager {
  public:
    MOCK_METHOD((array<string, 2>), readFile, (string&));
    MOCK_METHOD(
      bool,
      isValid,
      (string inpuFilePath)
    );
};


TEST(ReduceTest, TestReduce) {

  string testInputFilePath = "SortOutput.txt";
  string testOutputDir = ".";

  Reduce r(testInputFilePath, testOutputDir);
  string word = "stack";
  vector<int> ones{1, 2, 3};
  r.reduce(word, ones);
  ifstream infile("./output.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }
  ASSERT_EQ(fileData, "(stack,6)");
  remove("./output.txt");
}


// TODO
/*
TEST(ReduceTest, TestProcessResult) {

  string testInputFilePath = "SortOutput.txt";
  string testOutputDir = ".";

  MockFileManager mfm;
  Reduce r(testInputFilePath, testOutputDir);

  array<string, 2> inputFile{"input.txt", "(\"stack\", [1,2,3])(\"over\", [2,3])(\"flow\",[3])"};


  ON_CALL(mfm, isValid(testInputFilePath)).WillByDefault(Return(true));
  ON_CALL(mfm, readFile(testInputFilePath)).WillByDefault(Return(inputFile));

  r.processSortResult();
  ifstream infile("./output.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }
  ASSERT_EQ(fileData, "(stack,6)\n(over,7)\n(flow,3)");
  remove("./output.txt");
  Mock::VerifyAndClear(&mfm);
}
*/
