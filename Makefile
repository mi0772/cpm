# Compilatore
CC = cc

# Flags di compilazione
CFLAGS = -Wall -Wextra -Iinclude

# Directory
SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

# Tutti i .c in src/
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Converti .c in .o nella build dir
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Output
TARGET = cpm

# Regole
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# Come compilare i .o nella build dir
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crea la build dir se non esiste
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Pulizia
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
