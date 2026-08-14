CC      = gcc
CFLAGS  = -g -Wall -Wextra
RM      = rm -f
SRC_DIR = src
INC_DIR = inc

# Collect C sources from src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Object files (keep path so the pattern rule works)
OBJS = $(SRCS:.c=.o)

TARGET = webserver

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)