.PHONY: default all clean print
default: all


#########################################################
# 确定机器平台
#
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
  TARGET_ARCH_ABI := x86_64
endif
ifeq ($(UNAME_M),aarch64)
  TARGET_ARCH_ABI := arm64-v8a
endif
$(warning TARGET_ARCH_ABI=$(TARGET_ARCH_ABI))


#########################################################
# 确定gcc版本
#
GCC_VERSION = $(shell gcc -dumpversion)
#$(warning GCC_VERSION=$(GCC_VERSION))


#########################################################
# 确定-j数目
#
MAKE_PID := $(shell echo $$PPID)
JOB_FLAG := $(filter -j%, $(subst -j ,-j,$(shell ps T | grep "^\s*$(MAKE_PID).*$(MAKE)")))
JOBS     := $(subst -j,,$(JOB_FLAG))
ifeq ($(JOBS),)
	JOBS := 1
endif
#$(warning JOBS=$(JOBS))


#########################################################
CC  := gcc
CXX	:= g++
CXXFLAGS +=
LDFLAGS +=
LIBS += 
AR := gcc-ar
ARFLAGS := rcs
glslangValidator := glslangValidator -Os

ObjectDir := obj
ifeq ($(TARGET_ARCH_ABI),x86_64)
	ObjectDir := obj/x86_64
endif
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
	ObjectDir := obj/arm64-v8a
endif

# for -D
CommonFlagsD :=
ifeq ($(TARGET_ARCH_ABI),x86_64)
	CommonFlagsD += 
endif
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
	CommonFlagsD += 
endif
CommonFlagsD += 

# for -I, -isystem
CommonFlagsI := 

# for compiler
O := -O0
CommonFlagsC := $(O) -std=c++20 -pipe -MMD -g -ggdb3 -fPIC -pthread
CommonLinkFlags := $(O) -pipe -MMD -g -ggdb3 -pthread -Wl,--no-undefined
CXX_ONLY := -std=c++20 -std=gnu++11 -std=gnu++17
C_ONLY := -std=gnu99

AllTargets :=

#######################################################################################################

#-------------------------------------------------------
# XXX
#-------------------------------------------------------
CommonFlags_XXX := \
	$(CommonFlagsC) \
	$(CommonFlagsD) \
	$(CommonFlagsI) \

SourceDir_XXX := src
Source_XXX := \
	aes.cpp
SourceMain_XXX := \
	test.cpp \
	test2.cpp \
	test3.cpp \
	test5_MyFile.cpp \

CXXFLAGS_XXX := \
	$(CommonFlags_XXX) \
	$(patsubst %, -I%, $(SourceDir_XXX)) \

CFLAGS_XXX := $(filter-out $(CXX_ONLY), $(CXXFLAGS_XXX))  #remove c++ only flags

LocalLib_XXX :=
ExtLib_XXX := 
#---------------------------------

Object_XXX := $(patsubst %.c, $(ObjectDir)/$(SourceDir_XXX)/%.o, $(Source_XXX))
Object_XXX := $(patsubst %.cpp, $(ObjectDir)/$(SourceDir_XXX)/%.o, $(Object_XXX))
Object_XXX := $(patsubst %.S, $(ObjectDir)/$(SourceDir_XXX)/%.o, $(Object_XXX))

ObjectMain_XXX := $(patsubst %.cpp, $(ObjectDir)/$(SourceDir_XXX)/%.o, $(SourceMain_XXX))
BinaryMain_XXX := $(patsubst %.cpp, $(ObjectDir)/$(SourceDir_XXX)/%, $(SourceMain_XXX))
AR_XXX := $(ObjectDir)/$(SourceDir_XXX)/libXXX.a

Depand_XXX := $(Object_XXX:.o=.d) $(ObjectMain_XXX:.o=.d)

# *.o <-- *.c
$(ObjectDir)/$(SourceDir_XXX)/%.o: $(SourceDir_XXX)/%.c
	@printf "\n" && mkdir -p $(@D)
	$(CC) $(CFLAGS_XXX) -c $< -o $@

# *.o <-- *.cpp
$(ObjectDir)/$(SourceDir_XXX)/%.o: $(SourceDir_XXX)/%.cpp
	@printf "\n" && mkdir -p $(@D)
	$(CXX) $(CXXFLAGS_XXX) -c $< -o $@

# *.o <-- *.S
$(ObjectDir)/$(SourceDir_XXX)/%.o: $(SourceDir_XXX)/%.S
	@printf "\n" && mkdir -p $(@D)
	$(CXX) $(CXXFLAGS_XXX) -c $< -o $@

# *.a <-- *.o
$(AR_XXX): $(Object_XXX)
	@printf "\n" && mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

# binary <-- *.o *.a
$(ObjectDir)/$(SourceDir_XXX)/%: $(ObjectDir)/$(SourceDir_XXX)/%.o $(Object_XXX) $(LocalLib_XXX)
	@printf "\n" && mkdir -p $(@D)
	$(CXX) $(CXXFLAGS_XXX) -Wl,--start-group $^ $(ExtLib_XXX) -Wl,--end-group -o $@
	@mkdir -p $(ObjectDir)/bin && ln -s -r -f $@ -t $(ObjectDir)/bin

.PHONY: XXX
XXX: $(Object_XXX) $(AR_XXX) $(ObjectMain_XXX) $(BinaryMain_XXX) 

-include $(Depand_XXX)
AllTargets += XXX


#######################################################################################################
all: $(AllTargets)

# debug print
print:
	@printf "JOBS=$(JOBS)\n"
	@printf "Object_XXX=$(Object_XXX)\n"
	@printf "ObjectMain_XXX=$(ObjectMain_XXX)\n"

clean:
	rm -rf $(ObjectDir)/*

