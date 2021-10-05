#pragma once
#include "Types.h"
#include <fstream>

class MeshLoader {
protected:
    std::vector<Node> nodes;
    std::vector<FiniteElement> finite_elements;
    std::vector<BoundaryFiniteElement> surfaces;

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
    std::vector<FiniteElement> finite_elements_ID(int, int, int);
    std::vector<FiniteElement> finite_elements_Edge(int, int);
    std::vector<FiniteElement> finite_elements_material(int);

    std::vector<BoundaryFiniteElement>& Surfaces() {
        return surfaces;
    }
    std::vector<BoundaryFiniteElement> surfaces_ID(int);

    void print() {
        std::cout << Nodes() << std::endl
            << Finite_Elements() << std::endl
            << Surfaces() << std::endl;
    }

    void middle_insert();

private:
    int last_nodes() {
        return nodes.rbegin()->ID;
    }

};

