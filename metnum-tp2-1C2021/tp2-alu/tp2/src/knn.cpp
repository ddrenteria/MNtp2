#include <algorithm>
//#include <chrono>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "knn.h"

using namespace std;

void increaseAppearences(map<double, int>& m, double category) {
    ( m.count(category) == 0) ? m[category] = 1 : m[category]++;
}

KNNClassifier::KNNClassifier(unsigned int n_neighbors): _k(n_neighbors) {
}

void KNNClassifier::fit(Matrix X, Matrix y) {
    _fit_instances = X;
    _fit_categories = y;
}


Vector KNNClassifier::predict(Matrix X) {
    // Creamos vector columna a devolver
    auto ret = Vector(X.rows());
    unsigned int k = min(_k, (unsigned int)_fit_instances.rows());

    for (unsigned i = 0; i < X.rows(); ++i) {
        vector<tuple<double,double>> indexesAndDistance;
        map<double,int> categoryCounter;

        // get the norms for each difference
        Vector distances = (_fit_instances.rowwise() - X.row((Eigen::Index)i)).rowwise().lpNorm<2>();
        double maxNorm = distances.maxCoeff();

        for (unsigned int j = 0; j < k; j++) {
            double index;
            double distance = distances.minCoeff(&index);
            increaseAppearences(categoryCounter, _fit_categories(index));
            indexesAndDistance.push_back(make_tuple(index, distance));
            distances(index) = maxNorm + 1;
        }
        auto max = max_element(categoryCounter.begin(), categoryCounter.end());
        ret(i) = max->first;
    }

    return ret;
}
