all:

include .config

path := $(patsubst %/,%,$(path))

app_dir := bh/app/

# check if $(path) is the sub directory of app
ifeq ($(patsubst $(app_dir)%,$(app_dir),$(path)),$(app_dir))
include build/rules/app.mk
else
include $(path)/Makefile

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(ASFLAGS) -c $< -o $@
endif

include build/rules/common.mk

PHONY := $(foreach n, $(PHONY), $(path)/$(n))

obj-y := $(foreach n, $(obj-y), $(path)/$(n))
subdir-obj := $(foreach n, $(dir-y), $(path)/$(n)/built-in.o)

builtin-obj := $(path)/built-in.o

all: $(dir-y) $(builtin-obj)

$(builtin-obj): $(obj-y) $(subdir-obj)
	@echo $(obj-y)
	$(LD) $(LDFLAGS) -r $^ -o $@

PHONY += $(dir-y)
$(dir-y):
	@make $(obj_build)$(path)/$@

clean: .config
	@for dir in $(dir-y); do \
		make $(obj_build)$(path)/$$dir clean; \
	 done
	@rm -vf $(path)/*.o

.PHONY: $(PHONY) FORCE