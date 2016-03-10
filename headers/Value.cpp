#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Node;
struct Value
{
    enum TYPE { TYPE_INT, TYPE_STRING, TYPE_DOUBLE, TYPE_BOOLEAN, TYPE_LIST, TYPE_FUNCTION, TYPE_NULL, TYPE_NIL, TYPE_BREAK } type;

    int int_val;
    bool bool_val;
    string string_val;
    double double_val;
    vector<Value> list_val;
    Node* function_val;
    
    bool return_value = false;
    
    static Value FromNumber(string n){
        if(n.find(".") != string::npos){
            return Value(stod(n)); //dec
        }
        return Value(stoi(n)); // int
    }
    
    Value setReturn(bool b=true){
        return_value = b;
        return *this;
    }
    
    static Value NIL(){
        Value v;
        v.type = TYPE_NIL;
        return v;
    }
    static Value BREAK(){
        Value v;
        v.type = TYPE_BREAK;
        return v;
    }
    Value(){
        type = TYPE_NULL;
    }    
    Value(int v){
        int_val = v;
        type = TYPE_INT;
    }
    Value(Node* v){
        function_val = v;
        type = TYPE_FUNCTION;
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
    
    bool isFunction() const{
        return type == TYPE_FUNCTION;
    }
    bool isNull() const{
        return type == TYPE_NULL;
    }
    bool isBreak() const{
        return type == TYPE_BREAK;
    }
    bool isReturn() const{
        return return_value;
    }
    
    bool isSingleElementList() const{
        return type == TYPE_LIST && list_val.size()==1;
    }

    
    bool isTrue(){
        switch(this->type){
            case TYPE_INT:
            	return this->int_val != 0;
            case TYPE_STRING:
            	return this->string_val.length() != 0;
            case TYPE_DOUBLE:
            	return this->double_val != 0;
            case TYPE_BOOLEAN:
            	return this->bool_val;
            case TYPE_LIST:
                return list_val.size()!=0;
            case TYPE_NULL:
            	return false;
        }
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
        exit(1);
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
        exit(1);
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
        exit(1);
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
        exit(1);
    }
    Value operator%(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value(int_val % v.int_val);
        cout << "Type not handled in % operator.";
        exit(1);
    }
    Value operator==(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value(int_val == v.int_val);
        cout << "Type not handled in == operator.";
        exit(1);
    }
    bool operator>(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return int_val > v.int_val;
        cout << "Type not handled in > operator.";
        exit(1);
    }
    
    bool operator<(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return int_val < v.int_val;
        cout << "Type not handled in < operator.";
        exit(1);
    }
    
    string print() {
        switch(this->type){
            case TYPE_INT:
            	return to_string(this->int_val);
            case TYPE_STRING:
            	return this->string_val;
            case TYPE_DOUBLE:
            	return to_string(this->double_val);
            case TYPE_BOOLEAN:
            	return to_string(this->bool_val);
            case TYPE_LIST:{
                string str = "";
                for(auto i : list_val){
                    str+=i.print()+",";
                }
                return str;}
            case TYPE_NIL:
            	return "nil";
            case TYPE_NULL:
            	return "NULL";
            case TYPE_BREAK:
            	return "BREAK";
            default:
                cout << "Unhandled print type" << this->type<< endl;
                exit(1);
        }
    }
    
    
};

inline ostream& operator<<(ostream& os, const Value& obj) {
    if(obj.isReturn()){
        os << "RETURN VAL ";
    }
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
        case Value::TYPE_FUNCTION:
        	os << "TYPE_FUNCTION " <<  obj.function_val;
        	break;
        case Value::TYPE_LIST:
        	os << "TYPE_LIST size " <<  obj.list_val.size();
        	break;
        case Value::TYPE_NULL:
        	os << "TYPE_NULL";
        	break;
    	default:
        	os << "Unknwown type " << obj.type;
        	break;
    }
    return os;
}

#endif  /* VALUE_H */