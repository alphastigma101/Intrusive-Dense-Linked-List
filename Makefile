CXX := g++
CXXFLAGS := -std=c++2b -g -O0
LDFLAGS := -L ./tests -lgtest -lgtest_main -pthread



TEST_DENSE_LIST := test_dense_list 
TEST_MAIN_DENSE_LIST := test_main_dense_list
SANITIZE_MAIN_DENSE_LIST := sanitize_main_dense_list
INCLUDE := -I include/

# -fno-exceptions

$(TEST_DENSE_LIST):
	$(CXX) $(CXXFLAGS) -fPIC -fno-rtti $(INCLUDE) -o $(TEST_DENSE_LIST) tests/dense_intrusive_linked_list.cpp $(LDFLAGS)

$(TEST_MAIN_DENSE_LIST):
	$(CXX) $(CXXFLAGS)  $(INCLUDE) -o $(TEST_MAIN_DENSE_LIST) main.cpp

$(SANITIZE_MAIN_DENSE_LIST) :
	$(CXX) $(CXXFLAGS) -fsanitize=address -fno-omit-frame-pointer $(INCLUDE) -o $(SANITIZE_MAIN_DENSE_LIST) main.cpp

clean:
	rm -rf $(TEST_DENSE_LIST) $(TEST_MAIN_DENSE_LIST) $(SANITIZE_MAIN_DENSE_LIST)

all: $(TEST_DENSE_LIST) $(TEST_MAIN_DENSE_LIST) $(SANITIZE_MAIN_DENSE_LIST)