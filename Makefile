ROOT_DIR=$(abspath .)
include makefiles/Lib.mk

all: lib #examples

examples:
	@$(MAKE) -C $@

lib_desktop: build_desktop 

lib_android: build_android

lib: lib_desktop lib_android 

.PHONY: examples

clean:
	@rm -rf build
	@$(MAKE) -C examples clean

clean_objs:
	find build -type f -name '*.o' -delete 

copy_include:
	@mkdir -p build/include
	cp -r include/* build/include/
