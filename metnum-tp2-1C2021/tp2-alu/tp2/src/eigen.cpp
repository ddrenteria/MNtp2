#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include "eigen.h"

using namespace std;

bool stoppingCriteria(const Vector& old_b, const Vector& b, const Matrix& A, double epsilon) {
    return old_b.isApprox(b, epsilon);
}

// bool stoppingCriteria2(const Vector& old_b, const Vector& b, const Matrix& A,
//                        double eigenvalue, double epsilon) {
//     return (A*b).isApprox(eigenvalue*b, epsilon);
// }

pair<double, Vector> power_iteration(const Matrix& X, unsigned num_iter, double eps) {
    bool correctEigenValue = false;
    double eigenvalue;
    Vector b;

    while(!correctEigenValue) {
        b = Vector::Random(X.cols());

        unsigned i = 0;
        for (i = 0; i < num_iter; i++) {
            Vector b_old = b;
            Vector Xb = X*b;
            b = Xb/ (Xb.lpNorm<2>());
            if (stoppingCriteria(b_old, b, X, eps)) {
                break;
            }
        }

        eigenvalue = (b.transpose()* X*b / (b.transpose() * b))(0);
        
        // validate eigenvalue
        if ((X*b).isApprox(eigenvalue*b, eps)) correctEigenValue = true;
        cout << "no sale" << endl;
    }

    return make_pair(eigenvalue, b /* / b.norm() */);
}

pair<Vector, Matrix> get_first_eigenvalues(const Matrix& X, unsigned alpha, unsigned num_iter, double epsilon) {
    Matrix A(X);
    Vector eigenvalues(alpha);
    Matrix eigenvectors(A.rows(), alpha);
    // code to print autovectors and autovalues
    // ofstream value_file;
    // value_file.open("eigenvalues_save.csv");
    // ofstream vectors_file;
    // vectors_file.open("eigenvectors_save.csv");


    for (unsigned i = 0; i < alpha; i++) {
        pair<double, Vector> pair = power_iteration(A, num_iter, epsilon);
        A = A-(pair.first * (pair.second * pair.second.transpose()));
        
        eigenvalues(i) = pair.first;
        eigenvectors.col(i) = pair.second;
        cout << i << " " << pair.first << endl;
        // value_file << pair.first << endl;
        // vectors_file << pair.second.transpose() << endl;
    }
    
    // value_file.close();
    // vectors_file.close();
    return make_pair(eigenvalues, eigenvectors);
}
