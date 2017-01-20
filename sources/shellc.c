#include "dankos.h"

int main(void) {
	int prompt_input;
	int attributes;

	os_putchar(0x0A);
	os_print_string_i("DankOS shell (C edition!)\n");
	os_print_string_i("        Welcome!\n");
	os_putchar(0x0A);

	prompt_input = os_declare_buffer("256");

	while (1) {
		while (1) {
			os_print_string_i("# ");
			if (os_input_string(prompt_input, 255) == 0) {
				os_putchar(0x0A);
				break;
			}
			os_putchar(0x0A);
		}

	attributes = os_cut_string(prompt_input);

	if (os_compare_strings_i("exit", prompt_input)) break;
	if (os_compare_strings_i("clear", prompt_input)) os_initialise_screen();

	if (os_start_program(prompt_input, attributes) == 0xFFFFFFFF) {
		os_print_string_i("Command or file not found: '");
		os_print_string_p(prompt_input);
		os_print_string_i("'.\n");
		}
	}

	return 0;
}
