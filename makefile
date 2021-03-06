# MIT License
# Copyright (c) 2017-2021, Zixuan Wang <zxwang42@gmail.com> and Collaborators
# URL : https://github.com/TheNetAdmin/Makefile-Templates

# Template modifier par nos soins voir le rapport pour contaitre nos modifications.

# path macros
BIN_PATH := bin
OBJ_PATH := obj
#OBJ_PATH := obj_test
SRC_PATH := sources
MAIN_FILE := mimage
TEST_PATH := tests
DBG_PATH := debug
HDR_PATH := headers

SRC_PATH_EXCEPT_MAIN := $(SRC_PATH)/!($(MAIN_FILE))

# tool macros
CC ?= gcc
CCFLAGS := -Wall
DBGFLAGS := -g
CCOBJFLAGS := $(CCFLAGS) -c -I./$(HDR_PATH)

# compile macros
TARGET_NAME := mimage
TARGET_TEST := run_tests
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_TEST := $(BIN_PATH)/$(TARGET_TEST)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

ALL_SRC := $(wildcard $(SRC_PATH)/*.c)
SRC_TEST := $(foreach x, $(filter-out $(SRC_PATH)/$(MAIN_FILE).c, $(ALL_SRC)), $(wildcard $(addprefix $(x)/*,.c*))) $(foreach x, $(TEST_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ_TEST := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC_TEST))))) $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(filter-out $(SRC_PATH)/$(MAIN_FILE).c, $(ALL_SRC))))))

OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
				  $(OBJ_TEST) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(TARGET_TEST) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ)

$(TARGET_TEST): $(OBJ_TEST)
	$(CC) $(CCFLAGS) -o $@ $(OBJ_TEST)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/%.o: $(TEST_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) $(DBGFLAGS) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CCFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: test
test: makedir build-test

.PHYNONY: build-test
build-test: $(TARGET_TEST)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)