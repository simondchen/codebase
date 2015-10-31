SPLASH = /
listf = $(filter %.c,$(wildcard $(addsuffix $(SPLASH)*,$(1))))
totarget = $(basename $(1))
files = $(call listf,.)
bins = $(call totarget,$(files))

define cc_template
$$(basename $(1)): $(1)
	gcc -o $$@ $$<
endef

define cc_compile
$$(foreach f,$(1),$$(eval $$(call cc_template,$$(f))))
endef

#TARGETS := $(bins)
$(bins):

$(eval $(call cc_compile,$(files)))

#here we can use invalid rules

.DEFAULT_GOAL := TARGETS
TARGETS: $(TARGETS)

all:
	@echo $(bins)

clean:
	rm $(bins)
