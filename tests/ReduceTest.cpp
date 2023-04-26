#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/headers/Reduce.h"
#include "../src/headers/FileManager.h"
#include <string>
#include <vector>

using std::string;

class MockFileManager: public FileManager {
  public:
    MOCK_METHOD(
      bool,
      writeFile,
      (FileManager::MODE, string outputDir, string fileName, string data)
    );
};


TEST(ReduceTest, OneInt) {

  string testInputFilePath = "unittest/temp/SortOutput.txt";
  string testOutputDir = "unittest/output";
  MockFileManager mfm;

  EXPECT_CALL(
    mfm,
    writeFile(FileManager::APPEND, testOutputDir, "/output.txt", "(\"stack\", 1)")
  ).WillOnce(Return(true));

  Reduce r(testInputFilePath, testOutputDir);
  string word = "stack";
  vector<int> ones{1};
  r.reduce(word, ones);
}
