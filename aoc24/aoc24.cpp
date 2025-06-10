#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <limits>
#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<char> v;
	std::string ln;
	while(std::getline(std::cin, ln))
		v.emplace_back(sv_to_t<char>(ln));

	return v;
}

int popcnt(unsigned u)
{
	unsigned b = 1;
	int cnt = 0;
	for(auto i = 0; i < 32; ++i)
	{
		if(u & b)
			++cnt;
		b <<= 1;
	}
	return cnt;
}

uint64_t qent(auto const& seq, uint32_t u)
{
	uint32_t b = 1;
	return std::ranges::fold_left(seq, 1ULL, [&](auto s, auto r)
			{
				bool g = b & u;
				b <<= 1;
				if(g)
					return s * r;
				return s;
			});
}

std::vector<uint32_t> possibles(auto const& seq, int tgt)
{
	const unsigned lmt = 1 << seq.size();
	std::vector<uint32_t> v;
	for(auto u = 1; u < lmt; ++u)
	{
		unsigned b = 1;
		if(std::ranges::fold_left(seq, 0, [&](auto s, auto r)
			{
				bool g = b & u;
				b <<= 1;
				if(g)
					return s + r;
				return s;
			}) == tgt)
			v.emplace_back(u);
	}
	std::ranges::sort(v, [](auto l, auto r){return popcnt(l) < popcnt(r);});

	return v;
}

bool has_triple(auto const& v, uint32_t o)
{
	for(auto f = 0; f < v.size(); ++f)
	{
		if(!(v[f] & o))
		{
			auto o2 = o | v[f];
			for(auto f2 = f + 1; f2 < v.size(); ++f2)
				if(!(v[f2] & o2))
					return true;
		}
	}
	return false;
}

bool has_quad(auto const& v, uint32_t o)
{
	for(auto f = 0; f < v.size(); ++f)
		if(!(v[f] & o))
		{
			auto o2 = o | v[f];
			for(auto f2 = f + 1; f2 < v.size(); ++f2)
				if(!(v[f2] & o2))
				{
					auto o3 = o2 | v[f2];
					for(auto f3 = f2 + 1; f3 < v.size(); ++f3)
					{
						if(!(v[f3] & o3))
							return true;
					}
				}
		}
	return false;
}

auto pt1(auto const& in)
{
	timer t("p1");
	auto sm = std::ranges::fold_left(in, 0, std::plus<>());
	auto v = possibles(in, sm / 3);
	uint64_t r = std::numeric_limits<uint64_t>::max();
	auto pci = popcnt(v[0]);
	for(int n = 0; n < v.size(); ++n)
	{
		if(popcnt(v[n]) > pci)
			break;
		if(has_triple(v, v[n]))
		{
			auto qt = qent(in, v[n]);
			if(qt < r)
				r = qt;
		}
	}

	return r;
}

auto pt2(auto const& in)
{
	timer t("p2");
	auto sm = std::ranges::fold_left(in, 0, std::plus<>());
	auto v = possibles(in, sm / 4);
	uint64_t r = std::numeric_limits<uint64_t>::max();
	auto pci = popcnt(v[0]);
	for(int n = 0; n < v.size(); ++n)
	{
		if(popcnt(v[n]) > pci)
			break;
		if(has_quad(v, v[n]))
		{
			auto qt = qent(in, v[n]);
			if(qt < r)
				r = qt;
		}
	}
	return r;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
