sources := main.c 

toobj = $(addsuffix .o,$(1))

-include $(sources:.c=.d)

$(sources:.c=.d): $(sources)
	gcc -MM $< -MT "abc bc" >$@
#gcc -MM $< -MT "$(patsubst %.d,%.o,$@) $@" >$@
#gcc -MM $< >$@

$(sources:.c=.o): $(sources)
	gcc -c $< -o $@

$(basename $(sources)): abc

$(basename $(sources)): $(sources:.c=.o)
	gcc $< -o $@

$(call toobj,a): a.c
	gcc -c $< -o $@

all:
	@echo $(obj)
	@echo $(a)

clean:
	rm main main.o main.d

abc:
	mkdir abc
