#include "AneuMeshLoader.h"
#include "Exceptions.h"
#include <algorithm>

int main() {
    const std::string FILEPATH = "..tests/test.aneu";
    MeshLoader* ptr = new AneuMeshLoader;
    std::vector<FiniteElement> elements;
    std::vector<Node> nodes;
    std::vector<std::vector<Node>> near_nodes;
    std::vector<BoundaryFiniteElement> boundary_elements;
    try {
        ptr->loadmesh(FILEPATH);

        std::cout << "print test:" << std::endl;
        ptr->print();

        std::cout << "finite_elements_by_ID test:" << std::endl;
        elements = ptr->finite_elements_by_ID(1, 2, 3);
        std::cout << elements << std::endl;
        elements.clear();

        std::cout << "finite_elements_by_Edge test:" << std::endl;
        elements = ptr->finite_elements_by_Edge(1, 2);
        std::cout << elements << std::endl;
        elements.clear();

        std::cout << "vertex test:" << std::endl;
        nodes = ptr->vertex(1);
        std::cout << nodes << std::endl;
        nodes.clear();

        std::cout << "finite_elements_by_material_ID test:" << std::endl;
        elements = ptr->finite_elements_by_material_ID(1);
        std::cout << elements << std::endl;
        elements.clear();

        std::cout << "boundary_finite_elements_by_ID test:" << std::endl;
        boundary_elements = ptr->boundary_finite_elements_by_ID(1);
        std::cout << boundary_elements;
        boundary_elements.clear();

        std::cout << "near_nodes test:" << std::endl;
        near_nodes = ptr->near_nodes();
        for (int i = 1; i < near_nodes.size(); ++i) {
            std::cout << i << "    " << near_nodes[i] << std::endl;
        }
        near_nodes.clear();

        std::cout << "square test:" << std::endl;
        ptr->square();
        ptr->print();

    }
    catch (std::exception& error) {
        std::cerr << error.what() << std::endl;
    }

    delete ptr;
    return 0;
}