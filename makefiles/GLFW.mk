
# Extracted from glfw/src/CMakeLists.txt
GLFW_COMMON_SRCS:= context.c init.c input.c monitor.c vulkan.c window.c
GLFW_COCOA_SRCS:= cocoa_init.m cocoa_joystick.m cocoa_monitor.m cocoa_window.m cocoa_time.c posix_thread.c nsgl_context.m egl_context.c osmesa_context.c
GLFW_WIN32_SRCS:= win32_init.c win32_joystick.c win32_monitor.c win32_time.c win32_thread.c win32_window.c wgl_context.c egl_context.c osmesa_context.c
GLFW_X11_SRCS:= x11_init.c linux_joystick.c x11_monitor.c x11_window.c xkb_unicode.c posix_time.c posix_thread.c glx_context.c egl_context.c osmesa_context.c
GLFW_WAYLAND_SRCS:= wl_init.c wl_monitor.c wl_window.c posix_time.c posix_thread.c xkb_unicode.c egl_context.c osmesa_context.c
GLFW_OMESA_SRCS:= null_init.c null_monitor.c null_window.c null_joystick.c posix_time.c posix_thread.c osmesa_context.c


# Set platform specific GLFW srcs and flags
GLFW_SRCS:= $(GLFW_COMMON_SRCS)

ifeq ($(UNAME), Linux)
GLFW_CFLAGS= -D_GLFW_X11
GLFW_LDFLAGS:= -ldl -lm -lrt -lX11 -lGL -lpthread
GLFW_SRCS+= $(GLFW_X11_SRCS)
endif


ifeq ($(UNAME), Darwin)
GLFW_CFLAGS= -D_GLFW_COCOA
GLFW_LDFLAGS:= -framework Cocoa -framework IOKit -framework CoreFoundation 
GLFW_SRCS+= $(GLFW_COCOA_SRCS)
endif

ifeq ($(OS), Windows_NT)
GLFW_CFLAGS= -D_GLFW_WIN32
GLFW_LDFLAGS+= -lgdi32
GLFW_SRCS+= $(GLFW_WIN32_SRCS)
endif
