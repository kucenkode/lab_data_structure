#include <iostream>
#include <complex>
#include <cblas.h>
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Функция для оценки сложности алгоритма
double calculateComplexity(int n) {
    return 2 * pow(n, 3);
}

// Функция для оценки производительности в MFlops
double calculatePerformance(double complexity, double timeInSeconds) {
    return complexity / (timeInSeconds * 1e-6);
}

// Перемножение матриц с простым алгоритмом
void matrix_multiply(const vector<vector<complex<float>>>& A,
    const vector<vector<complex<float>>>& B,
    vector<vector<complex<float>>>& result) {
    int N = A.size();
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = complex<float>(0.0, 0.0);
            for (int k = 0; k < N; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Перемножение матриц с блочной структурой
void multiplyMatricesOptimized(const vector<vector<complex<float>>>& A,
    const vector<vector<complex<float>>>& B,
    vector<vector<complex<float>>>& C) {
    int n = static_cast<int>(A.size());
    int block_size = 32; // Размер блока

#pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < n; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int k = 0; k < n; k += block_size) {
                for (int ii = i; ii < min(i + block_size, n); ++ii) {
                    for (int jj = j; jj < min(j + block_size, n); ++jj) {
                        complex<float> sum(0.0, 0.0);
                        for (int kk = k; kk < min(k + block_size, n); ++kk) {
                            sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] += sum;
                    }
                }
            }
        }
    }
}

// Сравнение матриц
bool compare_matrices(const vector<vector<complex<float>>>& A,
    const vector<vector<complex<float>>>& B,
    float epsilon = 1e-2f) {
    int rows = A.size();
    int cols = A[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (fabs(A[i][j].real() - B[i][j].real()) > epsilon || fabs(A[i][j].imag() - B[i][j].imag()) > epsilon) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");
    const int N = 500;

    // Создание и заполнение матриц
    vector<vector<complex<float>>> A(N, vector<complex<float>>(N));
    vector<vector<complex<float>>> B(N, vector<complex<float>>(N));
    vector<vector<complex<float>>> C1(N, vector<complex<float>>(N, complex<float>(0.0, 0.0)));
    vector<vector<complex<float>>> C2(N, vector<complex<float>>(N, complex<float>(0.0, 0.0)));
    vector<vector<complex<float>>> C3(N, vector<complex<float>>(N, complex<float>(0.0, 0.0)));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            complex<float> value((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
            A[i][j] = value;
            B[i][j] = value;
        }
    }

    cout << "A[10][20] = " << A[10][20] << "\n";
    cout << "B[20][10] = " << B[20][10] << "\n";

    float alpha[] = { 1.0, 0.0 };
    float beta[] = { 0.0, 0.0 };

    // Перемножение матриц с использованием OpenBLAS
    vector<float> A_flat(2 * N * N);
    vector<float> B_flat(2 * N * N);
    vector<float> C_flat(2 * N * N, 0.0);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A_flat[2 * (i * N + j)] = A[i][j].real();
            A_flat[2 * (i * N + j) + 1] = A[i][j].imag();
            B_flat[2 * (i * N + j)] = B[i][j].real();
            B_flat[2 * (i * N + j) + 1] = B[i][j].imag();
        }
    }

    auto start = high_resolution_clock::now();
    cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, alpha, A_flat.data(), N, B_flat.data(), N, beta, C_flat.data(), N);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count();
    cout << "Время выполнения умножения матриц с использованием cblas_cgemm: " << duration / 1e6 << " секунд.\n";
    cout << "Достигнутая производительность: " << calculatePerformance(2.0 * N * N * N, duration) << " MFlops.\n";

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C1[i][j] = complex<float>(C_flat[2 * (i * N + j)], C_flat[2 * (i * N + j) + 1]);
        }
    }

    cout << "C1[10][20] = " << C1[10][20] << "\n";

    // Перемножение матриц с использованием простого алгоритма
    start = high_resolution_clock::now();
    matrix_multiply(A, B, C2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start).count();
    cout << "Время выполнения умножения матриц с использованием matrix_multiply: " << duration / 1e6 << " секунд.\n";
    cout << "Достигнутая производительность: " << calculatePerformance(2.0 * N * N * N, duration) << " MFlops.\n";
    cout << "C2[10][20] = " << C2[10][20] << "\n";

    // Перемножение матриц с использованием блочного алгоритма
    start = high_resolution_clock::now();
    multiplyMatricesOptimized(A, B, C3);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start).count();
    cout << "Время выполнения умножения матриц с использованием multiplyMatricesOptimized: " << duration / 1e6 << " секунд.\n";
    cout << "Достигнутая производительность: " << calculatePerformance(2.0 * N * N * N, duration) << " MFlops.\n";
    cout << "C3[10][20] = " << C3[10][20] << "\n";

    // Проверка равенства матриц
    if (compare_matrices(C1, C2) && compare_matrices(C1, C3))
        cout << "Все матрицы равны.\n";
    else
        cout << "Матрицы не равны!\n";

    cout << endl;

    return 0;
}