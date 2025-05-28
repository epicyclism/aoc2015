#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

struct ingredient
{
	std::string nm_;
	std::array<int, 4> properties_;
/*	int64_t capacity_;
	int64_t durability_;
	int64_t flavor_;
	int64_t texture_;
	*/
	int64_t calories_;
	ingredient(std::string_view nm, int cap, int dur, int fla, int txt, int cal) : nm_(nm), calories_(cal)
	{
		properties_[0] = cap;
		properties_[1] = dur;
		properties_[2] = fla;
		properties_[3] = txt;
	}
};

auto get_input()
{
	std::vector<ingredient> vr;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, n, c, d, f, t, cl] = ctre::match<"([a-zA-Z]+): capacity (\\-?\\d+), durability (\\-?\\d+), flavor (\\-?\\d+), texture (\\-?\\d+), calories (\\-?\\d+)">(ln); m)
			vr.emplace_back(n.to_string(), c.to_number<int>(), d.to_number<int>(), f.to_number<int>(), t.to_number<int>(), cl.to_number<int>());
		else
			fmt::println("failed to parse \"{}\"", ln);
	}

	return vr;
}

std::pair<int, int> compute(std::vector<ingredient> const& v, std::vector<int> const& cnts)
{
	std::array<int, 4> tmp;
	tmp.fill(0);
	int c = 0;
	for(auto n = 0; n < v.size(); ++n)
	{
		for(int m = 0; m < 4; ++m)
			tmp[m] += cnts[n] * v[n].properties_[m];	
		c += cnts[n] * v[n].calories_;
	}
	for(auto& t: tmp)
		if(t < 0)
			t = 0;
	return {std::ranges::fold_left(tmp, 1, std::multiplies<>()), c};
}

std::pair<int, int> pt12(auto const& in)
{
	timer t("p1");
	int p1 = 0;
	int p2 = 0;
	if(in.size() == 2)
	{
		std::vector<int> cnts(2);
		for( cnts[0] = 0; cnts[0] < 101; ++cnts[0])
		{
			cnts[1] = 100 - cnts[0];
			auto[s, c] = compute(in, cnts);
			if(s > p1)
				p1 = s;
			if(c == 500 && s > p2)
				p2 = s;
		}
	}
	else
	{
		std::vector<int> cnts(4);
		for( cnts[0] = 0; cnts[0] < 101; ++cnts[0])
			for( cnts[1] = 0; cnts[1] < 101 - cnts[0]; ++cnts[1])
				for(cnts[2] = 0; cnts[2] < 101 - cnts[0] - cnts[1]; ++cnts[2])
		{
			cnts[3] = 100 - cnts[0] - cnts[1] - cnts[2];
			auto[s, c] = compute(in, cnts);
			if(s > p1)
				p1 = s;
			if(c == 500 && s > p2)
				p2 = s;
		}
	}
	return {p1, p2};
}

int main()
{
	auto in = get_input();
	auto[p1, p2] = pt12(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
