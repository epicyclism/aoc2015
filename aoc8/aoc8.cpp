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

char from_hex(char c)
{
	char r = 0;
	if(::isdigit(c))
		return c - '0';
	return c - 'a';
}

size_t quote_oh(std::string_view s)
{
	size_t l = 0;
	int estate = 0;
	for(auto c : s)
	{
		switch(estate)
		{
			case 0:
				if(c == '\\')
					estate = 1;
				else
				if(c != '"')
					++l;
				break;
			case 1:
				if(c == '\\' || c == '"')
				{
					++l;
					estate = 0;
				}
				else
				if(c == 'x')
					++estate;
				else
					estate = 0; // bad input?
				break;
			case 2:
				++estate;
				break;
			case 3:
				++l;
				estate = 0;
				break;
			default:
				// bug
				estate = 0;
				break;
		}
	}
	return s.size() - l;
}

int64_t pt1(auto const& in)
{
	timer t("p1");
	auto ql = std::ranges::fold_left(in, 0ULL, [](auto sm, auto const& s){ return sm + quote_oh(s);});

	return ql;
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	return 0;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
