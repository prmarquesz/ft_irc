# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: proberto <proberto@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/10 11:50:51 by proberto          #+#    #+#              #
#    Updated: 2023/10/15 14:11:46 by proberto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

HEADER = include
SRC_DIR = src
OBJ_DIR = obj

SRCS = $(addprefix $(SRC_DIR)/, main.cpp Server.cpp ServerEventHandler.cpp) 		\
       $(addprefix $(SRC_DIR)/, ServerConnectionManager.cpp ServerChannelManager.cpp) 			\
       $(addprefix $(SRC_DIR)/, ServerCommands.cpp ServerCommandExecutor.cpp ) 		\
       $(addprefix $(SRC_DIR)/, ServerIO.cpp ServerReplies.cpp ServerUtils.cpp)	\
       $(addprefix $(SRC_DIR)/, Channel.cpp Client.cpp Logger.cpp utils.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CC = c++

RM = rm -rf
MKDIR_P = mkdir -p


all:	$(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(NAME):    $(OBJS)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
	$(CC) -c $(CPPFLAGS) -I$(HEADER)/ $< -o $@

clean:		
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
