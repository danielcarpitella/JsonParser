
#include "../include/json.hpp"
#include <sstream>
#include <fstream>

using std::istream;
using std::ostream;
using std::pair; 

/*
*   Grammar:
*
*   json ->  [List] | {Dictionary} | "string" | number | bool | null
*
*   List ->  [] | json | List , List
*
*   Dictionary ->  {} | "string" : json | Dictionary , Dictionary
*/


enum Type{Null, Double, Bool, List, String, Dictionary};

struct list{
    json info;
    list *next;
};

struct dictionary{
    pair<std::string, json>data;
    dictionary *next;
};

typedef list* jlist;
typedef dictionary* jdictionary;

struct json::impl {
    impl(); 
    
    Type type;              

    std::string string; 
    double val;            
    bool bol;

    jdictionary dic_tail;
    jdictionary dic_head;
    
    jlist list_head; 
    jlist list_tail; 

    void destroy();  
};


/*=============  CONSTRUCTORS & DESTRUCTORS  ===============*/


// impl default costructor
json::impl::impl() : string(""),val(0), bol(false), list_head(nullptr),
                     list_tail(nullptr), dic_head(nullptr), dic_tail(nullptr), 
                     type(Null) {}

// json default costructor
json::json() : pimpl{ new impl{} }{}

//json destructor
json::~json() {

    if (pimpl) {
        pimpl->destroy();
        delete pimpl;
    }
}

// json copy constructor
json::json(json const& data){

    pimpl = new impl;
    pimpl->type = data.pimpl->type;
    pimpl->string = data.pimpl->string;
    pimpl->val = data.pimpl->val;
    pimpl->bol = data.pimpl->bol;
    
    pimpl -> list_head = nullptr; 
    pimpl -> list_tail = nullptr; 
    pimpl -> dic_head = nullptr;
    pimpl -> dic_tail = nullptr;  

    if(data.is_list()){    

        jlist cell = data.pimpl -> list_head;
        while (cell != nullptr){
            push_back(cell -> info);
            cell = cell -> next;   
        }

    }else if (data.is_dictionary()){

        jdictionary cell = data.pimpl -> dic_head;
        while (cell != nullptr){
            insert(cell -> data);
            cell = cell -> next;
        }     
    }
}

// json move costructor
json::json(json&& jsn) : pimpl(nullptr){ 

    *this = std::move(jsn); 
}

void json::impl::destroy(){

    if(dic_head != nullptr){
        jdictionary next = dic_head;
        while (dic_head != nullptr){
            next = dic_head -> next;
            delete dic_head;
            dic_head = next;
        }
        dic_head = nullptr;
        dic_tail = nullptr;
    }

    if(list_head != nullptr){
        jlist next = list_head;
        while (list_head != nullptr){
            next = list_head -> next;
            delete list_head;
            list_head = next;
        }
        list_head = nullptr;
        list_tail = nullptr;
    }

    type = Null;
    string = "";
    val = 0;
    bol = false;
}



/*================   OPERATOR =   ============*/

json& json::operator=(json const& value){

    if (this == &value) {
        return *this;
    }
        
    if (pimpl -> type != Null){
        pimpl -> destroy();
        delete pimpl;  
        pimpl = nullptr;
    }


    switch (value.pimpl->type){

        case Null : {
            set_null();
            break;
        }
        case Bool : {
            set_bool(value.pimpl -> bol);
            break;
        }
        case String : {
            set_string(value.pimpl -> string);
            break;
        }
        case Double : {
            set_number(value.pimpl -> val);
            break;
        }
        case List : {

            set_list();
            jlist cell = value.pimpl -> list_head;
            while (cell != nullptr){
                push_back(cell -> info);
                cell = cell -> next;   
            }

            break;
        }
        case Dictionary : {

            set_dictionary();
            jdictionary cell = value.pimpl -> dic_head;
            while (cell != nullptr){
                insert(cell -> data);
                cell = cell -> next;
            }
            
            break;
        }
        default:
            throw json_exception {"error copy costructor"};
    }
       
    return *this;
}

