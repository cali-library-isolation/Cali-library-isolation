#include "library.h"

int library_function(int* numbers) {
	pid_printf("codes @ %p", numbers);
	return numbers[0] + numbers[1] + numbers[2];
}
