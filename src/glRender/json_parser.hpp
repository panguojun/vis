using namespace std;
namespace JSON
{
    // JSON value类型
    enum ValueType {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        BOOLEAN,
        NULL_VALUE
    };

    // JSON value结构体
    struct JSONValue {
        ValueType type;
        string strValue;
        double numValue;
        bool boolValue;
        vector<JSONValue> arrValue;
        std::map<string, JSONValue> objValue;
    };

    // 解析JSON字符串
    JSONValue parseJSON(string json) {
        JSONValue value;
        size_t pos = 0;
        if (json[pos] == '{') {
            value.type = OBJECT;
            pos++;
            while (pos < json.length()) {
                if (json[pos] == '}') {
                    break;
                }
                string key = "";
                while (json[pos] != ':') {
                    key += json[pos];
                    pos++;
                }
                pos++;
                JSONValue val = parseJSON(json.substr(pos));
                value.objValue[key] = val;
                pos += val.strValue.length() + 1;
                if (json[pos] == ',') {
                    pos++;
                }
            }
        }
        else if (json[pos] == '[') {
            value.type = ARRAY;
            pos++;
            while (pos < json.length()) {
                if (json[pos] == ']') {
                    break;
                }
                JSONValue val = parseJSON(json.substr(pos));
                value.arrValue.push_back(val);
                pos += val.strValue.length() + 1;
                if (json[pos] == ',') {
                    pos++;
                }
            }
        }
        else if (json[pos] == '\"') {
            value.type = STRING;
            pos++;
            while (json[pos] != '\"') {
                value.strValue += json[pos];
                pos++;
            }
            pos++;
        }
        else if (json[pos] == 't' || json[pos] == 'f') {
            value.type = BOOLEAN;
            if (json.substr(pos, 4) == "true") {
                value.boolValue = true;
                pos += 4;
            }
            else {
                value.boolValue = false;
                pos += 5;
            }
        }
        else if (json[pos] == 'n') {
            value.type = NULL_VALUE;
            pos += 4;
        }
        else {
            value.type = NUMBER;
            while (isdigit(json[pos]) || json[pos] == '.') {
                value.strValue += json[pos];
                pos++;
            }
            value.numValue = value.strValue.empty() ? 0 : stod(value.strValue);
        }
        return value;
    }

    JSONValue read(crstr filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file!" << std::endl;
            return JSONValue();
        }
        string jsonstr;
        file >> jsonstr;
        return parseJSON(jsonstr);
    }
}