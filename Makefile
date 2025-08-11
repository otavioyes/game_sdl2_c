CC := gcc
SRCDIR := src
OBJDIR := build
BINDIR := bin

CFLAGS := -Wall -Wextra -g -Iinclude -MMD -MP $(shell pkg-config --cflags sdl2 SDL2_image)
LDLIBS := $(shell pkg-config --libs sdl2 SDL2_image)

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)
TARGET := $(BINDIR)/game

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ) | $(BINDIR)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

-include $(DEP)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: all
	./$(TARGET)
