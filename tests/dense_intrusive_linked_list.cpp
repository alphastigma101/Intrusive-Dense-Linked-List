#include <gtest/gtest.h>
#include "../include/dense_intrusive_linked_list.h"

static mlc::intrusive_dense_list<int> list;

class DenseListTest : public ::testing::Test {

    protected:
        void TestBody() override { return; };
        
        void SetUp() override {

            return;
        }


};

TEST_F(DenseListTest, Insertion) {

    list.push_back(10);
    list.insert_before(0, 20);
    list.insert_before(0, 30);
    list.insert_before(0, 50);
    list.insert_before(0, 100);
    list.insert_before(0, 200);
    list.insert_before(0, 300);

    mlc::intrusive_dense_list_iterator<int> iter1 = list.begin();

    EXPECT_EQ(iter1[0].lvalue, 20);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 10);

    // Testing to see if the nodes are different 
    list.push_back(199);
    list.insert_before(1, 700);
    list.insert_before(1, 900);
    list.insert_before(1, 1000);
    list.insert_before(1, 5000);

    mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);
    EXPECT_EQ(iter2[1].lvalue, 700);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 199);

}

TEST_F(DenseListTest, Cycling) {

    // Test at() method 
    mlc::intrusive_dense_list_iterator<int> iter1 = list.at(0);
    mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);

    EXPECT_EQ(iter1[0].lvalue, 20);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 10);
    
    EXPECT_EQ(iter1[0].lvalue, 10);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 20);

    EXPECT_EQ(iter1[0].lvalue, 20);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 10);

    EXPECT_EQ(iter2[1].lvalue, 700);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 199);

    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 700);

    EXPECT_EQ(iter2[1].lvalue, 700);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 199);

}

TEST_F(DenseListTest, InsertAfter) {

    list.insert_after(0, 40);
    list.insert_after(0, 60);
    list.insert_after(0, 80);
    list.insert_after(0, 90);
    list.insert_after(0, 110);
    list.insert_after(0, 120);

    mlc::intrusive_dense_list_iterator<int> iter1 = list.at(0);

    EXPECT_EQ(iter1[0].lvalue, 10);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 120);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 110);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 90);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 80);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 60);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 40);

    list.insert_after(1, 130);
    list.insert_after(1, 160);
    list.insert_after(1, 190);
    list.insert_after(1, 225);

    mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);
    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 225);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 190);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 160);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 130);

    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2--;
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 700);

}


TEST_F(DenseListTest, PushNodeBack) {

    /*list.insert_end(0, 666);
    list.insert_end(1, 1234);
    list.insert_end(0, 2345);
    list.insert_end(1, 5678);*/

    mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);
    mlc::intrusive_dense_list_iterator<int> iter1 = list.at(0);
    
   

}

TEST_F(DenseListTest, PushNodeFront) {
    
   /*list.insert_begining(0, 774);
   list.insert_begining(1, 890);
   list.insert_begining(0, 4987);
   list.insert_begining(1, 224);*/
   
   mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);
   mlc::intrusive_dense_list_iterator<int> iter1 = list.at(0);


}




TEST_F(DenseListTest, Erase) {
    

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}