#pragma once
#include "Types.h"
#include <fstream>

class MeshLoader {
protected:
    std::vector<Node> nodes;
    std::vector<FiniteElement> finite_elements;
    std::vector<BoundaryFiniteElement> surfaces;

private:
    Node get_node(int);
    int last_nodes() {
        return nodes.rbegin()->ID;
    }

public:
    virtual void loadmesh(const std::string&) = 0;
    virtual ~MeshLoader() = default;

    std::vector<Node>& Nodes() {
        return nodes;
    }
    std::vector<FiniteElement>& Finite_Elements() {
        return finite_elements;
    }
    std::vector<BoundaryFiniteElement>& Surfaces() {
        return surfaces;
    }
    void print() {
        std::cout << Nodes() << std::endl
            << Finite_Elements() << std::endl
            << Surfaces() << std::endl;
    }

    std::vector<FiniteElement> finite_elements_ID(int, int, int);
    std::vector<FiniteElement> finite_elements_Edge(int, int);
    std::vector<Node> vertex(int);
    std::vector<FiniteElement> finite_elements_material(int);
    std::vector<BoundaryFiniteElement> surfaces_ID(int);
    void middle_insert();
    std::vector<std::vector<Node>> near_nodes();

};

