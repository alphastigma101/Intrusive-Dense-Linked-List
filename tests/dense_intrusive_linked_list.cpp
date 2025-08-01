#include <gtest/gtest.h>
#include "../include/dense_intrusive_linked_list.h"



class DenseListTest : public ::testing::Test {

    protected:
        void TestBody() override { return; };
        
        void SetUp() override {

            return;
        }


};

TEST_F(DenseListTest, Insertion) {
    
    mlc::intrusive_dense_list<int> list;
    mlc::intrusive_dense_list_node<int> root, new_root, node2, node3, node4, node5;

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
    // Should start at root
    //EXPECT_EQ(iter1[0].lvalue, 45);

    // Test the cycling
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 10);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 90);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 45); // this is the root 
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 67);

    // Testing the cycle
    //iter1++;
    //EXPECT_EQ(iter1[0].lvalue, 45);


    // Test out of range
    //EXPECT_THROW(iter1[1], std::out_of_range);

    // Test push_back
    //list.push_back(root);
    //EXPECT_NE(iter1[1], nullptr); 

    // Test insert 
    /*list.insert(0, node3);
    EXPECT_EQ(list[0].lvalue, 10);
    EXPECT_EQ(list[0].next, nullptr);
    EXPECT_EQ(list[0].prev, nullptr);*/

}

/*TEST_F(DenseListTest, Erase) {
    
    mlc::intrusive_dense_list<int> list;
    mlc::intrusive_dense_list_node<int> root;
    mlc::intrusive_dense_list_node<int> node2;
    mlc::intrusive_dense_list_node<int> node3;

    // Initialize the node 
    root.lvalue = 45;
    node2.lvalue = 67;
    node3.lvalue = 10;
    root.next = &node3;
    root.prev = &node2;

    // Test erase
    list.push_front(root);
    list.erase(0);
    EXPECT_THROW(list[0], std::out_of_range); 

    // test pop_front
    list.push_front(root);
    list.push_back(node2);
    list.pop_front();
    EXPECT_NE(list[0].lvalue, 45);
    EXPECT_EQ(list[0].lvalue, 67);

    // Test pop_back
    list.push_front(root);
    list.push_back(node2);
    list.pop_back();
    EXPECT_THROW(list[1], std::out_of_range); 

}

TEST_F(DenseListTest, FrontAndBack) {
    
    mlc::intrusive_dense_list<int> list;
    mlc::intrusive_dense_list_node<int> root;
    mlc::intrusive_dense_list_node<int> node2;
    mlc::intrusive_dense_list_node<int> node3;

    // Initialize the node 
    root.lvalue = 45;
    node2.lvalue = 67;
    node3.lvalue = 10;
    root.next = &node3;
    root.prev = &node2;

    // Test front
    list.push_front(root);
    auto node = list.front();
    EXPECT_EQ(node.lvalue, 45); 

    // test back
    list.push_back(node2);
    auto back = list.back();
    EXPECT_EQ(back.lvalue, 67);

}

TEST_F(DenseListTest, Swap) {
    
    mlc::intrusive_dense_list<int> list;
    mlc::intrusive_dense_list<int> list2;
    mlc::intrusive_dense_list_node<int> root;
    mlc::intrusive_dense_list_node<int> node2;
    mlc::intrusive_dense_list_node<int> node3;

    // Initialize the root
    root.lvalue = 45;
    node2.lvalue = 67;
    node3.lvalue = 10;
    root.next = &node3;
    root.prev = &node2;


    // Test swap
    list.push_front(root);
    list.swap(list2);
    EXPECT_TRUE(list[0] == nullptr);

}*/


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}