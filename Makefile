.PHONY: all clean error_example

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

$(if $(wildcard math.so), \
	$(eval load math.so), \
	$(eval math.so:))

all: error_example
	$(info 4 + 5 + 9 = $(add 4, 5, 9))
	$(info 4 - 5 - 9 = $(sub 4, 5, 9))
	$(info 4 * 5 * 9 = $(mul 4, 5, 9))
	$(info 10 / 2 / 5 = $(div 10, 2, 5))

error_example:
	$(info This should be an error: 5 - apple = $(sub 5, apple))
	$(info This should be an error: 1 / 0 = $(div 1, 0))

math.so: math.c
	$(CC) -shared -fPIC -o $@ $<
	$(if $(wildcard $@),,$(MAKE) -f $(ROOT_DIR)Makefile)

clean:
	$(RM) *.so
