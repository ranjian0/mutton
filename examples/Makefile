SUBDIRS = android_triangle desktop_triangle

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean:
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done