# SpiritJson

一个基于boost.spirit x3 的json解析器(玩具)。

json结构参考了json11，语法规则是简化版的json语法(参见https://www.json.org/json-en.html ，不支持unicode转义、数字统一用double)

[JSONTestSuit](https://github.com/nst/JSONTestSuite)通过率75%

# test

```
$ g++ -o t -Wall test.cpp -g -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined
$ ./t | jq .
{
  "answer": {
    "everything": 42.000000
  }, 
  "happy": true, 
  "list": [
    1.000000, 
    0.000000, 
    2.000000
  ], 
  "name": "Niels", 
  "nothing": null, 
  "object": {
    "currency": "USD", 
    "value": 42.990000
  }, 
  "pi": 3.141000
}

```
