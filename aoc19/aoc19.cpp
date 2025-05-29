#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	std::vector<std::pair<std::string, std::string>> replacements;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(ln.empty())
			break;
		if(auto[m, f, t] = ctre::match<"([A-Za-z]+) => ([A-Za-z]+)">(ln); m)
			replacements.emplace_back(f.to_string(), t.to_string());
	}
	std::getline(std::cin, ln);
	return std::make_pair(replacements, ln);
}

int pt1(auto const& in)
{
	timer t("p1");
	std::set<std::string> s;
	auto& tgt = in.second;
	auto& reps = in.first;

	for(auto& rep: reps)
	{
		auto ht = tgt.find(rep.first, 0);
		while(ht != std::string::npos)
		{
			std::string cmpnd;
			cmpnd.assign(tgt.begin(), tgt.begin() + ht);
			cmpnd += rep.second;
			cmpnd.append(tgt.begin() + ht + rep.first.size(), tgt.end());
			s.insert(cmpnd);
			ht = tgt.find(rep.first, ht + 1);
		}
	}
	return s.size();
}

int pt2(auto const& in)
{
	timer t("p2");
	return 0;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
