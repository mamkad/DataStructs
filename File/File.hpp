#pragma once

#include "../String/String.hpp"

using DataStructs::String;

namespace DataStructs
{
    // класс для работы с файлами
    class File final
    {
    private:
        String fileName; // имя файла
        String fileData; // данные из файла
        bool isEmpty;    // флаг того, что былы загружены данные

    public:
        File();
        explicit File(String const& fileName);
        File(File const& otherFile);
        File(File&& otherFile);
        ~File();

    public:
        File& operator= (File const& otherFile);
        File& operator= (File&& otherFile) noexcept;

    public:
        bool empty() const noexcept; // пусть ли файл

    public:
        void load(String const& fileName); // загрузить файл
        void clear();                      // очистить содержимое файла

        String getData();                           // получить содержимое файла
        String const& getFileName() const noexcept; // получить имя файла
    };
}