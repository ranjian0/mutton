ROOT_DIR:=$(abspath ../../)
EXE:=desktop_triangle_mutton

include $(ROOT_DIR)/makefiles/OS.mk
include $(ROOT_DIR)/makefiles/Directories.mk

LDFLAGS:=-L$(DESKTOP_BUILD_DIR)
LDFLAGS+= -lDesktop -lm -ldl -lpthread -lGL

CFLAGS:=-I$(LIB_DIR)/extern/GLAD -I$(LIB_DIR)/extern/glfw/include -I$(LIB_DIR)/intern -I$(LIB_DIR)/extern/freetype/include

all:
	@mkdir -p build
	clang App.c -o build/$(EXE) $(LDFLAGS) $(CFLAGS)

clean:
	rm -rf build