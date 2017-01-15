#define os_putchar(c) asm volatile ("mov eax, %0; push 0x01; int 0x80" : : "r" (c) : "eax");
#define os_print_integer(x, y) asm volatile ("mov eax, %0; mov ecx, %1; xor edx, edx; push 0x06; int 0x80" : : "r" (x), "r" (y) : "eax", "ecx", "edx");
#define os_get_integer(x) asm volatile ("push 0x07; int 0x80; mov %0, eax" : "=r" (x) : : "eax");
#define os_print_string(s) asm volatile ("jmp 2f; 1: .asciz \""s"\"; 2: lea esi, 1b; push 0x02; int 0x80" : : : "esi");

asm(".code16");
asm(".org 0x100");
asm("call .+7");
asm("push 0x00; int 0x80");
