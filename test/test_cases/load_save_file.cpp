#include <iostream>
#include <fstream>


#include "../../src/equal.cpp"


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../external/doctest.h"

TEST_CASE("load, save, re-load and compare") {
    try {
        std::string path1 = "../test_data/redundant.json";
        std::string path2 = "../test_data/deleteme1.json";

        json a;
        json b;

        {
            std::ifstream ifs(path1);
            ifs >> a;
        }

        {
            std::ofstream ofs(path2);
            ofs << a;
        }

        {
            std::ifstream ifs(path2);
            ifs >> b;
        }

        bool equal = (a == b);
        REQUIRE_MESSAGE(equal == true,
                        "error: loaded/saved json do not match.");
    } catch (json_exception e) {
        REQUIRE_MESSAGE(false,
                        "ERROR unexpected json_exception thrown: " << e.msg);
    }
}

TEST_CASE("load two times") {
    try {
        std::string path1 = "../test_data/redundant.json";

        json a;
        json b;

        {
            std::ifstream ifs(path1);
            ifs >> a;
        }

        {
            std::ifstream ifs(path1);
            ifs >> a;
        }

        {
            std::ifstream ifs(path1);
            ifs >> b;
        }

        bool equal = (a == b);

        REQUIRE_MESSAGE(equal == true,
                        "error: loaded/saved json do not match.");
    } catch (json_exception e) {
        REQUIRE_MESSAGE(false,
                        "ERROR unexpected json_exception thrown: " << e.msg);
    }
}

TEST_CASE("format error 1") {
    std::string path = "../test_data/format_error1.json";
    json a;
    std::ifstream ifs(path);

    bool exception_thrown = false;

    try {
        ifs >> a;
    } catch (json_exception e) {
        // std::cout << e.msg << std::endl;
        exception_thrown = true;
    }

    REQUIRE_MESSAGE(
        exception_thrown == true,
        "error: file not in correct format but exception not thrown");
}

TEST_CASE("format error 2") {
    std::string path = "../test_data/format_error2.json";
    json a;
    std::ifstream ifs(path);

    bool exception_thrown = false;

    try {
        ifs >> a;
    } catch (json_exception e) {
        // std::cout << e.msg << std::endl;
        exception_thrown = true;
    }

    REQUIRE_MESSAGE(
        exception_thrown == true,
        "error: file not in correct format but exception not thrown");
}

TEST_CASE("format error 3") {
    std::string path = "../test_data/format_error3.json";
    json a;
    std::ifstream ifs(path);

    bool exception_thrown = false;

    try {
        ifs >> a;
    } catch (json_exception e) {
        // std::cout << e.msg << std::endl;
        exception_thrown = true;
    }

    REQUIRE_MESSAGE(
        exception_thrown == true,
        "error: file not in correct format but exception not thrown");
}

TEST_CASE("format error 4") {
    std::string path = "../test_data/format_error4.json";
    json a;
    std::ifstream ifs(path);

    bool exception_thrown = false;

    try {
        ifs >> a;
    } catch (json_exception e) {
        // std::cout << e.msg << std::endl;
        exception_thrown = true;
    }

    REQUIRE_MESSAGE(
        exception_thrown == true,
        "error: file not in correct format but exception not thrown");
}

TEST_CASE("format error 5") {
    std::string path = "../test_data/format_error5.json";
    json a;
    std::ifstream ifs(path);

    bool exception_thrown = false;

    try {
        ifs >> a;
    } catch (json_exception e) {
        // std::cout << e.msg << std::endl;
        exception_thrown = true;
    }

    REQUIRE_MESSAGE(
        exception_thrown == true,
        "error: file not in correct format but exception not thrown");
}

TEST_CASE("format error 6") {
    std::string path = "../test_data/format_error6.json";
    json a;
    std::ifstream ifs(path);

    bool exception_thrown = false;

    try {
        ifs >> a;
    } catch (json_exception e) {
        // std::cout << e.msg << std::endl;
        exception_thrown = true;
    }

    REQUIRE_MESSAGE(
        exception_thrown == true,
        "error: file not in correct format but exception not thrown");
}

TEST_CASE("format error 7") {
    std::string path = "../test_data/format_error7.json";
    json a;
    std::ifstream ifs(path);

    bool exception_thrown = false;

    try {
        ifs >> a;
    } catch (json_exception e) {
        // std::cout << e.msg << std::endl;
        exception_thrown = true;
    }

    REQUIRE_MESSAGE(
        exception_thrown == true,
        "error: file not in correct format but exception not thrown");
}
