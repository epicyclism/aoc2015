#include <iostream>
#include <string>
#include <bitset>
#include <array>

#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

template<int D> struct grid_t
{
	std::bitset<(D+2)*(D+2)> bs_;
	void set(int x, int y, bool b)
	{
		bs_.set(x + y * (D+2), b);
	}
	bool get(int x, int y) const
	{
		return bs_.test(x + y * (D+2));
	}
};

grid_t<100> get_input()
{
	grid_t<100> bs;
	std::string ln;
	int y = 1;
	while(std::getline(std::cin, ln))
	{
		int x = 1;
		for(auto c: ln)
		{
			bs.set(x, y, c == '#');
			++x;
		}
		++y;
	}
	fmt::println("got {} bits", bs.bs_.count());
	return bs;
}

constexpr std::pair<int, int> offsets [] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

int cnt_neighbours(grid_t<100> const& g, int x, int y)
{
	int c = 0;
	for(auto& xy: offsets)
		c += g.get(x + xy.first, y + xy.second);
	return c;
}

grid_t<100> step(grid_t<100> const& g)
{
	grid_t<100> nxt;
	for(int y = 1; y < 101; ++y)
		for(int x = 1; x < 101; ++x)
		{
			auto neighbours = cnt_neighbours(g, x, y);
			if(g.get(x, y))
				nxt.set(x, y, neighbours == 2 || neighbours == 3);
			else
				nxt.set(x, y, neighbours == 3);
		}
	return nxt;
}

int pt1(auto const& in)
{
	timer t("p1");
	grid_t<100> wking(in);
	for(int n = 0; n < 100; ++n)
	{
		wking = step(wking);
	}
	return wking.bs_.count();
}

constexpr std::pair<int, int> corners [] = { {1, 1}, {100, 1}, {1, 100}, {100, 100}};

int pt2(auto const& in)
{
	timer t("p2");
	grid_t<100> wking(in);
	for(int n = 0; n < 100; ++n)
	{
		wking = step(wking);
		for(auto& xy: corners)
			wking.set(xy.first, xy.second, true);
		
	}
	return wking.bs_.count();
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
