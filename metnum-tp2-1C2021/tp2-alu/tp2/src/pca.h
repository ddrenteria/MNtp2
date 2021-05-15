#pragma once
#include "types.h"
#include "knn.h"

class PCA {
public:
    PCA(unsigned int n_components, unsigned int num_iter, double epsilon);

    void fit(Matrix X);

    Eigen::MatrixXd transform(Matrix X);
private:
    unsigned int _alpha;

    unsigned int _num_iter;

    double _epsilon;
    
    Matrix _eigenvectors_tc;
};
