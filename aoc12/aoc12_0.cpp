#include <iostream>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>

#include <fmt/format.h>

#define BOOST_JSON_NO_LIB
#define BOOST_CONTAINER_NO_LIB
#include <boost/json/src.hpp>
#include <boost/json.hpp>
#include <boost/json/basic_parser_impl.hpp>

#include "ctre_inc.h"
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

int64_t sum = 0;
int64_t sum2 = 0;
int object_depth = 0;
bool red = false;
int red_depth = 0;
bool inobject = false;

class aoc_parser
{
    struct handler
    {
        constexpr static std::size_t max_object_size = std::size_t(-1);
        constexpr static std::size_t max_array_size = std::size_t(-1);
        constexpr static std::size_t max_key_size = std::size_t(-1);
        constexpr static std::size_t max_string_size = std::size_t(-1);

        bool on_document_begin( boost::system::error_code& ) { return true; }
        bool on_document_end( boost::system::error_code& ) { return true; }
        bool on_object_begin( boost::system::error_code& )
		{
			++object_depth;
			inobject = true;
			return true; 
		}
        bool on_object_end( std::size_t, boost::system::error_code& )
		{
			 --object_depth;
			 if(object_depth < red_depth)
			 	 red = false;
			 return true;
		}
        bool on_array_begin( boost::system::error_code& ) { inobject = false; return true; }
        bool on_array_end( std::size_t, boost::system::error_code& ) { return true; }
        bool on_key_part( string_view, std::size_t, boost::system::error_code& ) { return true; }
        bool on_key( string_view, std::size_t, boost::system::error_code& ) { return true; }
        bool on_string_part( string_view, std::size_t, boost::system::error_code& ) { return true; }
        bool on_string( string_view sv, std::size_t, boost::system::error_code& )
		{
			if(sv == "red" && inobject)
			{
				if(!red)
				{
					red_depth = object_depth;
					red = true;
				}
			}
			return true;
		}
        bool on_number_part( string_view, boost::system::error_code& ) { return true; }
        bool on_int64( std::int64_t v, string_view, boost::system::error_code& )
		{
			sum += v;
			if(!red)
				sum2 += v;
			return true;
		}
        bool on_uint64( std::uint64_t v, string_view, boost::system::error_code& ) 
		{
			sum += v;
			if(!red)
				sum2 += v;
			return true;
		}
        bool on_double( double, string_view, boost::system::error_code& ) { return true; }
        bool on_bool( bool, boost::system::error_code& ) { return true; }
        bool on_null( boost::system::error_code& ) { return true; }
        bool on_comment_part(string_view, boost::system::error_code&) { return true; }
        bool on_comment(string_view, boost::system::error_code&) { return true; }
    };

    basic_parser<handler> p_;

public:
    aoc_parser()
        : p_(parse_options())
    {
    }

    ~aoc_parser()
    {
    }

    std::size_t
    write(
        char const* data,
        std::size_t size,
        boost::system::error_code& ec)
    {
        auto const n = p_.write_some( false, data, size, ec );
        if(! ec && n < size)
            ec = error::extra_data;
        return n;
    }
};

int64_t pt2(auto const& in)
{
	timer t("p2");
	aoc_parser p;
    boost::system::error_code ec;
    p.write( in.data(), in.size(), ec );
    if( ec )
        return 0;
	return sum2;
}

int main()
{
	auto in = get_input();
	auto p1 = pt1(in);
	auto p2 = pt2(in);
	fmt::println("pt1 = {}", p1);
	fmt::println("pt2 = {}", p2);
}

// 27769 too low