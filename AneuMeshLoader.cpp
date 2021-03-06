#include "AneuMeshLoader.h"
#include "Exceptions.h"

void AneuMeshLoader::loadmesh(const std::string& filepath, const std::string& str1, const std::string& str2) {
    std::ifstream file(filepath + str1);
    if (!file) {
        std::ifstream file2(filepath + str2);
        if (file2)
            throw FileFormatException();
        else
            throw FileException();
    }

    int Q, N;
    file >> Q >> N;
    nodes.reserve(Q);

    for (int i = 1; i <= Q; ++i) {
        Node temp;
        temp.ID = i;
        file >> temp.cd[0] >> temp.cd[1] >> temp.cd[2];
        temp.vertex = true;
        nodes.push_back(temp);
    }

    file >> Q >> N;
    finite_elements.reserve(Q);

    for (int i = 1; i <= Q; ++i) {
        FiniteElement temp;
        temp.element_ID = i;
        file >> temp.material_ID;
        temp.nodes_ID.reserve(N);
        int ID;

        for (int j = 0; j < N; ++j) {
            file >> ID;
            temp.nodes_ID.push_back(ID);
        }

        finite_elements.push_back(std::move(temp));
    }

    file >> Q >> N;
    surfaces.reserve(Q);

    for (int i = 1; i <= Q; ++i) {
        BoundaryFiniteElement temp_surface;
        temp_surface.element_ID = i;
        file >> temp_surface.edge_ID;
        temp_surface.nodes_ID.reserve(N);
        int ID;

        for (int j = 0; j < N; ++j) {
            file >> ID;
            temp_surface.nodes_ID.push_back(ID);
        }

        std::vector<FiniteElement> temp_elements = finite_elements_ID(temp_surface.nodes_ID[0], temp_surface.nodes_ID[1], temp_surface.nodes_ID[2]);
        temp_surface.element_ID = temp_elements[0].element_ID;
        temp_surface.material_ID = temp_elements[0].material_ID;
        surfaces.push_back(std::move(temp_surface));
    }
    file.close();
}
