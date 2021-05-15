#include <iostream>
#include <fstream>
#include <unistd.h> 
#include <vector>
#include "pca.h"
#include "eigen.h"

using namespace std;

#define KNN 0
#define PCA_KNN 1

// Convierte los datos del csv "input" a una matriz.
Matrix create_matrix(const std::string& input) {
    Matrix X;
    std::ifstream fileInput;
    fileInput.open(input);
    std::string row;
    int row_n = -1;
    int column_n;
    int max_column_n = 0;

    while(std::getline(fileInput, row)) {
        std::stringstream lineStream(row);
        std::string cell;
        if (row_n >= 0) {
            X.conservativeResize(row_n + 1, max_column_n);

            column_n = 0;

            while(std::getline(lineStream,cell,',')) {
                if (column_n >= 0) {
                    X(row_n, column_n) = std::stoi(cell);
                }
                column_n++;
            }
        } else {
            while(std::getline(lineStream,cell,',')) {
                max_column_n++;
            }
        }
        row_n++;
    }
    return X;
}

void write_classification(string pathname, Vector results) {

}

int main(int argc, char** argv){
    // poder correr knn y pca knn con k y alpha como parametros
    int c;
    string train_filename, test_filename, output_filename;
    int alpha, k, method;

    while ((c = getopt(argc, argv, "m:i:q:o:")) != -1) {
        switch (c) {
            case 'm': 
                method = stoi(optarg);
                break;
            case 'i': 
                train_filename = optarg;
                break;
            case 'q':
                test_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break; 
            case 'a':
                alpha = stoi(optarg);
                break;
            case 'k':
                k = stoi(optarg);
                break;   
        }
    }
    
    Matrix train_set_matrix = create_matrix(train_filename);
    Matrix solution_train_data = train_set_matrix.col(0);
    Matrix read_train_data = train_set_matrix.block(0, 1, train_set_matrix.rows(), train_set_matrix.cols() - 1);
    Matrix read_test_data = create_matrix(test_filename);

    switch (method) {
        case KNN: {
            KNNClassifier classifier = KNNClassifier(k);
            classifier.fit(read_train_data, solution_train_data);
            Vector result = classifier.predict(read_test_data);
            // write(result, output_filename);
            break;
        }
        case PCA_KNN: {
            PCA transformer = PCA(alpha, 1000, 1e-16);
            transformer.fit(read_train_data);
            MatrixXd transformed_train_data = transformer.transform(read_train_data);
            MatrixXd transformed_test_data = transformer.transform(read_test_data);

            KNNClassifier classifier = KNNClassifier(k);
            classifier.fit(transformed_train_data, solution_train_data);
            Vector result = classifier.predict(read_test_data);
            // write(result, output_filename);
            break;
        }
    }

    return 0;
}
