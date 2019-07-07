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
 * @file CircuiSolver.h
 * @author Víctor A. Auñón <hola@victoraunon.com>
 *
 * @section DESCRIPTION
 * This file includes the declaration of the functions and variables required
 * to build and solve the circuit.
 */

#include <iostream>
#include <vector>
#include <iterator>
#include <ctime>
#include <fstream>
#include "pugixml.hpp"
#include "LinearSystemSolver.h"

/*!
 * \brief An electric mesh.
 *
 * A class that represents the electric mesh
 */
class Mesh {

    private:
        std::string ID = "";                    // The mesh identifier
        double m_powerSource = 0;               // The total mesh voltage  (V)
        double m_impedance = 0;                 // The total mesh impedance (Ω)
        double m_current = 0;                   // The resulting mesh current (A)
        std::vector<std::string> m_branchesIDs; // Vector which stores the IDs of the mesh branches

    public:
        /*!
        * \brief Default constructor.
        *
        * Initialises the mesh.
        * 
        * \param t_ID   ID assigned to the mesh
        * \param t_mesh The XML node that defines the mesh
        */
        Mesh(std::string t_ID, pugi::xml_node t_mesh);
        
        /*!
        * \brief Destructor.
        *
        * Destructs the mesh.
        * 
        * Note: we are not going to call this destructor since the meshes are not going to be removed.
        */
        ~Mesh();

        /*!
        * \brief Function to read the mesh elements defined in the XML node.
        * 
        * \param t_mesh The XML node that defines the mesh.
        */
        void readElements(pugi::xml_node t_mesh);


        /*!
        * \brief Function that returns the mesh voltage.
        * 
        * \return The mesh voltage
        */
        double getPowerSource() {
            return m_powerSource;
        };

        /*!
        * \brief Function that returns IDs of the branches in the mesh
        * 
        * \return The IDs of the branches in the mesh
        */
        std::vector<std::string> getBranchesIDs() {
            return m_branchesIDs;
        };

        /*!
        * \brief Function that returns the mesh impedance
        * 
        * \return The mesh impedance
        */
        double getImpedance() {
            return m_impedance;
        };

        /*!
        * \brief Function that returns the mesh ID
        * 
        * \return The mesh ID
        */
        std::string getID() {
            return ID;
        }

        /*!
        * \brief Function that assign the already calculated mesh current.
        */
        void setCurrent(double t_current) {
            m_current = t_current;
        }

        /*!
        * \brief Function that returns the mesh current
        * 
        * \return The mesh current
        */
        double getCurrent() {
            return m_current;
        }

};


/*!
 * \brief An electric branch.
 *
 * An struct which defines a branch. A branch is a set of elements in a mesh portion
 * between two consecutive nodes.
 */
struct Branch {
    std::string ID;                         // The branch ID
    double current;                         // The resulting branch current (A)
    double branchImpedance;                 // The branch impedance (Ω)
    std::vector<std::string> impedanceIDs;  // The IDs of the impedances in the branch
    std::vector<double> impedances;         // The values of the impedances in the branch (Ω)
    std::vector<double> powerDissipated;    // The resulting power dissipated by each impedance of the branch (W)
};

std::vector<Mesh> meshesVector;     // The vector of meshes

std::vector<Branch> branchesVector; // The vector of branches

/*!
 * \brief A linear equations system.
 *
 * An struct which defines the equations system required to calculate the current at each mesh.
 * The system to solve is R x I = V, where:
 * R is the impedance matrix of the circuit,
 * I is the vector of mesh currents and
 * V is the vector of mesh voltages
 */
struct System {
    std::vector<std::vector<double>> impedanceMatrix;   // The impedance matrix of the circuit (Ω)
    std::vector<double> voltages;                       // The vector of mesh voltages (V)
};

/*!
* \brief Function that returns the linear equations system to be solved.
* 
* \param t_meshesVector The vector of meshes
* \param t_branchesVector The vector of branches 
* 
* \return the linear equations system struct (impedance matrix and vector of voltages)
*/
System createSystem(std::vector<Mesh> &t_meshesVector, std::vector<Branch> &t_branchesVector);

/*!
* \brief Function that assigns the already calculated currents to each mesh and branch.
* 
* It also calculates the power dissipated by each impedance.
* 
* \param t_meshesVector The vector of meshes
* \param t_branchesVector The vector of branches
* \param t_currents The vector of meshes current (A)
*/
void setCurrents(std::vector<Mesh> &t_meshesVector, std::vector<Branch> &t_branchesVector,
    std::vector<double> &t_currents);

/*!
* \brief Function that save the results into a text file.
* 
* \param t_meshesVector The vector of meshes
* \param t_branchesVector The vector of branches
* \param t_fileName The name of the file where the results are written on
*/
void saveToFile(std::vector<Mesh> &t_meshesVector, std::vector<Branch> &t_branchesVector,
     std::string &t_fileName);