//move assignment
json& json::operator=(json&& rhs) {

    if (this != &rhs) {
        if (pimpl != nullptr) {
            pimpl->destroy();
            delete pimpl;
            pimpl = nullptr;
        }
        pimpl = rhs.pimpl;
        rhs.pimpl = nullptr;
    }
    return *this;
}



/*===============  BOOL METHODS  =================*/

bool json::is_list() const{
    return pimpl -> type == List;
} 

bool json::is_dictionary() const{
    return pimpl -> type == Dictionary;
}    

bool json::is_string() const{
    return pimpl -> type == String;
}

bool json::is_number() const{
    return pimpl -> type == Double;
}

bool json::is_bool() const{
    return pimpl -> type == Bool;
}

bool json::is_null() const{
    return pimpl -> type == Null;
}



/*================  SET FUNCTIONS   ==============*/

void json::set_string(std::string const &s){

    if (pimpl != nullptr){ 
        pimpl->destroy();
    }else{
        pimpl = new impl;   
    }
    pimpl->type = String;
    pimpl->string = s;
}

void json::set_bool(bool b){

    if (pimpl != nullptr){ 
        pimpl->destroy();
    }else{
        pimpl = new impl;   
    }
    pimpl -> type = Bool;
    pimpl -> bol = b;  
}

void json::set_number(double number){

    if (pimpl != nullptr){ 
        pimpl->destroy();
    }else{
        pimpl = new impl;   
    }
    pimpl -> type = Double;
    pimpl -> val = number;  
}

void json::set_list(){

    if (pimpl != nullptr){
        pimpl->destroy();
    }else{
        pimpl = new impl;
    }
    pimpl -> list_head = nullptr;
    pimpl -> list_tail = nullptr;
    pimpl -> type = List;
}

void json::set_dictionary(){

    if (pimpl != nullptr){  
        pimpl->destroy();
    }else{
        pimpl = new impl;   
    }
    pimpl -> type = Dictionary;
    pimpl -> dic_head = nullptr;
    pimpl -> dic_tail = nullptr;
}

void json::set_null(){

    if (pimpl != nullptr){ 
        pimpl->destroy();
    }else{
        pimpl = new impl;   
    }
    pimpl -> type = Null;
}



/*=================  PUSH & INSERT   ===================*/

void json::push_back(json const& value){

    if (!is_list()){
        throw json_exception {"error calling function push_back(): element is not a list"};
    }
      
    jlist newCell = new list {value, nullptr};

    if (pimpl -> list_head == nullptr){
        pimpl -> list_head = newCell;
    }else{
        pimpl -> list_tail -> next = newCell;
    }
    pimpl -> list_tail = newCell;
}


void json::insert(pair<std::string, json> const& kayValue){

    if(!is_dictionary()){
        throw json_exception {"error calling function insert(): element is not a dictionary"};
    }
    
    jdictionary newCell = new dictionary {kayValue, nullptr};
   
    if (pimpl -> dic_head == nullptr){
        pimpl -> dic_head = newCell;
    }else{
        pimpl -> dic_tail -> next = newCell;
    }
    pimpl -> dic_tail = newCell;
}


void json::push_front(json const& value){

    if (!is_list()){
        throw json_exception {"error calling function push_front(): element is not a list"};
    }

    jlist newCell = new list {value, nullptr};

    if (pimpl -> list_head == nullptr){
        pimpl -> list_head = newCell;
        pimpl -> list_tail = newCell;
    }else{
        newCell -> next = pimpl -> list_head;
        pimpl -> list_head = newCell;
    }
}



/*====================  GET FUNCTIONS  ==================*/

double& json::get_number() {

    if(is_number()) {
        return pimpl -> val;
    } else {
        throw json_exception {"error - should be a number"};
    }
}

double const& json::get_number() const {

    if(is_number()) {
        return pimpl -> val;
    } else {
        throw json_exception {"error - should be a number (const)"};
    }
}

bool& json::get_bool() {

    if(is_bool()) {
        return pimpl -> bol;
    } else {
        throw json_exception {"error - should be a bool (const)"};
    }
}

