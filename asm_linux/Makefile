cpuid_print:cpuid_print.o
	ld -m elf_i386 -lc --dynamic-linker /lib/ld-linux.so.2 -o cpuid_print cpuid_print.o
cpuid_print.o:cpuid_print.s
	as --32 -o cpuid_print.o cpuid_print.s

clean:
	rm cpuid_print.o cpuid_print
