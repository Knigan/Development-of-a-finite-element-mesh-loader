#include "Types.h"
#include <iomanip>

std::ostream& operator << (std::ostream& out, const Node& N) {
    out <<
        std::setw(4) << N.ID << "  " <<
        std::right << std::setw(11) <<
        N.vertex << std::endl;
    for (const auto& it : N.cd)
        out << N << " ";
    return out;
}

std::ostream& operator << (std::ostream& out, const std::vector<Node>& nodes) {
    out << nodes.size() << (nodes.size() > 1 ? " Nodes:\n" : " Node:\n") << " id"
        << std::setw(10) << "x" << std::setw(11) << "y" << std::setw(11) << "z" << std::setw(14) << "Vertex\n";
    for (const auto& it : nodes) {
        out << it;
    }
    return out;
}

std::ostream& operator << (std::ostream& out, const FiniteElement& elem) {
    out <<
        std::setw(4) <<
        elem.element_ID << "  " <<
        std::setw(11) <<
        elem.material_ID << "  ";
    for (const auto& it : elem.nodes_ID)
        out << std::right << std::setw(4) << it;
    out << std::endl;
    return out;
}

std::ostream& operator << (std::ostream& out, const std::vector<FiniteElement>& elements) {
    int n = elements.size();
    out <<
        n << (n > 1 ? " Elements:\n" : " Element:\n") << " id" <<
        std::setw(15) << "Material_ID" << std::setw(15) << "Nodes_ID" << std::endl;
    for (const auto& it : elements)
        out << it;
    return out;
}

std::ostream& operator << (std::ostream& out, const BoundaryFiniteElement& elem) {
    out <<
        std::setw(9) << elem.element_ID << "  " <<
        std::setw(9) << elem.edge_ID << "  ";
    for (const auto& it : elem.nodes_ID) {
        out << std::right << std::setw(4) << it;
    }
    out << std::endl;
    return out;
}

std::ostream& operator << (std::ostream& out, const std::vector<BoundaryFiniteElement>& elements) {
    int n = elements.size();
    out <<
        n <<
        (n > 1 ? " BoundaryFiniteElement:\n" : " BoundaryFiniteElement:\n") << "BoundaryFiniteElement_ID" <<
        std::setw(11) << "Border_ID" << std::setw(14) << "Nodes_ID\n";
    for (const auto& it : elements)
        out << it;
    return out;
}