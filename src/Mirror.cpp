/*
 * Created by Aurelien Chagnon
 */

#include "../headers/Mirror.h"


Mirror::Mirror(const uint32_t positionRow, const uint32_t positionColumn, const emirrorKind mirrorKind):
        mRow(positionRow), mColumn(positionColumn), mKind(mirrorKind) {
}

Mirror::Mirror(const std::vector<uint32_t>& position, const emirrorKind mirrorKind): mKind(mirrorKind) {
    /// Check if given vector can represent a 2D position, display an error if not
    if(position.size() != 2){
        /// Invalid size, set the mirror at position (0, 0)
        std::cerr << "Must have a vector of size two (2) to initialize a mirror ! Mirror at (0, 0)." << std::endl;
        mRow = 0u;
        mColumn = 0u;
    }else {
        mRow = position.at(0);
        mColumn = position.at(1);
    }
}

std::vector<uint32_t> Mirror::position() const {
    return {mRow, mColumn};  ///< Position as vector
}

uint32_t Mirror::column() const {
    return mColumn;
}

uint32_t Mirror::row() const {
    return mRow;
}

Mirror::edirection Mirror::reflect(const Mirror::edirection incomingDirection) const {

    /// By default the mirror does not reflect (pass-through). Behaviour associated with kind kindNone
    edirection reflectedDirection = incomingDirection;

    /// If mirror is of kind kindNone, no reflection: return here.
    if(mKind == emirrorKind::eKindNone) return reflectedDirection;

    /// For each incoming direction, associate a new direction (reflection) depending on the kind of mirror
    /// (either rightLeft / or leftRight \)
    switch (incomingDirection) {
        case edirection::eDirUp :
            reflectedDirection = mKind == emirrorKind::eKindRightLeft ? edirection::eDirRight : edirection::eDirLeft;
            break;
        case edirection::eDirDown :
            reflectedDirection = mKind == emirrorKind::eKindRightLeft ? edirection::eDirLeft : edirection::eDirRight;
            break;
        case edirection::eDirLeft :
            reflectedDirection = mKind == emirrorKind::eKindRightLeft ? edirection::eDirDown : edirection::eDirUp;
            break;
        case edirection::eDirRight :
            reflectedDirection = mKind == emirrorKind::eKindRightLeft ? edirection::eDirUp : edirection::eDirDown;
            break;
    }
    return reflectedDirection;
}
