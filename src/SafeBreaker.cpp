/*
 * Created by Aurelien Chagnon
 */

#include "../headers/SafeBreaker.h"

SafeBreaker::SafeBreaker(Safe safeToBreak): mSafe(std::move(safeToBreak)), mMirrorsInColumns(), mMirrorsInRows(),
mForwardRows(), mForwardColumns(), mBackwardRows(), mBackwardColumns(){

    /// Laser position: (row: 1, column: 0). Laser is considered being outside of the Safe
    mLaserPos = {1u, 0u};

    /// Detector position: (row: maxRow, column: maxColumn+1). Detector is considered being outside of the Safe
    mDetectorPos = {mSafe.rows(), mSafe.columns() + 1u};

    /// Create two (2) maps to link non empty Rows/Columns to the ordered map of Mirrors contained in each one of them.
    /// This helps reducing computing time when looking for the solutions to open the Safe.
    for(auto &mirror: mSafe.mirrors()){
        std::vector<uint32_t> position = mirror.position();
        mMirrorsInRows[position.at(0)][position.at(1)] = mirror;  ///< operator [] automatically creates new entry
        mMirrorsInColumns[position.at(1)][position.at(0)] = mirror;
    }

    /// Add virtual mirrors to represent the laser and the detector
    mMirrorsInRows[1][0] = Mirror(mLaserPos, Mirror::emirrorKind::eKindNone);
    mMirrorsInRows[mDetectorPos.at(0)][mDetectorPos.at(1)] = Mirror(mDetectorPos, Mirror::emirrorKind::eKindNone);
}

void SafeBreaker::solve(int &nbSolution, uint32_t &row, uint32_t &column){

    /// Compute the Laser beam trajectory
    const bool detectorReached = computeTrajectories();

    /// Check if laser beam reaches detector already
    if(detectorReached){
        /// Laser has reached the detector without having to add any mirror, no solution needed
        nbSolution = 0;
        row = column = 0u;
    }
    else{
        /// Laser does not reach the detector, check intersections between the laser beam and the backward trajectory
        /// to determine the number of solutions and their positions
        checkIntersections(nbSolution, row, column);

        /// No intersection: no solution, impossible to open the Safe
        if(nbSolution==0u) nbSolution = -1;
    }
}

bool SafeBreaker::computeTrajectories(){

    /// Init status of reached detector by laser, false at the beginning
    bool detectorReached = false;

    /// Forward laser beam: start from the laser position and direction to the right.
    std::vector<uint32_t> forwardPos(mLaserPos);  ///< By copy
    Mirror::edirection forwardDir = Mirror::edirection::eDirRight;

    /// Compute laser trajectory
    trajectoryTracking(mForwardRows, mForwardColumns, forwardPos, forwardDir);

    /// Check if detector is reached by laser, ie the laser beam has stopped in the detector position
    if(forwardPos == mDetectorPos) detectorReached = true;  ///< Detector reached by Laser
    else{
        /// The detector is not reached by the laser, compute the backward trajectory.
        /// "Backward" laser beam: start from detector position and direction to the left.
        std::vector<uint32_t> backwardPos(mDetectorPos);  ///< By copy
        Mirror::edirection backwardDir = Mirror::edirection::eDirLeft;

        /// Compute backward trajectory
        trajectoryTracking(mBackwardRows, mBackwardColumns, backwardPos, backwardDir);
    }

    return detectorReached;
}

