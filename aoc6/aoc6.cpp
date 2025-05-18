#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class task_t { turn_on, turn_off, toggle };
struct job
{
	task_t task_;
	int tx_;
	int ty_;
	int bx_;
	int by_;
};

task_t get_task(std::string const& s)
{
	switch(s[6])
	{
		case 'n':
			return task_t::turn_on;
		case 'f':
			return task_t::turn_off;
		default:
			return task_t::toggle;
	}
}

auto get_input()
{
	std::vector<job> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, tx, ty, bx, by] = ctre::match<"[^\\d]*(\\d+),(\\d+) through (\\d+),(\\d+)">(ln); m)
			rv.emplace_back(get_task(ln), tx.to_number<int>(), ty.to_number<int>(), bx.to_number<int>(), by.to_number<int>());
	}
	return rv;
}

template<typename F> void act(job const& j, F&& f)
{
	int offset = j.ty_ * 1000 + j.tx_;
	int rows = j.by_ - j.ty_ + 1;
	int cols = j.bx_ - j.tx_ + 1;
	for(int r = 0; r < rows; ++r )
	{
		for( int p = offset; p < offset + cols; ++p)
			f(p);
		offset += 1000;
	}
}

int pt1(auto const& in)
{
	timer t("p1");
	std::bitset<1000*1000> bs;
	for(auto const& j: in)
		switch(j.task_)
		{
			case task_t::turn_on:
				act(j, [&](auto p){bs.set(p);});
				break;
			case task_t::turn_off:
				act(j, [&](auto p){bs.reset(p);});
				break;
			case task_t::toggle:
				act(j, [&](auto p){bs.flip(p);});
				break;
		}
	return bs.count();
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	std::array<int, 1000 * 1000> vs;
	vs.fill(0);
	for(auto const& j: in)
		switch(j.task_)
		{
			case task_t::turn_on:
				act(j, [&](auto p){++vs[p];});
				break;
			case task_t::turn_off:
				act(j, [&](auto p){if(vs[p] > 0) --vs[p];});
				break;
			case task_t::toggle:
				act(j, [&](auto p){vs[p] += 2;});
				break;
		}

	return std::ranges::fold_left(vs, 0LL, std::plus<>());
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
