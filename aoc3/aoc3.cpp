#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <set>

#include <fmt/format.h>

#include "timer.h"

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

void update_xy(auto c, int& x, int& y)
{
	switch(c)
	{
		case '>':
			++x;
			break;
		case '<':
			--x;
			break;
		case '^':
			++y;
			break;
		case 'v':
			--y;
			break;
		default:
			break;
	}
}
auto pt1(auto const& in)
{
	timer t("p1");
	std::set<std::pair<int, int>> houses;
	int x = 0;
	int y = 0;
	houses.emplace(x, y);
	for(auto c: in)
	{
		update_xy(c, x, y);
		houses.emplace(x, y);
	}
	return houses.size();
}

auto pt2(auto const& in)
{
	timer t("p2");
	std::set<std::pair<int, int>> houses;
	int x = 0;
	int y = 0;
	houses.emplace(x, y);
	for(size_t n = 0; n < in.size(); n += 2)
	{
		update_xy(in[n], x, y);
		houses.emplace(x, y);
	}
	x = 0;
	y = 0;
	houses.emplace(x, y);
	for(size_t n = 1; n < in.size(); n += 2)
	{
		update_xy(in[n], x, y);
		houses.emplace(x, y);
	}
	return houses.size();
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
