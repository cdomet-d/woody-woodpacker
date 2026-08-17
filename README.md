# woody-woodpacker

This project is about coding a simple packer

## ELF files

![alt text](.readme-ressources/elf-file-diagram.png)

## ELF Headers and Program Headers

- [Program Headers](https://docs.oracle.com/cd/E19683-01/816-1386/6m7qcoblk/index.html#chapter6-69880)

## How to

- [X] Unpack binary into memory using nmap 
- [X] Parse Elf Headers to recover Program Headers and executable segment
- [X] Yoink `.text` and check integrity
- [ ] Write simple stub in ASM and get its size 
- [ ] Allocate (nmap result + stub_size) so we can start creating `woody`
- [ ] In that allocated buffer, insert the stub inplace of PT_LOAD with X
- [ ] Update and pad memory map + update entrypoint

## Reading 

- [The OG packer for malware](https://aeb.win.tue.nl/linux/hh/virus/unix-viruses.txt)
- [Virology blog regarding malware](https://cryptohub.nl/zines/vxheavens/lib/-index=UN&lang=en.htm)