# ASM to C interaction

We chose to write the stub in NASM, to ensure its lightness and speed. However, in order to be functionnal, some variables need to be set with values that are computed at `./woody-woodpacker` runtime, and so must be somehow passed to the stub. We also need to get the stub itself into the program header.

## Get the stubs raw bytes in the executable header

To do that, we compiled the stub in several steps:

```bash
	# first compilation from a nasm file to a regular .o file, complete with symbols
	nasm -f elf64 $(SRC_DIR)stub.nasm -o $(BUILD_DIR)stub.o 
	# converting that .o file to raw bytes
	objcopy -O binary $(BUILD_DIR)stub.o $(BUILD_DIR)stub.bin
	# retransforming that binary file to an .o to link it with the .woody-woodpacker executable
	cd $(BUILD_DIR) && objcopy -I binary -O elf64-x86-64 -B i386:x86-64 stub.bin stub_embed.o
```

Great ! You may now access the native variables directly from your `.c` files :

```c
	extern unsigned char _binary_stub_bin_start[];
	extern unsigned char _binary_stub_bin_end[];
```

This will allow you to get the start address and the size of your stub to be able to copy it in your executable segment.

## Patching variable values

In order to patch the values of our variables, we applied the following strategy:

1. In a terminal, use `nm` on your first `.o` file to get the offsets of your symbols and take notes of the relevant ones
2. In your source files, after you copied the stub in your file segment, incorporate those offsets to your calculations in order to reach the exact adress at which your symbol is stored, and patch its value (it's just a pointer !)
3. Voila, you stub variables have the correct values !

Let's see an example with `o_entry`, that we have to patch with the program's old entrypoint:

```bash
# we grep on 't' because we only care about variables in the t section
# if you declared them elsewhere, you're on your own 
[woody-woodpacker] nm .dir_build/stub.o | grep -w 't' 
0000000000000071 t _decrypt_text
000000000000001b t _get_offset
00000000000001a1 t hex_key
000000000000007d t _init_S
0000000000000173 t key
000000000000008f t _ksa
00000000000000a0 t _loop_ksa
00000000000000ed t _loop_prga
000000000000002c t _mprotect
0000000000000183 t msg
00000000000001c2 t o_entry # <= here !
00000000000000d1 t _prga
0000000000000148 t _run_text
00000000000001d2 t S
00000000000002d2 t _stub_end
00000000000001ca t stub_vaddr
000000000000013b t _swap_values
0000000000000163 t text
000000000000016b t text_size
```

```c
// in the relevant .c, report the offsets 
#define OENTRY_OFF 0x1c2
// and then use them to declare pointers to the addresses :
Elf64_Addr *o_entry = (Elf64_Addr *)(file_map + exec_seg->xphdr.cave_offset + OENTRY_OFF);
// you can then assign it a value like any other old pointer: 
*o_entry = exec_seg->original_entrypoint;
```