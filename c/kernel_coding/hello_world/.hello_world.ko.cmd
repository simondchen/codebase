cmd_/home/simon/test/c/kernel_coding/hello_world/hello_world.ko := ld -r -m elf_x86_64 -T /usr/src/linux-headers-3.13.0-34-generic/scripts/module-common.lds --build-id  -o /home/simon/test/c/kernel_coding/hello_world/hello_world.ko /home/simon/test/c/kernel_coding/hello_world/hello_world.o /home/simon/test/c/kernel_coding/hello_world/hello_world.mod.o