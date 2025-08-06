#include "include/dense_intrusive_linked_list.h"
#include <stdexcept>

mlc::intrusive_dense_list<int> list;
//static mlc::intrusive_dense_list_node<int> root, new_root, node2, node3, node4, node5;




int main(void) {
    //TEST_CYCLING();
    //TEST_BACKEND();
    list.push_back(10);
    list.insert_before(0, 20);
    list.insert_before(0, 30);
    list.insert_before(0, 50);
    list.insert_before(0, 100);
    list.insert_before(0, 200);
    list.insert_before(0, 300);

    mlc::intrusive_dense_list_iterator<int> iter1 = list.begin();
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1++;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1++;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1++;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1++;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1++;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1++;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    
    std::cout << "\n================================\n" << std::endl;
    
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1--;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1--;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1--;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1--;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1--;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1--;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;




    return 0;
}