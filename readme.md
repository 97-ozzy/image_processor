````md
# Image Processor

`image_processor` — консольное C++ приложение для обработки BMP-изображений.

Программа читает входной BMP-файл, последовательно применяет указанные фильтры и сохраняет результат в новый BMP-файл.

## Возможности

- чтение 24-битных BMP-файлов без сжатия;
- сохранение результата в BMP;
- последовательное применение нескольких фильтров;
- проверка некорректных аргументов;
- вывод справки при запуске без аргументов;
- модульная архитектура: BMP, фильтры, парсер аргументов, пайплайн обработки.

## Поддерживаемый формат

Поддерживаются BMP-файлы со следующими параметрами:

- 24-bit BMP;
- без сжатия;
- DIB header: `BITMAPINFOHEADER`.

## Запуск

Формат команды:

```bash
./image_processor <input_file> <output_file> [filters...]
````

Пример:

```bash
./image_processor input.bmp output.bmp -gs
```

Пример с несколькими фильтрами:

```bash
./image_processor input.bmp output.bmp -crop 800 600 -gs -sharp
```

## Реализованные фильтры

### `-crop width height`

Обрезает изображение до заданной ширины и высоты.

Параметры:

* `width` — новая ширина;
* `height` — новая высота.

```bash
./image_processor input.bmp output.bmp -crop 800 600
```

### `-gs`

Преобразует изображение в оттенки серого.

```bash
./image_processor input.bmp output.bmp -gs
```

### `-neg`

Преобразует изображение в негатив.

```bash
./image_processor input.bmp output.bmp -neg
```

### `-sharp`

Повышает резкость изображения.

```bash
./image_processor input.bmp output.bmp -sharp
```

### `-edge threshold`

Выделяет границы объектов на изображении.

Параметры:

* `threshold` — порог выделения границ от `0` до `1`.

```bash
./image_processor input.bmp output.bmp -edge 0.3
```

### `-blur sigma`

Применяет размытие изображения.

Параметры:

* `sigma` — сила размытия, неотрицательное значение.

```bash
./image_processor input.bmp output.bmp -blur 2.0
```

### `-fisheye strength`

Применяет эффект «рыбьего глаза».

Параметры:

* `strength` — сила искажения от `0` до `1`.

```bash
./image_processor input.bmp output.bmp -fisheye 0.8
```

## Пример с тестовым изображением

Тестовые изображения находятся в директории:

```text
test_script/data
```

Пример запуска:

```bash
./image_processor test_script/data/lenna.bmp test_script/data/lenna_fisheye.bmp -fisheye 0.8
```

Пример для Windows:

```bash
image_processor.exe Fullpath\image-processor\test_script\data\lenna.bmp Fullpath\image-processor\test_script\data\lenna_fisheye.bmp -fisheye 0.8
```

## Сборка

Требования:

* C++20;
* CMake 3.20 или выше.

```bash
git clone https://github.com/97-ozzy/image_processor.git
cd image_processor
mkdir build
cd build
cmake ..
cmake --build .
```

После сборки исполняемый файл будет находиться в директории `build`.

```
```
