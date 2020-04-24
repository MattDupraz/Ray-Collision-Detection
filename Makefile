OBJDIR = obj
INCDIR = inc
SRCDIR = src

CC = g++
CFLAGS = -I./$(INCDIR)
LIBS = -lsfml-graphics -lsfml-window -lsfml-system


_HEADERS = app.h collisions.h math_utils.h ball.h
HEADERS = $(patsubst %,$(INCDIR)/%,$(_HEADERS))

_OBJECTS = app.o collisions.o math_utils.o ball.o main.o
OBJECTS = $(patsubst %,$(OBJDIR)/%,$(_OBJECTS))

TARGET = app

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
