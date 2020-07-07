#include <iostream>
#include "JsonParser.hpp"

using namespace SpiritJson;

int main()
{
    try {
        Json j;
        j = Parse(R"(
{
  "pi": 3.141,
  "happy": true,
  "name": "Niels",
  "nothing": null,
  "answer": {
    "everything": 42
  },
  "名字": "兰德里",
  "list": [1, 0, 2],
  "object": {
    "currency": "USD",
    "value": 42.99,
    "empty": [],
    "variant": [
      1,
      false,
      true,
      "abcd\tef",
      {}
    ]
  }
})");
        std::string str;
        j.dump(str);
        std::cout << str;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
