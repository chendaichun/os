__asm__(".code16gcc");

#define LOADER_START_ADD 0x8000
void entry_loader() { ((void (*)(void))LOADER_START_ADD)(); }