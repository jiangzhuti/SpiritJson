#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <cmath>

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
    using array_type = std::vector<Json>;
    using object_type = std::map<std::string, Json>;

    Json();
    Json(double val);
    Json(std::nullptr_t);
    Json(bool val);
    Json(const std::string& val);
    Json(std::string&& val);
    Json(const Json::array_type& val);
    Json(Json::array_type&& val);
    Json(const Json::object_type& val);
    Json(Json::object_type&& val);

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
    void dump(std::string &out, int indent = 2, int left = 0) const;

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
    virtual void dump(std::string &out, int indent, int left) const = 0;
};

void Json::dump(std::string &out, int indent, int left) const
{
    m_jv_ptr->dump(out, indent, left);
}
    
static void dump(NullStruct, std::string &out, int indent, int left)
{
    (void) indent;
    (void) left;
    out += "null";
}

static void dump(double value, std::string &out, int indent, int left)
{
    (void) indent;
    (void) left;
    if (std::isfinite(value)) {
        out += std::to_string(value);
    } else {
        out += "null";
    }
}

static void dump(bool value, std::string &out, int indent, int left)
{
    (void) indent;
    (void) left;
    out += value ? "true" : "false";
}

static void dump(const std::string &value, std::string &out, int indent, int left)
{
    (void) indent;
    (void) left;
    out += '"';
    for (size_t i = 0; i < value.length(); i++) {
        const char ch = value[i];
        if (ch == '\\') {
            out += "\\\\";
        } else if (ch == '"') {
            out += "\\\"";
        } else if (ch == '\b') {
            out += "\\b";
        } else if (ch == '\f') {
            out += "\\f";
        } else if (ch == '\n') {
            out += "\\n";
        } else if (ch == '\r') {
            out += "\\r";
        } else if (ch == '\t') {
            out += "\\t";
        } else if (static_cast<uint8_t>(ch) <= 0x1f) {
            char buf[8];
            snprintf(buf, sizeof buf, "\\u%04x", ch);
            out += buf;
        } else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i+1]) == 0x80
                   && static_cast<uint8_t>(value[i+2]) == 0xa8) {
            out += "\\u2028";
            i += 2;
        } else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i+1]) == 0x80
                   && static_cast<uint8_t>(value[i+2]) == 0xa9) {
            out += "\\u2029";
            i += 2;
        } else {
            out += ch;
        }
    }
    out += '"';
}

static void dump(const Json::array_type &values, std::string &out, int indent, int left)
{
    bool first = true;
    out += "[";
    for (const auto &value : values) {
        if (!first)
            out += ", \n";
        else
            out += "\n";
        out += std::string(left + indent, ' ');
        value.dump(out, indent, left + indent);
        first = false;
    }
    out += "\n";
    out += std::string(left, ' ');
    out += "]";
}

static void dump(const Json::object_type &values, std::string &out, int indent, int left)
{
    bool first = true;
    out += "{";
    for (const auto &kv : values) {
        if (!first)
            out += ", \n";
        else
            out += "\n";
        out += std::string(left + indent, ' ');
        dump(kv.first, out, 0, 0);
        out += ": ";
        kv.second.dump(out, indent, left + indent);
        first = false;
    }
    out += "\n";
    out += std::string(left, ' ');
    out += "}";
}



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
    void dump(std::string &out, int indent, int left) const override
    {
        SpiritJson::dump(m_val, out, indent, left);
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
Json::Json(std::nullptr_t) : m_jv_ptr(Statics::null)
{
}
Json::Json(double val) : m_jv_ptr(std::make_shared<JsonNumber>(val))
{
}
Json::Json(bool val) : m_jv_ptr(val ? Statics::t : Statics::f)
{
}
Json::Json(const std::string& val) : m_jv_ptr(std::make_shared<JsonString>(val))
{
}
Json::Json(std::string&& val) : m_jv_ptr(std::make_shared<JsonString>(std::move(val)))
{
}
Json::Json(const Json::array_type& val) : m_jv_ptr(std::make_shared<JsonArray>(val))
{
}
Json::Json(Json::array_type&& val) : m_jv_ptr(std::make_shared<JsonArray>(std::move(val)))
{
}
Json::Json(const Json::object_type& val) : m_jv_ptr(std::make_shared<JsonObject>(val))
{
}
Json::Json(Json::object_type&& val) : m_jv_ptr(std::make_shared<JsonObject>(std::move(val)))
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

