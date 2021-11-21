#include "MeshLoader.h"
#include <iomanip>
#include <algorithm>
#include <set>
#include <unordered_set>

std::ostream& operator << (std::ostream& out, const std::vector<int>& cont) {
    for (size_t i : std::ranges::views::iota(size_t(0), cont.size()))
        out << cont[i] << " ";
    out << std::endl;
    return out;
}

std::vector<FiniteElement> MeshLoader::finite_elements_ID(int ID1, int ID2, int ID3) {
    std::vector<FiniteElement> result;
    auto iter = finite_elements.begin();
    auto last_iter = finite_elements.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, 
            [ID1, ID2, ID3](const FiniteElement& elem) { //!!! Где const (исправлено)
            return std::find(elem.nodes_ID.begin(), elem.nodes_ID.end(), ID1) != elem.nodes_ID.end()
                || std::find(elem.nodes_ID.begin(), elem.nodes_ID.end(), ID2) != elem.nodes_ID.end()
                || std::find(elem.nodes_ID.begin(), elem.nodes_ID.end(), ID3) != elem.nodes_ID.end();
            });

        if (iter != last_iter) {
            result.push_back(*iter);
            ++iter;
        }
    }

    return result;
}

std::vector<FiniteElement> MeshLoader::finite_elements_Edge(int ID1, int ID2) {
    std::vector<FiniteElement> result;
    auto iter = finite_elements.begin();
    auto last_iter = finite_elements.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, 
        [ID1, ID2](const FiniteElement& element) { //!!! Где const (исправлено)
            return std::find(element.nodes_ID.begin(), element.nodes_ID.end(), ID1) != element.nodes_ID.end()
                || std::find(element.nodes_ID.begin(), element.nodes_ID.end(), ID2) != element.nodes_ID.end();
        });

        if (iter != last_iter) {
            result.push_back(*iter);
            ++iter;
        }
    }

    return result;
}

//!!! Все контейнера должны содержать только ID узлов или КЭ (Исправлено)

std::vector<int> MeshLoader::vertex_nodes_by_surface_ID(int ID) {
    std::set<int> result;
    auto iter = surfaces.begin();
    auto last_iter = surfaces.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, 
        [ID](const BoundaryFiniteElement& elem) { 
            return elem.edge_ID == ID;
        });

        if (iter != last_iter) {
            for (const auto& id : iter->nodes_ID) { //!!! it - не итератор. Переименуйте (исправлено)
                
                //!!! Крайне неэффективная реализация. Потеряны преимущества контейнера (исправлено)
                result.insert(id);
            }
            ++iter;
        }
    }

    return std::vector<int>(result.begin(), result.end());
}

std::vector<BoundaryFiniteElement> MeshLoader::surfaces_ID(int ID) {
    std::vector<BoundaryFiniteElement> result; 
    auto iter = surfaces.begin();
    auto last_iter = surfaces.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, 
        [ID](const BoundaryFiniteElement& elem) {
            return elem.element_ID == ID;
        });

        if (iter != last_iter) {
            result.push_back(*iter);
            ++iter;
        }
    }

    return std::vector<BoundaryFiniteElement>(result.begin(), result.end());
}

std::vector<FiniteElement> MeshLoader::finite_elements_material(int material_ID) { 
    std::vector<FiniteElement> result;
    auto iter = finite_elements.begin();
    auto last_iter = finite_elements.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, 
        [material_ID](const FiniteElement& element) { 
            return element.material_ID == material_ID;
        });

        if (iter != last_iter) {
            result.push_back(*iter);
            ++iter;
        }
    }

    return std::vector<FiniteElement>(result.begin(), result.end());
}

