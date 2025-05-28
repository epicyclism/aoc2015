#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<int> v;
	std::string ln;
	while(std::getline(std::cin, ln))
		v.push_back(sv_to_t<int>(ln));
	std::ranges::sort(v, std::greater<>());
	return v;
}

template<int tgt> void compute_ways(std::vector<int> const& v, int n, int bkts, int total, std::vector<int>& ways)
{
//	fmt::println("{} {} {}", n, total, ways);
	if(total > tgt)
		return;
	if(total == tgt)
	{
		ways[bkts] = ways[bkts] + 1;
//		fmt::println("ways[{}] = {}", bkts, ways[bkts]);
		return;
	}
	for(; n < v.size(); ++n)
		compute_ways<tgt>(v, n + 1, bkts + 1, total + v[n], ways);
}

std::pair<int, int> pt12(auto const& in)
{
	timer t("p1");
	int bkts = 0;
	std::vector<int> cnts(in.size(), 0);
	if(in.size() == 5)
		compute_ways<25>(in, 0, 0, bkts, cnts);
	else
		compute_ways<150>(in, 0, 0, bkts, cnts);

	return {std::ranges::fold_left(cnts, 0, std::plus<>()),
		std::ranges::find_if(cnts, [](auto n){ return n > 0;}) - cnts.begin()};
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
