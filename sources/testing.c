#include "dankos.h"

int main(void) {
	int x;
	os_declare_buffer(x, "16");
	os_print_string_i("Hello, what's your name? ");
	os_input_string(x, 15);
	if (os_compare_strings_i("druaga", x)) {
		os_print_string_i("\nboi");
	}
	os_print_string_i("\nHi ");
	os_print_string_p(x);
	os_print_string_i("\n");
	return 0;
}
