SRCS = $(wildcard $(SRC_DIR)/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

DEPS = $(wildcard $(INC_DIR)/*.hpp)

NAME = ircserv

CC = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

INC_FLAGS = -I$(INC_DIR)

SRC_DIR = sources
INC_DIR = includes
OBJ_DIR = objects

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) $(INC_FLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re