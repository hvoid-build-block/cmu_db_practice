//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>

namespace bustub {

/*
 * The base class defining a Matrix
 */

template <typename T>
class Matrix {
 protected:
  // TODO(P0): Add implementation
  Matrix(int r, int c) {
    rows = r;
    cols = c;
    linear = new T[r * c]{};
  }

  // # of rows in the matrix
  int rows;
  // # of Columns in the matrix
  int cols;
  // Flattened array containing the elements of the matrix
  // TODO(P0) : Allocate the array in the constructor. Don't forget to free up
  // the array in the destructor.
  T *linear;

 public:
  // Return the # of rows in the matrix
  virtual int GetRows() = 0;

  // Return the # of columns in the matrix
  virtual int GetColumns() = 0;

  // Return the (i,j)th  matrix element
  virtual T GetElem(int i, int j) = 0;

  // Sets the (i,j)th  matrix element to val
  virtual void SetElem(int i, int j, T val) = 0;

  // Sets the matrix elements based on the array arr
  virtual void MatImport(T *arr) = 0;

  // TODO(P0): Add implementation
  virtual ~Matrix() { delete[] linear; }
};

template <typename T>
class RowMatrix : public Matrix<T> {
 public:
  // TODO(P0): Add implementation
  RowMatrix(int r, int c) : Matrix<T>(r, c) {}

  // TODO(P0): Add implementation
  int GetRows() override { return this->rows; }

  // TODO(P0): Add implementation
  int GetColumns() override { return this->cols; }

  // TODO(P0): Add implementation
  T GetElem(int i, int j) override { return *(this->linear + i * this->cols + j); }

  // TODO(P0): Add implementation
  void SetElem(int i, int j, T val) override { *(this->linear + i * this->cols + j) = val; }

  // TODO(P0): Add implementation
  void MatImport(T *arr) override {
    for (int i = 0; i < this->rows; i++) {
      for (int j = 0; j < this->cols; j++) {
        this->SetElem(i, j, *(arr + i * this->cols + j));
      }
    }
  }

  // TODO(P0): Add implementation
  ~RowMatrix() override = default;

 private:
  // 2D array containing the elements of the matrix in row-major format
  // TODO(P0): Allocate the array of row pointers in the constructor. Use these pointers
  // to point to corresponding elements of the 'linear' array.
  // Don't forget to free up the array in the destructor.
  T **data_;
};

template <typename T>
class RowMatrixOperations {
 public:
  // Compute (mat1 + mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> AddMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                   std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    int mat1Rows = mat1->GetRows();
    int mat1Cols = mat1->GetColumns();
    int mat2Rows = mat2->GetRows();
    int mat2Cols = mat2->GetColumns();

    std::unique_ptr<RowMatrix<T>> result_ptr{new RowMatrix<T>(mat1Rows, mat1Cols)};

    if (!(mat1Rows == mat2Rows && mat1Cols == mat2Cols)) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    for (int i = 0; i < mat1Rows; i++) {
      for (int j = 0; j < mat1Cols; j++) {
        result_ptr->SetElem(i, j, mat1->GetElem(i, j) + mat2->GetElem(i, j));
      }
    }

    return result_ptr;
  }

  // Compute matrix multiplication (mat1 * mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> MultiplyMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                        std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
    int mat1Rows = mat1->GetRows();
    int mat1Cols = mat1->GetColumns();
    int mat2Rows = mat2->GetRows();
    int mat2Cols = mat2->GetColumns();
    std::unique_ptr<RowMatrix<T>> result_ptr{new RowMatrix<int>(mat1Rows, mat2Cols)};

    if (!(mat1Rows == mat2Cols && mat1Cols == mat2Rows)) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    for (int i = 0; i < mat1Rows; i++) {
      for (int j = 0; j < mat2Cols; j++) {
        T tmp = 0;
        for (int c = 0; c < mat2Rows; c++) {
          T v1 = mat1->GetElem(i, c);
          T v2 = mat2->GetElem(c, j);
          tmp += v1 * v2;
        }
        result_ptr->SetElem(i, j, tmp);
      }
    }
    return result_ptr;
  }

  // Simplified GEMM (general matrix multiply) operation
  // Compute (matA * matB + matC). Return nullptr if dimensions mismatch for input matrices
  static std::unique_ptr<RowMatrix<T>> GemmMatrices(std::unique_ptr<RowMatrix<T>> matA,
                                                    std::unique_ptr<RowMatrix<T>> matB,
                                                    std::unique_ptr<RowMatrix<T>> matC) {
    // TODO(P0): Add code
    std::unique_ptr<RowMatrix<T>> tmp = MultiplyMatrices(matA, matB);
    if (!tmp) {
      return tmp;
    }
    tmp = AddMatrix(matC, tmp);
    return tmp;
  }
};
}  // namespace bustub
