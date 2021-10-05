#include "MeshLoader.h"
#include <iomanip>
#include <algorithm>
#include <list>
#include <set>
#include <unordered_set>

Node MeshLoader::get_node(int ID) {
    std::set<Node> N(nodes.begin(), nodes.end());
    for (auto& it : N) {
        if (it.ID == ID)
            return it;
    }
    return {};
}

std::vector<FiniteElement> MeshLoader::finite_elements_ID(int ID1, int ID2, int ID3) {
    std::vector<FiniteElement> result;
    auto iter = finite_elements.begin();
    auto last_iter = finite_elements.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, [ID1, ID2, ID3](FiniteElement& elem) {
            return std::find(elem.nodes_ID.begin(), elem.nodes_ID.end(), ID1) != elem.nodes_ID.end()
                && std::find(elem.nodes_ID.begin(), elem.nodes_ID.end(), ID2) != elem.nodes_ID.end()
                && std::find(elem.nodes_ID.begin(), elem.nodes_ID.end(), ID3) != elem.nodes_ID.end();
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
        iter = std::find_if(iter, last_iter, [ID1, ID2](FiniteElement& element) {
            return std::find(element.nodes_ID.begin(), element.nodes_ID.end(), ID1) != element.nodes_ID.end()
                && std::find(element.nodes_ID.begin(), element.nodes_ID.end(), ID2) != element.nodes_ID.end();
            });

        if (iter != last_iter) {
            result.push_back(*iter);
            ++iter;
        }
    }

    return result;
}

std::vector<Node> MeshLoader::vertex(int ID) {
    std::set<Node> result;
    auto iter = surfaces.begin();
    auto last_iter = surfaces.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, [ID](BoundaryFiniteElement& surface) {
            return surface.edge_ID == ID;
        });

        if (iter != last_iter) {
            for (const auto& it : iter->nodes_ID) {
                auto node_it = std::find_if(nodes.begin(), nodes.end(), [it](Node& N) {
                    return N.ID == it;
                });
                result.insert(*node_it);
            }
            ++iter;
        }
    }

    return std::vector<Node>(result.begin(), result.end());
}

std::vector<BoundaryFiniteElement> MeshLoader::surfaces_ID(int ID) {
    std::list<BoundaryFiniteElement> result;
    auto iter = surfaces.begin();
    auto last_iter = surfaces.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, [ID](BoundaryFiniteElement& surface) {
            return surface.element_ID == ID;
            });

        if (iter != last_iter) {
            result.push_back(*iter);
            ++iter;
        }
    }

    return std::vector<BoundaryFiniteElement>(result.begin(), result.end());
}

std::vector<FiniteElement> MeshLoader::finite_elements_material(int ID) {
    std::list<FiniteElement> result;
    auto iter = finite_elements.begin();
    auto last_iter = finite_elements.end();

    while (iter != last_iter) {
        iter = std::find_if(iter, last_iter, [ID](FiniteElement& element) {
            return element.material_ID == ID;
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
    for (auto& iter : finite_elements) {
        std::vector<int> N(iter.nodes_ID);

        for (auto i = N.begin(); i < N.end(); ++i) {

            for (auto j = i + 1; j < N.end(); ++j) {
                Edge E(*i, *j, 0);
                auto E2 = std::find(edges.begin(), edges.end(), E);

                if (E2 == edges.end()) {
                    Node left_node = get_node(*i);
                    Node right_node = get_node(*j);
                    int center_id_ = last_nodes() + 1;

                    Node center_node(center_id_,
                        std::array<double, 3>{(left_node.cd[0] + right_node.cd[0]) / 2,
                        (left_node.cd[1] + right_node.cd[1]) / 2,
                        (left_node.cd[2] + right_node.cd[2]) / 2},
                        false);

                    E.UpdateCenter(center_id_);
                    nodes.push_back(center_node);
                    iter.nodes_ID.push_back(center_id_);
                    edges.insert(E);

                } else {
                    iter.nodes_ID.push_back(E2->center);
                }
            }
        }

        iter.nodes_ID.shrink_to_fit();
    }

    for (auto& it : surfaces) {
        std::vector<int> N(it.nodes_ID);

        for (auto i = N.begin(); i < N.end() - 1; ++i) {
            for (auto j = i + 1; j < N.end(); ++j) {
                Edge E(*i, *j, 0);
                auto E2 = std::find(edges.begin(), edges.end(), E);

                if (E2 != edges.end()) {
                    it.nodes_ID.push_back(E2->center);
                }
            }
        }

        it.nodes_ID.shrink_to_fit();
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

