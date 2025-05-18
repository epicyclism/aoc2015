#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "timer.h"

auto get_input()
{
	std::vector<std::string> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
		rv.emplace_back(std::move(ln));
	return rv;
}

bool is_nice(std::string const& s)
{
	auto is_vowel = [](char c)
	{
		return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
	};
	int vc = 0;
	bool two = false;
	char p = '_';
	for(auto c : s)
	{
		vc += is_vowel(c);
		if( c == p)
			two = true;
		if( (c == 'b' && p =='a' ) ||
			 (c == 'd' && p == 'c') ||
			 (c == 'q' && p == 'p') ||
			 (c == 'y' &&  p == 'x'))
			return false;
		p = c; 
	}
	return vc > 2 && two;
}

int pt1(auto const& in)
{
	timer t("p1");
	return std::ranges::count_if(in, is_nice);
}

bool is_very_nice(std::string const& s)
{
	bool c1 = false;
	bool c2 = false;

	for(auto n = 0; n < s.size() - 3; ++n)
		for(auto m = n + 2; m < s.size() - 1; ++m)
			if(s[n] == s[m] && s[n + 1] == s[m + 1])
			{
				c1 = true;
				break;
			}

	for(auto n = 0; n < s.size() - 2; ++n)
		if(s[n] == s[n + 2])
		{
			c2 = true;
			break;
		}

	return c1 && c2;
}

int pt2(auto const& in)
{
	timer t("p2");
	return std::ranges::count_if(in, is_very_nice);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
