# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Libraries
LIBS = -lcurl

# Source files
SRCS = main.c fetchData.c processData.c logFile.c anomaly.c anomaly_error.c cJSON/cJSON.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = my_program

# Default rule
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to clean up the directory
clean:
	rm -f $(TARGET) $(OBJS)

