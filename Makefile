# Makefile
# Copyright (c) 2022 Mattis Dalleau <mattis.dalleau@epitech.eu>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC 			=	gcc

AR 			=	ar rc

IFLAGS 		=	-I./include

WFLAGS 		=	-W -Wall -Werror -Wextra

CFLAGS 		=	$(WFLAGS) $(IFLAGS)

SRC_UTIL 	= 	./util/cjson_free.c \
				./util/cjson_allocator.c \
				\
				./util/object/cjson_object.c \
				./util/object/cjson_object_bucket.c \
				./util/object/cjson_object_resize.c \
				./util/object/cjson_free_object.c \
				\
				./util/array/cjson_append_array.c \
				./util/array/cjson_create_array.c \
				./util/array/cjson_free_array.c

SRC_PARSER 	=	./parser/cjson_parse_array.c \
				\
				./parser/cjson_parse_bool.c \
				\
				./parser/cjson_parse_null.c \
				\
				./parser/cjson_parse_number.c \
				\
				./parser/cjson_parse_object.c \
				\
				./parser/cjson_parse_string.c \
				\
				./parser/cjson_parse_value.c \
				\
				./parser/cjson_skip_withespaces.c

SRC 		=	cjsonify.c \
				$(SRC_UTIL) \
				$(SRC_PARSER)

OBJ 		=	$(SRC:.c=.o)

NAME 		=	libcjson.a

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

clean:
	rm -rf $(NAME)
	find . \( -name "*.o" -o -name "*.swp" -o -name "\#*\#" \) -delete

re: clean all
