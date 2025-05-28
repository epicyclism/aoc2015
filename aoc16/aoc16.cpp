#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

constexpr char const* property_names[] = {"children", "cats", "samoyeds", "pomeranians", "akitas",
					"vizslas", "goldfish", "trees", "cars", "perfumes" };

inline int from_sv(std::string_view p)
{
	return std::ranges::find(property_names, p) - std::ranges::begin(property_names);
}

struct aunt
{
	std::array<int, 10> properties_;
	std::array<int, 10> properties2_;
	aunt()
	{
		properties_ = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};
		properties2_ = {3, 8, 2, 2, 0, 0, 4, 4, 2, 1};
	}
	void apply(std::string_view p, int n)
	{
		properties_[from_sv(p)] = n;
		properties2_[from_sv(p)] = n;
	}
	bool operator==(aunt const&) const = default;
};


auto get_input()
{
	std::vector<aunt> va;
	std::string ln;
	int n = 1;
	while(std::getline(std::cin, ln))
	{
		aunt a;
		for (auto m : ctre::search_all<"([a-z]+: \\d+)">(ln))
			if(auto[m1, p, n] = ctre::match<"([a-z]+): (\\d+)">(m); m)
				a.apply(p.view(), n.to_number<int>());
		va.emplace_back(std::move(a));
	}
	return va;
}

int pt1(auto const& in)
{
	timer t("p1");

	return std::ranges::find(in, aunt()) - in.begin() + 1;
}

int pt2(auto const& in)
{
	timer t("p2");
	aunt ad;

	return std::ranges::find_if(in, [&](auto const& a){
		return a.properties2_[0] == ad.properties_[0] &&
				a.properties2_[1] > ad.properties_[1] &&
				a.properties2_[2] == ad.properties_[2] &&
				a.properties2_[3] < ad.properties_[3] &&
				a.properties2_[4] == ad.properties_[4] &&
				a.properties2_[5] == ad.properties_[5] &&
				a.properties2_[6] < ad.properties_[6] &&
				a.properties2_[7] > ad.properties_[7] &&
				a.properties2_[8] == ad.properties_[8] &&
				a.properties2_[9] == ad.properties_[9]
		;}) - in.begin() + 1;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
