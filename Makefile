# List of all test files
TESTS = boolean_functions copy_move exeptions load_save_file push_insert_iterators_operator[] set_get_functions

# Test file folder path
TEST_PATH = test/test_cases

# Build folder for executable files
BUILD_PATH = test/build

all: build $(TESTS)

build:
	mkdir -p $(BUILD_PATH)

# Rule for each test file
$(TESTS): 
	@g++ -g -std=c++17 -I ../../src -I ../../ -o $(BUILD_PATH)/$@ $(TEST_PATH)/$@.cpp
	@echo
	@echo   
	@echo " **** running test: $@ **** "
	@echo
	@-$(BUILD_PATH)/$@


clean:
	@rm -rf $(BUILD_PATH)
