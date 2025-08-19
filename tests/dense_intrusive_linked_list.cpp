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

TEST_F(DenseListTest, InsertBefore) {

    list.push_back(10);
    list.insert_before(0, 20);
    list.insert_before(0, 30);
    list.insert_before(0, 50);
    list.insert_before(0, 100);
    list.insert_before(0, 200);
    list.insert_before(0, 300);

    mlc::intrusive_dense_list_iterator<int> iter1 = list.begin();

    EXPECT_EQ(iter1[0].lvalue, 10);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 20);

    list.push_back(199);
    list.insert_before(1, 700);
    list.insert_before(1, 900);
    list.insert_before(1, 1000);
    list.insert_before(1, 5000);

    mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);
    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 700);

}

TEST_F(DenseListTest, Cycling) {

    // Test at() method 
    mlc::intrusive_dense_list_iterator<int> iter1 = list.at(0);
    mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);

    EXPECT_EQ(iter1[0].lvalue, 10);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 20);
    
    EXPECT_EQ(iter1[0].lvalue, 20);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 10);

    EXPECT_EQ(iter1[0].lvalue, 10);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 300);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 200);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 100);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 50);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 30);
    iter1--;
    EXPECT_EQ(iter1[0].lvalue, 20);

    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 700);

    EXPECT_EQ(iter2[1].lvalue, 700);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 199);

    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 5000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 1000);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 900);
    iter2--;
    EXPECT_EQ(iter2[1].lvalue, 700);

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

}

TEST_F(DenseListTest, PushNodeBack) {

    list.push_back(0, 666);
    list.push_back(1, 1234);
    list.push_back(0, 2345);
    list.push_back(1, 5678);

    mlc::intrusive_dense_list_iterator<int> iter2 = list.at(1);

    mlc::intrusive_dense_list_iterator<int> iter1 = list.at(0);
    EXPECT_EQ(iter2[1].lvalue, 199);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 225);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 190);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 160);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 130);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 1234);
    iter2++;
    EXPECT_EQ(iter2[1].lvalue, 5678);

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
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 666);
    iter1++;
    EXPECT_EQ(iter1[0].lvalue, 2345);
    iter1++;

}

TEST_F(DenseListTest, Insert) {

    list.push_back(5);
    list.push_back(2, 10);
    list.push_back(2, 20);
    list.insert_before(2, 30);

    mlc::intrusive_dense_list_iterator<int> iter3 = list.at(2);

    EXPECT_EQ(iter3[2].lvalue, 5);
    iter3++;
    EXPECT_EQ(iter3[2].lvalue, 10);
    iter3++;
    EXPECT_EQ(iter3[2].lvalue, 20);
    iter3++;

    list.insert_after(2, 40);
    list.insert_before(2, 60);
    iter3--;
    iter3--;
    EXPECT_EQ(iter3[2].lvalue, 30);
    iter3++;
    EXPECT_EQ(iter3[2].lvalue, 60);
    iter3++;
    EXPECT_EQ(iter3[2].lvalue, 5);
    iter3++;
    EXPECT_EQ(iter3[2].lvalue, 40);
    iter3++;
    EXPECT_EQ(iter3[2].lvalue, 10);
    iter3++;
    EXPECT_EQ(iter3[2].lvalue, 20);

}


TEST_F(DenseListTest, Iteration) {

    // Test begin() - should point to first element (root)
    /*{
        auto it = list.begin();
        EXPECT_NE(it, list.end());
        EXPECT_EQ(*it, 10);
        
        // Test const begin()
        const auto& const_list = list;
        auto const_it = const_list.begin();
        EXPECT_EQ(*const_it, 10);
        
        // Test cbegin()
        auto cbegin_it = list.cbegin();
        EXPECT_EQ(*cbegin_it, 10);
    }

    // Test end() - should be valid for lists with size > 1
    {
        auto end_it = list.end();
        EXPECT_NE(end_it, list.begin());
        
        // Test accessing last element via end()
        --end_it;
        EXPECT_EQ(*end_it, 50);
        
        // Test const end()
        const auto& const_list = list;
        auto const_end_it = const_list.end();
        --const_end_it;
        EXPECT_EQ(*const_end_it, 50);
        
        // Test cend()
        auto cend_it = list.cend();
        --cend_it;
        EXPECT_EQ(*cend_it, 50);
    }

    // Test reverse iterators
    {
        auto rit = list.rbegin();
        EXPECT_EQ(*rit, 50);  // Should point to last element
        
        auto rend_it = list.rend();
        --rend_it;
        EXPECT_EQ(*rend_it, 10);  // Should point to first element when decremented
        
        // Test const reverse iterators
        const auto& const_list = list;
        auto const_rit = const_list.rbegin();
        EXPECT_EQ(*const_rit, 50);
        
        auto crbegin_it = list.crbegin();
        EXPECT_EQ(*crbegin_it, 50);
    }

    // Test forward iteration through all elements

    // Test iterator comparisons
    {
        EXPECT_NE(list.begin(), list.end());
        EXPECT_EQ(list.begin(), list.begin());
        EXPECT_EQ(list.end(), list.end());
        EXPECT_NE(list.rbegin(), list.rend());
    }*/

}

TEST_F(DenseListTest, Pop) {
    list.pop_front(0);

}

TEST_F(DenseListTest, Erase) {
    

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}