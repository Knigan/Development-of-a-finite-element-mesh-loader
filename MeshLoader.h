#pragma once
#include "Types.h"
#include <fstream>
#include <iostream>
#include <ranges>

class MeshLoader {
protected:
    std::vector<Node> nodes;
    std::vector<FiniteElement> finite_elements;
    std::vector<BoundaryFiniteElement> surfaces;

private:
    Node get_node(int ID) {
        return nodes[ID];
    }
    int last_nodes() {
        return nodes.rbegin()->ID;
    }

    double volume(const FiniteElement&); // задание на защиту

public:
    virtual void loadmesh(const std::string&, const std::string&, const std::string&) = 0;
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
        std::cout << Nodes() << std::endl << std::endl
            << Finite_Elements() << std::endl << std::endl
            << Surfaces() << std::endl << std::endl;
    }

    std::vector<FiniteElement> finite_elements_ID(int, int, int);
    std::vector<FiniteElement> finite_elements_Edge(int, int);
    std::vector<int> vertex_nodes_by_surface_ID(int);
    std::vector<FiniteElement> finite_elements_material(int);
    std::vector<BoundaryFiniteElement> surfaces_ID(int);
    void middle_insert();
    std::vector<std::vector<Node>> near_nodes();

    void volume_range(int, double, double, const std::string&); // задание на защиту
};

