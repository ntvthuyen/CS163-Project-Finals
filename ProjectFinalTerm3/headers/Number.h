#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include "Misc.h"

using namespace std;

enum NumberType {
	NORMAL,
	DOLLAR,
	INVALID,
};

/// <summary> 
/// Representing a numerical value (normal number, currency in dollar or invalid value)
/// </summary>
struct Number {
	/// <summary>
	/// The value of Number
	/// </summary>
	double x;

	/// <summary>
	/// The type of Number (normak, dollar or invalid)
	/// </summary>
	NumberType type;

	Number(double x = 0, NumberType type = NumberType::NORMAL) : x(x), type(type) {}
};

/// <summary> 
/// Convert from String to Number
/// </summary>
Number strToNumber(string s);

/// <summary> 
/// Convert from Number to String
/// </summary>
string numberToStr(Number x);

#endif // !NUMBER_H
