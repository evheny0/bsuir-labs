#ifndef MARTIX_H
#define MARTIX_H

#include <cstdlib>
#include <iostream>
#include <cstdio>


template <size_t size, class T>
class Matrix {
    // T data[size][size];
    T **data;
public:
    Matrix()
    {
        data = new T**;
    }
    ~Matrix() 
    {
        if (data != nullptr) {
            for (int i; i < size; i++) {
                delete data[i];
            }
            delete[] data;
        }
    }

    Matrix(const Matrix<size, T>&) = delete;
    Matrix(Matrix<size, T>&& rhs)
    {
        data = rhs.data;
        rhs.data = nullptr;
    }

    T at(size_t x, size_t y)
    {
        return data[x][y];
    }

    T set_at(size_t x, size_t y, T value)
    {
        return data[x][y] = value;
    }

    Matrix operator * (const Matrix<size, T> &other_matrix)
    {
        Matrix result;
        for (unsigned i = 0; i < size; ++i) {
            for (unsigned j = 0; j < size; ++j) {
                for (unsigned k = 0; k < size; ++k) {
                    result->data[i][j] = result->data[i][j] + data[i][k] * other_matrix.data[k][j]; 
                }
            }
        }
        return result;
    }

    // Matrix operator + (const Matrix<size, T> &other_matrix)
    // {
    //     Matrix *result = new Matrix();
    //     for(unsigned i = 0; i < size; i++) {
    //         for(unsigned j = 0; j < size ; j++) {
    //             result->data[i][j] = result->data[i][j] + mat2(i, j); 
    //         }
    //     }
    //     return (*temp);
    // }



    void print()
    {
        for (unsigned i = 0; i < size; ++i) {
            for (unsigned j = 0; j < size; ++j) {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void fill_with_rand()
    {
        for (unsigned i = 0; i < size; ++i) {
            for (unsigned j = 0; j < size; ++j) {
                data[i][j] = 3;
            }
        }
    }

    void fill_with_rand_matrices()
    {
        for (unsigned i = 0; i < size; ++i) {
            for (unsigned j = 0; j < size; ++j) {
            }
        }
    }
};

#endif // MARTIX_H
