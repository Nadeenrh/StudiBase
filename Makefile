CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
TARGET = studibase
SOURCE = studiBase.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).exe
