// Copyright 2022, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@cs.uni-freiburg.de>
//         Johannes Kalmbach <kalmbach@cs.uni-freiburg.de>

#ifndef CELL_H_
#define CELL_H_

#include <cstddef>
#include <unordered_map>
#include <utility>

// A 2D cell with size_t coordinates.
class Cell {
 public:
  size_t row_;
  size_t col_;

  Cell(size_t row, size_t col) : row_{row}, col_{col} {}
  // To store the Cells in a hashMap we have
  // to define the equality == between Cells.
  bool operator==(const Cell &other) const {
    return row_ == other.row_ && col_ == other.col_;
  }
};

// We use Cell as the key type to a hash map, so we have to define
// a hash function
template <> struct std::hash<Cell> {
  size_t operator()(const Cell &p) const {
    auto hash1 = std::hash<size_t>{}(p.row_);
    auto hash2 = std::hash<size_t>{}(p.col_);
    if (hash1 != hash2) {
      return hash1 ^ hash2;
    }
    // If hash1 == hash2, their XOR is zero.
    return hash1;
  }
};

#endif  // CELL_H_
