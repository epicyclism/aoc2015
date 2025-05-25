#include <string>
#include <algorithm>
#include <fmt/format.h>

#include "timer.h"

void increment(std::string& pw)
{
	++pw[0];
	if(pw[0] > 'z')
	{
		pw[0] = 'a';
		for(int n = 1; n < pw.size(); ++n)
		{
			++pw[n];
			if(pw[n] <= 'z')
				break;
			pw[n] = 'a';
		}	
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
	for(; n < pw.size() - 2; ++n)
		if( pw[n - 1] == pw[n])
			break;
	if(n == pw.size() - 2)
		return false;
	n += 2;
	for(; n < pw.size(); ++n)
		if( pw[n - 1] == pw[n])
			break;
	if( n == pw.size())
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
