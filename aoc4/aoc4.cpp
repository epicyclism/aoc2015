#include <iostream>
#include <string>
#include <array>

#include <fmt/format.h>
#include <openssl/md5.h>

#include "timer.h"

#if defined (USE_OPENSSL)

int pt1(auto const& in)
{
	timer t("p1");
	std::array<uint8_t, 16> md5;
	const unsigned key = 0x00f0ffff;
	int n = 1;
	std::string wsp;
	while(1)
	{
		wsp = in;
		wsp += std::to_string(n);
		MD5(reinterpret_cast<unsigned char const*>(wsp.data()), wsp.size(), md5.data());
		if((key & *reinterpret_cast<unsigned const*>(md5.data())) == 0)
			return n;
		++n;
	}
	return -1;
}

int pt2(auto const& in, int ni)
{
	timer t("p2");
	std::array<uint8_t, 16> md5;
	const unsigned key = 0x00ffffff;
	int n = ni;
	std::string wsp;
	while(1)
	{
		wsp = in;
		wsp += std::to_string(n);
		MD5(reinterpret_cast<unsigned char const*>(wsp.data()), wsp.size(), md5.data());
		if((key & *reinterpret_cast<unsigned const*>(md5.data())) == 0)
			return n;
		++n;
	}
	return -1;
}

int main(int ac, char** av)
{
	std::string in;
	if(ac == 2)
		in = av[1];
	else
		in = "iwrupvqb";
	auto p1 = pt1(in);
	auto p2 = pt2(in, p1);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
#else
int main()
{
	fmt::println("aoc4 : OpenSSL not found, install it and rebuild.");
}
#endif