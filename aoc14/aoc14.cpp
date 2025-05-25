#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

struct reindeer
{
	std::string nm_;
	int speed_;
	int go_;
	int rest_;
};

auto get_input()
{
	std::vector<reindeer> vr;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		// Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
		if(auto[m, n, s, g, r] = ctre::match<"([a-zA-Z]+) can fly (\\d+) km/s for (\\d+) seconds, but then must rest for (\\d+) seconds.">(ln); m)
			vr.emplace_back(n.to_string(), s.to_number<int>(), g.to_number<int>(), r.to_number<int>());
		else
			fmt::println("failed to parse \"{}\"", ln);
	}
	fmt::println("got {} reindeer", vr.size());
	return vr;
}

int calc_distance(reindeer const& r, int time)
{
	int complete_cycles = time / (r.go_ + r.rest_);
	int remainder = time % (r.go_ + r.rest_);
	if( remainder >= r.go_)
	{
		++complete_cycles;
		remainder = 0;
	}
	return (complete_cycles * r.go_ + remainder) * r.speed_;
}

int pt1(auto const& in)
{
	timer t("p1");
	int mx = 0;
	for(auto& r : in)
	{
		auto rd = calc_distance(r, 2503);
		if( rd > mx)
			mx = rd;
	}
	return mx;
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	std::vector<int> scores(in.size(), 0);
	std::vector<int> distance(in.size(), 0);
	for(int time = 1; time <= 2503; ++time)
	{
//		std::ranges::transform(in, distance.begin(), [=](auto& r){ return calc_distance(r, time);});
		for(int rd = 0; rd < in.size(); ++rd)
			distance[rd] = calc_distance(in[rd], time);
		auto mx = *std::ranges::max_element(distance);
		for(int rd = 0; rd < in.size(); ++rd)
			if( distance[rd] == mx)
				++scores[rd];
	}
	return *std::ranges::max_element(scores);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
