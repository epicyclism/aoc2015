#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

enum code : uint8_t { hlf, tpl, inc, jmp, jie, jio };

struct inst
{
	code    i_;
	uint8_t r_;
	int8_t  o_;
} ;

struct machine
{
	uint64_t r_[2] = {0, 0};
	void execute(std::vector<inst> const& v)
	{
		size_t ip = 0;
		while( ip < v.size())
		{
			auto& i = v[ip];
			switch(i.i_)
			{
				case code::hlf:
					r_[i.r_] = r_[i.r_] / 2;
					++ip;
					break;
				case code::tpl:
					r_[i.r_] = r_[i.r_] * 3;
					++ip;	
					break;
				case code::inc:
					r_[i.r_] = r_[i.r_] + 1;
					++ip;
					break;
				case code::jmp:
					ip += i.o_;
					break;
				case code::jie:
					if(!(r_[i.r_] & 1))
						ip += i.o_;
					else
						++ip;
					break;
				case code::jio:
					if(r_[i.r_] == 1)
						ip += i.o_;
					else
						++ip;
					break;
			}
		}
	}
};

uint8_t to_r(std::string_view v)
{
	return v[0] == 'a' ? uint8_t(0) : uint8_t(1);
}

int from_s(std::string_view s)
{
	return s[0] == '-' ? -1 : 1;
}

auto get_input()
{
	std::vector<inst> vr;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, r] = ctre::match<"hlf (a|b)">(ln); m)
			vr.emplace_back(code::hlf, to_r(r), 0);
		else
		if(auto[m, r] = ctre::match<"tpl (a|b)">(ln); m)
			vr.emplace_back(code::tpl, to_r(r), 0);
		else
		if(auto[m, r] = ctre::match<"inc (a|b)">(ln); m)
			vr.emplace_back(code::inc, to_r(r), 0);
		else
		if(auto[m, s, o] = ctre::match<"jmp ([\\+\\-])(\\d+)">(ln); m)
			vr.emplace_back(code::jmp, 0, from_s(s) * o.to_number<int8_t>());
		else
		if(auto[m, r, s, o] = ctre::match<"jie (a|b), ([\\+\\-])(\\d+)">(ln); m)
			vr.emplace_back(code::jie, to_r(r), from_s(s) * o.to_number<int8_t>());
		else
		if(auto[m, r, s, o] = ctre::match<"jio (a|b), ([\\+\\-])(\\d+)">(ln); m)
			vr.emplace_back(code::jio, to_r(r), from_s(s) * o.to_number<int8_t>());
		else
			fmt::println("Unparsed instruction : {}", ln);
	}
	return vr;
}

uint64_t pt1(auto const& in)
{
	timer t("p1");
	machine m;
	m.execute(in);
	return m.r_[1];
}

uint64_t pt2(auto const& in)
{
	timer t("p2");
	machine m;
	m.r_[0] = 1;
	m.execute(in);
	return m.r_[1];

}

int main()
{
	auto in = get_input();
	fmt::println("got {} instructions", in.size());
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
