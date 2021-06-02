SUBDIRS = lib examples

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: $(SUBDIRS)

clean:
	@rm -rf build
	@$(MAKE) -C examples clean

	
