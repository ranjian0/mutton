SUBDIRS = android_triangle android_triangle_touch desktop_triangle

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done