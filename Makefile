# executables name
NAME = webserv
TEST_NAME = tests/webtest
DEBUG_NAME = debug_webserv

# compiler + flags
CXX = c++
GXX = g++
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
TEST_OBJ_DIR = objects_test
OBJ_DIR_DEBUG = objects_debug

# SRC files
SRC = $(shell find $(SRC_DIR) -name "*.cpp" | sed 's|^\./||')
TEST_SRC = 	$(shell find $(TEST_DIR) -name "*.cpp" | sed 's|^\./||') \
			$(shell find $(SRC_DIR) -name "*.cpp" ! -name "main.cpp"  | sed 's|^\./||')	

# OBJ files
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:%.cpp=$(TEST_OBJ_DIR)/%.o)
OBJ_DEBUG = $(SRC:%.cpp=$(OBJ_DIR_DEBUG)/%.o)

# neat temp file (design flag)
TEMP_FILE = .compiled
TEMP_FILE_DEBUG = .compiled_debug

# ANSI Colors and Commands
GREEN = \033[0;32m
RESET = \033[0m
CYAN = \033[0;36m
ORANGE = \033[38;5;208m
HIDE_CURSOR = \033[?25l
SHOW_CURSOR = \033[?25h
MOVE_UP_TWO_LINES = \033[A\033[A
RESIZE_WINDOW = \e[8;50;120t

all: design emoticon $(NAME)

debug: design_debug emoticon_debug $(DEBUG_NAME)

nof:
	@$(eval CFLAGS = $(HDRFLAGS))
	@:

TOG = 0
TOG_DEBUG = 0

$(OBJ_DIR)/%.o : %.cpp $(HDR)
	@if [ $(TOG) -eq 0 ]; then \
		echo "$(GREEN) compiling...        🚀 "; \
	fi;	
	$(eval TOG=1)
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $< -c -o $@

$(TEST_OBJ_DIR)/%.o : %.cpp $(HDR)
	@mkdir -p $(TEST_OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(TFLAGS) $< -c -o $@

# DEBUG
$(OBJ_DIR_DEBUG)/%.o : %.cpp $(HDR)
	@if [ $(TOG_DEBUG) -eq 0 ]; then \
		echo "$(GREEN) compiling debug...        🚀 "; \
	fi;	
	$(eval TOG_DEBUG=1)
	@mkdir -p $(dir $@)
	@$(GXX) $(CFLAGS) $< -c -o $@

$(NAME) : $(OBJ)
#@python3 -m pip install --upgrade pip --user -q --no-warn-script-location
# @python3 -m pip uninstall flatbuffers -y -q
# @pip install --force-reinstall --no-cache-dir flatbuffers==2.0 --user -q --no-warn-script-location

# @pip install --upgrade openai --user -q --no-warn-script-location

	@{ \
		pip install --force-reinstall --no-cache-dir flatbuffers==2.0 --user -q --no-warn-script-location \
		python3 -m pip install --upgrade pip --user -q --no-warn-script-location; \
		pip install --upgrade openai --user -q --no-warn-script-location; \
	} > /dev/null 2>&1	

	@echo -n "$(HIDE_CURSOR)"
	@$(MAKE) -s backline
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b compiled            ✅$(RESET)"	
	@sleep .2
	@echo "$(CYAN) linking...          🚀 "
	@sleep .2
	@$(MAKE) -s backline
	@$(CXX) $(OBJ) $(LDFLAGS) -o $@
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b linked              ☑️\n$(RESET)"
	@echo -n "$(SHOW_CURSOR)"
	@touch $(TEMP_FILE)  # Create a temporary file after a successful build

$(TEST_NAME) : $(TEST_OBJ)
	@echo "$(CYAN) linking tests...    🚀 "
	@$(CXX) $(TEST_OBJ) $(GTEST_LIBS) -o $@
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b tests linked              ☑️\n$(RESET)"

# DEBUG
$(DEBUG_NAME) : $(OBJ_DEBUG)
	@echo -n "$(HIDE_CURSOR)"
	@$(MAKE) -s backline
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b debug compiled            ✅$(RESET)"	
	@sleep .2
	@echo "$(CYAN) linking...          🚀 "
	@sleep .2
	@$(MAKE) -s backline
	@$(GXX) $(OBJ_DEBUG) $(LDFLAGS) -o $@
	@echo "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b debug linked              ☑️\n$(RESET)"
	@echo ""
	@echo " $(ORANGE)⛔ Do Not Launch ./debug_webserv on the CLI ⛔$(RESET)"
	@echo " $(GREEN)✅     Just click on "RUN DEBUG" in VSCode    ✅$(RESET)"
	@echo " $(ORANGE)⚠️     Don't forget to check args in launch.json    ⚠️$(RESET)"
	@echo -n "$(SHOW_CURSOR)"
	@touch $(TEMP_FILE_DEBUG)  # Create a temporary file after a successful build

test: $(TEST_NAME)
	@echo "$(CYAN) 🎉✨ Compilation successfull ! 🥳🚀$(RESET)\n"
	
emoticon:
	@mkdir -p $(OBJ_DIR)
	@echo "\n 🌐 📡 👽 🛸\n"

emoticon_debug:
	@mkdir -p $(OBJ_DIR_DEBUG)
	@echo "\n 🐛 👽 👾 🛸\n"

newline: 
	@echo ""

backline: 
	@echo "$(MOVE_UP_TWO_LINES)"

design:
	@if [ ! -f $(TEMP_FILE) ]; then \
		echo "$(RESIZE_WINDOW)"; \
		echo "$(GREEN)"; \
		./makescript.sh; \
		echo "				by The TryHardTeam Compagny © $(RESET)"; \
	fi

design_debug:
	@if [ ! -f $(TEMP_FILE_DEBUG) ]; then \
		echo "$(RESIZE_WINDOW)"; \
		echo "$(ORANGE)"; \
		./makescript_debug.sh; \
		echo "				by The DebugTryHardSuperMegaUltraGigaStrongTeam Inc.©$(RESET)"; \
	fi

clean:
	@echo "\n ▫️  cleanning $(NAME) and $(DEBUG_NAME) objects 🧻"
	@rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) $(OBJ_DIR_DEBUG)
	@$(MAKE) -s newline	

fclean:
	@echo "\n ▫️  cleanning $(NAME) and $(DEBUG_NAME) exec 🚽" 
	@rm -f $(NAME) $(TEST_NAME) $(DEBUG_NAME)
	@$(MAKE) -s clean	
	@rm -f $(TEMP_FILE) $(TEMP_FILE_DEBUG)  # Remove the temporary file during fclean

re: fclean backline all

redebug: fclean backline debug

# phony mon ami
.PHONY: all debug nof test emoticon emoticon_debug newline backline design \
	design_debug clean fclean re redebug deb eval