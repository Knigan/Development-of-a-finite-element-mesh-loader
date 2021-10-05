#pragma once
#include "Types.h"
#include <fstream>

class MeshLoader {
protected:
    std::vector<Node> nodes;
    std::vector<FiniteElement> finite_elements;
    std::vector<BoundaryFiniteElement> boundary_finite_elements;

public:
    virtual void loadmesh(const std::string&) = 0;
    virtual ~MeshLoader() = default;

    std::vector<Node>& Nodes() {
        return nodes;
    }
    std::vector<Node> vertex(int);
    Node get_node(int);
    std::vector<std::vector<Node>> near_nodes();

    std::vector<FiniteElement>& Finite_Elements() {
        return finite_elements;
    }
    std::vector<FiniteElement> finite_elements_by_ID(int, int, int);
    std::vector<FiniteElement> finite_elements_by_Edge(int, int);
    std::vector<FiniteElement> finite_elements_by_material_ID(int);

    std::vector<BoundaryFiniteElement>& Boundary_Finite_Elements() {
        return boundary_finite_elements;
    }
    std::vector<BoundaryFiniteElement> boundary_finite_elements_by_ID(int);

    void print() {
        std::cout << Nodes() << std::endl
            << Finite_Elements() << std::endl
            << Boundary_Finite_Elements() << std::endl;
    }

    void square();

private:
    int last_nodes() {
        return nodes.rbegin()->ID;
    }

};

