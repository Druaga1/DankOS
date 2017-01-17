#define os_putchar(c) asm volatile ("mov eax, %0; push 0x01; int 0x80" : : "r" (c) : "eax");
#define os_print_integer(x, y) asm volatile ("mov eax, %0; mov ecx, %1; xor edx, edx; push 0x06; int 0x80" : : "r" (x), "r" (y) : "eax", "ecx", "edx");
#define os_get_integer(x) asm volatile ("push 0x07; int 0x80; mov %0, eax" : "=r" (x) : : "eax");
#define os_print_string_i(s) asm volatile ("jmp 2f; 1: .asciz \""s"\"; 2: lea esi, 1b; push 0x02; int 0x80" : : : "esi");
/* #define os_print_string_p(x) asm volatile ("mov esi, %0; push 0x02; int 0x80" : : "r" (x) : "esi"); */

#define os_declare_string(s, x) asm volatile ("jmp 2f; 1: .asciz \""s"\"; 2: lea %0, 1b" : "=r" (x) : : "esi");
#define os_declare_buffer(x, y) asm volatile ("jmp 2f; 1: .fill "y",1,0; 2: lea %0, 1b" : "=r" (x) : : "esi");
#define os_input_string(x, y) asm volatile ("mov ebx, %0; mov edi, %1; push 0x10; int 0x80" : : "r" (y), "r" (x) : "ebx", "edi");


#define os_compare_strings_i(s, x) ({		\
	int return_value;						\
	asm volatile ("jmp 2f;"					\
				  "1: .asciz \""s"\";"		\
				  "2: lea esi, 1b;"			\
				  "mov edi, %1;"			\
				  "xor edx, edx;"			\
				  "push 0x08;"				\
				  "int 0x80;"				\
				  "mov %0, edx;"			\
				   : "=r" (return_value)	\
				   : "r" (x)				\
				   : "edx", "esi", "edi");	\
	return_value;							\
})


#define os_print_string_p(x) ({	\
	int return_value=0;			\
	asm ("mov esi, %0;"			\
		 "push 0x02;"			\
		 "int 0x80;"			\
		  :						\
		  : "r" (x)				\
		  : "esi");				\
	return_value;				\
})

asm(".code16");
asm(".org 0x100");
asm("call program_begin");
asm("push 0x00; int 0x80");

asm("program_begin: ");
