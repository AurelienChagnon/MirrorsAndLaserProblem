/*
 * Created by Aurelien Chagnon
 */

#ifndef SAFEANDMIRRORSPROBLEM_MIRROR_H
#define SAFEANDMIRRORSPROBLEM_MIRROR_H

#include <vector>
#include <iostream>

/**
 * Description of a mirror in a safe using its 2D position and mirror kind (\ or /).
 *
 * A mirror can reflect a light beam comming from any 4-ways direction.
 */
class Mirror {

public:

    /**
     * Definition of all possible kind of mirror.
     *
     * These values are used in arguments to various functions to determine the reflected direction.
     */
    enum class emirrorKind {
        eKindNone,  ///< Undefined mirror, considered pass-through
        eKindRightLeft,  ///< Mirror of type (/)
        eKindLeftRight  ///< Mirror of type (\)
    };

    /**
     * Definition of all possible directions.
     *
     * These values represent absolutes directions and not relatives to the mirror ("going to ...", not "comming from ...").
     */
    enum class edirection{
        eDirRight,  ///< Going to the right
        eDirLeft,  ///< Going to the left
        eDirUp,  ///< Going up
        eDirDown  ///< Going down
    };

    /**
     * Construct a Mirror object using its position and kind.
     *
     * @param positionRow, positionColumn: position in the Safe
     * @param mirrorKind: kind of mirror, need the emirrorKind enum
     */
    explicit Mirror(uint32_t positionRow=0u, uint32_t positionColumn=0u, emirrorKind mirrorKind=emirrorKind::eKindNone);

    /**
     * Construct a Mirror from a vector representing its position.
     *
     * @param position: Position vector
     * @param mirrorKind: Kind of mirror, need the emirrorKind enum
     */
    Mirror(const std::vector<uint32_t>& position, emirrorKind mirrorKind);

    /**
     * Retrieve the position of the mirror as a vector of int
     *
     * @return position of mirror
     */
    [[nodiscard]] std::vector<uint32_t> position() const;

    /**
     * Retrieve the row position of the mirror
     *
     * @return row position of mirror
     */
    [[nodiscard]] uint32_t row() const;

    /**
     * Retrieve the column position of the mirror
     *
     * @return column position of mirror
     */
    [[nodiscard]] uint32_t column() const;

    /**
     * Gives the direction of the light after the mirror has reflected the incoming light
     *
     * @param incomingDirection: direction of the incoming light beam
     * @return new direction of the light beam after reflection
     */
    [[nodiscard]] edirection reflect(edirection incomingDirection) const;

private:
    /// Position in the Safe
    uint32_t mRow, mColumn;

    /// Kind of mirror (/, \ or none)
    emirrorKind mKind;
};


#endif //SAFEANDMIRRORSPROBLEM_MIRROR_H
