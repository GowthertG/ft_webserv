# Color 
RED     = \033[0;31m
DEFAULT = \033[0;39m
PINK    = \033[1;35m
BLUE    = \033[1;34m

# MakeFILE
CFLAGS   = -Wall -Wextra -Werror -std=c++98 -I include -fsanitize=address -static-libsan
CPP      = c++
NAME     = server
SRCS_DIR = src
OBJS_DIR = obj
PRINTF   = printf

all: $(NAME)

SRCS := $(shell find $(SRCS_DIR) -type f -name "*.cpp")

# Convert source file paths to object file names in the obj directory
OBJS = $(patsubst $(shell find $(SRCS_DIR) -type f -name "*.cpp") \
/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))

# Find subdirectories in the source directory
SUBDIRS := $(shell find $(SRCS_DIR) -type d)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)
	@$(PRINTF) "\r%100s\r$(BLUE)$(NAME) is up to date!$(DEFAULT)\n"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@$(foreach dir,$(SUBDIRS),mkdir -p $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(dir));)

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.cpp 
	@$(CPP) $(CFLAGS) -c $< -o $@
	@$(PRINTF) "\rCompiling $(BLUE)$<$(DEFAULT)..."

clean:
	@rm -rf $(OBJS_DIR)
	@$(PRINTF) "$(PINK)Cleaning $(NAME) ... $(DEFAULT)\n"
	@$(PRINTF) "$(RED) Objects_files removed successfully. $(DEFAULT)\n"

fclean : clean
	@rm -rf $(NAME)
	@$(PRINTF) "$(RED) $(NAME) removed successfully. $(DEFAULT)\n"
	
re : fclean all
		@clear

.PHONY: all clean fclean re 
