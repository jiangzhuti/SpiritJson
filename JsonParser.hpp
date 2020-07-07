#pragma once
#include <exception>
#include <stdexcept>
#include <iostream>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include "JsonTypes.hpp"

namespace SpiritJson {


namespace x3 = boost::spirit::x3;
using x3::double_;
using x3::_attr;
using x3::phrase_parse;
using x3::ascii::space;
using x3::ascii::char_;
using x3::_val;
using x3::_pass;
using x3::lit;
using x3::lexeme;
using x3::attr;
using x3::eps;
using boost::fusion::at_c;

x3::rule<class json_null, std::nullptr_t> const json_null = "json_null";
x3::rule<class json_number, double> const json_number = "json_number";
x3::rule<class json_bool, bool> const json_bool = "json_bool";
x3::rule<class escaped_string, std::string> const escaped_string = "escaped_string";
x3::rule<class json_string, std::string> const json_string = "json_string";
x3::rule<class json_array, Json::array_type> const json_array = "json_array";
x3::rule<class json_object, Json::object_type> const json_object = "json_object";
x3::rule<class json_json, Json> const json_json = "json_json";

struct nullptrs_ : x3::symbols<std::nullptr_t>
{
    nullptrs_()
    {
        add("null", nullptr);
    }
} const nullptrs;
    

struct bools_ : x3::symbols<bool>
{
    bools_()
    {
        add ("true",  true)
            ("false", false);
    }
} const bools;

struct escapes_ : x3::symbols<char>
{
    escapes_()
    {
        add
            ( "\\n", '\n')
            ( "\\b", '\b')
            ( "\\f", '\f')
            ( "\\t", '\t')
            ( "\\v", '\v')
            ( "\\0", '\0')
            ( "\\r", '\r')
            ( "\\\\", '\\')
            ( "\\\"", '"');
    }

} const escapes;

auto escaped_push = [](auto& ctx) {_val(ctx).push_back(_attr(ctx));};
auto array_push = [](auto& ctx) {_val(ctx).push_back(_attr(ctx));};
auto json_cons = [](auto& ctx) {_val(ctx) = Json(_attr(ctx));};
auto object_emplace = [](auto& ctx) {_val(ctx).emplace(at_c<0>(_attr(ctx)), at_c<1>(_attr(ctx)));};
    
auto const json_null_def =
    nullptrs;
auto const json_number_def =
    double_;
auto const json_bool_def =
    bools;
auto const escaped_string_def =
    lexeme['"' >> *(escapes[escaped_push] | (~x3::char_('"'))[escaped_push]) >> '"'];
auto const json_string_def =
    escaped_string;
auto const json_array_def =
    "[" >>
        -(json_json[array_push] >> *("," >> json_json[array_push])) >>
    "]";
    
auto const json_object_def =
    "{" >>
        -(
            (escaped_string >> ":" >> json_json)[object_emplace] >>
                *("," >>
                    (escaped_string >> ":" >> json_json)[object_emplace]
                )
            ) >>
    "}";

auto const json_json_def =
    json_null[json_cons]   |
    json_number[json_cons] |
    json_bool[json_cons]   |
    json_string[json_cons] |
    json_array[json_cons]  |
    json_object[json_cons];


BOOST_SPIRIT_DEFINE(json_null, json_bool, json_number, escaped_string, json_string, json_array, json_object, json_json)

Json Parse(const std::string& str)
{
    Json j;
    auto it = str.begin();
    auto const end = str.end();
    bool ret = phrase_parse(it, end, json_json, space, j);
    if (ret && it == end) {
        return j;
    } else {
        throw std::runtime_error(std::string("failed to parse at: \"").append(std::string(it, end)).append("\""));
    }
}

}
