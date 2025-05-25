#include <iostream>
#include <string>

#include <fmt/format.h>

#define BOOST_JSON_NO_LIB
#include <boost/json/src.hpp>

#include "timer.h"

auto get_input()
{
	std::string ln;
	std::getline(std::cin, ln);
	return ln;
}

int pt1(auto const& in)
{
	timer t("p1");
	int n = 0;
	int sign = 1;
	int s = 0;
	for(auto c: in)
	{
		if(::isdigit(c))
		{
			n *= 10;
			n += c - '0';
		}
		else
		if( c == '-')
		{
			sign = -1;
		}
		else
		{
			s += sign * n;
			sign = 1;
			n = 0;
		}
	}
	return s;
}

using namespace boost::json;

int64_t sum_not_red(auto& jv)
{
	int64_t total = 0;
	switch(jv.kind())
	{
	case kind::int64:
		total += jv.as_int64();
		break;
	case kind::uint64:
		total += jv.as_uint64();
		break;
	case kind::array:
		for(auto& vv : jv.as_array())
			total += sum_not_red(vv);
		break;
	case kind::object:
	{
		bool red = false;
		for(auto& vv : jv.as_object())
			if(vv.value().is_string() && vv.value().as_string() == "red")
				red = true;
		if(!red)
			for(auto& vv : jv.as_object())
				total += sum_not_red(vv.value());
	}
		break;
	default:
		break;
	}
	return total;
}

int64_t pt2(auto const& in)
{
	timer t("p2");
	auto jv = parse(in);

	return sum_not_red(jv);
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}