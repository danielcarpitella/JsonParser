# Json Parser

This project provides a simple JSON parser implemented in C++ using the following files:

- `json.hpp`: Header file containing the `json` class definition and related functions.
- `json.cpp`: Source file containing the implementation of the `json` class and related functions.

## Description

The JSON parser is designed to parse and manipulate JSON data via a C++ container, allowing access (via iterators and operator overloading) of the content read from the json file. It provides a `json` class with various methods for handling different JSON data types, such as null, boolean, number, string, list, and dictionary.

## Features

`json.cpp` contains the implementation of the `json` class methods and operators defined in `json.hpp`. It includes functions for:

- Parsing JSON data from a JSON file.
- Outputting JSON data to a JSON file.
- Handling different JSON data types (null, boolean, number, string, list, and dictionary).

## Structure

The `json.cpp` file is organized as follows:

1. Implementation of the `json` class methods, such as `is_list`, `is_dictionary`, `is_string`, `is_number`, `is_bool`, and `is_null`.
2. Accessors and modifiers for JSON data, such as `get_number`, `get_bool`, `get_string`, `set_string`, `set_bool`, `set_number`, `set_null`, `set_list`, and `set_dictionary`.
3. Iterators for lists and dictionaries, such as `begin_list`, `end_list`, `begin_dictionary`, and `end_dictionary`.
4. Overloaded input and output stream operators for reading and writing JSON data.

## Parse

The `parse` function in `json.cpp` takes a string as input and returns a `json` object [1]. It checks the input string for different JSON data types (such as lists, dictionaries, strings, numbers, booleans, and null values) and sets the corresponding value in the `json` object. The function uses a series of conditional statements and loops to identify and process each JSON data type.

For example, when parsing a list, the function iterates through the input string and checks for opening and closing brackets, as well as nested lists and dictionaries. If the input string is not well-formed (e.g., a list is not closed or a string is not enclosed in double quotes), the function throws a `json_exception` with a specific error message.

## Exceptions for Debugging

The `json_exception` struct is defined in `json.hpp` and is used to throw exceptions when an error occurs during parsing or other operations. The `json_exception` struct contains a `msg` field, which is a string that stores the error message. This exception can be used to debug the code or the input files by providing more information about the error encountered.

For instance, when the `parse` function encounters an ill-formed list or dictionary, it throws a `json_exception` with a message indicating the specific issue, such as "La lista non è ben formata - Dizionario non chiuso - parse" or "La lista non è ben formata - Stringa non chiusa - parse". By catching and handling these exceptions in your code, you can identify and fix issues with the input JSON data or the parser implementation.



## Examples:


#### Example 1:

If we consider this json file that is sent as input:

    [
        1,
        {
            "first key" : 5,
            "second key" : [4.12,2,true],
            "third key" : "a string",
            "fourth key" : {"a" : 4, "b" : [4,5]}
        },
        3
    ]

then, the following code is valid and should print "4" on the screen:

    json j;
    std::cin >> j;
    json& y = *(++j.begin_list());
    std::cout << y["fourth key"]["a"];

#### Esample 2:

    json z;

If we consider this json object that contains the following data inside:

    {"c" : 5, "d" : 6}

and j is the variable of type json that we defined in the first example, then after this assignment:

    (*(++j.begin_list()))["first key"] = z

the variable j will contain the following data:

    [
        1,
        {
            "first key" : {"c" : 5, "d" : 6},
            "second key" : [4.12,2,true],
            "third key" : "a string",
            "fourth key" : {"a" : 4, "b" : [4,5]}
        },
        3
    ]


#### Dependencies

It is necessary to do

    git submodule update --init --recursive

before compiling a test to pull the `external/doctest` dependency.

#### Test data

The tests use the json files contained in the folder `test_data`.


