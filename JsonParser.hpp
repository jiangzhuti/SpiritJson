#pragma once
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

    auto null_val = [](auto& ctx) {_val(ctx) = nullptr;};
    x3::rule<class json_null, std::nullptr_t> const json_null = "json_null";
    auto const json_null_def = lit("null")[null_val];
    BOOST_SPIRIT_DEFINE(json_null);

    auto number_val = [](auto& ctx) {_val(ctx) = _attr(ctx);};
    x3::rule<class json_number, double> const json_number = "json_number";
    auto const json_number_def = double_[number_val];
    BOOST_SPIRIT_DEFINE(json_number);

    auto bool_true_val = [](auto& ctx) {_val(ctx) = true;};
    auto bool_false_val = [](auto& ctx) {_val(ctx) = false;};
    x3::rule<class json_bool, bool> const json_bool = "json_bool";
    auto const json_bool_def = lit("true")[bool_true_val] | lit("false")[bool_false_val];
    BOOST_SPIRIT_DEFINE(json_bool);

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

    } escapes;
    auto escaped_push = [](auto& ctx) {_val(ctx).push_back(_attr(ctx));};
    x3::rule<class escaped_string, std::string> const escaped_string = "escaped_string";
    auto const escaped_string_def = lexeme['"' >> *(escapes[escaped_push] | (~x3::char_('"'))[escaped_push]) >> '"'];
    BOOST_SPIRIT_DEFINE(escaped_string);

    auto string_val = [](auto& ctx) {_val(ctx) = _attr(ctx);};
    x3::rule<class json_string, std::string> const json_string = "json_string";
    auto const json_string_def = escaped_string[string_val];
    BOOST_SPIRIT_DEFINE(json_string);

//
    auto array_push = [](auto& ctx) {_val(ctx).push_back(_attr(ctx));};
    x3::rule<class json_array, Json::array_type> const json_array = "json_array";
    x3::rule<class json_json, Json> const json_json = "json_json";
    x3::rule<class json_object, Json::object_type> const json_object = "json_object";
    auto json_cons = [](auto& ctx) {_val(ctx) = Json(_attr(ctx));};
    auto const json_json_def = json_null[json_cons] | json_number[json_cons] | json_bool[json_cons] | json_string[json_cons] | json_array[json_cons] | json_object[json_cons];
    auto const json_array_def = "[" >> -(json_json[array_push] >> *("," >> json_json[array_push])) >> "]";
    auto object_emplace = [](auto& ctx) {_val(ctx).emplace(at_c<0>(_attr(ctx)), at_c<1>(_attr(ctx)));};
    auto const json_object_def = "{" >>
                                -(
                                    (escaped_string >> ":" >> json_json)[object_emplace] >>
                                    *("," >>
                                         (escaped_string >> ":" >> json_json)[object_emplace]
                                     )
                                  ) >>
                                  "}";


    BOOST_SPIRIT_DEFINE(json_array, json_object, json_json);
    
    
    void test()
    {
      std::string s = "[1, 2, true, null, \"abc\", {\"1\": \"a\", \"2\": null, \"xyz\": [1, 2, 3, 4, 5]}]";

      Json j;
      auto it = s.begin();
      auto const end = s.end();
      bool ret = phrase_parse(it, end, json_json, space, j);
      std::string out;
      j.dump(out);
      std::cout << out;
      
    }
Json Parse(const std::string& str)
{
    Json j;
    return j;
}


}
