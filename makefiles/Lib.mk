include ${ROOT_DIR}/makefiles/OS.mk
include ${ROOT_DIR}/makefiles/Freetype.mk
include ${ROOT_DIR}/makefiles/Directories.mk
include ${ROOT_DIR}/makefiles/AndroidToolchain.mk 

INTERN_SRCS:= platform/platform.c \
			  application/app.c \
			  utils/String.c \
			  utils/Math.c

LIB_INTERN_SRCS:= $(addprefix $(ROOT_DIR)/src/, $(INTERN_SRCS))
LIB_EXTERN_FREETYPE_SRCS:= $(addprefix $(ROOT_DIR)/extern/freetype/src/, $(FREETYPE_SRCS))

all: build_android build_desktop 

# Build Android Static Library
ANDROID_LIB_NAME:= libMobile.a
ANDROID_LIB_SRCS:= $(NDK)/sources/android/native_app_glue/android_native_app_glue.c
ANDROID_LIB_SRCS+= $(ROOT_DIR)/extern/glfm/src/glfm_platform_android.c
ANDROID_LIB_SRCS+= $(LIB_INTERN_SRCS)
ANDROID_LIB_SRCS+= $(LIB_EXTERN_FREETYPE_SRCS)

ANDROID_CFLAGS+= -I$(NDK)/sources/android/native_app_glue -I$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/include -I$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/include/android -fPIC -DANDROIDVERSION=$(ANDROIDVERSION)
ANDROID_CFLAGS+= -I$(ROOT_DIR)/extern/glfm/include -I$(ROOT_DIR)/extern/freetype/include -I$(ROOT_DIR)/extern/freetype/src -DFT2_BUILD_LIBRARY=1
ANDROID_CFLAGS+= -I$(ROOT_DIR)/include

android_objs_arm64:
	@mkdir -p $(ANDROID_BUILD_DIR)/arm64-v8a
	$(foreach src, $(ANDROID_LIB_SRCS), $(ANDROID_CC_ARM64) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_ARM64) -c -o $(ANDROID_BUILD_DIR)/arm64-v8a/$(notdir $(basename $(src)).o) $(src);)

android_objs_arm32:
	@mkdir -p $(ANDROID_BUILD_DIR)/armeabi-v7a
	$(foreach src, $(ANDROID_LIB_SRCS), $(ANDROID_CC_ARM32) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_ARM32) -c -o $(ANDROID_BUILD_DIR)/armeabi-v7a/$(notdir $(basename $(src)).o) $(src);)

android_objs_x86:
	@mkdir -p $(ANDROID_BUILD_DIR)/x86
	$(foreach src, $(ANDROID_LIB_SRCS), $(ANDROID_CC_x86) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_x86) -c -o $(ANDROID_BUILD_DIR)/x86/$(notdir $(basename $(src)).o) $(src);)

android_objs_x86_64:
	@mkdir -p $(ANDROID_BUILD_DIR)/x86_64
	$(foreach src, $(ANDROID_LIB_SRCS), $(ANDROID_CC_x86_64) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_x86_64) -c -o $(ANDROID_BUILD_DIR)/x86_64/$(notdir $(basename $(src)).o) $(src);)


$(ANDROID_BUILD_DIR)/arm64-v8a/$(ANDROID_LIB_NAME) : android_objs_arm64
	$(ANDROID_AR_ARM64)	rcs $@ $(wildcard $(ANDROID_BUILD_DIR)/arm64-v8a/*.o)

$(ANDROID_BUILD_DIR)/armeabi-v7a/$(ANDROID_LIB_NAME) : android_objs_arm32
	$(ANDROID_AR_ARM32)	rcs $@ $(wildcard $(ANDROID_BUILD_DIR)/armeabi-v7a/*.o)

$(ANDROID_BUILD_DIR)/x86/$(ANDROID_LIB_NAME) : android_objs_x86
	$(ANDROID_AR_x86) rcs $@ $(wildcard $(ANDROID_BUILD_DIR)/x86/*.o)

$(ANDROID_BUILD_DIR)/x86_64/$(ANDROID_LIB_NAME) : android_objs_x86_64
	$(ANDROID_AR_x86_64) rcs $@ $(wildcard $(ANDROID_BUILD_DIR)/x86_64/*.o)


ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/arm64-v8a/$(ANDROID_LIB_NAME)
ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/armeabi-v7a/$(ANDROID_LIB_NAME)
ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/x86/$(ANDROID_LIB_NAME)
ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/x86_64/$(ANDROID_LIB_NAME)
build_android: $(ANDROID_LIB_TARGETS)

# Build Desktop Static Library
include ${ROOT_DIR}/makefiles/GLFW.mk

DESKTOP_LIB_NAME:= libDesktop.a
DESKTOP_LIB_SRCS:= $(foreach src,$(GLFW_SRCS),$(ROOT_DIR)/extern/glfw/src/$(src))
DESKTOP_LIB_SRCS+= $(wildcard $(ROOT_DIR)/extern/GLAD/*.c)
DESKTOP_LIB_SRCS+= $(LIB_INTERN_SRCS)
DESKTOP_LIB_SRCS+= $(LIB_EXTERN_FREETYPE_SRCS)


DESKTOP_CC=$(CC)
DESKTOP_CFLAGS+= -I$(ROOT_DIR)/extern/glfw/include -I$(ROOT_DIR)/extern/GLAD #-fPIC
DESKTOP_CFLAGS+= -I$(ROOT_DIR)/extern/freetype/include -I$(LIB_DIR)/extern/freetype/src -DFT2_BUILD_LIBRARY=1
DESKTOP_CFLAGS+= $(GLFW_CFLAGS) -I$(ROOT_DIR)/include

$(DESKTOP_BUILD_DIR)/$(DESKTOP_LIB_NAME) : desktop_objs
	ar rcs $@ $(wildcard $(DESKTOP_BUILD_DIR)/*.o)

desktop_objs:
	@mkdir -p $(DESKTOP_BUILD_DIR)
	$(foreach src, $(DESKTOP_LIB_SRCS), $(DESKTOP_CC) $(DESKTOP_CFLAGS) -c -o $(DESKTOP_BUILD_DIR)/$(notdir $(basename $(src)).o) $(src);)

DESKTOP_LIB_TARGETS += $(DESKTOP_BUILD_DIR)/$(DESKTOP_LIB_NAME)
build_desktop: $(DESKTOP_LIB_TARGETS)