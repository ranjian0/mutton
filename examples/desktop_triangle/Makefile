ROOT_DIR:=$(abspath ../../)
EXE:=desktop_triangle

include ../../makefiles/OS.mk
include ../../makefiles/Directories.mk

LDFLAGS:=-L$(DESKTOP_BUILD_DIR)
LDFLAGS+= -lDesktop -lm -ldl -lpthread

CFLAGS:=-I$(LIB_DIR)/extern/GLAD -I$(LIB_DIR)/extern/glfw/include

all:
	@mkdir -p build
	clang App.c -o build/$(EXE) $(LDFLAGS) $(CFLAGS)

clean:
	rm -rf build