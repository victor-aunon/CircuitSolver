#include <iostream>
#include <vector>
#include <iterator>
#include <ctime>
#include <fstream>
#include "pugixml.hpp"
#include "LinearSystemSolver.h"

class Mesh {

    private:
        std::string ID = "";
        double m_powerSource = 0;
        double m_impedance = 0;
        double m_current = 0;
        std::vector<std::string> m_branchesIDs;

    public:
        Mesh(std::string t_ID, pugi::xml_node t_mesh);
        
        ~Mesh();

        void readElements(pugi::xml_node t_mesh);

        double getPowerSource() {
            return m_powerSource;
        };

        std::vector<std::string> getBranchesIDs() {
            return m_branchesIDs;
        };

        double getImpedance() {
            return m_impedance;
        };

        std::string getID() {
            return ID;
        }

        void setCurrent(double t_current) {
            m_current = t_current;
        }

        double getCurrent() {
            return m_current;
        }

};

struct Branch {
    std::string ID;
    double current;
    double branchImpedance;
    std::vector<std::string> impedanceIDs;
    std::vector<double> impedances;
    std::vector<double> powerDissipated;
};

std::vector<Mesh> meshesVector;

std::vector<Branch> branchesVector;

struct System {
    std::vector<std::vector<double>> impedanceMatrix;
    std::vector<double> voltages;
};


System createSystem(std::vector<Mesh> &t_meshesVector, std::vector<Branch> &t_branchesVector);


void setCurrents(std::vector<Mesh> &t_meshesVector, std::vector<Branch> &t_branchesVector,
    std::vector<double> &t_currents);


void saveToFile(std::vector<Mesh> &t_meshesVector, std::vector<Branch> &t_branchesVector,
     std::string &t_fileName);



