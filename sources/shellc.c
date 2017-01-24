#include "dankos.h"

int main(void) {
	int prompt_input;
	int attributes;
	int current_dir;
	int file_buffer;

	/* Declare buffers */

	prompt_input = os_declare_buffer("260");
	current_dir = os_declare_buffer("128");

	/* Allocate file buffer */

	file_buffer = os_allocate_memory(0x2000);

	/* Print intro */

	os_putchar(0x0A);
	os_print_string_i("DankOS shell (C edition!)\n");
	os_print_string_i("Welcome! (use 'exit' to quit)\n");
	os_putchar(0x0A);

	/* Main loop */

	while (1) {

		/* Draw prompt loop */

		while (1) {
			os_get_current_dir(current_dir);
			os_print_string_p(current_dir);
			os_print_string_i("# ");
			if (os_input_string(prompt_input, 255) == 0) {
				os_putchar(0x0A);
				break;
			}
			os_putchar(0x0A);
		}

		/* Command calling */

		attributes = os_cut_string(prompt_input, ' ');

		if (os_compare_strings_i("exit", prompt_input)) break;
		else if (os_compare_strings_i("clear", prompt_input)) os_initialise_screen();

		else if (os_start_program(prompt_input, attributes) == 0xFFFFFFFF) {
			os_string_copy_i(".bin", os_string_end(prompt_input));
			if (os_start_program(prompt_input, attributes) == 0xFFFFFFFF) {
				os_print_string_i("Command or file not found.\n");
			}
		}
	}

	return 0;
}
