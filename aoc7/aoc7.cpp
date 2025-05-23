#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum class op_t { SET, AND, OR, NOT, LSHIFT, RSHIFT };
using val_t = uint32_t;
constexpr size_t mx_tab = 0x3ff; // more than 27*27

val_t nm_to_keyvar(std::string_view s)
{
	uint32_t r = 0;
	if(std::isdigit(s[0]))
		r = sv_to_t<uint16_t>(s);
	else
	{
		for(auto c: s)
		{
			r *= 27;
			r += c - 'a' + 1;
		}
		r |= 0x80000000;
	}
	return r;
}

struct table_t
{
	std::array<uint16_t, mx_tab> tab_;
	table_t ()
	{
		tab_.fill(0xffff);
	}
	uint16_t get(val_t a)
	{
		if(a & 0x80000000)
			return tab_[a & mx_tab];
		return static_cast<uint16_t>(a);
	}
	void set(uint32_t a, uint16_t v)
	{
		tab_[a & mx_tab] = v;
	}
};

struct gate_t
{
	op_t op_;
	val_t in0_;
	val_t in1_;
	val_t out_;
};

void act(gate_t const& g, table_t& t)
{
	switch(g.op_)
	{
		case op_t::SET:
			t.set(g.out_, t.get(g.in0_));
			break;
		case op_t::AND:
			t.set(g.out_, t.get(g.in0_) & t.get(g.in1_));
			break;
		case op_t::OR:
			t.set(g.out_, t.get(g.in0_) | t.get(g.in1_));
			break;
		case op_t::NOT:
			t.set(g.out_, ~t.get(g.in0_));
			break;
		case op_t::LSHIFT:
			t.set(g.out_, t.get(g.in0_) << t.get(g.in1_));
			break;
		case op_t::RSHIFT:
			t.set(g.out_, t.get(g.in0_) >> t.get(g.in1_));
			break;
	}
}

std::vector<int> topo_sort(std::vector<gate_t> const& vg)
{
	std::vector<int> rv;
	rv.reserve(vg.size());
	std::vector<uint8_t> flgs (vg.size(), 1);
	// 0x01 not dealt with
	// 0x02 waiting i0_
	// 0x04 waiting i1_;
	std::ranges::transform(vg, flgs.begin(), [](auto const& g)
		{
			uint8_t u = 1;
			if(g.in0_ > 0x80000000)
				u |= 0x02;
			if(g.in1_ > 0x80000000)
				u |= 0x04;
			return u;
		});
	while(!std::ranges::all_of(flgs, [](auto f){ return f == 0;} ))
	{
		for(int n = 0; n < vg.size(); ++n)
		{
			if((flgs[n] & 0x07) == 0x01) // inputs satisfied
			{
				rv.push_back(n);
				flgs[n] = 0;
				auto o = vg[n].out_;
				for(int m = 0; m < vg.size(); ++m)
				{
					if(vg[m].in0_ == o)
						flgs[m] &= ~0x02;
					if(vg[m].in1_ == o)
						flgs[m] &= ~0x04;
				}
			}
		}
	}
	return rv;
}

auto get_input()
{
	std::vector<gate_t> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, i0, d] = ctre::match<"(\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// SET
			rv.emplace_back(op_t::SET, nm_to_keyvar(i0), 0x80000000, nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) AND (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::AND, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) OR (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::OR, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
		if(auto[m, i0, d] = ctre::match<"NOT (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::NOT, nm_to_keyvar(i0), 0x80000000, nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) LSHIFT (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::LSHIFT, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) RSHIFT (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::RSHIFT, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
			fmt::println("Line {} : didn't parse!", ln);
	}
	return rv;
}

uint16_t pt1(auto const& in, auto const& order)
{
	timer t("p1a");
	table_t tab;
	for(auto g: order)
			act(in[g], tab);
	return tab.get(nm_to_keyvar("a"));
}
uint16_t pt2(auto& in, uint16_t b, auto const& order)
{
	timer t("p2");
	// edit the code
	auto bd = nm_to_keyvar("b");
	for(auto& g: in)
		if(g.op_ == op_t::SET && g.out_ == bd )
		{
			g.in0_ = b;
			break;
		}
	table_t tab;
	for(auto g: order)
			act(in[g], tab);
	return tab.get(nm_to_keyvar("a"));
}

int main()
{
	auto in = get_input();
	auto order = topo_sort(in);
	auto p1 = pt1(in, order);
	auto p2 = pt2(in, p1, order);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}