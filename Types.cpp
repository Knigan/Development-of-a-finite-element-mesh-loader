#include "Types.h"
#include <iomanip>

std::ostream& operator << (std::ostream& out, const Node& N) {
    out << std::endl << std::setw(4) << N.ID << " " << std::setw(11) << N.vertex;   
    for (const auto& it : N.cd)
        out << std::setw(10) << it;
    return out;
}

std::ostream& operator << (std::ostream& out, const std::vector<Node>& nodes) {    
    out << "Nodes (" << nodes.size() << "):" << std::endl << "  ID"
        << std::setw(15) << "Vertex" << std::setw(7) << "x" << std::setw(10) << "y" << std::setw(10) << "z" << std::endl;
    for (const auto& it : nodes)
        out << it;
    return out;
}

std::ostream& operator << (std::ostream& out, const FiniteElement& elem) {
    out <<
        std::setw(4) <<
        elem.element_ID << "  " <<
        std::setw(11) <<
        elem.material_ID << "  ";
    for (const auto& it : elem.nodes_ID)
        out << std::setw(11) << it;
    out << std::endl;
    return out;
}

std::ostream& operator << (std::ostream& out, const std::vector<FiniteElement>& elements) {
    out <<
        "Elements (" << elements.size() << "):" << std::endl << "  ID" <<
        std::setw(18) << "Material_ID" << std::setw(11) << "Nodes_ID" << std::endl;
    for (const auto& it : elements)
        out << it;
    return out;
}

std::ostream& operator << (std::ostream& out, const BoundaryFiniteElement& elem) {
    out <<
        std::setw(4) << elem.element_ID << "  " <<
        std::setw(6) << elem.edge_ID << "  ";
    for (const auto& it : elem.nodes_ID) {
        out << std::setw(6) << it;
    }
    out << std::endl;
    return out;
}

std::ostream& operator << (std::ostream& out, const std::vector<BoundaryFiniteElement>& elements) {
    out <<
        "BoundaryFiniteElement (" << elements.size() << "):" << std::endl << "  ID" <<
        std::setw(12) << "Border_ID" << std::setw(11) << "Nodes_ID\n";
    for (const auto& it : elements)
        out << it;
    return out;
}