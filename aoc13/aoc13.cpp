#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <limits>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "graph.h"

#include "timer.h"

int nm_to_id(std::string const& nm)
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

using graph_t = std::vector<std::vector<std::pair<int, int>>>;

auto get_input()
{
	graph_t g;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, f, u, t] = ctre::match<"([a-zA-Z]+) would lose (\\d+) happiness units by sitting next to ([a-zA-Z]+).">(ln); m)
			add_edge(g, nm_to_id(f.to_string()), nm_to_id(t.to_string()), -u.to_number<int>());
		else
		if(auto[m, f, u, t] = ctre::match<"([a-zA-Z]+) would gain (\\d+) happiness units by sitting next to ([a-zA-Z]+).">(ln); m)
			add_edge(g, nm_to_id(f.to_string()), nm_to_id(t.to_string()), u.to_number<int>());
		else
			fmt::println("parse fail on input \"{}\"", ln);
	}
	for(auto v = 0; v < g.size(); ++v)
	{
		add_edge(g, v, v, 0);
		std::ranges::sort(g[v], [](auto& l, auto& r){ return l.first < r.first;});
	}
	return g;
}

int score(auto const& g, std::vector<int> const& v)
{
	int sc = 0;
	for(int n = 0; n < v.size(); ++n)
	{
		int l = (n == 0) ? v.size() - 1 : n - 1;
		int r = (n == v.size() - 1) ? 0 : n + 1;
		sc += g[v[n]][v[l]].second;
		sc += g[v[n]][v[r]].second;
	}
	return sc;
}

int compute(auto const& in)
{
	std::vector<int> v(in.size());
	std::iota(v.begin(), v.end(), 0);
	int most = std::numeric_limits<int>::min();
	do
	{
		int sc = score(in, v);
		if(sc > most)
			most = sc;
	} while (std::ranges::next_permutation(v).found);
	
	return most;
}

int pt1(auto const& in)
{
	timer t("p1");
	return compute(in);
}

int64_t pt2(auto& in)
{
	timer t("p2");
	int me = in.size();
	for(auto& n : in)
		n.emplace_back(me, 0);
	in.push_back({});
	for(int c = 0; c < me + 1; ++c)
		in.back().emplace_back(c, 0);
	return compute(in);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
