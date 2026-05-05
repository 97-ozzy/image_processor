#include <iostream>
#include <memory>

#include "CmdArgsParser.h"
#include "FilterFactory.h"
#include "Pipeline.h"
#include "Utils.h"

int RunPipeline(const CmdArgsParser& args) {
    FilterFactory factory;
    Bmp pic;
    pic.Load(args.GetInputFileName());
    auto pipeline = std::unique_ptr<Pipeline>(factory.CreatePipeline(args.GetFilters()));
    pipeline->Apply(pic);
    pic.Save(args.GetOutputFileName());
    return 0;
}

// TODO: реализовать для 124 header
int main(int argc, char** argv) {
    try {
        CmdArgsParser parsed(argc, argv);

        switch (parsed.GetMode()) {
            case CmdArgsParser::Mode::Error:
                std::cerr << "Error: Invalid filters format\n";
                return 1;
            case CmdArgsParser::Mode::ShowHelp:
                Utils::PrintHelp();
                return 0;
            default:
                return RunPipeline(parsed);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return 1;
    }
}
