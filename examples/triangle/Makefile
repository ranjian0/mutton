EXE:=Triangle
APPNAME:=Triangle
SRCS:=App.c
ANDROIDSRCS:= $(SRCS)
ROOT_DIR:=$(abspath ../../)

all: exe apk

clean:
	rm -rf build
	rm -f my-release-key.keystore

include $(ROOT_DIR)/makefiles/Executable.mk
include $(ROOT_DIR)/makefiles/AndroidAPK.mk