bool const& json::get_bool() const {

    if(is_bool()) {
        return pimpl -> bol;
    } else {
        throw json_exception {"error - should be a bool (const)"};
    }
}

std::string& json::get_string() {

    if(is_string()) {
        return pimpl->string;
    } else {
        throw json_exception {"error - should be a string"};
    }
}

std::string const& json::get_string() const {

    if(is_string()) {
        return pimpl->string;
    } else {
        throw json_exception {"error - should be a string (const)"};
    }
}



/*==================  LIST ITERATORS  ====================*/

struct json::list_iterator 
{
    list_iterator(jlist node) : current(node) {};

    list_iterator& operator++() {
        if (current){
            current = current->next;
            return *this;
        }else{
            throw json_exception {"error: list iterator out of bounds"};  
        }
    }

    list_iterator operator++(int) {
        list_iterator temp = *this;
        ++(*this);
        return temp;
    }

    json& operator*() const {
        return current->info;
    }

    json* operator->() const {
        return &(current->info);
    }

    bool operator==(json::list_iterator const& other) const {  
        return current == other.current;
    }

    bool operator!=(list_iterator const& other) const {  
        return current != other.current;
    }

    private:
     list* current;    
};


struct json::const_list_iterator 
{ 
    const_list_iterator(jlist node) : current(node) {}

    const_list_iterator& operator++() {
           if (current){
            current = current->next;
            return *this;
        }else{
            throw json_exception {"error: const_list iterator out of bounds"};  
        }
    }

    const_list_iterator operator++(int) {
        const_list_iterator temp = *this;
        ++(*this);
        return temp;
    }

    json const& operator*() const {
        return current->info;
    }

    json const* operator->() const {
        return &(current->info);
    }

    bool operator==(const_list_iterator const& other) const {
        return current == other.current;
    }

    bool operator!=(const_list_iterator const& other) const {
        return current != other.current;
    }

    private:
        jlist current;
};



/*===================  DICTIONARY ITERATORS  ===================*/

struct json::dictionary_iterator 
{
    dictionary_iterator(jdictionary node) : current(node) {}

    dictionary_iterator& operator++() {
    if (current){
            current = current->next;
            return *this;
        }else{
            throw json_exception {"error: dictionary iterator out of bounds"};  
        }
    }

    dictionary_iterator operator++(int) {
        dictionary_iterator temp = *this;
        ++(*this);
        return temp;
    }

    pair< std::string, json>& operator*() const {  
        return current->data;
    }

    pair< std::string, json>* operator->() const {   
        return &(current->data);
    }

    bool operator==(dictionary_iterator const& other) const {
        return current == other.current;
    }

    bool operator!=(dictionary_iterator const& other) const {
        return current != other.current;
    }
    private:
        jdictionary current;
};


struct json::const_dictionary_iterator 
{
    const_dictionary_iterator(jdictionary node) : current(node) {}

    const_dictionary_iterator& operator++() {
        if (current){
            current = current->next;
            return *this;
        }else{
            throw json_exception {"error: const_dictionary iterator out of bounds"};  
        }
    }

    const_dictionary_iterator operator++(int) {
        const_dictionary_iterator temp = *this;
        ++(*this);
        return temp;
    }

    pair< std::string, json> const& operator*() const {
        return current->data;
    }

    pair< std::string, json> const* operator->() const {    
        return &(current->data);
    }

    bool operator==(const_dictionary_iterator const& other) const {
        return current == other.current;
    }

    bool operator!=(const_dictionary_iterator const& other) const {
        return current != other.current;
    }

    private:
        jdictionary current;
};



/*=================   ITERATORS BEGIN & END METHODS   ==================*/

json::list_iterator json::begin_list() {
    if (is_list())
        return list_iterator{pimpl->list_head};
    else 
        throw json_exception {"error - should be a list (begin)"};
}

json::const_list_iterator json::begin_list() const {
    if (is_list())
       return const_list_iterator{pimpl->list_head};
    else
      throw json_exception {"error - should be a list (begin const)"};
}


