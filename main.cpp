#include "include/dense_intrusive_linked_list.h"
#include <stdexcept>

static mlc::intrusive_dense_list<int> list;







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
    //list.insert_end(0, 90);
    //list.make_bi_node();

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

    /*list.push_back(199);
    list.insert_before(1, 700);
    mlc::intrusive_dense_list_iterator<int> iter2 = list.begin();
    std::cout << "iter2 value is : " << iter2[1].lvalue << std::endl;
    iter2++;
    std::cout << "iter2 value is : " << iter2[1].lvalue << std::endl;
    
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;
    iter1++;
    std::cout << "iter1 value is : " << iter1[0].lvalue << std::endl;*/



    return 0;
}