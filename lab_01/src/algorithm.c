#include "../include/io.h"
#include "../include/util.h"
int doComputation(int a, float b) {
	int c = max(3, 4);
	// int c = maxxx(3, 4) вместо предыдущей строки даст undefined reference
	// int c = 0; - если добавить, получим redefinition
	logDebug('d');
	return c;
}
