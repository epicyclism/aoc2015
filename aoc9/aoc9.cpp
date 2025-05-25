#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <limits>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "graph.h"
#include "timer.h"

size_t nm_to_id(std::string const& nm)
{
	static std::map<std::string, int> mp;
	if(auto it = mp.find(nm); it == mp.end())
	{
		auto rv = mp.size();
		mp.emplace(nm, mp.size());
		return rv;
	}
	else
		return (*it).second;
}

unsigned add_flg(unsigned u, size_t id)
{
	return u | (1 << id);
}

bool seen(unsigned u, size_t id)
{
	return u & (1 << id);
}

using graph_t = std::vector<std::vector<std::pair<size_t, int>>>;

auto get_input()
{
	graph_t g;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, f, t, d] = ctre::match<"([a-zA-Z]+) to ([a-zA-Z]+) = (\\d+)">(ln); m)
			add_edge_undirected(g, nm_to_id(f.to_string()), nm_to_id(t.to_string()), d.to_number<int>());
	}
	return g;
}

int hamilton(graph_t const& g, size_t from, unsigned visited, int d)
{
	int d0 = std::numeric_limits<int>::max();
	auto& al = g[from];
	for(size_t n = 0; n < al.size(); ++n)
	{
		auto& v = al[n];
		if(!seen(visited, v.first))
		{
			auto d2 = hamilton(g, v.first, add_flg(visited, v.first), d + v.second);
			if(d2 < d0)
				d0 = d2;
		}
	}
	return d0 == std::numeric_limits<int>::max() ? d : d0;
}

int64_t pt1(auto const& in)
{
	timer t("p1");
	int d = std::numeric_limits<int>::max();
	for(size_t n = 0; n < in.size(); ++n)
	{
		auto d2 = hamilton(in, n, add_flg(0, n), 0);
		if(d2 < d)
			d = d2;
	}
	return d;
}

int anti_hamilton(graph_t const& g, size_t from, unsigned visited, int d)
{
	int d0 = 0;
	auto& al = g[from];
	for(size_t n = 0; n < al.size(); ++n)
	{
		auto& v = al[n];
		if(!seen(visited, v.first))
		{
			auto d2 = anti_hamilton(g, v.first, add_flg(visited, v.first), d + v.second);
			if(d2 > d0)
				d0 = d2;
		}
	}
	return d0 == 0 ? d : d0;
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	int d = 0;
	for(size_t n = 0; n < in.size(); ++n)
	{
		auto d2 = anti_hamilton(in, n, add_flg(0, n), 0);
		if(d2 > d)
			d = d2;
	}
	return d;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
