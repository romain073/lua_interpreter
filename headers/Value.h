#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>
using namespace std;


struct Value
{
    enum TYPE { TYPE_INT, TYPE_STRING, TYPE_DOUBLE, TYPE_BOOLEAN, TYPE_LIST, TYPE_NULL } type;

    int int_val;
    bool bool_val;
    string string_val;
    double double_val;
    vector<Value> list_val;
    
    static Value FromNumber(string n){
        if(n.find(".") != string::npos){
            return Value(stod(n)); //dec
        }
        return Value(stoi(n)); // int
    }
    
    Value(){
        type = TYPE_NULL;
    }    
    Value(int v){
        int_val = v;
        type = TYPE_INT;
    }
    Value(bool v){
        bool_val = v;
        type = TYPE_BOOLEAN;
    }
    Value(double v){
        double_val = v;
        type = TYPE_DOUBLE;
    }
    Value(string v){
        string_val = v;
        type = TYPE_STRING;
    }
    Value(vector<Value> v){
        list_val = v;
        type = TYPE_LIST;
    }
    
    bool isBoolean(){
        return type == TYPE_BOOLEAN;
    }
    Value operator+(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value(int_val + v.int_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_INT)
            return Value(double_val + v.int_val);
        if(this->type == TYPE_INT && v.type == TYPE_DOUBLE)
            return Value(v.int_val + double_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_DOUBLE)
            return Value(double_val + v.double_val);
        cout << "Type not handled in + operator.";
        throw;
    }
    Value operator*(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value(int_val * v.int_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_INT)
            return Value(double_val * v.int_val);
        if(this->type == TYPE_INT && v.type == TYPE_DOUBLE)
            return Value(v.int_val * double_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_DOUBLE)
            return Value(double_val * v.double_val);
        cout << "Type not handled in * operator.";
        throw;
    }
    Value operator/(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value(int_val / v.int_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_INT)
            return Value(double_val / v.int_val);
        if(this->type == TYPE_INT && v.type == TYPE_DOUBLE)
            return Value(v.int_val / double_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_DOUBLE)
            return Value(double_val / v.double_val);
        cout << "Type not handled in / operator.";
        throw;
    }
    Value operator-(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value(int_val - v.int_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_INT)
            return Value(double_val - v.int_val);
        if(this->type == TYPE_INT && v.type == TYPE_DOUBLE)
            return Value(v.int_val - double_val);
        if(this->type == TYPE_DOUBLE && v.type == TYPE_DOUBLE)
            return Value(double_val - v.double_val);
        cout << "Type not handled in - operator.";
        throw;
    }
    
    
};

inline ostream& operator<<(ostream& os, const Value& obj) {
    switch(obj.type){
        case Value::TYPE_INT:
        	os << "TYPE_INT " << obj.int_val;
        	break;
        case Value::TYPE_STRING:
        	os << "TYPE_STRING " <<  obj.string_val;
        	break;
        case Value::TYPE_DOUBLE:
        	os << "TYPE_DOUBLE " <<  obj.double_val;
        	break;
        case Value::TYPE_BOOLEAN:
        	os << "TYPE_BOOLEAN " <<  obj.bool_val;
        	break;
        case Value::TYPE_LIST:
        	os << "TYPE_LIST size " <<  obj.list_val.size();
        	break;
        case Value::TYPE_NULL:
        	os << "TYPE_NULL";
        	break;
    }
    return os;
}   

#endif  /* VALUE_H */