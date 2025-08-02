#include "include/dense_intrusive_linked_list.h"
#include <stdexcept>

static mlc::intrusive_dense_list<int> list;
static mlc::intrusive_dense_list_node<int> root, new_root, node2, node3, node4, node5;

static void TEST_CYCLING() {
    std::cout << "\n==== STARTING CYCLING TEST =====\n";
    try {

        // Initialize the node 
        root.lvalue = 45;
        node2.lvalue = 67;
        node3.lvalue = 10;
        root.next = &node3;
        root.prev = &node2;
        root.next->prev = &root;
        //node5.lvalue = 44;

        new_root.lvalue = 90;
        new_root.prev = &root;
        new_root.next = &node4;


        // Test push_back 
        list.push_back(root);
        list.push_node_front(new_root);

        // Test begin
        mlc::intrusive_dense_list_iterator<int> iter1 = list.node_begin();

        // Test one
        std::cout << "iter1[0].lvalue == 10 : ";
        static std::string lhs = std::to_string(iter1[0].lvalue);
        static std::string rhs = std::to_string(10).c_str();
        static std::string err = "TEST ONE FAILED:\n\t Value Expected : ";
        err.append(rhs.c_str()).append("\n\t").append("Value Is : ").append(lhs.c_str()).append("\n");
        iter1[0].lvalue == 10 ? std::cout << "PASSED\n" : throw std::runtime_error(err.c_str());

        // Test two
        iter1--;
        std::cout << "iter1[0].lvalue == 90 : ";
        lhs = std::to_string(iter1[0].lvalue);
        rhs = std::to_string(90).c_str();
        err = "TEST TWO FAILED:\n\t Value Expected : ";
        err.append(rhs.c_str()).append("\n\t").append("Value Is : ").append(lhs.c_str()).append("\n");
        iter1[0].lvalue == 90 ? std::cout << "PASSED\n" : throw std::runtime_error(err.c_str());

        // Testing three
        iter1--;
        std::cout << "iter1[0].lvalue == 45 : ";
        lhs = std::to_string(iter1[0].lvalue);
        rhs = std::to_string(45).c_str();
        err = "TEST THREE FAILED:\n\t Value Expected : ";
        err.append(rhs.c_str()).append("\n\t").append("Value Is : ").append(lhs.c_str()).append("\n");
        iter1[0].lvalue == 45 ? std::cout << "PASSED\n" : throw std::runtime_error(err.c_str());

        // Testing Four
        iter1--;
        std::cout << "iter1[0].lvalue == 67 : ";
        lhs = std::to_string(iter1[0].lvalue);
        rhs = std::to_string(67).c_str();
        err = "\nTEST FOUR FAILED:\n\t Value Expected : ";
        err.append(rhs.c_str()).append("\n\t").append("Value Is : ").append(lhs.c_str()).append("\n");
        iter1[0].lvalue == 67 ? std::cout << "PASSED\n" : throw std::runtime_error(err.c_str());

    } catch(std::runtime_error& e) { std::cout << e.what() << std::endl; }
}


int main(void) {
    TEST_CYCLING();
    return 0;
}