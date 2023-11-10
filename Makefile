# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: proberto <proberto@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/10 11:50:51 by proberto          #+#    #+#              #
#    Updated: 2023/11/09 22:49:44 by proberto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

HEADER = include
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = test

MAIN = $(SRC_DIR)/main.cpp
SRCS = $(addprefix $(SRC_DIR)/, Server.cpp ServerEventHandler.cpp) 		\
       $(addprefix $(SRC_DIR)/, ServerConnectionManager.cpp ServerChannelManager.cpp) 			\
       $(addprefix $(SRC_DIR)/, ServerCommands.cpp ServerCommandExecutor.cpp ) 		\
       $(addprefix $(SRC_DIR)/, ServerIO.cpp ServerReplies.cpp ServerUtils.cpp)	\
       $(addprefix $(SRC_DIR)/, Channel.cpp Client.cpp Logger.cpp utils.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_MAIN = $(MAIN:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TEST_SRCS = $(addprefix $(TEST_DIR)/, main-test.cpp) \
			$(addprefix $(TEST_DIR)/, kick.cpp invite.cpp topic.cpp mode.cpp) \
			$(addprefix $(TEST_DIR)/, join.cpp pass.cpp nick.cpp user.cpp) \
			$(addprefix $(TEST_DIR)/, privmsg.cpp part.cpp quit.cpp) 
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g
CC = c++

RM = rm -rf
MKDIR_P = mkdir -p


all:	$(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(NAME):    $(OBJS) $(OBJ_MAIN)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
	$(CC) -c $(CPPFLAGS) -I$(HEADER)/ $< -o $@

$(OBJ_MAIN).o:	$(SRC_DIR).cpp
	$(CC) -c $(CPPFLAGS) -I$(HEADER)/ $< -o $@

test_exec:    $(OBJS) $(TEST_OBJS)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o:	$(TEST_DIR)/%.cpp
	$(CC) -c -std=c++11 -Wall -I$(HEADER)/ -I$(TEST_DIR)/ $< -o $@

run_test:	test_exec
	./test_exec

clean:		
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME) test/test

re:	fclean all

.PHONY: all clean fclean re
