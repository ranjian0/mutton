ROOT_DIR=$(abspath .)
include makefiles/Lib.mk

all: lib examples

examples:
	@$(MAKE) -C $@

lib_desktop: build_desktop 

lib_android: build_android

lib: lib_android lib_desktop

.PHONY: examples

clean:
	@rm -rf build
	@$(MAKE) -C examples clean

	
