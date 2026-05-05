
#ifndef CPP_HSE_FILTER_H
#define CPP_HSE_FILTER_H

#include "../Bmp.h"

class Filter {
public:
    Filter() = default;
    // Деструктор всегда генерируется компилятором для каждого класса,
    // и если базовый деструктор виртуальный,
    // деструкторы производных классов автоматически становятся виртуальными
    //(даже если они не объявлены явно).
    virtual ~Filter() = default;

    // Запрет копирование
    Filter(const Filter&) = delete;
    Filter& operator=(const Filter&) = delete;

    // Запрет перемещение
    Filter(Filter&&) = delete;
    Filter& operator=(Filter&&) = delete;

    virtual Bmp& Apply(Bmp& pic) const = 0;
};

#endif  // CPP_HSE_FILTER_H
