#include <iostream>
#include <string>

#include <fmt/format.h>

#include "ctre_inc.h"
#include "timer.h"

auto get_input()
{
	int row = 0;
	int col = 0;
	std::string ln;
	std::getline(std::cin, ln);
	if(auto[m, r, c] = ctre::match<"To continue, please consult the code grid in the manual.  Enter the code at row (\\d+), column (\\d+).">(ln); m)
	{
		row = r.to_number<int>();
		col = c.to_number<int>();
	}
	return std::make_pair(row, col);
}

int row_col_n(int r, int c)
{
	int br = r + c - 2;
	int cnt = ((1 + br) * br) / 2;
	return cnt + c;
}

uint64_t step(uint64_t from)
{
	return (from * 252533ULL) % 33554393ULL;
}

int64_t pt1(auto const& in)
{
	timer t("p1");
	int cnt = row_col_n(in.first, in.second);
	uint64_t v = 20151125;
	for(int n = 1; n < cnt; ++n)
		v = step(v);
	return v;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	fmt::println("pt1 = {}", p1);
}
