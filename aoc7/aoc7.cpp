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

uint32_t nm_to_keyvar(std::string_view s)
{
	uint32_t r = 0;
	if(std::isdigit(s[0]))
		r = sv_to_t<uint16_t>(s);
	else
	{
		for(auto c: s)
		{
			r *= 26;
			r += c - 'a';
		}
		r |= 0x80000000;
	}
	return r;
}

constexpr size_t mx_tab = 0x3ff; // more than 26*26

struct table_t
{
	std::array<uint16_t, mx_tab> tab_;
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

void act(gate_t& g, table_t& t)
{
	switch(g.op_)
	{
		case op_t::SET:
		if(g.out_ == 0x80000000)
			fmt::println("Set a = {} = {}", g.in0_, t.get(g.in0_));
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

//OR, NOT, LSHIFT, RSHIFT
auto get_input()
{
	std::vector<gate_t> rv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, i0, d] = ctre::match<"(\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// SET
			rv.emplace_back(op_t::SET, nm_to_keyvar(i0), 0, nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) AND (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::AND, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) OR (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::OR, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
		if(auto[m, i0, d] = ctre::match<"NOT (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::NOT, nm_to_keyvar(i0), 0, nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) LSHIFT (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::LSHIFT, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
		if(auto[m, i0, i1, d] = ctre::match<"(\\d+|[a-z]+) RSHIFT (\\d+|[a-z]+) -> ([a-z]+)">(ln); m)		// AND
			rv.emplace_back(op_t::RSHIFT, nm_to_keyvar(i0), nm_to_keyvar(i1), nm_to_keyvar(d));
		else
			fmt::println("Line {} : didn't parse!", ln);
	}
	fmt::println("Got {} gates.", rv.size());
	return rv;
}

uint16_t pt1(auto& in)
{
	timer t("p1");
	table_t tab;
	for(int n = 0; n < 100; ++n)
	{
		for(auto& g: in)
			act(g, tab);
		std::println("lx = {} a = {}", tab.get(nm_to_keyvar("lx")), tab.get(nm_to_keyvar("a")));
	}
	return tab.get(nm_to_keyvar("a"));
}

uint16_t pt2(auto const& in)
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
