#include "../include/io.h"
#include "../include/algorithm.h"
#include "../include/util.h"
int main() {
	int a = doComputation(5, 7.0);
	int b = 5;
	max(a, b);
	logDebug('!');
	return 0;
}
