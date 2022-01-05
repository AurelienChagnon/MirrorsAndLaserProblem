/*
 * Created by Aurelien Chagnon
 */

#ifndef SAFEANDMIRRORSPROBLEM_API_H
#define SAFEANDMIRRORSPROBLEM_API_H

#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <utility>
#include "Safe.h"
#include "Mirror.h"
#include "SafeBreaker.h"

/**
 * API to solve several safe opening problems from an input file.
 * Solutions are displayed and saved in an output file.
 */
class Api {

public:

    /**
     * Constructor defining the inputs/outputs files.
     *
     * @param inputFileName: name of the input file
     * @param outputFileName: name of the output file
     */
    Api(std::string inputFileName, std::string  outputFileName);

    /**
     * Read the input file and solve each case of opening a safe using mirrors one by one. Solutions are displayed and
     * saved in the the output file.
     */
    void launch();

private:

    /// Define input and output file names
    const std::string mInputFileName, mOutputFileName;

    /// List of lines of the input file
    std::list<std::string> mInputLines;

    /// Number of solved case. Start from 0.
    uint32_t mNbCases;

    /// Safe to open, is configured during the launch sequence
    Safe mSafe;

    /**
     * Read the input file and store each line in the mInputLines list
     */
    void readFile();

    /**
     * Retrieve the next case, ie the safe configuration, from the inputs lines.
     *
     * @returns true if a case has been retrieved (correctly or not), false otherwise
     */
    bool getNextCase();

    /**
     * Configure the safe mirrors from the new case
     *
     * @param nbMirrorRightLeft: number of mirrors of type /
     * @param nbMirrorLeftRight: number of mirrors of type \
     */
    void configureSafe(uint32_t nbMirrorRightLeft, uint32_t nbMirrorLeftRight);

    /**
     * Display and save in the output file the number of solution and the lexicographically closest solution.
     * @param nbSolution: number of solution.
     * @param row, column: position of the closest solution
     */
    void outputSolution(int nbSolution, uint32_t row, uint32_t column);

};


#endif //SAFEANDMIRRORSPROBLEM_API_H
