/* --------------------------------------------------------------------------------*\
Copyright 2019 Víctor A Auñón

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. 
\*--------------------------------------------------------------------------------*/

/**
 * @file LinearSystemSolver.h
 * @author Víctor A. Auñón <hola@victoraunon.com>
 *
 * @section DESCRIPTION
 * This file includes the declaration of the functions and variables required
 * to solve the system of linear equations which defines the Ohm's law (V = I x R)
 */

#pragma once
#include <iostream>
#include <vector>


/*!
 * \brief The LU decomposition of an squared matrix.
 *
 * An struct which defines the two matrices of the LU decomposition
 * L is the lower triangular matrix
 * U is the upper triangular matrix
 */
struct LU {
    std::vector<std::vector<double>> L; // The lower triangular matrix
    std::vector<std::vector<double>> U; // The upper triangular matrix
};

/*!
* \brief Function that returns the LU decomposition of an squared matrix.
* 
* \param t_Matrix The input square matrix
* 
* \return the LU decomposition struct (L and U matrices)
*/
LU LUdecomposition(std::vector<std::vector<double>> &t_Matrix);

/*!
* \brief Function that returns the mesh currents vector.
* 
* It solves the V = I x R system of linear equations by calling the LU decomposition
* function of the R matrix:
* L x Y = Voltages
* U x Currents = Y
* 
* \param t_impedanceMatrix The circuit impedance matrix, R
* \param t_voltages The circuit voltages, V

* \return the resulting currents vector
*/
std::vector<double> solveSystem(std::vector<std::vector<double>> &t_impedanceMatrix,
    std::vector<double> &t_voltages);
