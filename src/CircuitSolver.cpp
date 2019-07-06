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
 * @file CircuiSolver.cpp
 * @author Víctor A. Auñón <hola@victoraunon.com>
 *
 * @section DESCRIPTION
 * This file includes the implementation of the functions required
 * to build and solve the circuit.
 */

#include "CircuitSolver.h"

using namespace std;

Mesh::Mesh(string t_ID, pugi::xml_node t_mesh) {
    cout << "\nCreating mesh with ID: " << t_ID << endl;
    // Assign the mesh ID
    this->ID = t_ID;
    // Read the elements in this mesh
    this->readElements(t_mesh);
}


Mesh::~Mesh() {}


void Mesh::readElements(pugi::xml_node t_mesh) {
    // Create an iterator of branches
    vector<Branch>::iterator br;

    // Read the branches in this mesh
    for (auto branch : t_mesh.children("branch")) {
        if (branchesVector.size() == 0) {
            // Always push this branch if the branchesVector is empty
            branchesVector.emplace_back(Branch{branch.attribute("ID").as_string()});
        } else {
            // Look for this branch in the branchesVector
            bool ID_found = false;
            for (br = branchesVector.begin(); br != branchesVector.end(); br++) {
                if (br->ID == branch.attribute("ID").as_string())
                    ID_found = true;
            }
            // If this branch is not in the branchesVector yet, then push it
            if (!ID_found) {
                branchesVector.emplace_back(Branch{branch.attribute("ID").as_string()});
            }
        }
        // The branch ID is always attached to the the mesh
        this->m_branchesIDs.push_back(branch.attribute("ID").as_string());

        // Read the batteries in this branch
        for (auto element : branch.children("battery")) {
            // Update the mesh voltage
            this->m_powerSource += element.attribute("value").as_double();
            cout << "--> Found battery with ID: " << element.attribute("ID").as_string() << endl;
        }

        // Read the resistances in this branch
        for (auto element : branch.children("resistance")) {
            // Look for this branch in the branchesVector
            for (br = branchesVector.begin(); br != branchesVector.end(); br++) {
                if (br->ID == branch.attribute("ID").as_string()) {
                    if (br->impedanceIDs.size() == 0) {
                        // Always push this impedance if the impedanceIDs vector is empty
                        br->impedanceIDs.push_back(element.attribute("ID").as_string());
                        // Update the branch impedance
                        br->branchImpedance += element.attribute("value").as_double();
                        br->impedances.push_back(element.attribute("value").as_double());
                    } else {
                        // Look for this impedance in the impedanceIDs vector
                        for (size_t i = 0; i < br->impedanceIDs.size(); i++) {
                            if (br->impedanceIDs[i] != element.attribute("ID").as_string()) {
                                // Push this impedance to the branch
                                br->impedanceIDs.push_back(element.attribute("ID").as_string());
                                // Update the branch impedance
                                br->branchImpedance += element.attribute("value").as_double();
                                br->impedances.push_back(element.attribute("value").as_double());
                            }
                        }
                    }
                    // Update the mesh impedance
                    this->m_impedance += element.attribute("value").as_double();
                    cout << "--> Found impedance with ID: " << element.attribute("ID").as_string() << endl;
                }
            }
        }
    }
}


System createSystem(vector<Mesh> &mVector, vector<Branch> &bVector) {

    // Declare the impedance matrix and the voltages vector
    vector<double> voltages(mVector.size());
    vector<vector<double>> impedance_matrix(mVector.size());

    // Initialize values
    for (int i = 0; i < mVector.size(); i ++) {
        // Initialize impedance_matrix with zeros
        impedance_matrix[i].resize(mVector.size());
        for (int j = 0; j < mVector.size(); j ++) {
            impedance_matrix[i][j] = 0.0;
        }
        // Fill the voltages array
        voltages[i] = mVector[i].getPowerSource();
    }

    // Build the impedance_matrix
    // Fill the matrix diagonal with the total impedance of each mesh
    for (int i = 0; i < mVector.size(); i ++) {
        impedance_matrix[i][i] = mVector[i].getImpedance();
    }
    // Fill the rest of the element of the matrix
    for (int i = 0; i < mVector.size(); i ++) {
        // Complementaries indexes
        vector<int> ind;
        for (int n = 0; n < mVector.size(); n ++){
            if (n != i)
                ind.push_back(n);
        }
        for (int index : ind){
            string common_branch = "";
            // Find the common branch impedance (elements outside the diagonal)
            for (auto br_i : mVector[i].getBranchesIDs()) {
                for (auto br_n : mVector[index].getBranchesIDs()) {
                    if (br_n == br_i)
                        common_branch = br_n;
                }
            }
            // Look for the branch that matches the common_branch ID
            for (auto br : bVector) {
                if (common_branch == br.ID) {
                    // If a common branch exists, assign its impedance to the current element
                    impedance_matrix[i][index] -= br.branchImpedance;
                } else if (common_branch == "") {
                    // If there is not any common branch, assign a zero to the current element
                    impedance_matrix[i][index] -= 0.0;
                }
            }
        }
    }
    return {impedance_matrix, voltages};
}


