CC = gcc
CFLAGS = -DNCURSES_STATIC
LDFLAGS = -lncurses

TARGET = snake
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)