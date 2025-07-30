CXX := g++
CXXFLAGS := -std=c++20 -g -O3 -fPIC
LDFLAGS := -L ./tests -lgtest -lgtest_main -pthread



TEST_DENSE_LIST := test_dense_list 
INCLUDE := -I include/



$(TEST_DENSE_LIST):
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(TEST_DENSE_LIST) tests/dense_intrusive_linked_list.cpp $(LDFLAGS)


clean:
	rm -rf $(TEST_DENSE_LIST)