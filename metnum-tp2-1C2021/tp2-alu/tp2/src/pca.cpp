#include <iostream>
#include "pca.h"
#include "eigen.h"

using namespace std;


PCA::PCA(unsigned int n_components, unsigned int num_iter, double epsilon):_alpha(n_components), _num_iter(num_iter), _epsilon(epsilon) {
}

void PCA::fit(Matrix X) {
    Vector mu = X.colwise().mean();
    Matrix Xm = (X.rowwise() - mu.transpose())/ sqrt(X.rows() - 1);
    Matrix Mx = Xm.transpose() * Xm;

    _eigenvectors_tc = get_first_eigenvalues(Mx, _alpha, _num_iter, _epsilon).second;
}   


MatrixXd PCA::transform(Matrix X) {
    return X*_eigenvectors_tc;
}
