#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"

struct box
{
	int l_;
	int w_;
	int h_;
};

auto get_input()
{
	std::vector<box> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, l, w, h] = ctre::match<"(\\d+)x(\\d+)x(\\d+)">(ln); m)
			rv.emplace_back(l.to_number<int>(), w.to_number<int>(), h.to_number<int>());
	}
	return rv;
}

int area(box const& b)
{
	std::array an = {
		b.l_ * b.h_,
		b.l_ * b.w_,
		b.h_ * b.w_};
	return 2 * std::ranges::fold_left(an, 0, std::plus<>()) +
				*std::ranges::min_element(an);
}

auto pt1(auto const& in)
{
	return std::ranges::fold_left(in, 0, [](auto s, auto& b){ return s + area(b);});
}

int ribbon(box const& b)
{
	std::array pn = {
		2 * b.l_ + 2 * b.h_,
		2 * b.l_ + 2 * b.w_,
		2 * b.h_ + 2 * b.w_};
		return *std::ranges::min_element(pn) + b.l_ * b.w_ * b.h_;
}

auto pt2(auto const& in)
{
	return std::ranges::fold_left(in, 0, [](auto s, auto& b){ return s + ribbon(b);});
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