json::list_iterator json::end_list() {   
    if (is_list())
       return list_iterator{nullptr}; 
    else 
       throw json_exception {"error - should be a list (end)"};   
}

json::const_list_iterator json::end_list() const {
    if (is_list())
        return const_list_iterator{nullptr};
    else 
        throw json_exception {"error - should be a list end(const)"};
}


/*===========================================================================*/


json::dictionary_iterator json::begin_dictionary() {
    if(is_dictionary())
       return dictionary_iterator{pimpl->dic_head};
    else 
        throw json_exception {"error - should be a dictionary (begin)"};
}

json::const_dictionary_iterator json::begin_dictionary() const {
    if(is_dictionary())
       return const_dictionary_iterator{pimpl->dic_head};
    else 
        throw json_exception {"error - should be a dictionary(beging const)"};
}

json::dictionary_iterator json::end_dictionary() {
    if(is_dictionary())
        return dictionary_iterator{nullptr};  
    else 
        throw json_exception {"error - should be a dictionary (end)"};
}

json::const_dictionary_iterator json::end_dictionary() const {
    if(is_dictionary())
       return const_dictionary_iterator{nullptr};
    else 
        throw json_exception {"error - should be a dictionary (end const)"};
}



/*========================  OPERTAOR []  ===========================*/

json& json::operator[]( std::string const& key) {

    if(!is_dictionary()){
        throw json_exception {"error [] is not a dictionary"};
    }
    
    for (auto it = this->begin_dictionary(); it != this->end_dictionary(); ++it) {
        if(it->first == key){
            return it->second;
        }
    }

    // If the key is not found, it creates a new json and inserts it
    json defaultValue; 
    this->insert(std::make_pair(key, defaultValue));

    // calls the opertaore so that it can return the reference
    return this->operator[](key);         
}

json const& json::operator[]( std::string const& key) const { 

    if(!is_dictionary()){
        throw json_exception {"error [] - is not a dictionary (const)"};
    }

    for (auto it = this->begin_dictionary(); it != this->end_dictionary(); ++it) {
        if(it->first == key){
            return it->second;
        }
    }

    throw json_exception {"error: Key not found in dictionary"};
}



/*=====================  PARSING FUNCTIONS   ======================*/

/*
*   Grammar:
*
*   json -> [List] | {Dictionary} | "string" | number | bool | null
*
*   List -> [] | json | List , List
*
*   Dictionary -> {} | "string" : json | Dictionary , Dictionary
*/

void read_list_node (istream& lhs, json& rhs);
void read_dic_node (istream &lhs, json &rhs);
void read_string (istream& lhs, json& rhs);
void read_null (istream& lhs, json& rhs);
void read_bool (istream& lhs, json& rhs);
void read_double (istream& lhs, json& rhs);
void read_list (istream& lhs, json& rhs);

void read_list(istream& lhs, json& rhs){
    
    char ch;
    lhs >> ch;
    if(ch == ']')
      return;
    else
      lhs.putback(ch);

    read_list_node(lhs, rhs);
}

void read_list_node(istream &lhs, json &rhs){

    char ch;
    json node;

    lhs >> node;
    rhs.push_back(node);
    lhs >> ch;  

    if (ch == ',')
        read_list_node(lhs, rhs);
    else if (ch == ']')
        return;
    else
        throw json_exception {"error reading list"};
    
}

void read_dic_value(istream &lhs, json &rhs, std::string key){

    json node;
    lhs >> node;
 
    rhs.insert(std::make_pair(key, node));  
}

void read_dic_node(istream &lhs, json &rhs){

    char ch;

    lhs >> ch; 
    if(ch == '}')
      return;

    if (ch != '"'){ 
       throw json_exception {"error reading key of dictionary"};
    }

    std::string key;
    char c;
    bool escape = false;

    while (lhs.get(c) && (c != '"' || escape)) {
            if (!escape && c == '\\' ) {
                escape = true;
                key.push_back(c);
            } else if(!escape){
                key.push_back(c);
            }else if (escape){
                if ( c != '\\')
                escape = false;
                key.push_back(c);
            }
        }

    lhs >> ch; 

    if (ch != ':'){
       throw json_exception {"error reading : of dictionary"};
    }

    read_dic_value(lhs, rhs, key);

    lhs >> ch;

    if (ch == ',')
       read_dic_node(lhs, rhs);
    else if (ch == '}')
       return;
    else
        throw json_exception {"error reading } of dictionary"};
    
}

