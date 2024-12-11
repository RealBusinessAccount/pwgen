# Compiler and Flags:
CC = gcc
CFLAGS = -Wall -Wextra

# Target and source files:
TARGET = mkpass
SRCS = mkpass.c
OBJS = $(SRCS:.c=.o)

#Default target:
all: $(TARGET)

# Rule to build the target:
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files:
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build files:
clean:
	rm -f $(TARGET) $(OBJS)

# Install the binary to /usr/local/bin:
install: $(TARGET)
	install -m 0755 $(TARGET) /usr/local/bin/

# Uninstall the binary:
uninstall: rm -f /user/local/bin/$(TARGET)

# Phony targets
.PHONY: all clean install uninstall
