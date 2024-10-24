# executables name
NAME = webserv
TEST_NAME = tests/webtest

# compiler + flags
CXX = c++
CFLAGS = $(HDRFLAGS) -g -Wall -Wextra -Werror -Wconversion -std=c++98
TFLAGS = $(HDRFLAGS) -std=c++14 -I$(GTEST_INCLUDE) -pthread -DUNIT_TEST
LDFLAGS = 

# Google Test
GTEST_LIBS = -L$(GTEST_DIR)/build/lib -lgtest -lgtest_main -lpthread
GTEST_DIR = ./googletest
GTEST_INCLUDE = $(GTEST_DIR)/$(GTEST_DIR)/include
GTEST_LIB = $(GTEST_DIR)/build/lib

# headers
HDRDIRS := $(shell find . \( -name '*.h' -o -name '*.hpp' -o -name '*.tpp' \) \
	-exec dirname {} \; | sort -u)
HDRFLAGS := $(foreach dir, $(HDRDIRS), -I$(dir))
HDR = $(shell find . \( -name "*.hpp" -o -name "*.h" -o -name "*.tpp" \))

# directories
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = objects
TEST_OBJ_DIR = test_objects

# SRC files
SRC = $(shell find $(SRC_DIR) -name "*.cpp" | sed 's|^\./||')
TEST_SRC = 	$(shell find $(TEST_DIR) -name "*.cpp" | sed 's|^\./||') \
			$(shell find $(SRC_DIR) -name "*.cpp" ! -name "main.cpp"  | sed 's|^\./||')	

# OBJ files
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:%.cpp=$(TEST_OBJ_DIR)/%.o)

# neat temp file (design flag)
TEMP_FILE = .compiled

# phony mon ami
.PHONY: all clean fclean re intro test l newline backline emoticon nof

all: design emoticon $(NAME) 

nof:
	@$(eval CFLAGS = $(HDRFLAGS))
	@:
	
TOG = 0

$(OBJ_DIR)/%.o : %.cpp $(HDR)
	@if [ $(TOG) -eq 0 ]; then \
		echo "\033[0;32m compiling...        🚀 "; \
	fi;	
	$(eval TOG=1)
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $< -c -o $@

$(TEST_OBJ_DIR)/%.o : %.cpp $(HDR)
	@mkdir -p $(TEST_OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(TFLAGS) $< -c -o $@

$(NAME) : $(OBJ)
	@echo -n "\033[?25l"
	@$(MAKE) -s backline
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b compiled            ✅\033[0m"	
	@sleep .2
	@echo "\033[0;36m linking...          🚀 "
	@sleep .2
	@$(MAKE) -s backline
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b linked              ☑️\n\033[0m"
	@echo -n "\033[?25h"
	@touch $(TEMP_FILE)  # Create a temporary file after a successful build

$(TEST_NAME) : $(TEST_OBJ)
	@echo "\033[0;36m linking tests...    🚀 "
	@$(CXX) $(TEST_OBJ) $(GTEST_LIBS) -o $@
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b tests linked              ☑️\n\033[0m"

test: $(TEST_NAME)
	@echo "\033[0;36m 🎉✨ Compilation successfull ! 🥳🚀\033[0m\n"
	
emoticon:
	@mkdir -p $(OBJ_DIR)
	@echo "\n 💗 😀 😃 😍\n"

newline: 
	@echo ""

backline: 
	@echo "\033[A\033[A"

design:
	@if [ ! -f $(TEMP_FILE) ]; then \
		echo "\e[8;50;120t"; \
		echo "\033[0;32m"; \
		./makescript.sh; \
		echo "					the HardTeam Compagny Copyright\033[0m"; \
	fi

clean:
	@echo "\n ▫️  cleanning $(NAME) objects 🧻"
	@rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR)
	@$(MAKE) -s newline	

fclean:
	@echo "\n ▫️  cleanning $(NAME) exec 🚽" 
	@rm -f $(NAME)	$(TEST_NAME)
	@$(MAKE) -s clean	
	@rm -f $(TEMP_FILE)  # Remove the temporary file during fclean

re: fclean backline all
