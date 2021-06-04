EXE?=app
SRCS?=

include $(ROOT_DIR)/makefiles/OS.mk
include $(ROOT_DIR)/makefiles/Directories.mk

LDFLAGS:=-L$(DESKTOP_BUILD_DIR)
LDFLAGS+= -lDesktop -lm -ldl -lpthread -lGL

CFLAGS:=-I$(ROOT_DIR)/extern/GLAD -I$(ROOT_DIR)/extern/glfw/include -I$(ROOT_DIR)/include -I$(ROOT_DIR)/extern/freetype/include

exe:
	@mkdir -p build/
	gcc -g $(SRCS) -o build/$(EXE) $(LDFLAGS) $(CFLAGS)
