#include "dankos.h"

int main(void) {
	int x;
	int y=0;
	int sleep;

	os_print_string_i("Count up to: ");
	x = os_get_integer();
	os_putchar('\n');

	os_print_string_i("Sleep for (between numbers): ");
	sleep = os_get_integer();
	os_putchar('\n');

	while (y <= x) {
		os_set_cursor_pos(0, os_get_cursor_y());			// move the cursor back
		os_print_integer(y, 0);
		y++;
		os_sleep(sleep);
	}
	os_putchar('\n');

	return 0;
}
