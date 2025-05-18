#include <iostream>
#include <string>
#include <algorithm>
#include <ranges>

#include <fmt/format.h>

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

auto pt1(auto const& in)
{
	return std::ranges::fold_left(in, 0, [](auto l, auto r){ return l + (r == '(' ? 1 : -1);});
}

auto pt2(auto const& in)
{
	int n = 0;
	return 1 + std::distance(in.begin(), std::ranges::find_if(in, [&n](auto c){ n += (c == '(' ? 1 : -1); return n < 0;}));
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
