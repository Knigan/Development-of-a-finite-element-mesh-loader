#include "AneuMeshLoader.h"

//!!! Задание на защиту (не удалять!)
//!!! Реализовать на основе ranges.
//!!! 1. Получить КЭ из данной области (материала).
//!!! 2. С помощью потокового итератора вывести в файл список ID узлов тех тетраэдров,          (сделано)
//!!! у которых объем в заданных пределах.

int main() {    
    MeshLoader* ptr = new AneuMeshLoader;

    try {
        //!!! У меня нет такого пути. Мне как пользователю перекомпилировать надо? (исправлено)
        //!!! Пути должны задаваться отностельно exe с помощью . и .. 
        
        ptr->loadmesh("./Files/fichera", ".neu", ".aneu");
    }
    catch (std::exception& error) {
        std::cerr << error.what() << std::endl;
        delete ptr;
        return 0;
    }

    std::vector<FiniteElement> elements;
    std::vector<Node> nodes;
    std::vector<std::vector<Node>> near_nodes;
    std::vector<BoundaryFiniteElement> surfaces;
    bool b = true;
    auto vertex_print = [ptr]() {
        std::vector<int> v = ptr->vertex_nodes_by_surface_ID(1);
        std::cout << std::endl << "The list of the ID: ";
        for (auto i = v.begin(); i != v.end(); ++i)
            std::cout << *i << " ";
        std::cout << std::endl;
    };
    while (b) {
        std::cout << std::endl << "---------------- MeshLoader Test ----------------" << std::endl;
        std::cout << "1: Nodes" << std::endl;
        std::cout << "2: Finite_Elements" << std::endl;
        std::cout << "3: Surfaces" << std::endl;
        std::cout << "4: print" << std::endl;
        std::cout << "5: finite_elements_ID" << std::endl;
        std::cout << "6: finite_elements_Edge" << std::endl;
        std::cout << "7: vertex_nodes_by_surface_ID" << std::endl;
        std::cout << "8: finite_elements_material" << std::endl;
        std::cout << "9: surfaces_ID" << std::endl;
        std::cout << "10: middle_insert" << std::endl;
        std::cout << "11: near_nodes" << std::endl;
        std::cout << "12: volume_range" << std::endl;
        std::cout << "13: clear screen" << std::endl;
        std::cout << "14: exit" << std::endl;
        int num;
        std::cin >> num;
        switch (num) {
            case 1: std::cout << std::endl << ptr->Nodes() << std::endl; break;
            case 2: std::cout << std::endl << ptr->Finite_Elements() << std::endl; break;
            case 3: std::cout << std::endl << ptr->Surfaces() << std::endl; break;
            case 4: ptr->print(); break;
            case 5: std::cout << std::endl << ptr->finite_elements_ID(1, 2, 3) << std::endl; break;
            case 6: std::cout << std::endl << ptr->finite_elements_Edge(1, 2) << std::endl; break;
            case 7: vertex_print(); break;
            case 8: std::cout << std::endl << ptr->finite_elements_material(1) << std::endl; break;
            case 9: std::cout << std::endl <<  ptr->surfaces_ID(2) << std::endl; break;
            case 10: ptr->middle_insert(); ptr->print(); break;
            case 11: ptr->near_nodes(); ptr->print(); break;
            case 12: ptr->volume_range(1, 0, 100, "1"); break;
            case 13: system("cls"); break;
            case 14: b = false; break;
            default: std::cout << "Error: The entered number is incorrect!" << std::endl;
        }
    }
    delete ptr;
    return 0;
}