#============================================================================
# PROJECT: GEM-Tools library
# FILE: Makefile
# DATE: 31/10/2012
# DESCRIPTION: Top-level makefile
#============================================================================

# Definitions
ROOT_PATH=$(CURDIR)
include Makefile.mk

all: devel

# Optimized 
release: setup
	$(MAKE) --directory=resources release
	$(MAKE) --directory=src release
	$(MAKE) --directory=tools release
	
# Optimized + Static
static:	setup
	$(MAKE) --directory=resources release
	$(MAKE) --directory=src release
	$(MAKE) --directory=tools static
	
# Optimized + DebugSymbols + GEMProfile
devel: setup
	$(MAKE) --directory=resources devel
	$(MAKE) --directory=src devel
	$(MAKE) --directory=tools devel
	
# Optimized + DebugSymbols + GEMProfile + VtuneLib
profile: setup
	$(MAKE) --directory=resources profile
	$(MAKE) --directory=src profile
	$(MAKE) --directory=tools profile
	
# DebugSymbols + GEMDebug + GEMProfile
debug: setup
	$(MAKE) --directory=resources debug
	$(MAKE) --directory=src debug
	$(MAKE) --directory=tools debug
	
check: setup debug
	$(MAKE) --directory=test check
	
setup: 
	@mkdir -p $(FOLDER_BIN) $(FOLDER_BUILD) $(FOLDER_LIB)

clean:
	$(MAKE) --directory=resources clean
	$(MAKE) --directory=test clean
	@rm -rf $(FOLDER_BIN) $(FOLDER_BUILD) $(FOLDER_LIB)
