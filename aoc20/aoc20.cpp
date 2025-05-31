#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

int pt1(auto const& in)
{
	timer t("p1");
	std::vector<int> houses(in / 20, 0);
	for(int e = 1; e < houses.size(); ++e)
	{
		for( int h = e; h < houses.size(); h += e)
			houses[h] += e * 10;
	}
	for(int h = 0; h < houses.size(); ++h)
		if( houses[h] >= in)
			return h;
	return 0;
}

int pt2(auto const& in)
{
	timer t("p2");
	std::vector<int> houses(in / 20, 0);
	for(int e = 1; e < houses.size(); ++e)
	{
		int cnt = 0; int h = e;
		for( ; cnt < 50 && h < houses.size(); ++cnt, h += e)
			houses[h] += e * 11;
	}
	for(int h = 0; h < houses.size(); ++h)
		if( houses[h] >= in)
			return h;
	return 0;
}

int main(int ac, char** av)
{
	auto in = 33100000;
	if(ac > 1)
		in = sv_to_t<int>(av[1]);
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
