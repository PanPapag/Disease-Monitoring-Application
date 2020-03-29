TARGET_EXEC ?= diseaseMonitor

OBJECTS_DIR ?= ./objects
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(OBJECTS_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC = gcc
CC_FLAGS = -w -g3

$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(OBJECTS_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $(CC_FLAGS) $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(OBJECTS_DIR)
	$(RM) $(TARGET_EXEC)


-include $(DEPS)

MKDIR_P ?= mkdir -p