void SafeBreaker::trajectoryTracking(std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>>& rows,
                                      std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>>& columns,
                                      std::vector<uint32_t>& currentPos,
                                      Mirror::edirection& currentDirection) {

    /// Create a virtual mirror to represent the end of Safe when no mirror is in the path
    std::vector<uint32_t> endOfSafe = {0, 0};

    /// Index indicating which part of the position of mirrors is discriminating in the search. Either 0 or 1.
    /// Ex:
    /// If the direction is to the right, the search occurs in a single precise row.
    /// Thus the index indicates that the discriminating data is the column position on this row.
    uint8_t index;

    /// Pointer to the list of Mirror to search in: the list should represent a single row or column for optimization time
    const std::map<uint32_t, Mirror> *searchVector;

    /// Map of segments to add the new movement segment to.
    std::list<std::vector<uint32_t>> *segmentMap;

    /// If the end of the Safe has not been reached, ie the trajectory is not over, redo operation
    while (currentPos != endOfSafe) {

        /// Set the endOfSafe at current position before computation of exact position
        endOfSafe = currentPos;

        /// Determine the position of the end of the Safe, the list of mirror to search in (the row or column) and the index
        /// depending on the incoming direction of the laser beam.
        /// Note: the [] operator creates a new empty list if the key is unknown, ie there is no mirror in this row/column.
        switch (currentDirection) {
            case Mirror::edirection::eDirLeft:
                endOfSafe.at(1) = 0u;  ///< End at the far left of the Safe (outside)
                searchVector = &mMirrorsInRows[currentPos.at(0)];  ///< Get the row where the current position is
                index = 1u;  ///< Indicates the columns
                segmentMap = &rows[currentPos.at(0)];  ///< Movement will be added in the rows map
                break;
            case Mirror::edirection::eDirRight:
                endOfSafe.at(1) = mSafe.columns() + 1u;  ///< End at the far right of the Safe (outside)
                searchVector = &mMirrorsInRows[currentPos.at(0)];  ///< Get the row where the current position is
                index = 1u;  ///< Indicates the columns
                segmentMap = &rows[currentPos.at(0)];  ///< Movement will be added in the rows map
                break;
            case Mirror::edirection::eDirUp:
                endOfSafe.at(0) = 0u;  ///< End at the top of the Safe (outside)
                searchVector = &mMirrorsInColumns[currentPos.at(1)];  ///< Get the column where the current position is
                index = 0u;  ///< Indicates the rows
                segmentMap = &columns[currentPos.at(1)];  ///< Movement will be added in the columns map
                break;
            case Mirror::edirection::eDirDown:
                endOfSafe.at(0) = mSafe.rows() + 1u;  ///< End at the bottom of the Safe (outside)
                searchVector = &mMirrorsInColumns[currentPos.at(1)];  ///< Get the column where the current position is
                index = 0u;  ///< Indicates the rows
                segmentMap = &columns[currentPos.at(1)];  ///< Movement will be added in the columns map
                break;
        }

        /// The next mirror is at the end of the Safe before the search.
        /// If it stays the same after the search, the trajectory has stopped.
        Mirror nextMirror(endOfSafe, Mirror::emirrorKind::eKindNone);

        /// Get the current mirror at current position.
        auto currentMirror = searchVector->find(currentPos[index]);

        /// Retrieve next mirror in path according to direction
        switch (currentDirection) {
            case Mirror::edirection::eDirRight:
            case Mirror::edirection::eDirDown:
                /// Next mirror is next in the list, if exists. Remember, list.end() is not in the list itself.
                ++currentMirror;
                if (currentMirror != searchVector->end()) nextMirror = currentMirror->second;
                break;
            case Mirror::edirection::eDirUp:
            case Mirror::edirection::eDirLeft:
                /// Next mirror is previous in the list, if exists. List.begin() is part of the list itself.
                if (currentMirror != searchVector->begin()) {
                    --currentMirror;
                    nextMirror = currentMirror->second;
                }
                break;
        }

        /// Add movement in the associated segment map
        const std::pair<uint32_t, uint32_t> minmaxPosition = std::minmax({currentPos.at(index),
                                                                          nextMirror.position().at(index)});
        segmentMap->push_back({minmaxPosition.first, minmaxPosition.second});

        /// Move to the next step (mirror or end of Safe)
        currentPos = nextMirror.position();

        /// Get the new direction for the next step
        currentDirection = nextMirror.reflect(currentDirection);
    }
}

void SafeBreaker::getIntersection(const std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>>& rowsMap,
                                  const std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>>& columnsMap,
                                  int& nbIntersection, std::vector<uint32_t*>& closestIntersection) {

    /// For movements in the rows, find movements in the columns that can intersect with the row
    for(const auto& [row, listOfMovementsInRows]:rowsMap) {
        for(const auto& moveInRow: listOfMovementsInRows){

            /// Find movements in columns
            for (uint32_t column = moveInRow.at(0); column <= moveInRow.at(1) ; ++column) {

                /// Find if a list of movements exists in this given column
                auto listOfMovementsInCol = columnsMap.find(column);

                /// A list of movement has been found in the column
                if(listOfMovementsInCol != columnsMap.end()){

                    /// Check for each movements if they intersect the row. Break when one is found (is unique).
                    for(const auto &moveInCol: listOfMovementsInCol->second){

                        /// The movement intersects the row if and only if the row is between its starting and stopping point
                        const std::pair<uint32_t, uint32_t> minmaxPos = std::minmax({moveInCol.at(0),
                                                                                     moveInCol.at(1),
                                                                                     row});
                        if((minmaxPos.first != row) && (minmaxPos.second != row)){

                            /// Intersection found, increase the number of intersections
                            ++nbIntersection;

                            /// Check if the new intersection is the closest so far, keep it in memory if true.
                            if((row <= *closestIntersection.at(0)) && (column <= *closestIntersection.at(1))) {
                                *closestIntersection.at(0) = row;
                                *closestIntersection.at(1) = column;
                            }

                            /// There can only have one intersection between a given row and a given column,
                            /// break the loop search when found.
                            break;
                        }
                    }
                }
            }
        }
    }
}

void SafeBreaker::checkIntersections(int &nbIntersection, uint32_t &row, uint32_t &column) const{

    /// At beginning, consider the Safe impossible to open with closest solution being the farthest position possible
    nbIntersection = 0;
    row = mSafe.rows();
    column = mSafe.columns();
    std::vector<uint32_t*> closestSolution = {&row, &column};

    /// Compute the intersection between movement along mRows during forward trajectory
    /// and movement along mColumns during backward trajectory
    getIntersection(mForwardRows, mBackwardColumns, nbIntersection, closestSolution);

    /// Compute the intersection between movement along mColumns during forward trajectory
    /// and movement along mRows during backward trajectory
    getIntersection(mBackwardRows, mForwardColumns, nbIntersection, closestSolution);

}
