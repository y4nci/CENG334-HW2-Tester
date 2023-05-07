/**
 * @file matrix.cpp
 * @brief This file contains the definition of matrix-related functions.
 */

#include "matrix.h"

Matrix::Matrix() {
    this->matrixValues = MatrixVector();
}

Matrix::Matrix(unsigned row, unsigned column, int value) {
    this->matrixValues = MatrixVector(row, std::vector<int>(column, value));
}

Matrix::Matrix(unsigned row, unsigned column, MatrixVector values) {
    this->matrixValues = values;
}

Matrix::~Matrix() {
    this->matrixValues.clear();
}

std::ostream & operator << (std::ostream &out, const Matrix& matrix) {
    for (unsigned r = 0; r < matrix.matrixValues.size(); r++) {
        for (unsigned c = 0; c < matrix.matrixValues[r].size(); c++) {
            out << matrix.matrixValues[r][c] << " ";
        }
        out << std::endl;
    }

    return out;
}

Matrix Matrix::operator*(const Matrix& rhs) {
    size_t xRowCount = this->matrixValues.size();
    size_t xColumnCount = rhs.matrixValues[0].size();
    size_t yColumnCount = rhs.matrixValues.size();
    Matrix result = Matrix(xRowCount, yColumnCount, 0);

    for (unsigned r = 0; r < xRowCount; r++) {
        for (unsigned c = 0; c < yColumnCount; c++) {
            for (unsigned i = 0; i < xColumnCount; i++) {
                result.matrixValues[r][c] += this->matrixValues[r][i] * rhs.matrixValues[i][c];
            }
        }
    }

    return result;
}

Matrix& Matrix::operator=(const Matrix& matrix) {
    this->matrixValues = matrix.matrixValues;
    return *this;
}

bool Matrix::operator==(const Matrix& rhs) {
    if (this->matrixValues.size() != rhs.matrixValues.size()) {
        return false;
    }

    for (unsigned r = 0; r < this->matrixValues.size(); r++) {
        if (this->matrixValues[r].size() != rhs.matrixValues[r].size()) {
            return false;
        }

        for (unsigned c = 0; c < this->matrixValues[r].size(); c++) {
            if (this->matrixValues[r][c] != rhs.matrixValues[r][c]) {
                return false;
            }
        }
    }

    return true;
}

Matrix generateRandomMatrix(unsigned rowCount, unsigned columnCount) {
    MatrixVector matrixValues;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 100);

    for (int r = 0; r < rowCount; r++) {
        std::vector<int> rowValues;
        for (int c = 0; c < columnCount; c++) {
            rowValues.push_back(dist(mt));
        }
        matrixValues.push_back(rowValues);
    }

    return {rowCount, columnCount, matrixValues};
}

std::vector<Matrix>* generateRandomMatrices(unsigned matrixCount, int minDimension, int maxDimension) {
    std::vector<Matrix>* matrices = new std::vector<Matrix> [matrixCount];

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist;

    if (minDimension <= 0 || maxDimension <= 0 || minDimension > maxDimension) {
        return matrices;
    }

    dist = std::uniform_int_distribution<int>(minDimension, maxDimension);

    for (int i = 0; i < matrixCount; i++) {
        int rowCount = dist(mt);
        int columnCount = dist(mt);
        matrices->push_back(generateRandomMatrix(rowCount, columnCount));
    }

    return matrices;
}
