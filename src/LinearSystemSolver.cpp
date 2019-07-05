#include "LinearSystemSolver.h"

using namespace std;

LU LUdecomposition(vector<vector<double>> &matrix) {
    /* Doolittle Algorithm */

    // Get A size
    int dim = matrix[0].size();

    // Initialize L and U matrices
    vector<vector<double>> L(dim);
    vector<vector<double>> U(dim);
    for (int i = 0; i < dim; i++) {
        L[i].resize(dim);
        U[i].resize(dim);
    }

    // Decomposing matrix into Upper and Lower
    // triangular matrix
    for (int i = 0; i < dim; i++)
    {
        // Upper Triangular
        for (int k = i; k < dim; k++)
        {
            // Summation of L[i][j] * U[j][k]
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += (L[i][j] * U[j][k]);
  
            // Evaluating U[i][k]
            U[i][k] = matrix[i][k] - sum;
        }
  
        // Lower Triangular 
        for (int k = i; k < dim; k++)
        {
            if (i == k)
                L[i][i] = 1; // Diagonal as 1
            else
            {
                // Summation of L[k][j] * U[j][i]
                double sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (L[k][j] * U[j][i]);
  
                // Evaluating L[k][i]
                L[k][i] = (matrix[k][i] - sum) / U[i][i];
            }
        }
    }
    return {L, U};
}

vector<double> solveSystem(vector<vector<double>> &impedanceMatrix, vector<double> &voltages) {
    
    LU L_U = LUdecomposition(impedanceMatrix);

    // Solve the system L*Y = B, where L is the lower diagonal matrix
    int dim = L_U.L.size();
    vector<double> Y(dim);
    // The Y0 element is calculated directly
    Y[0] = voltages[0] / L_U.L[0][0];
    for(int i = 1; i < dim; i++){
        double substract = 0;
        for (int j = i - 1; j >= 0; j--){
            substract -= Y[j] * L_U.L[i][j];
        }
        Y[i] = (voltages[i] + substract) / L_U.L[i][i];
    }

    // Solve the system U*X = Y, where U is the upper diagonal matrix
    vector<double> X(dim);
    // The Xn element is calculated directly
    X[dim-1] = Y[dim-1] / L_U.U[dim-1][dim-1];
    for(int i = dim - 2; i >= 0; i--){
        double substract = 0;
        for (int j = i + 1; j < dim; j++){
            substract -= X[j] * L_U.U[i][j];
        }
        X[i] = (Y[i] + substract) / L_U.U[i][i];
    }
    return X;
}