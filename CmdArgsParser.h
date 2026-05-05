#ifndef CPP_HSE_CMDARGSPARSER_H
#define CPP_HSE_CMDARGSPARSER_H

#include <string>
#include <vector>
#include <string_view>

struct FilterDescriptor {
    std::string name;
    std::vector<std::string> params;
};

class CmdArgsParser {
public:
    static constexpr int MinArgsCount = 3;
    static constexpr int HelpArgsCount = 0;
    static constexpr int InputFileIndex = 1;
    static constexpr int OutputFileIndex = 2;

    // Определяет режим работы программы
    enum class Mode {
        Main,  // Режим открытия изображения с применением фильтров
        ShowHelp,
        Error
    };
    CmdArgsParser() : mode_(Mode::Main) {
    }
    explicit CmdArgsParser(int argc, char** argv);

    ~CmdArgsParser() = default;

    CmdArgsParser(CmdArgsParser&) = delete;
    bool operator=(const CmdArgsParser&) const = delete;

    CmdArgsParser(CmdArgsParser&&) = delete;
    bool operator=(const CmdArgsParser&&) const = delete;

    // Метод получает параметры командной строки, выполняет их актуальный разбор
    // записывает выявленный результат работы программы в поле mode_ и его
    // возвращает по значению
    Mode Parse(int argc, char** argv);

    // Возвращает выявленный режим работы приложения(По значению, экземпляр класса
    // не меняем, поэтому const)
    Mode GetMode() const {
        return mode_;
    }

    const std::string& GetInputFileName() const {
        return input_file_name_;
    }

    const std::string& GetOutputFileName() const {
        return output_file_name_;
    }

    const std::vector<FilterDescriptor>& GetFilters() const {
        return filters_;
    }

private:
    Mode mode_;  // Хранит режим работы
    std::string input_file_name_;
    std::string output_file_name_;
    std::vector<FilterDescriptor> filters_;
};

#endif  // CPP_HSE_CMDARGSPARSER_H
