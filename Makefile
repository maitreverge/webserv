# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svidot <svidot@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/28 13:41:27 by seblin            #+#    #+#              #
#    Updated: 2024/09/04 19:04:44 by svidot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe
CXX = c++

HDRDIRS := $(shell find . \( -name '*.h' -o -name '*.hpp' -o -name '*.tpp' \) \
	-exec dirname {} \; | sort -u)
SRC_DIR = .
OBJ_DIR = objects
CFLAGS = $(HDRFLAGS) -g -Wall -Wextra -Werror -Wconversion -std=c++98
LDFLAGS = 

HDRFLAGS := $(foreach dir, $(HDRDIRS), -I$(dir))
HDR = $(shell find . \( -name "*.hpp" -o -name "*.h" -o -name "*.tpp" \))
SRC = $(shell find . -name "*.cpp" | sed 's|^\./||')
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean fclean re intro l newline backline emoticon nof

all: intro $(NAME) 

l: design emoticon $(NAME) 

nof:
	@$(eval CFLAGS = $(HDRFLAGS))
	@:
	
TOG = 0

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(HDR)
	@if [ $(TOG) -eq 0 ]; then \
		echo "\033[0;32m compiling...        🚀 "; \
	fi;	
	$(eval TOG=1)
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $< -c -o $@

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

emoticon:
	@echo "\n 💗 😀 😃 😍\n"
	@mkdir -p $(OBJ_DIR)

newline: 
	@echo ""

backline: 
	@echo "\033[A\033[A"

design:
	@echo "\033[0;32m"
	@cat mfile_design
	@echo "\033[0m"

intro:
	@echo -e "\033[?25l"
	clear
	@sleep 1
	@echo -n " Wake"
	@sleep .4
	@echo " up...\n"
	@sleep .1
	@echo -n " "
	@letters="The Makefile has you..."; \
	echo "\033[1;92m$$letters\033[0m" | awk '{split($$0, chars, ""); \
	for (i = 1; i <= length($$0); i++) \
	{printf "%s", chars[i]; system("sleep 0.2");}} \
	system("sleep 1");' 
	@words="\n\n\tFollow \n\t\tthe \t\b\b\033[0;96mColored! \
	\n\n\t\t\b\b\b\b\b\b\033[1;95mUnicorne!\033[0m...🦄\n\n"; \
	for j in $${words}; do \
		echo -n "$$j"; \
		sleep .4; \
	done
	@sleep .4
	@$(MAKE) -s design
	@$(MAKE) -s emoticon
	@echo "\033[?25h"
	@$(MAKE) -s backline

clean:
	@echo "\n ▫️  cleanning $(NAME) objects 🧻"
	@rm -rf $(OBJ_DIR) 
	@$(MAKE) -s newline	

fclean:
	@echo "\n ▫️  cleanning $(NAME) 🚽" 
	@rm -f $(NAME)	
	@$(MAKE) -s clean	

re: fclean backline l
