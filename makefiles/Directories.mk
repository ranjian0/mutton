# This must be redifined in makefiles that use this one
ROOT_DIR?=$(abspath ../)

LIB_DIR:=$(ROOT_DIR)/lib
BUILD_DIR:=$(ROOT_DIR)/build
EXAMPLES_DIR:=$(ROOT_DIR)/examples
ANDROID_BUILD_DIR:=$(BUILD_DIR)/android
DESKTOP_BUILD_DIR:=$(BUILD_DIR)/$(OS_NAME)

