#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;


#define OPERATOR_DEF(X) \
Value operator X (Value v)\
{\
    if(this->type == TYPE_INT && v.type == TYPE_INT)\
        return Value(int_val X v.int_val);\
    if(this->type == TYPE_DOUBLE && v.type == TYPE_INT)\
        return Value(double_val X v.int_val);\
    if(this->type == TYPE_INT && v.type == TYPE_DOUBLE)\
        return Value(int_val X v.double_val);\
    if(this->type == TYPE_DOUBLE && v.type == TYPE_DOUBLE)\
        return Value(double_val X v.double_val);\
    cout << "Type not handled in "<< #X <<" operator.";\
    exit(1);\
}

#define OPERATOR_EQ_DEF(X) \
Value operator X (Value v)\
{\
    if(this->type == TYPE_INT && v.type == TYPE_INT)\
        return Value(int_val X v.int_val);\
    if(this->type == TYPE_DOUBLE && v.type == TYPE_INT)\
        return Value(double_val X v.int_val);\
    if(this->type == TYPE_INT && v.type == TYPE_DOUBLE)\
        return Value(int_val X v.double_val);\
    if(this->type == TYPE_DOUBLE && v.type == TYPE_DOUBLE)\
        return Value(double_val X v.double_val);\
    if(this->type == TYPE_BOOLEAN && v.type == TYPE_BOOLEAN)\
        return Value(bool_val X v.bool_val);\
    cout << "Type not handled in "<< #X <<" operator.";\
    exit(1);\
}

class Node;
struct Value
{
    enum TYPE { TYPE_INT, TYPE_STRING, TYPE_DOUBLE, TYPE_BOOLEAN, TYPE_LIST, TYPE_FUNCTION, TYPE_NULL, TYPE_NIL, TYPE_BREAK, TYPE_ARRAY } type;

    int int_val;
    bool bool_val;
    string string_val;
    double double_val;
    vector<Value> list_val;
    Node* function_val;
    vector<int> array_val;
    
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
    
    static Value ARRAY(){
        Value v;
        v.type = TYPE_ARRAY;
        return v;
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
    bool isArray() const{
        return type == TYPE_ARRAY;
    }
    int arrayLength() const{
        return array_val.size();
    }
    
    bool isSingleElementList() const{
        return type == TYPE_LIST && list_val.size()==1;
    }

    Value negate(){
        if(type == TYPE_INT){
            return Value(-int_val);
        } else if(type == TYPE_DOUBLE){
            return Value(-double_val);
        } else if(type == TYPE_BOOLEAN){
            return Value(!bool_val);
        } else {
            cout << "Unhandled negate() for type "<< type << endl;
            exit(1);
        }
    }
    
    Value pow(Value v){
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value((int)std::pow(int_val, v.int_val));
        if(this->type == TYPE_DOUBLE && v.type == TYPE_INT)
            return Value(std::pow(double_val, v.int_val));
        if(this->type == TYPE_INT && v.type == TYPE_DOUBLE)
            return Value(std::pow(int_val, v.double_val));
        if(this->type == TYPE_DOUBLE && v.type == TYPE_DOUBLE)
            return Value(std::pow(double_val, v.double_val));
        cout << "Unhandled type in pow()" << endl;
        exit(1);
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
        	default:
                cout << "Unhandled isTrue() for type "<< type << endl;
                exit(1);
        }
    }
    OPERATOR_DEF(+)
    OPERATOR_DEF(*)
    OPERATOR_DEF(/)
    OPERATOR_DEF(-)
    OPERATOR_DEF(>)
    OPERATOR_DEF(<)
    OPERATOR_DEF(>=)
    OPERATOR_DEF(<=)
    OPERATOR_EQ_DEF(==)
    OPERATOR_EQ_DEF(!=)
    Value operator%(Value v)
    {
        if(this->type == TYPE_INT && v.type == TYPE_INT)
            return Value(int_val % v.int_val);
        cout << "Type not handled in % operator.";
        exit(1);
    }
    
    string print() const{
        switch(this->type){
            case TYPE_INT:
            	return to_string(this->int_val);
            case TYPE_STRING:
            	return this->string_val;
            case TYPE_DOUBLE:
            	return to_string(this->double_val);
            case TYPE_BOOLEAN:
            	return this->bool_val?"true":"false";
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
            case TYPE_ARRAY:{
                string s;
                for(auto i : array_val){
                    s += i + ", ";
                }
            	return s;
            }
            default:
                cout << "Unhandled print type" << this->type<< endl;
                exit(1);
        }
    }
    
    
};

inline ostream& operator<<(ostream& os, const Value& obj) {
    if(obj.isReturn()){
        os << "RETURN VALUE ";
    }
    switch(obj.type){
        case Value::TYPE_INT:
        	os << "TYPE_INT ";
        	break;
        case Value::TYPE_STRING:
        	os << "TYPE_STRING ";
        	break;
        case Value::TYPE_DOUBLE:
        	os << "TYPE_DOUBLE ";
        	break;
        case Value::TYPE_BOOLEAN:
        	os << "TYPE_BOOLEAN ";
        	break;
        case Value::TYPE_FUNCTION:
        	os << "TYPE_FUNCTION ";
        	break;
        case Value::TYPE_LIST:
        	os << "TYPE_LIST ";
        	break;
    	case Value::TYPE_ARRAY:
        	os << "TYPE_ARRAY ";
        	break;
        case Value::TYPE_NULL:
        	return os << "TYPE_NULL";
        case Value::TYPE_BREAK:
        	return os << "TYPE_BREAK";
    	default:
        	return os << "Unknwown type " << obj.type;
    }
    return os << obj.print();
}

#endif  /* VALUE_H */