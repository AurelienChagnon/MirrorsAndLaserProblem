/*
 * Created by Aurelien Chagnon
 */

#include "../headers/Api.h"

Api::Api(std::string inputFileName, std::string  outputFileName):
    mInputFileName(std::move(inputFileName)), mOutputFileName(std::move(outputFileName)){

    /// Number of case start from 0
    mNbCases = 0u;

    /// Create or clear output file
    std::ofstream outputFile(mOutputFileName, std::ios::out);
    outputFile.close();
}

void Api::readFile() {

    /// Open input file
    std::ifstream file(mInputFileName);
    std::string line;

    if (file.is_open()) {
        /// Read line by line and put each line in a list: file opened only once
        while (std::getline(file, line)) {
            if(!line.empty()) mInputLines.push_back(line);
        }
        file.close();
    }
    else std::cerr << "Cannot open file " << mInputFileName << " !" << std::endl;  ///< File could not be opened
}

bool Api::getNextCase() {

    /// Check if a case can be created from input
    if(mInputLines.empty())
        return false;

    /// Input data
    uint32_t number;
    std::vector<uint32_t> vectCase;

    /// Read line by line. Make stringstream to retrieve data in proper format (int)
    std::stringstream line(mInputLines.front());

    /// Retrieve data from input line
    while ((line >> number))
        vectCase.push_back(number);

    /// Remove read line
    mInputLines.pop_front();

    /// Line should have four (4) integers to create a new case:
    /// Number of row, number of column, number of mirrors / and number of mirrors \ .
    if(vectCase.size() == 4){
        /// New case, configure the safe. Clear mirrors before setting context to avoid automatic removing (longer).
        mSafe.clearMirrors();
        mSafe.setContext(vectCase.at(0), vectCase.at(1));

        /// Add mirrors to the safe if any
        if(vectCase.at(2) > 0 || vectCase.at(3) > 0) {
            /// Check validity of number of mirrors, must be less than 200000 for each kind
            if (vectCase.at(2) > 200000 || vectCase.at(3) > 200000)
                std::cerr << "Number of mirrors should not be superior to 200000 for each kind !" << std::endl;

            configureSafe(vectCase.at(2), vectCase.at(3));
        }
    } else
        /// Line does not represent a new case
        std::cerr << "Missing data to create a case: Need a line of type 'nb_rows nb_columns nb_mirror_/ nb_mirror_\\'" << std::endl;

    return true;

}

void Api::configureSafe(uint32_t nbMirrorRightLeft, uint32_t nbMirrorLeftRight) {

    uint32_t rowColumn;
    std::vector<uint32_t> mirrorPos;

    /// Retrieve mirrors until no more mirror is needed or input file is empty
    while ((nbMirrorRightLeft || nbMirrorLeftRight) && !mInputLines.empty()) {

        /// Clear previous position
        mirrorPos.clear();

        /// Read position from line
        std::stringstream line(mInputLines.front());

        /// Retrieve position in proper type
        while ((line >> rowColumn))
            mirrorPos.push_back(rowColumn);

        /// Line should represent a position: row position, column position
        if (mirrorPos.size() == 2) {
            /// First lines are for mirrors of kind /, then lines are for mirrors of kind \ .
            if (nbMirrorRightLeft) {
                mSafe.addMirror(mirrorPos.at(0), mirrorPos.at(1), Mirror::emirrorKind::eKindRightLeft);
                --nbMirrorRightLeft;
            } else if (nbMirrorLeftRight) {
                mSafe.addMirror(mirrorPos.at(0), mirrorPos.at(1), Mirror::emirrorKind::eKindLeftRight);
                --nbMirrorLeftRight;
            } else
                /// Error: reading should have stopped. Should never be reached.
                std::cerr << "Error: file reading should have stopped at previous iteration !" << std::endl;
        } else {
            /// Line does not represent a position: could be a new case (ie a mirror is missing)
            std::cerr << "Improper line format to make a mirror ! Expected two (2) integers." << std::endl;
            break;
        }

        /// Remove read line only if the line has the proper format for a mirror. Could be a new case if not.
        mInputLines.pop_front();
    }
}

void Api::outputSolution(const int nbSolution, const uint32_t row, const uint32_t column) {

    /// Fill string depending on the number of solutions
    std::stringstream output;

    /// Basic state
    output << "Case " << mNbCases << ": ";

    /// If impossible to solve the case, add "impossible", otherwise add the number of solution
    if(nbSolution<0) output << "impossible";
    else output << nbSolution;

    /// If at least one solution exist, add the position given in parameters
    if(nbSolution>0) output << " " << row << " " << column;

    /// Display message
    std::cout << output.str() << std::endl;

    /// Save message in output file
    std::ofstream file(mOutputFileName, std::ios::app);
    if (file.is_open()) {
        file << output.str() << "\n";
        file.close();
    }

    /// Increase the number of solved cases
    ++mNbCases;
}

void Api::launch() {

    /// Load input file containing cases scenario
    readFile();

    /// Retrieve the next case and configure the Safe accordingly until each case is solved
    while (getNextCase()){

        /// Solve the case: open the Safe
        int nbSolution = 0;
        uint32_t solutionRow, solutionColumn = 0u;
        SafeBreaker breaker(mSafe);
        breaker.solve(nbSolution, solutionRow, solutionColumn);

        /// Display and save solutions to open the Safe
        outputSolution(nbSolution, solutionRow, solutionColumn);

    }
}
