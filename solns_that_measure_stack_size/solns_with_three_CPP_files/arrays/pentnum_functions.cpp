#include "stdafx.h"
#include "pentnum_functions.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <functional>
#include <math.h>
#include <vector>

pentnum_functions::pentnum_functions()
{
}

bool pentnum_functions::is_pentnum(int x) {

	//	Test if parameter "x" is a pentagonal number.

	//	http://www.divye.in/2012/07/how-do-you-determine-if-number-n-is.html

	float testval = (float)(sqrt(24.0f*(float)(x)+1.0f) + 1.0f) / 6.0f;
	return (((testval - trunc(testval)) == 0.0) ? true : false);
};

int pentnum_functions::gen_pentnum(int n) {

	//	Map parameter "n" to a pentagonal number.

	//	Pentagon number definition from
	//
	//		https://projecteuler.net/problem=44

	return (int)(((3 * pow((float)n, 2)) - (float)n) / 2);
}