#include "dankos.h"

int main(void) {
	int prompt_input;

	os_putchar(0x0A);
	os_print_string_i("DankOS shell (C edition!)\n");
	os_print_string_i("        Welcome!\n");
	os_putchar(0x0A);

	prompt_input = os_declare_buffer("256");

	while (1) {
		os_print_string_i("# ");
		if (os_input_string(prompt_input, 256) == 0) {
			os_putchar(0x0A);
			break;
		}
		os_putchar(0x0A);
	}

	return 0;
}