void read_string(istream& lhs, json& rhs){

    std::string stringa;
    char c;
    bool escape = false;

     while (lhs.get(c) && (c != '"' || escape)) {
        if (!escape && c == '\\' ) {
            escape = true;
            stringa.push_back(c);
        } else if(!escape){
            stringa.push_back(c);
        }else if (escape){
            if ( c != '\\')
               escape = false;
            stringa.push_back(c);
        }
    }
    rhs.set_string(stringa);
}


void read_double(istream& lhs, json& rhs){

    double number {};
    lhs >> number;
    rhs.set_number(number);
}


void read_bool(istream &lhs, json &rhs){

    char ch;
    lhs >> ch;
    if (ch == 't'){             // case where there is true
        lhs.putback(ch);
        char buffer[5];
        lhs.read(buffer, 4);
        buffer[4] = '\0';
        std::string str(buffer);

        if (str == "true")
            rhs.set_bool(true);
        else
            throw json_exception {"error reding bool (true)"};
    }else{
        lhs.putback(ch);
        char buffer[6];
        lhs.read(buffer, 5);
        buffer[5] = '\0';
        std::string str(buffer);

        if (str == "false")
            rhs.set_bool(false);
        else
            throw json_exception {"error reding bool (false)"};
    }
}

void read_null(istream& lhs, json& rhs){
    
    char buffer[5];
    lhs.read(buffer, 4);
    buffer[4] = '\0';  
    std::string str(buffer);

    if(str == "null")
       rhs.set_null();
    else
       throw json_exception {"error reding null"};
}


istream& operator>>(istream& lhs, json& rhs){

    char ch;
    lhs >> ch;

    if (lhs.eof())
       return lhs;
 
    switch (ch){

        case '[' :{
            rhs.set_list();
            read_list(lhs, rhs);
            break;
        }
        case '{' :{
            rhs.set_dictionary();
            read_dic_node(lhs, rhs);
            break;
        }
        case '"' :{
            read_string(lhs, rhs); 
            break;
        }
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            lhs.putback(ch);
            read_double(lhs, rhs); 
            break;
        }
        case 'n' :{
            lhs.putback(ch);
            read_null(lhs, rhs);
            break;
        }
        case 't': case 'f' :{
            lhs.putback(ch);
            read_bool(lhs, rhs);
            break;
        }
        default:
            throw json_exception {"error reading json"};

    }

   return lhs;
}



/*=====================  PRINT FUNCTION  =====================*/

ostream& operator<<(ostream& lhs, json const& rhs) {

    if (rhs.is_bool()) {
        lhs << std::boolalpha << rhs.get_bool();
    } 
    else if (rhs.is_number()) {
        lhs << rhs.get_number();
    } 
    else if (rhs.is_string()) {
        lhs << '"' << rhs.get_string() << '"';
    } 
    else if(rhs.is_null()){
        lhs <<"null";
    } 
    else if (rhs.is_list()) {
        lhs << '[';

        for (auto it = rhs.begin_list(); it != rhs.end_list(); ) {
            lhs << *it;
            if (++it != rhs.end_list()) {
                lhs << ",";
            }
        }
        lhs << ']';
    } 
    else if (rhs.is_dictionary()) {
        lhs << '{';

        for (auto it = rhs.begin_dictionary(); it != rhs.end_dictionary(); ) {
            lhs << '"' << it->first << "\" : " << it->second;
            if (++it != rhs.end_dictionary()) {
                lhs << ", ";
            }
        }
        lhs << '}';
    }else
       throw json_exception {"error printing json"};

    return lhs;
}

