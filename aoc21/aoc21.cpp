#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

struct object
{
	int cost_;
	int damage_;
	int armour_;
};

constexpr object weapons[] = {
	{  8, 4, 0},
	{ 10, 5, 0},
	{ 25, 6, 0},
	{ 40, 7, 0},
	{ 74, 8, 0}
};

constexpr object armour[] = {
	{   0, 0, 0},
	{  13, 0, 1},
	{  31, 0, 2},
	{  53, 0, 3},
	{  75, 0, 4},
	{ 102, 0, 5}
};

constexpr object ring[] = {
	{   0, 0, 0},
	{   0, 0, 0},
	{  25, 1, 0},
	{  50, 2, 0},
	{ 100, 3, 0},
	{  20, 0, 1},
	{  40, 0, 2},
	{  80, 0, 3}
};

struct player
{
	int hit_;
	int damage_;
	int armour_;
};

auto get_input()
{
	int param[3];
	std::string ln;
	int n = 0;
	while(std::getline(std::cin, ln))
	{
		auto[m, p] = ctre::match<"[a-zA-Z ]+: (\\d+)">(ln);
		if(m)
			param[n] = p.to_number<int>();
		++n;
	}
	return player(param[0], param[1], param[2]);
}

inline int compute_hit(player const& p1, player const& p2)
{
	auto damage = p1.damage_ - p2.armour_;
	return damage < 1 ? 1 : damage;
}

bool play(player p, player b)
{
	while(1)
	{
		b.hit_ -= compute_hit(p, b);
		if( b.hit_ < 1)
			return true;
		p.hit_ -= compute_hit(b, p);
		if(p.hit_ < 1)
			return false;
	}
}

int pt1(auto b)
{
	timer t("p1");
	player p;
	int min_cost = std::numeric_limits<int>::max();
	for(int w = 0; w < 5; ++w)
		for(int a = 0; a < 6; ++a)
			for(int r0 = 0; r0 < 8; ++r0)
				for(int r1 = r0 + 1; r1 < 8; ++r1)
				{
					int this_cost = weapons[w].cost_ + armour[a].cost_ + ring[r0].cost_ + ring[r1].cost_;
					int this_damage = weapons[w].damage_ + armour[a].damage_ + ring[r0].damage_ + ring[r1].damage_;
					int this_armour = weapons[w].armour_ + armour[a].armour_ + ring[r0].armour_ + ring[r1].armour_;
					player p { 100, this_damage, this_armour};
					if(play(p, b) && this_cost < min_cost)
						min_cost = this_cost;
				}
	return min_cost;
}

int pt2(auto b)
{
	timer t("p2");
	player p;
	int max_cost = 0;
	for(int w = 0; w < 5; ++w)
		for(int a = 0; a < 6; ++a)
			for(int r0 = 0; r0 < 8; ++r0)
				for(int r1 = r0 + 1; r1 < 8; ++r1)
				{
					int this_cost = weapons[w].cost_ + armour[a].cost_ + ring[r0].cost_ + ring[r1].cost_;
					int this_damage = weapons[w].damage_ + armour[a].damage_ + ring[r0].damage_ + ring[r1].damage_;
					int this_armour = weapons[w].armour_ + armour[a].armour_ + ring[r0].armour_ + ring[r1].armour_;
					player p { 100, this_damage, this_armour};
					if( !play(p, b) && this_cost > max_cost)
						max_cost = this_cost;
				}
	return max_cost;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
