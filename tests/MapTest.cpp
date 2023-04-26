#include <gtest/gtest.h>
#include "../src/headers/Map.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

TEST(MapTest, BasicString){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "hey this is a string");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    ASSERT_EQ(fileData, "(\"hey\",1)(\"this\",1)(\"is\",1)(\"a\",1)(\"string\",1)");

}

TEST(MapTest, Punctuation){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "this,,,, is.... testing//// punctuation");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    ASSERT_EQ(fileData, "(\"this\",1)(\"is\",1)(\"testing\",1)(\"punctuation\",1)");

}

TEST(MapTest, Whitespace){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "we               have              whitespace");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    ASSERT_EQ(fileData, "(\"we\",1)(\"have\",1)(\"whitespace\",1)");

}

TEST(MapTest, Newline){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "we\nhave\n newlines");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    ASSERT_EQ(fileData, "(\"we\",1)(\"have\",1)(\"newlines\",1)");

}

TEST(MapTest, IsContractions){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "it's he's she's who's what's when's where's why's how's which's this's that's here's there's somebody's someone's something's");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"it\",1)(\"is\",1)(\"he\",1)(\"is\",1)(\"she\",1)(\"is\",1)(\"who\",1)(\"is\",1)(\"what\",1)(\"is\",1)(\"when\",1)(\"is\",1)(\"where\",1)(\"is\",1)(\"why\",1)(\"is\",1)(\"how\",1)(\"is\",1)";
    compareStr += "(\"which\",1)(\"is\",1)(\"this\",1)(\"is\",1)(\"that\",1)(\"is\",1)(\"here\",1)(\"is\",1)(\"there\",1)(\"is\",1)(\"somebody\",1)(\"is\",1)(\"someone\",1)(\"is\",1)(\"something\",1)(\"is\",1)";

    ASSERT_EQ(fileData, compareStr);

}

TEST(MapTest, AreContractions){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "you're we're they're who're what're when're where're why're how're which're");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"you\",1)(\"are\",1)(\"we\",1)(\"are\",1)(\"they\",1)(\"are\",1)(\"who\",1)(\"are\",1)(\"what\",1)(\"are\",1)(\"when\",1)(\"are\",1)(\"where\",1)(\"are\",1)(\"why\",1)(\"are\",1)";
    compareStr += "(\"how\",1)(\"are\",1)(\"which\",1)(\"are\",1)";

    ASSERT_EQ(fileData, compareStr);

}

TEST(MapTest, DidContractions){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "I'd You'd he'd she'd it'd we'd they'd");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"i\",1)(\"did\",1)(\"you\",1)(\"did\",1)(\"he\",1)(\"did\",1)(\"she\",1)(\"did\",1)(\"it\",1)(\"did\",1)(\"we\",1)(\"did\",1)(\"they\",1)(\"did\",1)";

    ASSERT_EQ(fileData, compareStr);

}

TEST(MapTest, ImContraction){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "I'm just checking the I'm contraction");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"i\",1)(\"am\",1)(\"just\",1)(\"checking\",1)(\"the\",1)(\"i\",1)(\"am\",1)(\"contraction\",1)";

    ASSERT_EQ(fileData, compareStr);

}

TEST(MapTest, WillContraction){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "I'll you'll they'll it'll this'll we'll somebody'll");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"i\",1)(\"will\",1)(\"you\",1)(\"will\",1)(\"they\",1)(\"will\",1)(\"it\",1)(\"will\",1)(\"this\",1)(\"will\",1)(\"we\",1)(\"will\",1)(\"somebody\",1)(\"will\",1)";

    ASSERT_EQ(fileData, compareStr);

}

TEST(MapTest, HaveContraction){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "I've You've we've they've who've what've how've");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"i\",1)(\"have\",1)(\"you\",1)(\"have\",1)(\"we\",1)(\"have\",1)(\"they\",1)(\"have\",1)(\"who\",1)(\"have\",1)(\"what\",1)(\"have\",1)(\"how\",1)(\"have\",1)";

    ASSERT_EQ(fileData, compareStr);

}

TEST(MapTest, NotContraction){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "ain't won't can't hasn't mustn't wouldn't shouldn't");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"am\",1)(\"not\",1)(\"will\",1)(\"not\",1)(\"can\",1)(\"not\",1)(\"has\",1)(\"not\",1)(\"must\",1)(\"not\",1)(\"would\",1)(\"not\",1)(\"should\",1)(\"not\",1)";

    ASSERT_EQ(fileData, compareStr);

}

TEST(MapTest, KeepApostrophe){

    Map map("/temp/out.txt");
    // clear out the out file
    std::ofstream outfile("/temp/out.txt");
    outfile.close();
    map.map("out.txt", "Checking weird words like o'er and 't");
    
    std::ifstream infile("/temp/out.txt");
    string fileData;
    if (infile.is_open()) {
        getline(infile, fileData);
        infile.close();
    }

    string compareStr = "(\"checking\",1)(\"weird\",1)(\"words\",1)(\"like\",1)(\"o'er\",1)(\"and\",1)(\"'t\",1)";

    ASSERT_EQ(fileData, compareStr);

}