void setCurrents(vector<Mesh> &mVector, vector<Branch> &bVector, vector<double> &currents){

    // Assign the current through each mesh
    for (int i = 0; i < mVector.size(); i++) {
        mVector[i].setCurrent(currents[i]);
    }

    // Calculate the current through each branch by taking the current of its corresponding mesh
    for(int i = 0; i < bVector.size(); i++) {
        for(int j = 0; j < currents.size(); j++) {
            for(auto br : mVector[j].getBranchesIDs()) {
                if (br == bVector[i].ID) {
                    if (bVector[i].current == 0) {
                        // If we are assigning the current from the first mesh in
                        // which this branch is included, then do not change the sign
                        bVector[i].current += currents[j];
                    } else {
                        // If we are assigning the current from other mesh in which
                        // this branch is included, then take the sign into account
                        if (currents[j] > 0.0)
                            bVector[i].current -= currents[j];
                        else
                            bVector[i].current += currents[j];
                    }
                }
            }
        }
        // Calculate dissipated powers in resistances
        for(int l = 0; l < bVector[i].impedances.size(); l++) {
            bVector[i].powerDissipated.push_back(pow( bVector[i].current, 2) * bVector[i].impedances[l]);
        }
    }
}


void saveToFile(vector<Mesh> &mVector, vector<Branch> &bVector, string &fileName) {

    ofstream results_file;
    // Open the results file to write on it
    results_file.open(fileName);

    // Write to file
    // Write meshes current
    results_file << "------------------" << endl;
    results_file << "----- Meshes -----" << endl;
    results_file << "------------------" << endl;
    for(auto mesh : mVector){
        results_file << "\nMesh with ID: " << mesh.getID() << ":" << endl;
        results_file << "--> Current: " << mesh.getCurrent() << " (A)" << endl;
    }

    // Write branches current
    results_file << "\n------------------" << endl;
    results_file << "---- Branches ----" << endl;
    results_file << "------------------" << endl;
    for(auto branch : bVector){
        results_file << "\nBranch with ID: " << branch.ID << ":" << endl;
        results_file << "--> Current: " << branch.current << " (A)" << endl;
        for(int i = 0; i < branch.impedanceIDs.size(); i++) {
            results_file << "--> Power dissipated in " << branch.impedanceIDs[i] << ": " 
                         << branch.powerDissipated[i] << " (W)" << endl;
       }
    }
    // Close the results file
    results_file.close();
}


int main(int argc, char *argv[]) {
    // Check if a circuit file has been provided as an argument
    if (argv[1] != nullptr) {
        string input_file = argv[1];

        // Check if it is a valid XML file
        if (input_file.substr(input_file.length() - 3) == "xml") {
            cout << "Reading circuit file: " << input_file << endl;

            // Read the input data file
            pugi::xml_document xml_file;
            auto res = xml_file.load_file(argv[1]);
            
            // Check if it was loaded
            if (res == false) {
                cout << "ERROR: There were problems loading " << input_file << endl;
                cout << "ERROR: " << res.description() << endl;
                cout << "Error offset: " << res.offset << endl;
                system("pause");
            } else {
                // Get the meshes XML node
                pugi::xml_node meshes_node = xml_file.child("meshes");

                // Read meshes
                for(auto mesh_node : meshes_node.children("mesh")) {
                    // Push this mesh to the meshesVector
                    meshesVector.push_back(Mesh(mesh_node.attribute("ID").as_string(), mesh_node));
                }

                cout << "\n" << "Solving circuit..." << endl;
                clock_t begin = clock();

                // Create the equation system
                System system_data = createSystem(meshesVector, branchesVector);

                // Solve the equation system
                vector<double> currents = solveSystem(system_data.impedanceMatrix, system_data.voltages);

                // Assign the currents to each branch
                setCurrents(meshesVector, branchesVector, currents);
                clock_t end = clock();
                double elapsed_secs = double(end - begin) * 1000 / CLOCKS_PER_SEC;
                cout << "\n" << "Circuit solved in " << elapsed_secs << " miliseconds" << endl;

                // Save results to file
                string results_file_name = input_file.substr(0, input_file.length() - 4) + "_solved.txt";
                cout << "\n" << "Saving results to " << results_file_name << endl;
                saveToFile(meshesVector, branchesVector, results_file_name);
                cout << "\nDONE!\n" << endl;
                system("pause");
            }
        } else {
            cout << "INVALID INPUT FILE, PLEASE PROVIDE AN XML INPUT FILE" << endl;
            system("pause");
        }
    } else {
        cout << "PLEASE PROVIDE AN XML INPUT FILE" << endl;
        system("pause");
    }
    return 0;
}
