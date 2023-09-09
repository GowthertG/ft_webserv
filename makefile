CFLAGS = -Wall -Werror -Wextra -I include -fsanitize=address -g -std=c++98
CPP = c++
SRC := $(shell find $(SRCS_DIR) -type f -name "*.cpp")
NAME = server
CLIENT = client

all: $(NAME)

$(NAME): $(SRC)
	@$(CPP) $(CFLAGS) $(SRC) -o $(NAME) 

clean:

fclean : clean
	@rm -rf $(NAME)
	@rm -rf $(CLIENT)


re : fclean all

client: client.cpp
	@$(CPP) $(CFLAGS) client.cpp -o client

run: 
	./server config/config_file.conf 


.PHONY: all clean fclean re