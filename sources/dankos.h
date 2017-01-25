#define os_putchar(c) asm volatile ("mov eax, %0; push 0x01; int 0x80" : : "r" (c) : "eax");
#define os_print_integer(x, y) asm volatile ("mov eax, %0; mov ecx, %1; xor edx, edx; push 0x06; int 0x80" : : "r" (x), "r" (y) : "eax", "ecx", "edx");

#define os_print_string_i(s) asm volatile ("jmp 2f; 1: .asciz \""s"\"; 2: lea esi, 1b; push 0x02; int 0x80" : : : "esi");

#define os_declare_string(s, x) asm volatile ("jmp 2f; 1: .asciz \""s"\"; 2: lea %0, 1b" : "=r" (x) : : "esi");

#define os_set_cursor_pos(x, y) ({			\
	int return_value=0;						\
	asm volatile ("mov eax, %0;"			\
				  "shl eax, 8;"				\
				  "or eax, %1;"				\
				  "push 0x0E;"				\
				  "int 0x80;"				\
				   :						\
				   : "r" (x), "r" (y)		\
				   : "eax");				\
	return_value;							\
})

#define os_get_cursor_y() ({				\
	int return_value;						\
	asm volatile ("push 0x0D;"				\
				  "int 0x80;"				\
				  "and eax, 0x000000FF;"	\
				  "mov %0, eax;"			\
				   : "=r" (return_value)	\
				   :						\
				   : "eax");				\
	return_value;							\
})

#define os_get_cursor_x() ({				\
	int return_value;						\
	asm volatile ("push 0x0D;"				\
				  "int 0x80;"				\
				  "and eax, 0x0000FF00;"	\
				  "shr eax, 8;"				\
				  "mov %0, eax;"			\
				   : "=r" (return_value)	\
				   :						\
				   : "eax");				\
	return_value;							\
})

#define os_get_integer() ({					\
	int return_value;						\
	asm volatile ("push 0x07;"				\
				  "int 0x80;"				\
				  "mov %0, eax;"			\
				   : "=r" (return_value)	\
				   :						\
				   : "eax");				\
	return_value;							\
})

#define os_sleep(x) ({						\
	int return_value=0;						\
	asm volatile ("mov ecx, %0;"			\
				  "push 0x1D;"				\
				  "int 0x80;"				\
				   :						\
				   : "r" (x)				\
				   : "ecx");				\
	return_value;							\
})

#define os_allocate_memory(x) ({			\
	int return_value;						\
	asm volatile ("mov eax, %1;"			\
				  "push 0xA0;"				\
				  "int 0x80;"				\
				  "mov %0, ecx;"			\
				   : "=r" (return_value)	\
				   : "r" (x)				\
				   : "eax", "ecx");			\
	return_value;							\
})

#define os_string_copy_i(s, x) ({			\
	int return_value=0;						\
	asm volatile ("jmp 2f;"					\
				  "1: .asciz \""s"\";"		\
				  "2: lea esi, 1b;"			\
				  "mov edi, %0;"			\
				  "push 0x27;"				\
				  "int 0x80;"				\
				   :						\
				   : "r" (x)				\
				   : "esi", "edi");			\
	return_value;							\
})

#define os_string_end(s) ({					\
	int return_value;						\
	asm volatile ("mov edi, %1;"			\
				  "push 0x2D;"				\
				  "int 0x80;"				\
				  "mov %0, edi;"			\
				   : "=r" (return_value)	\
				   : "r" (s)				\
				   : "edi");				\
	return_value;							\
})

#define os_get_current_dir(s) ({			\
	int return_value=0;						\
	asm volatile ("mov edi, %0;"			\
				  "push 0x2E;"				\
				  "int 0x80;"				\
				   :						\
				   : "r" (s)				\
				   : "edi");				\
	return_value;							\
})

#define os_initialise_screen() ({			\
	int return_value=0;						\
	asm volatile ("push 0x0A;"				\
				  "int 0x80;"				\
				   :						\
				   :						\
				   : );						\
	return_value;							\
})

#define os_start_program(x, y) ({			\
	int return_value;						\
	asm volatile ("xor esi, esi;"			\
				  "mov esi, %1;"			\
				  "xor edi, edi;"			\
				  "mov edi, %2;"			\
				  "push 0x14;"				\
				  "int 0x80;"				\
				  "mov %0, eax;"			\
				   : "=r" (return_value)	\
				   : "r" (x), "r" (y)		\
				   : "eax", "esi", "edi");	\
	return_value;							\
})

#define os_cut_string(s, c) ({				\
	int return_value;						\
	asm volatile ("mov esi, %1;"			\
				  "mov ebx, %2;"			\
				  "push 0x1A;"				\
				  "int 0x80;"				\
				  "mov %0, ebx;"			\
				   : "=r" (return_value)	\
				   : "r" (s), "r" (c)		\
				   : "ebx", "esi");			\
	return_value;							\
})

#define os_input_string(x, y) ({					\
	int return_value;								\
	asm volatile ("mov ebx, %1;"					\
				  "mov edi, %2;"					\
				  "push 0x10;"						\
				  "int 0x80;"						\
				  "cmp byte ptr es:[di], 0x00;"		\
				  "je 1f;"							\
				  "mov %0, 0;"						\
				  "jmp 2f;"							\
				  "1: mov %0, 1;"					\
				  "2: ;"							\
				   : "=r" (return_value)			\
				   : "r" (y), "r" (x)				\
				   : "ebx", "edi");					\
	return_value;									\
})

#define os_declare_buffer(x) ({				\
	int return_value;						\
	asm volatile ("jmp 2f;"					\
				  "1: .fill "x",1,0;"		\
				  "2: lea %0, 1b"			\
				   : "=r" (return_value)	\
				   :						\
				   : );						\
	return_value;							\
})

#define os_compare_strings_p(x, y) ({		\
	int return_value;						\
	asm volatile ("mov esi, %1;"			\
				  "mov edi, %2;"			\
				  "xor edx, edx;"			\
				  "push 0x08;"				\
				  "int 0x80;"				\
				  "mov %0, edx;"			\
				   : "=r" (return_value)	\
				   : "r" (x), "r" (y)		\
				   : "edx", "esi", "edi");	\
	return_value;							\
})

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
