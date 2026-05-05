//
// Created by Huawei on 10-Mar-26.
//

#include "CmdArgsParser.h"
#include <iostream>
#include <string_view>
#include <stdexcept>

CmdArgsParser::CmdArgsParser(int argc, char** argv) {
    Parse(argc, argv);
}

CmdArgsParser::Mode CmdArgsParser::Parse(int argc, char** argv) {
    if (argc == MinArgsCount) {
        mode_ = Mode::ShowHelp;
        return mode_;
    }

    if (argc < MinArgsCount) {
        mode_ = Mode::Error;
        return mode_;
    }

    mode_ = Mode::Error;

    input_file_name_ = argv[InputFileIndex];
    output_file_name_ = argv[OutputFileIndex];

    int i = MinArgsCount;
    while (i < argc) {
        if (argv[i][0] != '-' || argv[i][1] == '\0' || std::isspace(argv[i][1])) {
            mode_ = Mode::Error;
            return mode_;
        }

        FilterDescriptor filter;
        filter.name = &argv[i][1];

        i++;

        while (i < argc &&
               (argv[i][0] != '-' || (argv[i][0] == '-' && (std::isdigit(argv[i][1]) || argv[i][1] == '.')))) {
            filter.params.push_back(argv[i]);
            i++;
        }
        filters_.push_back(filter);
    }

    mode_ = Mode::Main;
    return mode_;
}
