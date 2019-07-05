include_directories(../pugixml/src)
include_directories(../resources)

add_library(
    pugixml
    ../pugixml/src/pugixml.cpp
    )

add_executable(CircuitSolver CircuitSolver.cpp LinearSystemSolver.cpp CircuitSolver.rc)

target_link_libraries(CircuitSolver pugixml)