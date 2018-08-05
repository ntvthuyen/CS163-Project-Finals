#include "headers/Number.h"

bool strToDouble(string s, double &res) {
	try {
		res = stod(s);
	}
	catch (const std::invalid_argument& ia) {
		return false;
	}
	catch (const std::out_of_range& ia) {
		return false;
	}
}

double strToDoubleWithSuf(string s, double &res) {
	s = toLower(s);
	if (s.length() >= 3 && s.substr(s.length() - 2) == "bn" && strToDouble(s.substr(0, s.length() - 2), res)) {
		res *= 1e9;
		return true;
	}
	if (s.length() >= 2 && s.substr(s.length() - 1) == "m" && strToDouble(s.substr(0, s.length() - 1), res)) {
		res *= 1e6;
		return true;
	}
	return strToDouble(s, res);
}

Number strToNumber(string s) {
	if (s.empty())
		return Number(0, NumberType::INVALID);

	double value;
	if (s[0] == '$') {
		if (strToDoubleWithSuf(s.substr(1), value))
			return Number(value, NumberType::DOLLAR);
		else
			return Number(0, NumberType::INVALID);
	}

	if (strToDoubleWithSuf(s, value))
		return Number(value, NumberType::NORMAL);
	else
		return Number(0, NumberType::INVALID);
}

string numberToStr(Number x) {
	if (x.type == NumberType::INVALID)
		return "N/A";

	string res = to_string(x.x);
	if (x.type == NumberType::DOLLAR)
		res = "$" + res;
	return res;
}