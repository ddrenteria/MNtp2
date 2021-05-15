#include <algorithm>
#include <chrono>
#include <iostream>
#include "eigen.h"

using namespace std;


pair<double, Vector> power_iteration(const Matrix& X, unsigned num_iter, double eps) {
    bool correctEigenValue = false;
    double eigenvalue;
    Vector b;

    while(!correctEigenValue) {
        b = Vector::Random(X.cols());

        for (unsigned i = 0; i < num_iter; i++) {
            // TO DO: Agregar un metodo de parada
            Vector Xb = X*b;
            b = Xb/ (Xb.lpNorm<2>());
        }

        eigenvalue = (b.transpose()* X*b / (b.transpose() * b))(0);
        
        // validate eigenvalue
        if ((X*b).isApprox(eigenvalue*b, eps)) correctEigenValue = true;
    }
    cout << "eigen " << b << endl;
    cout << "X " << X << endl;

    return make_pair(eigenvalue, b /* / b.norm() */);
}

pair<Vector, Matrix> get_first_eigenvalues(const Matrix& X, unsigned alpha, unsigned num_iter, double epsilon) {
    Matrix A(X);
    Vector eigenvalues(alpha);
    Matrix eigenvectors(A.rows(), alpha);

    for (unsigned i = 0; i < alpha; i++) {
        pair<double, Vector> pair = power_iteration(A, num_iter, epsilon);
        A = A-(pair.first * (pair.second * pair.second.transpose()));
        cout << "resta" << (pair.second * pair.second.transpose()) << endl;
        
        eigenvalues(i) = pair.first;
        eigenvectors.col(i) = pair.second;
    }
    
    return make_pair(eigenvalues, eigenvectors);
}
