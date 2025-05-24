#include <string>
#include <algorithm>
#include <fmt/format.h>

#include "timer.h"

void increment(std::string& pw)
{
	int carry = 0;
	++pw[0];
	for(auto& c: pw)
	{
		c += carry;
		if( c > 'z')
		{
			carry = 1;
			c = 'a';
		}
		else
			return;
	}
}

bool valid(std::string const& pw)
{
	// valid letters?
	for(auto c: pw)
		if(c == 'i' || c == 'l' || c == 'o')
			return false;
	// valid pairs?
	int n = 1;
	bool p1 = false;
	for(; n < pw.size(); ++n)
		if( pw[n - 1] == pw[n])
		{
			n += 2;
			p1 = true;
			break;
		}
	bool p2 = false;
	for(; n < pw.size(); ++n)
		if( pw[n - 1] == pw[n])
		{
			p2 = true;
			break;
		}
	if( !(p1 && p2))
		return false;
	// valid 'straight' (remember password is backwards...)
	for(n = 2; n < pw.size(); ++n)
		if(pw[n - 2] - 1 == pw[n - 1] && pw[n - 1] == pw[n] + 1)
			return true;
	return false;
}

auto pt1(auto in)
{
	timer t("p1");
	std::reverse(in.begin(), in.end());
	increment(in);
	while(!valid(in))
		increment(in);
	std::reverse(in.begin(), in.end());
	return in;
}

int main(int ac, char** av)
{
	std::string in = "hepxcrrq";
	if(ac > 1)
		in = av[1];
	auto p1 = pt1(in);
	auto p2 = pt1(p1);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}
