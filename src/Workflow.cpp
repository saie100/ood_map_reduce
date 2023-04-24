#include "Workflow.h"
#include <string>

using std::string;

/**
 * Class Constructor specifying directories
 */
Workflow::Workflow(string input_dir, string temp_dir, string output_dir):
inputDir(input_dir),tempDir(temp_dir),outputDir(output_dir){
    
}

/**
 * Q for the team: should we make FileManager a member variable for Map, Sort,
 * and Reduce so its easier for them to write and read files.
*/


void Workflow::start() {
    /**
     * The Workflow object should first interact with the FileManager and get
     * a list of files that's in the input directory
     * 
     * FileManager fm = FileManager()
     * 
     * array<string> listOfInputs = fm.getFilesFromDir(inputDir)
     * 
     * the Workflow object should then instantiate a Map object
     * 
     * string tempMapFilePath;
     * string tempSortFilePath;
     * string outputFilePath
     * 
     * Map m = Map()
     * 
     * for (string input: listOfInputs){
     *      
     *     either the read the file line by line, or get all the lines at once
     *     for line in lines:
     *          m.map(input, line, tempFilePath);
     * }
     * 
     * once mapping is done, we can start with sorting
     * 
     * Sort s = Sort();
     * s.Reduce(tempFilePath, tempSortFilePath);
     * 
     * Reduce r = Reduce(tempSortFilePath, outputFilePath);
     * 
     * r.processSortResult();
     * 
    */
}
