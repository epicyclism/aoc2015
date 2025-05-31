#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

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
	std::unordered_set<std::string> s;
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

size_t count_substrings(std::string_view tgt, std::string_view s)
{
	size_t cnt = 0;
	auto ht = tgt.find(s, 0);
	while(ht != std::string::npos)
	{
		++cnt;
		ht = tgt.find(s, ht + 1);
	}
	return cnt;
}

int pt2(auto const& in)
{
	timer t("p2");
	auto& tgt = in.second;
	auto elements = std::ranges::count_if(tgt, [](auto c){ return std::isupper(c);});
	auto Y = std::ranges::count(tgt, 'Y');
	auto RnAr = count_substrings(tgt, "Rn") + count_substrings(tgt, "Ar");
	// every substitution adds an element other than the first which adds two.
	// Rn Ar and Y are free bonuses and you can get two Ys from a single substitution.
	// So this gives the minimum, assume it's possible?
	return elements - RnAr - 2 * Y - 1;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
