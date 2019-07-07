# CircuitSolver

## Table of contents
1. [Description](#description)
2. [Building the circuit](#building)
3. [Solving the circuit](#solving)
4. [Acknowledgments](#acknowledgments)

---

## 1. Description <a name="description"></a>
_CircuitSolver_ is a C++ program that calculates the electric currents in a DC circuit. The project was created with the aim of encourage C++ learners to build their own projects and provide engineering students with a tool to solve this type of circuits.

As a learning resource, this project includes a fully commented code that uses:
* Classes
* Structs
* Pointers
* Code separated in headers and implementation files.
* Third party libraries.
* Examples

## 2. Building the circuit <a name="building"></a>
Electric circuits must be built in XML format. The main node is the `<circuit>` node, which holds the circuit meshes and branches. A `<mesh>` node should be declared with the attribute `ID` as its identifier, the same happens with a `<branch>` node. 

The circuit elements should be declared inside a `<branch>` node. If the element is a battery, a `<battery>` node should be created with the attributes `ID` (the identifier) and `value`, which is the voltage of the battery in volts. Alternatively, if the element is an impedance, a `<resistance>` node should be created with the attributes `ID` (the identifier) and `value`, which is the resistance in ohms.

When creating a mesh, the user must define an initial direction of the current flow. The program solves the mesh currents by considering that the current flows clockwise through each mesh. So, taking this direction into account, the user must define the battery voltage according to this. If the current goes through the battery from anode to cathode, then the `value` attribute of the `<battery>` must be positive; on the contrary, it must be negative. For resistance elements, the user must define the `value` attribute of the `<resistance>` always positive.

As an example, the following circuit, composed of two meshes, should be declared as follows:

![alt text](https://raw.githubusercontent.com/victor-aunon/CircuitSolver/master/examples/circuit_2_meshes_scheme.png "Two meshes circuit")

```XML
<circuit>
    <mesh ID="mesh-1">
        <branch ID="branch-1">
            <battery ID="battery-1" value="28"/>
            <resistance ID="resistance-1" value="4"/>
        </branch>
        <branch ID="branch-2">
            <resistance ID="resistance-2" value="2"/>
        </branch>
    </mesh>
    <mesh ID="mesh-2">
        <branch ID="branch-2">
            <resistance ID="resistance-2" value="2"/>
        </branch>
        <branch ID="branch-3">
            <battery ID="battery-2" value="-7"/>
            <resistance ID="resistance-3" value="1"/>
        </branch>
    </mesh>
</circuit>
```

## 3. Solving the circuit <a name="solving"></a>
Once the circuit has been created, it must be solved by passing it as an argument to the program. In windows, for instance, the user must call the program in this way:

`CircuitSolver.exe <name-of-the-circuit-file>.xml`

If the file does not exist, or it is not an XML file, or it can't be read, the program will raise an error.

Once the circuit XML file has been read correctly, the program solves the circuit and creates a text file called `<name-of-the-circuit-file>_solved.txt`. This results file contains the electric currents at each branch and mesh, as well as the power dissipated by each resistance.

Regarding the sign of the current, if the value is positive, it means that the resulting direction of the current matches the initial one, which is clockwise. In case it is negative, the current direction would be anticlockwise. The same happens for branches which are shared between two meshes: its resulting current sign is referred to the one of the first mesh in which the branch was declared.

## 4. Acknowledgments <a name="acknowledgments"></a>
This software is based on pugixml library (http://pugixml.org). pugixml is Copyright (C) 2006-2018 Arseny Kapoulkine.
