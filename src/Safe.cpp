/*
 * Created by Aurelien Chagnon
 */

#include "../headers/Safe.h"

/// Max number of mRows/mColumns
const uint32_t C_MAX_LENGTH = 1000000u;

Safe::Safe(const uint32_t nbRows, const uint32_t nbColumns) : mRows(nbRows), mColumns(nbColumns), mMirrors(){
    /// Check validity of the number of mRows/mColumns
    if(nbRows > C_MAX_LENGTH || nbColumns > C_MAX_LENGTH || nbRows < 1u || nbColumns < 1u)
        std::cerr << "Invalid number of row/column: expected between 1 and 1000000, got " << nbRows << " and " <<
                  nbColumns << " instead." << std::endl;
}

void Safe::setColumns(const uint32_t newColumns) {
    /// Check validity of the number of columns
    if(newColumns > C_MAX_LENGTH || newColumns < 1u)
        std::cerr << "Invalid number of column: expected between 1 and 1000000, got " << newColumns << " instead." <<
                  std::endl;
    mColumns = newColumns;  ///< Change the number of columns in the Safe

    /// Remove mirrors that are outside the safe
    mMirrors.remove_if([newColumns](Mirror mirr){ return mirr.column() > newColumns; });
}

void Safe::setRows(const uint32_t newRows) {
    /// Check validity of the number of rows
    if(newRows > C_MAX_LENGTH || newRows < 1u)
        std::cerr << "Invalid number of row/column: expected between 1 and 1000000, got " << newRows << " instead." <<
                  std::endl;
    mRows = newRows;  ///< Change the number of rows in the Safe

    /// Remove mirrors that are outside the safe
    mMirrors.remove_if([newRows](Mirror mirr){ return mirr.row() > newRows; });
}

void Safe::setContext(const uint32_t contextRows, const uint32_t contextColumns) {
    setRows(contextRows);  ///< Change the number of rows in the Safe
    setColumns(contextColumns);  ///< Change the number of columns in the Safe
}

std::list<Mirror> Safe::mirrors() const{
    return mMirrors;  ///< Return the list of mirrors inside the Safe
}

uint32_t Safe::rows() const {
    return mRows;  ///< Return the number of rows in the Safe
}

uint32_t Safe::columns() const {
    return mColumns;  ///< Return the number of columns in the Safe
}

void Safe::clearMirrors() {
    mMirrors.clear();  ///< Clear the mirror list, now empty
}

void Safe::addMirror(const uint32_t rowPosition, const uint32_t columnPosition, const Mirror::emirrorKind kind) {
    if(rowPosition <= mRows && columnPosition <= mColumns && rowPosition > 0u && columnPosition > 0u) {
        /// Create a new Mirror and push it back if the mirror is inside the Safe
        mMirrors.emplace_back(rowPosition, columnPosition, kind);
    }
    else
        /// The mirror is outside the Safe, it is not created nor added to the list
        std::cerr << "New mirror (" << rowPosition << ", " << columnPosition <<
                  ") is outside the Safe, mirror not added !" << std::endl;
}
