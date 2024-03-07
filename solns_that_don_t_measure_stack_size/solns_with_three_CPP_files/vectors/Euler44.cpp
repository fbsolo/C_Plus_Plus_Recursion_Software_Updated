#include "stdafx.h"
#include "pentnum_functions.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <functional>
#include <math.h>
#include <vector>

int main() {

	pentnum_functions pfObj;

	//	Signature for the inner recursive lambda. The compiler
	//	builds a recursive lambda here when it sees the
	//	second line "&" capture clause
	//
	//		innerRecLambda = [&innerRecLambda]
	//
	//	that has the name of the lambda itself.
	//
	//	Pass the integer vector parameter by reference (&)
	//	because a vector does not operate like an array.
	//	A vector is not a "pure" pointer. If we pass a
	//	vector by value, each stack frame will get a
	//	separate copy of the vector. With a vector
	//	reference parameter, each stack frame operates
	//	on the same vector - exactly what we want.

	std::function<bool (std::vector<int>&)> innerRecLambda;
	innerRecLambda = [&innerRecLambda, &pfObj](std::vector<int>& x) {

		//	In separate variables, add and subtract pentagonal
		//	numbers, and see if those values are pentagonal
		//	numbers.

		bool inner_is_sum_pentnum = pfObj.is_pentnum(pfObj.gen_pentnum(x[1]) + pfObj.gen_pentnum(x[0]));
		bool inner_is_diff_pentnum = pfObj.is_pentnum(pfObj.gen_pentnum(x[1]) - pfObj.gen_pentnum(x[0]));

		if ((x[0] < (x[1] - 1)) && (!inner_is_sum_pentnum || !inner_is_diff_pentnum)) {

			//	The inner recursive lambda moved to an x[0] that, in "combination"
			//	with x[1], did not solve the problem. The x[0] value has room to
			//	move up towards x[1], so move x[0] up by 1, and recursively call
			//	innerRecLambda with the updated x[] vector.

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
	//	As explained above, pass the integer vector parameter
	//	by reference (&) in the lambda signature.

	std::function<std::vector<int>(std::vector<int>&)> outerRecLambda;
	outerRecLambda = [&outerRecLambda, &innerRecLambda, &pfObj](std::vector<int>& x) {

		//	In separate variables, add and subtract pentagonal
		//	numbers, and see if those values are pentagonal
		//	numbers.

		bool outer_is_sum_pentnum = pfObj.is_pentnum(pfObj.gen_pentnum(x[1]) + pfObj.gen_pentnum(x[0]));
		bool outer_is_diff_pentnum = pfObj.is_pentnum(pfObj.gen_pentnum(x[1]) - pfObj.gen_pentnum(x[0]));


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

				//	We found the solution x[] vector values

				return (x);
			}
		}
		else {

			//	We found the solution x[] vector values

			return (x);
		}
	};

	//	Declarations for integer vector variables / lambda
	//	call; initialize with 1 and 2 as the first possible
	//	x[0] and x[1] pair.

	std::vector<int> startValues = { 1, 2 };
	std::vector<int> finishedValues;

	//	Set up a timer.

	auto startTime = std::chrono::high_resolution_clock::now();

	finishedValues = outerRecLambda(startValues);

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> timeDifference = endTime - startTime;

	//	Show everything in the console box.

	std::cout << "\n\tThe calculated x vector values --> pentagonal numbers: " << "\n\n";
	std::cout << "\tx[1] = " << finishedValues[1] << " --> pfObj.gen_pentnum(x[1]) = pfObj.gen_pentnum(" << finishedValues[1] << ") = " << pfObj.gen_pentnum(finishedValues[1]) << "\n";
	std::cout << "\tx[0] = " << finishedValues[0] << " --> pfObj.gen_pentnum(x[0]) = pfObj.gen_pentnum(" << finishedValues[0] << ") = " << pfObj.gen_pentnum(finishedValues[0]) << "\n\n";
	std::cout << "\t\t\tpfObj.gen_pentnum(x[1]) + pfObj.gen_pentnum(x[0]) = " << pfObj.gen_pentnum(finishedValues[1]) + pfObj.gen_pentnum(finishedValues[0]) << "\n";
	std::cout << "\t\t\tpfObj.gen_pentnum(" << finishedValues[1] << ") + pfObj.gen_pentnum(" << finishedValues[0] << ") = " << pfObj.gen_pentnum(finishedValues[1]) + pfObj.gen_pentnum(finishedValues[0]) << "\n\n";

	//	For std::cout, format the boolean values as "true" or "false":

	std::cout << std::boolalpha;

	std::cout << "\t\t\t" << pfObj.gen_pentnum(finishedValues[1]) + pfObj.gen_pentnum(finishedValues[0]) << " is a pentagonal number: " << pfObj.is_pentnum(pfObj.gen_pentnum(finishedValues[1]) + pfObj.gen_pentnum(finishedValues[0])) << "\n\n";
	std::cout << "\t______________________________________________\n";
	std::cout << "\t______________________________________________\n\n\n";
	std::cout << "\tx[1] = " << finishedValues[1] << " --> pfObj.gen_pentnum(x[1]) = pfObj.gen_pentnum(" << finishedValues[1] << ") = " << pfObj.gen_pentnum(finishedValues[1]) << "\n";
	std::cout << "\tx[0] = " << finishedValues[0] << " --> pfObj.gen_pentnum(x[0]) = pfObj.gen_pentnum(" << finishedValues[0] << ") = " << pfObj.gen_pentnum(finishedValues[0]) << "\n\n";
	std::cout << "\t\t\tpfObj.gen_pentnum(x[1]) - pfObj.gen_pentnum(x[0]) = " << pfObj.gen_pentnum(finishedValues[1]) - pfObj.gen_pentnum(finishedValues[0]) << "\n";
	std::cout << "\t\t\tpfObj.gen_pentnum(" << finishedValues[1] << ") - pfObj.gen_pentnum(" << finishedValues[0] << ") = " << pfObj.gen_pentnum(finishedValues[1]) - pfObj.gen_pentnum(finishedValues[0]) << "\n\n";
	std::cout << "\t\t\t" << pfObj.gen_pentnum(finishedValues[1]) - pfObj.gen_pentnum(finishedValues[0]) << " is a pentagonal number: " << pfObj.is_pentnum(pfObj.gen_pentnum(finishedValues[1]) - pfObj.gen_pentnum(finishedValues[0])) << "\n\n";

	std::cout << "\n\t\t\tCalculation Time = " << timeDifference.count() << " seconds\n\n";
}