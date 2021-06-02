APPNAME:=AndroidTriangle
ANDROIDSRCS:= src/App.c
ROOT_DIR:=$(abspath ../../)

all: apk

clean:
	rm -rf build
	rm -f my-release-key.keystore

include $(ROOT_DIR)/makefiles/AndroidAPK.mk