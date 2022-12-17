#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <queue>
#include <vector>

std::vector<std::string> split(const std::string& str, const std::string& delim = " ") {
	std::vector<std::string> result;
	size_t pos = 0;
	while (pos < str.length()) {
		size_t tokenLen = str.find(delim, pos) != std::string::npos ? str.find(delim, pos) - pos : str.length() - pos;
		result.emplace_back(str.substr(pos, tokenLen));
		pos += tokenLen + delim.length();
	}
	return result;
}

std::vector<std::string> getLines(std::istream& is) {
	std::vector<std::string> result;
	std::string tmp;
	while (std::getline(is, tmp)) {
		result.emplace_back(std::move(tmp));
	}
	return result;
}

template<typename T>
std::vector<T> getData(std::istream& is) {
	std::vector<T> result;
	T tmp;
	while (is >> tmp) {
		result.emplace_back(std::move(tmp));
	}
	return result;
}

template<typename T>
std::vector<T> getDataStr(const std::string& s, const std::string& delim = " ") {
	const auto tokens = split(s, delim);
	std::vector<T> result(tokens.size());
	for (unsigned i = 0; i < tokens.size(); i++) {
		tokens[i] >> result[i];
	}
	return result;
}

template<typename T>
void printPartResult(int part, const T& x) {
	std::cout << "Part " << part << ": " << x << '\n';
}

template<typename T>
void part_1(const T& x) {
	printPartResult(1, x);
}

template<typename T>
void part_2(const T& x) {
	printPartResult(2, x);
}

template<typename T>
bool operator>>(const std::string& s, T& x) {
	std::istringstream iss{s};
	if (iss >> x) {
		iss.peek();
		return iss.eof();
	}
	return false;
}

//no checks! call only if sure that conversion can take place
template<typename T>
T convertStr(const std::string& s) {
	T x;
	s >> x;
	return x;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
	os << '[';
	bool first = true;
	for (const T& x : vec) {
		if (first)
			first = false;
		else
			os << ", ";
		os << x;
	}
	os << ']';
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& vec) {
	os << '{';
	bool first = true;
	for (const T& x : vec) {
		if (first)
			first = false;
		else
			os << ", ";
		os << x;
	}
	os << '}';
	return os;
}

template <typename T>
std::set<T> intersect(const std::set<T>& a, const std::set<T>& b) {
	std::set<T> result;
	std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::inserter(result, result.begin()));
	return result;
}
