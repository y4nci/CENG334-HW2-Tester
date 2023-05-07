/**
 * @file matrix.h
 * @brief This file contains the declaration of matrix-related functions.
 */

#ifndef TESTER_HW2_MATRIX_H
#define TESTER_HW2_MATRIX_H

#include <iostream>
#include <random>

typedef std::vector<std::vector<int>> MatrixVector;

class Matrix {
private:
    MatrixVector matrixValues;
public:
    Matrix();
    Matrix(unsigned row, unsigned column, int value);
    Matrix(unsigned row, unsigned column, MatrixVector values);
    ~Matrix();

    // Operators
    friend std::ostream & operator << (std::ostream &out, const Matrix& matrix);
    std::vector<int> operator*(const Matrix& rhs);
    Matrix& operator=(const Matrix& matrix);
    bool operator==(const Matrix& rhs);
};

Matrix generateRandomMatrix(unsigned rowCount, unsigned columnCount);

#endif //TESTER_HW2_MATRIX_H
