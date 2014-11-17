#==================================================================================================
# PROJECT: GEMMapper
# FILE: Makefile.mk
# DATE: 02/10/2012
# AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
# DESCRIPTION: Makefile definitions' file
#==================================================================================================

# Sys
PLATFORM=$(shell uname)

# Utilities
CC=gcc
AR=ar

# Folders
FOLDER_BIN=$(ROOT_PATH)/bin
FOLDER_BUILD=$(ROOT_PATH)/build
FOLDER_DATASETS=$(ROOT_PATH)/datasets
FOLDER_INCLUDE=$(ROOT_PATH)/include
FOLDER_LIB=$(ROOT_PATH)/lib
FOLDER_RESOURCES=$(ROOT_PATH)/resources
FOLDER_RESOURCES_BUILD=$(ROOT_PATH)/resources/build

ifeq ($(PLATFORM),Darwin)
FOLDER_RESOURCES_LIB=$(ROOT_PATH)/resources/lib/mac
else
FOLDER_RESOURCES_LIB=$(ROOT_PATH)/resources/lib
endif

FOLDER_RESOURCES_INCLUDE=$(ROOT_PATH)/resources/include
FOLDER_SOURCE=$(ROOT_PATH)/src
FOLDER_TEST=$(ROOT_PATH)/test
FOLDER_TOOLS=$(ROOT_PATH)/tools

# Configure flags
HAVE_ZLIB = 1
HAVE_BZLIB = 1
HAVE_OPENMP = 1
HAVE_CUDA = 1

# OpenMP Lib
ifeq ($(HAVE_OPENMP),1)
  DEF_OPENMP=-DHAVE_OPENMP
  LIBS_OPENMP+=-fopenmp
endif
# GZip Lib
ifeq ($(HAVE_ZLIB),1)
  DEF_ZLIB=-DHAVE_ZLIB
  LIBS_ZLIB+=-lz
endif
# BZip Lib
ifeq ($(HAVE_BZLIB),1)
  DEF_BZLIB=-DHAVE_BZLIB
  LIBS_BZLIB+=-lbz2
endif
# CUDA Lib
ifeq ($(HAVE_CUDA),1)
  DEF_CUDA=-DHAVE_CUDA
  CUDA_PATH=/usr/local/cuda
  NVCC=$(CUDA_PATH)/bin/nvcc
  LIBS_CUDA=-L$(CUDA_PATH)/lib64 -lcuda -lcudart
  INC_CUDA=-I$(CUDA_PATH)/include
endif

# Flags
ARCH_FLAGS=-D__LINUX__
GENERAL_FLAGS=-fPIC -Wall $(ARCH_FLAGS)

ARCH_OPT_FLAGS=-msse3 -mssse3 -msse4.2
OPT_FLAGS=-Ofast $(ARCH_OPT_FLAGS) # -fomit-frame-pointer -ftree-vectorize 

INCLUDE_FLAGS=-I$(FOLDER_INCLUDE) -I$(FOLDER_RESOURCES_INCLUDE)
LIB_PATH_FLAGS=-L$(FOLDER_LIB) -L$(FOLDER_RESOURCES_LIB)

SUPPRESS_CHECKS=-DNDEBUG -DGEM_NOPROFILE
DEBUG_FLAGS=-g -ggdb3 -rdynamic
GEM_DEBUG_FLAGS=-DGEM_DEBUG 
