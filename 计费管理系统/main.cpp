//main
#include "menu.h"

int main() {
	int opt = menu();
	while (opt != 0) {
		opt = menu();
	}
	return 0;
}