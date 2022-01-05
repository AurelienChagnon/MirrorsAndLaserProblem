/*
 * Created by Aurelien Chagnon
 */

#ifndef SAFEANDMIRRORSPROBLEM_SAFE_H
#define SAFEANDMIRRORSPROBLEM_SAFE_H

#include <list>
#include <iostream>
#include "Mirror.h"

/**
 * Description of a safe of size rows*columns containing several mirrors.
 *
 * Constraints: 1 <= rows, columns <= 1000000.
 * The safe can be changed during its lifetime and will readjust itself automatically to keep logical coherency:
 * all of its mirrors should be inside of the safe itself.
 */
class Safe {

public:
    /**
     * Construct a Safe using the number of rows and columns. The list of mirrors is empty.
     *
     * The list of mirrors should be filled seperatedly using the addMirror method.
     *
     * @param nbRows, nbColumns: number of Rows and Columns composing the safe
     */
    explicit Safe(uint32_t nbRows = 1u, uint32_t nbColumns = 1u);

    /**
     * Set the number of Rows composing the Safe.
     *
     * Erase mirrors that are now outside the safe if necessary.
     *
     * @param newRows: new number of Rows
     */
    void setRows(uint32_t newRows);

    /**
     * Set the number of Columns composing the Safe.
     *
     * Erase mirrors that are now outside the safe if necessary.
     *
     * @param newColumns: new number of Columns
     */
    void setColumns(uint32_t newColumns);

    /**
     * Set the number of Rows and Columns composing the Safe.
     *
     * Erase mirrors that are now outside the safe if necessary.
     *
     * @param contextRows, contextColumns: new number of Rows and Columns
     */
    void setContext(uint32_t contextRows, uint32_t contextColumns);

    /**
     * Clear (emtpy) the list of mirror in the Safe
     */
    void clearMirrors();

    /**
     * Creates and add a mirror in the list of mirror only if it is inside the safe.
     *
     * @param row, column: position of the new mirror
     * @param kind: kind of the new mirror.
     */
    void addMirror(uint32_t row, uint32_t column, Mirror::emirrorKind kind);

    /**
     * Retrieve the number of rows composing the Safe
     *
     * @return number of rows
     */
    [[nodiscard]] uint32_t rows() const;

    /**
     * Retrieve the number of columns composing the Safe
     *
     * @return number of columns
     */
    [[nodiscard]] uint32_t columns() const;

    /**
     * Retrieve the list of mirror in the Safe
     *
     * @return list of mirror in the Safe
     */
    [[nodiscard]] std::list<Mirror> mirrors() const;

private:
    /// Number of Rows and Columns in the 2D Safe
    uint32_t mRows, mColumns;

    /// List of Mirrors in the Safe.
    std::list<Mirror> mMirrors;

};


#endif //SAFEANDMIRRORSPROBLEM_SAFE_H
