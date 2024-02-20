CC = w64gcc
OUTPUT = build
NAME = test
OS = WINDOWS

SRC-DIR = src
SRC-FILES = $(notdir $(wildcard $(SRC-DIR)/*.c))

DEPS-DIR = $(SRC-DIR)/deps
DEPS-SRC = $(notdir $(wildcard $(DEPS-DIR)/*.c))

INCLUDE = -I"include" -I"." -I"deps"
LIBS = -luser32 -lkernel32 -lgdi32 -lopengl32 -luuid -lole32 -lcomctl32
FLAGS = -std=c99 -Wall -Wextra -Wpedantic

# do not edit this
SRC-OBJ = $(addprefix $(OUTPUT)/, $(SRC-FILES:.c=.o))
DEPS-OBJ = $(addprefix $(OUTPUT)/, $(DEPS-SRC:.c=.o))

ifeq ($(OS),WINDOWS)
	EXE = $(OUTPUT)/$(NAME).exe
else
	EXE = $(OUTPUT)/$(NAME)
endif

# 'make'
all: $(OUTPUT) $(EXE) run

# Run the exe.
run: $(EXE)
	./$(EXE)

# Clean the 'build' folder.
clean:
	rm $(OUTPUT)/**


# Compile each time the main file or `sili.h` is changed.
$(EXE): $(DEPS-OBJ) $(SRC-OBJ)
	$(CC) $(FLAGS) $^ $(LIBS) -o $@

$(OUTPUT)/%.o: $(SRC-DIR)/%.c
	$(CC) $(FLAGS) $(INCLUDE) -c $^ -o $(OUTPUT)/$(notdir $@)

$(OUTPUT)/%.o: deps/%.h
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) -c $(DEPS-DIR)/$(basename $(notdir $^)).c -o $(OUTPUT)/$(notdir $@)


# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
