#include "AneuMeshLoader.h"
#include "Exceptions.h"

void AneuMeshLoader::loadmesh(const std::string& filename) {
    std::ifstream file(filename);
    if (!file)
        throw FileException();

    int amount, dim;

    file >> amount >> dim;
    nodes.reserve(amount);

    for (int i = 1; i <= amount; ++i) {
        Node temp;
        temp.ID = i;
        file >> temp.cd[0] >> temp.cd[1] >> temp.cd[2];
        temp.vertex = true;
        nodes.push_back(temp);
    }

    file >> amount >> dim;
    finite_elements.reserve(amount);

    for (int i = 1; i <= amount; ++i) {
        FiniteElement temp;
        temp.element_ID = i;
        file >> temp.material_ID;
        temp.nodes_ID.reserve(dim);
        int ID;

        for (int j = 0; j < dim; ++j) {
            file >> ID;
            temp.nodes_ID.push_back(ID);
        }

        finite_elements.push_back(std::move(temp));
    }

    file >> amount >> dim;
    boundary_finite_elements.reserve(amount);

    for (int i = 1; i <= amount; ++i) {
        BoundaryFiniteElement temp;
        temp.element_ID = i;
        file >> temp.edge_ID;
        temp.nodes_ID.reserve(dim);
        int ID;

        for (int j = 0; j < dim; ++j) {
            file >> ID;
            temp.nodes_ID.push_back(ID);
        }

        std::vector<FiniteElement> temp_elements = finite_elements_by_ID(temp.nodes_ID[0], temp.nodes_ID[1], temp.nodes_ID[2]);
        temp.element_ID = temp_elements[0].element_ID;
        temp.material_ID = temp_elements[0].material_ID;
        boundary_finite_elements.push_back(std::move(temp));
    }

    file.close();
}
