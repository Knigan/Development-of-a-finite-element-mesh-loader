#pragma once
#include <iostream>
#include <vector>
#include <array>

struct Node {
    int ID;
    std::array<double, 3> cd;
    bool vertex;

    Node() = default;
    Node(int ID, const std::array<double, 3>& cd, bool vertex) : //!!! cd по ссылке на константу (исправлено)
        ID(ID), cd(cd), vertex(vertex) {}

    bool operator == (const Node& N) const {
        return ID == N.ID &&
            cd == N.cd &&
            vertex == N.vertex;
    }
    bool operator < (const Node& N) const {
        return ID < N.ID;
    }

    friend std::ostream& operator << (std::ostream&, const Node&);
    friend std::ostream& operator << (std::ostream&, const std::vector<Node>&);
};

struct FiniteElement {
    int element_ID;
    int material_ID;
    std::vector<int> nodes_ID;

    friend std::ostream& operator << (std::ostream&, const FiniteElement&);
    friend std::ostream& operator << (std::ostream&, const std::vector<FiniteElement>&);
};

struct BoundaryFiniteElement : public FiniteElement {
    int element_ID;
    int edge_ID;
    std::vector<int> nodes_ID;

    friend std::ostream& operator << (std::ostream&, const BoundaryFiniteElement&);
    friend std::ostream& operator << (std::ostream&, const std::vector<BoundaryFiniteElement>&);
};

struct Edge {
    int first;
    int last;
    int center;

    Edge() = default;
    Edge(int ID1, int ID2, int ID3) :
        first(ID1), last(ID2), center(ID3) {}

    bool operator == (const Edge& R) const {
        return (R.first == first && R.last == last) ? 
            true : (R.first == last && R.last == first);
    }
    bool operator < (const Edge& R) const {
        return center < R.center;
    }
    void UpdateCenter(int ID) {
        center = ID;
    }
};

struct Hash {
    void hash_combine(size_t& seed, const int& value) const {
        seed ^= std::hash<int>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    size_t operator () (const Edge& E) const {
        size_t seed = 0;
        
        //!!! Поля должны быть отсортированны, в противном случае ребра (1,2,3) и (2,1,3) имеют разные хэш-значения (исправлено)
        
        int first = std::min(std::min(E.first, E.last), E.center);
        int last = std::max(std::max(E.first, E.last), E.center);
        int center = (E.first + E.last + E.center) - (first + last);
        hash_combine(seed, first);
        hash_combine(seed, last);
        hash_combine(seed, center);
        return seed;
    }
};



