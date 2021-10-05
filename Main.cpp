#include "AneuMeshLoader.h"
#include "Exceptions.h"
#include <algorithm>

int main() {
    const std::string FILEPATH = "..tests/test.aneu";
    MeshLoader* ptr = new AneuMeshLoader;
    std::vector<FiniteElement> elements;
    std::vector<Node> nodes;
    std::vector<std::vector<Node>> near_nodes;
    std::vector<BoundaryFiniteElement> surfaces;
    try {
        ptr->loadmesh(FILEPATH);

        std::cout << "Test of print:" << std::endl;
        ptr->print();

        std::cout << "Test of finite_elements_ID:" << std::endl;
        elements = ptr->finite_elements_ID(1, 2, 3);
        std::cout << elements << std::endl;
        elements.clear();

        std::cout << "Test of finite_elements_Edge:" << std::endl;
        elements = ptr->finite_elements_Edge(1, 2);
        std::cout << elements << std::endl;
        elements.clear();

        std::cout << "Test of vertex:" << std::endl;
        nodes = ptr->vertex(1);
        std::cout << nodes << std::endl;
        nodes.clear();

        std::cout << "Test of finite_elements_material:" << std::endl;
        elements = ptr->finite_elements_material(1);
        std::cout << elements << std::endl;
        elements.clear();

        std::cout << "Test of surfaces_ID:" << std::endl;
        surfaces = ptr->surfaces_ID(1);
        std::cout << surfaces;
        surfaces.clear();

        std::cout << "Test of near_nodes:" << std::endl;
        near_nodes = ptr->near_nodes();
        for (int i = 1; i < near_nodes.size(); ++i) {
            std::cout << i << "    " << near_nodes[i] << std::endl;
        }
        near_nodes.clear();

        std::cout << "Test of middle_insert:" << std::endl;
        ptr->middle_insert();
        ptr->print();

    }
    catch (std::exception& error) {
        std::cerr << error.what() << std::endl;
    }

    delete ptr;
    return 0;
}