#include <catch.hpp>
#include <vector>
#include <string>

#include "CmdArgsParser.h"

struct TestArgs {
    std::vector<std::string> strings;
    std::vector<char*> pointers;

    TestArgs(std::initializer_list<const char*> args) {
        strings.reserve(args.size());
        pointers.reserve(args.size());
        for (const char* arg : args) {
            strings.emplace_back(arg);
            pointers.push_back(strings.back().data());
        }
    }

    int Argc() const {
        return static_cast<int>(pointers.size());
    }
    char** Argv() {
        return pointers.data();
    }
};

TEST_CASE("CmdArgsParser: no arguments") {
    TestArgs args({"program"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Error);
}

TEST_CASE("CmdArgsParser: two arguments") {
    TestArgs args({"program", "input.bmp"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Error);
}

TEST_CASE("CmdArgsParser: three arguments show help") {
    TestArgs args({"program", "input.bmp", "output.bmp"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::ShowHelp);
    REQUIRE(parser.GetInputFileName().empty());
    REQUIRE(parser.GetOutputFileName().empty());
    REQUIRE(parser.GetFilters().empty());
}

TEST_CASE("CmdArgsParser: four arguments one filter without params") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-gs"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Main);
    REQUIRE(parser.GetInputFileName() == "in.bmp");
    REQUIRE(parser.GetOutputFileName() == "out.bmp");
    REQUIRE(parser.GetFilters().size() == 1);
    REQUIRE(parser.GetFilters()[0].name == "gs");
    REQUIRE(parser.GetFilters()[0].params.empty());
}

TEST_CASE("CmdArgsParser: five arguments filter with one param") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-blur", "0.5"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Main);
    REQUIRE(parser.GetInputFileName() == "in.bmp");
    REQUIRE(parser.GetOutputFileName() == "out.bmp");
    REQUIRE(parser.GetFilters().size() == 1);
    REQUIRE(parser.GetFilters()[0].name == "blur");
    REQUIRE(parser.GetFilters()[0].params.size() == 1);
    REQUIRE(parser.GetFilters()[0].params[0] == "0.5");
}

TEST_CASE("CmdArgsParser: filter with multiple params") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-crop", "800", "600"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Main);
    REQUIRE(parser.GetFilters().size() == 1);
    REQUIRE(parser.GetFilters()[0].name == "crop");
    REQUIRE(parser.GetFilters()[0].params.size() == 2);
    REQUIRE(parser.GetFilters()[0].params[0] == "800");
    REQUIRE(parser.GetFilters()[0].params[1] == "600");
}

TEST_CASE("CmdArgsParser: multiple filters") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-gs", "-neg", "-sharp"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Main);
    REQUIRE(parser.GetFilters().size() == 3);
    REQUIRE(parser.GetFilters()[0].name == "gs");
    REQUIRE(parser.GetFilters()[1].name == "neg");
    REQUIRE(parser.GetFilters()[2].name == "sharp");
}

TEST_CASE("CmdArgsParser: filters with params mixed") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-crop", "800", "600", "-blur", "0.5", "-sharp"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Main);
    REQUIRE(parser.GetFilters().size() == 3);
    REQUIRE(parser.GetFilters()[0].name == "crop");
    REQUIRE(parser.GetFilters()[0].params.size() == 2);
    REQUIRE(parser.GetFilters()[0].params[0] == "800");
    REQUIRE(parser.GetFilters()[0].params[1] == "600");
    REQUIRE(parser.GetFilters()[1].name == "blur");
    REQUIRE(parser.GetFilters()[1].params.size() == 1);
    REQUIRE(parser.GetFilters()[1].params[0] == "0.5");
    REQUIRE(parser.GetFilters()[2].name == "sharp");
    REQUIRE(parser.GetFilters()[2].params.empty());
}

TEST_CASE("CmdArgsParser: negative number as param") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-blur", "-0.5"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Main);
    REQUIRE(parser.GetFilters().size() == 1);
    REQUIRE(parser.GetFilters()[0].name == "blur");
    REQUIRE(parser.GetFilters()[0].params.size() == 1);
    REQUIRE(parser.GetFilters()[0].params[0] == "-0.5");
}

TEST_CASE("CmdArgsParser: dash not followed by letter error") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Error);
}

TEST_CASE("CmdArgsParser: error on missing dash for filter") {
    TestArgs args({"program", "in.bmp", "out.bmp", "gs"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Error);
}

TEST_CASE("CmdArgsParser: error on empty filter name") {
    TestArgs args({"program", "in.bmp", "out.bmp", "-"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Error);
}

TEST_CASE("CmdArgsParser: error on space after dash") {
    TestArgs args({"program", "in.bmp", "out.bmp", "- ", "param"});
    CmdArgsParser parser(args.Argc(), args.Argv());
    REQUIRE(parser.GetMode() == CmdArgsParser::Mode::Error);
}