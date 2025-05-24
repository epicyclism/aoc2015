#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

std::string transform(std::string_view s)
{
	std::string sn;
	auto c = s[0];
	auto cnt = 0;
	for(auto ch : s)
	{
		if (ch == c)
			++cnt;
		else
		{
			sn.push_back(cnt + '0');
			sn.push_back(c);
			cnt = 1;
			c = ch;
		}
	}
	if(cnt)
	{
		sn.push_back(cnt + '0');
		sn.push_back(c);
	}
	return sn;
}

int64_t pt1(auto const& in)
{
	timer t("p1");
	std::string proc = in;
	for(int n = 0; n < 40; ++n)
		proc = transform(proc);
	return proc.size();
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	std::string proc = in;
	for(int n = 0; n < 50; ++n)
		proc = transform(proc);
	return proc.size();
}

int main(int ac, char** av)
{
	std::string in = "1113222113";
	if(ac > 1)
		in = av[1];
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
