#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include "JsonTypes.hpp"

namespace SpiritJson {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

Json Parse(const std::string& str)
{
    Json j;
    return j;
}


}
