SUBDIRS := cubemx_dfu_bootloader \
           test_app

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ -j4

.PHONY: all $(SUBDIRS)

clean: 
	@for dir in $(SUBDIRS);       									\
  do                            									\
		echo "======== Cleaning $$dir ========";      \
    $(MAKE) -C $$dir clean;     									\
  done;
