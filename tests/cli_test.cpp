#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "cli.h"

TEST_CASE("parses text argument") {
    char* argv[] = {(char*)"qrcode", (char*)"hello"};
    auto cli = Cli::parse(2, argv);
    CHECK(cli.text == "hello");
    CHECK(cli.output.empty());
}

TEST_CASE("parses text with -o flag") {
    char* argv[] = {(char*)"qrcode", (char*)"hello", (char*)"-o", (char*)"out.svg"};
    auto cli = Cli::parse(4, argv);
    CHECK(cli.text == "hello");
    CHECK(cli.output == "out.svg");
}

TEST_CASE("parses text with --output flag") {
    char* argv[] = {(char*)"qrcode", (char*)"hello", (char*)"--output", (char*)"out.svg"};
    auto cli = Cli::parse(4, argv);
    CHECK(cli.text == "hello");
    CHECK(cli.output == "out.svg");
}

TEST_CASE("parses -o flag before text") {
    char* argv[] = {(char*)"qrcode", (char*)"-o", (char*)"out.svg", (char*)"hello"};
    auto cli = Cli::parse(4, argv);
    CHECK(cli.text == "hello");
    CHECK(cli.output == "out.svg");
}

TEST_CASE("throws on missing text") {
    char* argv[] = {(char*)"qrcode"};
    CHECK_THROWS_AS(Cli::parse(1, argv), std::runtime_error);
}

TEST_CASE("throws on -o without file path") {
    char* argv[] = {(char*)"qrcode", (char*)"hello", (char*)"-o"};
    CHECK_THROWS_AS(Cli::parse(3, argv), std::runtime_error);
}

TEST_CASE("throws on unexpected argument") {
    char* argv[] = {(char*)"qrcode", (char*)"hello", (char*)"extra"};
    CHECK_THROWS_AS(Cli::parse(3, argv), std::runtime_error);
}
