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
     * FileManager fm = FileManager(inputDir, tempDir, outputDir)
     * 
     * array<string> listOfInputs = fm.getInputFiles()
     * 
     * the Workflow object should then instantiate a Map object
     * 
     * Map m = Map()
     * 
     * for (string input: listOfInputs){
     *      
     *     either the read the file line by line, or get all the lines at once
     *     for line in lines:
     *          m.map(input, line);
     * }
     * 
     * once mapping is done, we can start with sorting
     * 
     * Sort s = Sort(tempDir);
     * 
     * we probably gonna have two different approaches here
     * 1. we either finish sorting first and then start reducing
     * 2. the Sort object calls the Reduce object as it finishes sorting for a
     *     word
     * 
     * it might be easier to go with the second approach, since then we dont
     * have to parse the intermediate files again.
     * 
    */
}