void MeshLoader::middle_insert() {

    std::unordered_set<Edge, Hash> edges;
    for (auto& elem : finite_elements) { //!!! iter не итератор - переименовать (исправлено)
                                         //!!! Копирование зачем? (не исправлено) Только хуже (исправлено)
                                                       //!!! Почему с iter работать нельзя? (исправлено)
        for (int i = 0; i < elem.nodes_ID.size(); ++i) { //!!! Лучше использовать индексы (исправлено)

            for (int j = i + 1; j < elem.nodes_ID.size(); ++j) { //!!! Лучше использовать индексы (исправлено)
                Edge E(elem.nodes_ID[i], elem.nodes_ID[j], 0);
                
                auto E2 = edges.find(E); 

                if (E2 == edges.end()) {
                    Node left_node = get_node(elem.nodes_ID[i]);
                    Node right_node = get_node(elem.nodes_ID[j]);
                    int center_id_ = last_nodes() + 1;

                    Node center_node(center_id_,
                        std::array<double, 3>{(left_node.cd[0] + right_node.cd[0]) / 2,
                        (left_node.cd[1] + right_node.cd[1]) / 2,
                        (left_node.cd[2] + right_node.cd[2]) / 2},
                        false);

                    E.UpdateCenter(center_id_);
                    nodes.push_back(std::move(center_node));
                    elem.nodes_ID.push_back(center_id_);
                    edges.insert(E);

                } else {
                    elem.nodes_ID.push_back(E2->center);
                }
            }
        }

        elem.nodes_ID.shrink_to_fit();
    }

    for (auto& S : surfaces) {
        for (int i = 0; i < S.nodes_ID.size() - 1; ++i) {
            for (int j = i + 1; j < S.nodes_ID.size(); ++j) {
                Edge E(S.nodes_ID[i], S.nodes_ID[j], 0);
                auto E2 = edges.find(E); 

                if (E2 != edges.end()) {
                    S.nodes_ID.push_back(E2->center);
                }
            }
        }

        S.nodes_ID.shrink_to_fit();
    }

    surfaces.shrink_to_fit();
    nodes.shrink_to_fit();
}

std::vector<std::vector<Node>> MeshLoader::near_nodes() {
    std::vector<std::set<int>> res(nodes.size() + 1);

    for (const auto& it : finite_elements) {
        for (auto& iter : it.nodes_ID) {
            for (auto& iter2 : it.nodes_ID) {
                res[iter].insert(iter2);
            }
            res[iter].erase(iter);
        }
    }

    std::vector<std::vector<Node>> result(nodes.size() + 1);
    for (int i = 1; i < nodes.size() + 1; ++i) {
        std::vector<Node> temp;
        temp.reserve(res[i].size());

        for (auto& it : res[i]) {
            temp.push_back(get_node(it));
        }

        result[i] = std::move(temp);
    }

    return result;
}

double MeshLoader::volume(const FiniteElement& elem) {
    if (elem.nodes_ID.size() < 4)
        return 0.0;

    Node A = get_node(elem.nodes_ID[0]);
    Node B = get_node(elem.nodes_ID[1]);
    Node C = get_node(elem.nodes_ID[2]);
    Node D = get_node(elem.nodes_ID[3]);
    double A1 = (B.cd[0] - A.cd[0]) * ((C.cd[1] - A.cd[1]) * (D.cd[2] - A.cd[2]) - (C.cd[2] - A.cd[2]) * (D.cd[1] - A.cd[1]));
    double A2 = -(B.cd[1] - A.cd[1]) * ((C.cd[0] - A.cd[0]) * (D.cd[2] - A.cd[2]) - (D.cd[0] - A.cd[0]) * (C.cd[2] - A.cd[2]));
    double A3 = (B.cd[2] - A.cd[2]) * ((C.cd[0] - A.cd[0]) * (D.cd[1] - A.cd[1]) - (D.cd[0] - A.cd[0]) * (C.cd[1] - A.cd[1]));
    return (A1 + A2 + A3) / 6.0;
}

void MeshLoader::volume_range(int m_ID, double a, double b, const std::string& filepath) {
    
    
     
    //!!! Что-то слишком сложно и не понятно. std::ranges позволяют выполнить задание по сути в одну строку,
    //!!! составив pipeline. Нужно сделать контейнер самих КЭ (не ID) по полученному котейнеру из ID и далее для него использовать
    //!!! std::ranges::views::transform, std::ranges::views::filter и std::ranges::copy (исправлено)
    
    
     
    std::vector<FiniteElement> elements = finite_elements_material(m_ID);
    if (elements.empty())
        return;
    std::ofstream file(filepath + ".txt");

    std::ranges::copy(
        elements |
        std::ranges::views::filter(
            [&](const FiniteElement& elem) {
                double V = volume(elem);
                
                //!!! Зачем изобратеть велосипед, если можно написать a < V && V < b; (исправлено)
                
                return a < V && V < b;
            }) |
        std::ranges::views::transform(
            [](const FiniteElement& elem) {
                return elem.nodes_ID;
            }),
        std::ostream_iterator<std::vector<int>>(file));
}

