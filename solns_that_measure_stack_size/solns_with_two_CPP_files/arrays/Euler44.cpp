#include "stdafx.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <functional>
#include <math.h>

//	Set Stack Reserve Size to 6000000 (6 megs)

bool is_pentnum(int x) {

	//	Test if parameter "x" is a pentagonal number.

	//	http://www.divye.in/2012/07/how-do-you-determine-if-number-n-is.html

	float testval = (float)(sqrt(24.0f*(float)(x)+1.0f) + 1.0f) / 6.0f;
	return (((testval - trunc(testval)) == 0.0) ? true : false);
};

int gen_pentnum(int n) {

	//	Map parameter "n" to a pentagonal number.

	//	Pentagon number definition from
	//
	//		https://projecteuler.net/problem=44

	return (int)(((3 * pow((float)n, 2)) - (float)n) / 2);
};

int main() {

	void *arrayPtr;
	void *stackStartPtr;

	unsigned int frameMaxAddress;
	unsigned int frameMinAddress;

	//	Signature for the inner recursive lambda. The compiler
	//	builds a recursive lambda here when it sees the
	//	second line "&" capture clause
	//
	//		innerRecLambda = [&innerRecLambda]
	//
	//	that has the name of the lambda itself.

	std::function<bool (int[])> innerRecLambda;
	innerRecLambda = [&innerRecLambda, &arrayPtr, &frameMinAddress](int x[]) {

		//	In separate variables, add and subtract pentagonal
		//	numbers, and see if those values are pentagonal
		//	numbers.

		bool inner_is_sum_pentnum = is_pentnum(gen_pentnum(x[1]) + gen_pentnum(x[0]));
		bool inner_is_diff_pentnum = is_pentnum(gen_pentnum(x[1]) - gen_pentnum(x[0]));

		if ((x[0] < (x[1] - 1)) && (!inner_is_sum_pentnum || !inner_is_diff_pentnum)) {

			//	The inner recursive lambda moved to an x[0] that, in "combination"
			//	with x[1], did not solve the problem. The x[0] value has room to
			//	move up towards x[1], so move x[0] up by 1, and recursively call
			//	innerRecLambda with the updated x[] array.

			x[0]++;

			return innerRecLambda(x);
		}
		else if ((x[0] == (x[1] - 1)) && (!inner_is_sum_pentnum || !inner_is_diff_pentnum)) {

			//	The inner recursive lambda moved x[0] to (x[1] - 1). This is the highest
			//	possible x[0] value below x[1]. Then, this lambda looked at the pentagonal
			//	number values "owned" by x[0] and x[1]. The x[0] and x[1] pentagonal number
			//	pair did not solve the problem. The innerRecLambda lambda returns false to
			//	outerRecLambda. The outerRecLambda lambda will then reset x[0] and x[1],
			//	and start looking for the x[0] and x[1] solution pair.

			return false;
		}
		else {

			//	arrayPtr is a void pointer, with the value of the x[] array ADDRESS.
			//	Every frame will have a unique arrayPtr value, but each arrayPtr has
			//	the exact same value - the x[] array ADDRESS value. The innerRecLambda
			//	frame that found the x[] array solution values also has the largest
			//	stack frame address from the stack origin. This happens because the
			//	stack has all the outerRecLambda frames, combined with the innerRecLambda
			//	frames.
			//
			//	In this block, start with the arrayPtr value. This has the x[] array
			//	address. arrayPtr has its own address. Convert this address to a
			//	void* pointer. Then, convert it to an integer pointer. Deref that
			//	integer pointer. This returns the arrayPtr address as an integer.
			//	We can use this integer later on, to calculate the stack usage.

			arrayPtr = (void*)(&x);
			frameMinAddress = *(unsigned int *)(&arrayPtr);

			//	The inner recursive lambda found the x[0] value that solves the
			//	problem - combined with the x[1] value and the x[1] pentagonal
			//	number. The innerRecLambda lambda returns true to outerRecLambda.

			return true;
		}
	};

	//	Signature for the outer recursive lambda. The compiler
	//	builds a recursive lambda here when it sees the second
	//	line "&" capture clause
	//
	//		outerRecLambda = [&outerRecLambda]
	//
	//	that has the name of the lambda itself.
	//
	//	As explained above, pass the integer array parameter
	//	by reference (&) in the lambda signature.

	std::function<int* (int[])> outerRecLambda;
	outerRecLambda = [&outerRecLambda, &innerRecLambda](int x[]) {

		//	In separate variables, add and subtract pentagonal
		//	numbers, and see if those values are pentagonal
		//	numbers.

		bool outer_is_sum_pentnum = is_pentnum(gen_pentnum(x[1]) + gen_pentnum(x[0]));
		bool outer_is_diff_pentnum = is_pentnum(gen_pentnum(x[1]) - gen_pentnum(x[0]));

		if (!outer_is_sum_pentnum || !outer_is_diff_pentnum) {

			//	The outerRecLambda lambda started its operations, and never
			//	found x[1] and x[0] pair values that solve the problem.

			//	Call innerRecLambda on the x parameter. It has the
			//	x[0] and x[1] pair. This nested recursive lambda will
			//	look at every integer pair between x[1] (as a fixed
			//	value) and x[0]. The x[0] value ranges from
			//	1 to (x[1] - 1):
			//
			//		x[1]		(fixed)
			//		x[0]		(ranges from 1 to (x[1] - 1) )
			//
			//	If innerRecLambda returns false, it did not find
			//	x values that solve the problem.

			if (!(innerRecLambda(x))) {

				//	The innerRecLambda lambda did not find x values
				//	that solve the problem. Reset x[0] to 1, and
				//	push x[1] up by 1. This gets everything ready
				//	to look at a new set of number pairs.

				x[1]++;
				x[0] = 1;

				return outerRecLambda(x);
			}
			else {

				//	We found the solution x[] array values

				return (x);
			}
		}
		else {

			//	We found the solution x[] array values

			return (x);
		}
	};

	//	Declare integer / array variables; initialize with 1
	//	and 2 as the first possible x[0] and x[1] pair.

	int startValues[2] = { 1, 2 };
	int* finishedValues;

	//	Set up a timer.

	auto startTime = std::chrono::high_resolution_clock::now();

	stackStartPtr = (void*)(startValues);

	//	Earlier comment re: frameMinAddress extends
	//	to frameMaxAddress.

	frameMaxAddress = *(unsigned int *)(&stackStartPtr);
	finishedValues = outerRecLambda(startValues);

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timeDifference = endTime - startTime;

	//	Show everything in the console box.

	std::cout << "\n\tThe calculated x array values --> pentagonal numbers: " << "\n\n";
	std::cout << "\tx[1] = " << finishedValues[1] << " --> gen_pentnum(x[1]) = gen_pentnum(" << finishedValues[1] << ") = " << gen_pentnum(finishedValues[1]) << "\n";
	std::cout << "\tx[0] = " << finishedValues[0] << " --> gen_pentnum(x[0]) = gen_pentnum(" << finishedValues[0] << ") = " << gen_pentnum(finishedValues[0]) << "\n\n";
	std::cout << "\t\t\tgen_pentnum(x[1]) + gen_pentnum(x[0]) = " << gen_pentnum(finishedValues[1]) + gen_pentnum(finishedValues[0]) << "\n";
	std::cout << "\t\t\tgen_pentnum(" << finishedValues[1] << ") + gen_pentnum(" << finishedValues[0] << ") = " << gen_pentnum(finishedValues[1]) + gen_pentnum(finishedValues[0]) << "\n\n";

	//	For std::cout, format the boolean values as "true" or "false":

	std::cout << std::boolalpha;

	std::cout << "\t\t\t" << gen_pentnum(finishedValues[1]) + gen_pentnum(finishedValues[0]) << " is a pentagonal number: " << is_pentnum(gen_pentnum(finishedValues[1]) + gen_pentnum(finishedValues[0])) << "\n\n";
	std::cout << "\t______________________________________________\n";
	std::cout << "\t______________________________________________\n\n\n";
	std::cout << "\tx[1] = " << finishedValues[1] << " --> gen_pentnum(x[1]) = gen_pentnum(" << finishedValues[1] << ") = " << gen_pentnum(finishedValues[1]) << "\n";
	std::cout << "\tx[0] = " << finishedValues[0] << " --> gen_pentnum(x[0]) = gen_pentnum(" << finishedValues[0] << ") = " << gen_pentnum(finishedValues[0]) << "\n\n";
	std::cout << "\t\t\tgen_pentnum(x[1]) - gen_pentnum(x[0]) = " << gen_pentnum(finishedValues[1]) - gen_pentnum(finishedValues[0]) << "\n";
	std::cout << "\t\t\tgen_pentnum(" << finishedValues[1] << ") - gen_pentnum(" << finishedValues[0] << ") = " << gen_pentnum(finishedValues[1]) - gen_pentnum(finishedValues[0]) << "\n\n";
	std::cout << "\t\t\t" << gen_pentnum(finishedValues[1]) - gen_pentnum(finishedValues[0]) << " is a pentagonal number: " << is_pentnum(gen_pentnum(finishedValues[1]) - gen_pentnum(finishedValues[0])) << "\n\n";

	std::cout << "\t\t\tCalculation Time = " << timeDifference.count() << " seconds\n";
	std::cout << "\n\t\t\tEstimated use of stack resources: " << (frameMaxAddress - frameMinAddress) << " bytes\n\n";
}