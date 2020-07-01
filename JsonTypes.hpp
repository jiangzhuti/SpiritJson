#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>

namespace SpiritJson {

class JsonValue;

class Json
{
public:
    enum Type {
        NIL,
        NUMBER,
        BOOL,
        STRING,
        ARRAY,
        OBJECT
    };
    using array_type = std::vector<std::shared_ptr<Json>>;
    using object_type = std::map<std::string, std::shared_ptr<Json>>;

    Json();

    Type type() const;

    bool is_null()
    {
        return type() == NIL;
    }
    bool is_number()
    {
        return type() == NUMBER;
    }
    bool is_bool()
    {
        return type() == BOOL;
    }
    bool is_string()
    {
        return type() == STRING;
    }
    bool is_array()
    {
        return type() == ARRAY;
    }
    bool is_object()
    {
        return type() == OBJECT;
    }
    
    double number_value() const;
    bool bool_value() const;
    const std::string& string_value() const;
    const Json::array_type& array_value() const;
    const Json::object_type& object_value() const;

private:
    std::shared_ptr<JsonValue> m_jv_ptr;    
};

struct NullStruct
{
};

class JsonValue
{
public:

    virtual Json::Type type() const = 0;
    virtual double number_value() const;
    virtual bool bool_value() const;
    virtual const std::string& string_value() const;
    virtual const Json::array_type& array_value() const;
    virtual const Json::object_type& object_value() const;
};


template <Json::Type JT, typename T>
class ValueWrapper : public JsonValue
{
public:
    ValueWrapper(const T& val) : m_val(val)
    {
    }
    ValueWrapper(T&& val) : m_val(std::move(val))
    {
    }
    Json::Type type() const override
    {
        return JT;
    }
protected:
    const T m_val;
};

class JsonNumber final : public ValueWrapper<Json::NUMBER, double>
{
public:
    JsonNumber(double val) : ValueWrapper(val)
    {
    }
    double number_value() const override
    {
        return m_val;
    }
    
};

class JsonBool final : public ValueWrapper<Json::BOOL, bool>
{
public:
    JsonBool(bool val) : ValueWrapper(val)
    {
    }
    bool bool_value() const override
    {
        return m_val;
    }
};

class JsonString final : public ValueWrapper<Json::STRING, std::string>
{
public:
    JsonString(const std::string& val) : ValueWrapper(val)
    {
    }
    JsonString(std::string&& val) : ValueWrapper(std::move(val))
    {
    }
    const std::string& string_value() const override
    {
        return m_val;
    }
};

class JsonArray final : public ValueWrapper<Json::ARRAY, Json::array_type>
{
public:
    JsonArray(const Json::array_type& val) : ValueWrapper(val)
    {
    }
    JsonArray(Json::array_type&& val) : ValueWrapper(std::move(val))
    {
    }
    const Json::array_type& array_value() const override
    {
        return m_val;
    }
};

class JsonObject final : public ValueWrapper<Json::OBJECT, Json::object_type>
{
public:
    JsonObject(const Json::object_type& val) : ValueWrapper(val)
    {
    }
    JsonObject(Json::object_type&& val) : ValueWrapper(std::move(val))
    {
    }
    const Json::object_type& object_value() const override
    {
        return m_val;
    }
};

class JsonNull final : public ValueWrapper<Json::NIL, NullStruct>
{
public:
    JsonNull() : ValueWrapper({}) {}
};


struct Statics
{
    static const std::shared_ptr<JsonValue> null;
    static const std::shared_ptr<JsonValue> t;
    static const std::shared_ptr<JsonValue> f;
    static const std::string empty_string;
    static const Json::array_type empty_array;
    static const Json::object_type empty_object;
};
    
Json::Json() : m_jv_ptr(Statics::null)
{
}

Json::Type Json::type() const
{
    return m_jv_ptr->type();
}
double Json::number_value() const
{
    return m_jv_ptr->number_value();
}
bool Json::bool_value() const
{
    return m_jv_ptr->bool_value();
}
const std::string& Json::string_value() const
{
    return m_jv_ptr->string_value();
}
const Json::array_type& Json::array_value() const
{
    return m_jv_ptr->array_value();
}
const Json::object_type& Json::object_value() const
{
    return m_jv_ptr->object_value();
}

const std::shared_ptr<JsonValue> Statics::null = std::make_shared<JsonNull>();
const std::shared_ptr<JsonValue> Statics::t = std::make_shared<JsonBool>(true);
const std::shared_ptr<JsonValue> Statics::f = std::make_shared<JsonBool>(false);
const std::string Statics::empty_string = std::string();
const Json::array_type Statics::empty_array = Json::array_type();
const Json::object_type Statics::empty_object = Json::object_type();


double JsonValue::number_value() const
{
    return 0;
}
bool JsonValue::bool_value() const
{
    return false;
}
const std::string& JsonValue::string_value() const
{
    return Statics::empty_string;
}
const Json::array_type&  JsonValue::array_value() const
{
    return Statics::empty_array;
}
const Json::object_type& JsonValue::object_value() const
{
    return Statics::empty_object;
}

}
