#pragma once
#include <iostream>
#include <vector>

struct LU {
    std::vector<std::vector<double>> L;
    std::vector<std::vector<double>> U;
};


LU LUdecomposition(std::vector<std::vector<double>> &t_Matrix);


std::vector<double> solveSystem(std::vector<std::vector<double>> &t_impedanceMatrix,
    std::vector<double> &t_voltages);
