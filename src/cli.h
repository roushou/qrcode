#pragma once

#include <stdexcept>
#include <string>

struct Cli {
    std::string text;
    std::string output;

    static Cli parse(int argc, char** argv) {
        Cli cli;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--output" || arg == "-o") {
                if (++i >= argc) {
                    throw std::runtime_error(arg + " requires a file path");
                }
                cli.output = argv[i];
            } else if (cli.text.empty()) {
                cli.text = arg;
            } else {
                throw std::runtime_error("unexpected argument: " + arg);
            }
        }
        if (cli.text.empty()) {
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <text-to-encode> [-o output.svg]");
        }
        return cli;
    }
};
