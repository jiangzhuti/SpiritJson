# SpiritJson

一个基于boost.spirit x3 的json解析器(玩具)。

json结构参考了json11，语法规则是简化版的json语法(参见https://www.json.org/json-en.html ，不支持unicode转义、数字统一用double)

# test

```
$ g++ -o t -Wall test.cpp -g -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined
$ ./t | jq .
{
  "answer": {
    "everything": 42
  },
  "happy": true,
  "list": [
    1,
    0,
    2
  ],
  "name": "Niels",
  "nothing": null,
  "object": {
    "currency": "USD",
    "value": 42.99
  },
  "pi": 3.141
}

```
