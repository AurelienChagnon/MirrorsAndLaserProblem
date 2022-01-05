/*
 * Created by Aurelien Chagnon
 */

#ifndef SAFEANDMIRRORSPROBLEM_SAFEBREAKER_H
#define SAFEANDMIRRORSPROBLEM_SAFEBREAKER_H

#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include "Safe.h"

/**
 * Let any user find the solution, if it exists, to open a given safe.
 *
 * Refer to the documentation to know more about the methodology used to find the solutions.
 */
class SafeBreaker {

public:
    /**
     * Constructor using only a Safe.
     *
     * Forward trajectory initialized with the laser position, backward trajectory with the detector position.
     * Mirrors of the safe are mapped into the columns and the rows.
     * Maps of segments are empty until trajectories are computed.
     *
     * @param safe: Safe to open.
     */
    explicit SafeBreaker(Safe safe);

    /**
     * Compute the solutions to open the Safe, ie where can a mirror be placed to open the Safe.
     *
     * @param[out] nbSolution: number of solution
     * @param[out] row, column: position of the lexicographically smallest solution
     */
    void solve(int &nbSolution, uint32_t &row, uint32_t &column);

private:

    /// Safe to open
    const Safe mSafe;

    /// Position of laser and detector
    std::vector<uint32_t> mLaserPos, mDetectorPos;

    /// Maps linking non empty rows/columns to the ordered map of mirrors contained in each one of them.
    /// This helps reducing computing time when looking for the solutions to open the Safe.
    std::unordered_map<uint32_t, std::map<uint32_t, Mirror>> mMirrorsInColumns, mMirrorsInRows;

    /// Maps linking rows and columns numbers to a list of segments defined by their outer points
    /// and representing the movements during forward and backward trajectory
    std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>> mForwardRows, mForwardColumns, mBackwardRows, mBackwardColumns;

    /**
     * Compute the forward and backward trajectories.
     *
     * Compute the trajectory of the laser beam first, ie the different positions passed by the laser, then the trajectory
     * of a virtual laser at the detector position, referenced as the backward trajectory. The backward trajectory is
     * computed only if the laser beam does not reached the detector. The backward trajectory is needed to compute all
     * solutions by checking its intersections with the laser beam.
     *
     * @return detectorReached: bool indicating if the laser reach the detector without having to compute a solution.
     */
    [[nodiscard]] bool computeTrajectories();

    /**
     * Compute the full trajectory by computing the next step in the trajectory, ie the closest mirror in the path or the end of the Safe.
     *
     * @param[out] rows: map associated to the trajectory linking rows numbers to a list of segments defined by their outer points
     * @param[out] columns: map associated to the trajectory linking columns numbers to a list of segments defined by their outer points
     * @param[in out] currentPos: current position
     * @param[in out] currentDirection: current direction headed to
     */
    void trajectoryTracking(std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>>& rows,
                            std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>>& columns,
                            std::vector<uint32_t> &currentPos,
                            Mirror::edirection &currentDirection);

    /**
     * Check the number of intersections and their positions between the forward and backward trajectories.
     *
     * @param[out] nbIntersection: number of intersections
     * @param[out] row, column: position of the lexicographically smallest solution
     */
    void checkIntersections(int& nbIntersection, uint32_t& row, uint32_t& column) const;

    /**
     * Compute the number of intersection between a map containing the movements along the rows and a map containing
     * the movements along the columns.
     *
     * @param[in] rowsMap: map linking rows numbers to a list of segments defined by their outer points
     * @param[in] columnsMap: map linking columns numbers to a list of segments defined by their outer points
     * @param[out] nbIntersection: incremented total number of intersection
     * @param[out] closestIntersection: position of the closest intersection
     */
    static void getIntersection(const std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>> &rowsMap,
                                const std::unordered_map<uint32_t, std::list<std::vector<uint32_t>>> &columnsMap,
                                int &nbIntersection, std::vector<uint32_t*> &closestIntersection);

};


#endif //SAFEANDMIRRORSPROBLEM_SAFEBREAKER_H
