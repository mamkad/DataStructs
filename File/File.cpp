#include "File.hpp"

#include <fstream>
#include <iostream>
#include <exception>
#include <utility>
	
using std::ifstream;
using std::logic_error;
using std::cout;
using std::exception;
using std::move;
using std::exchange;

namespace DataStructs
{
    File::File() : fileName(""), fileData(""), isEmpty(true)
    {
        cout << "File()\n";
    }

    File::File(String const& fileName) try : isEmpty(true)
    {
        cout << "File(string const& fileName)\n";
        load(fileName);
    }
    catch(exception const& e)
    {
        throw;
    }

    File::File(File const& otherFile) : fileName(otherFile.fileName), fileData(otherFile.fileData), isEmpty(otherFile.isEmpty)
    {
        cout << "File(File const& otherFile)\n";
    }

    File::File(File&& otherFile) : fileName(move(otherFile.fileName)), fileData(move(otherFile.fileData)), isEmpty(exchange(otherFile.isEmpty, true))
    {
        cout << "File(File&& otherFile)\n";
    }

    File::~File()
    {
        clear();
    }
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    File& File::operator= (File const& otherFile)
    {   
        cout  << "File::operator= (File const& otherFile)\n";

        if (this != &otherFile)
        {
            clear();
            fileName = otherFile.fileName;
            fileData = otherFile.fileData;
            isEmpty = otherFile.isEmpty;
        }

        return *this;
    }

    File& File::operator= (File&& otherFile) noexcept
    {
        cout << "File::operator= (File&& otherFile)\n";

        if (this != &otherFile)
        {
            fileName = move(otherFile.fileName);
            fileData = move(otherFile.fileData);
            isEmpty  = exchange(otherFile.isEmpty, true);
        }

        return *this;
    }
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // пусть ли файл
    bool File::empty() const noexcept    
    {
        return isEmpty;
    }
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // загрузить файл
    void File::load(String const& filename)   
    {
        if (empty())
        {
            // открываем файл
            ifstream fileIn(filename.getPtr(), ifstream::binary);
            
            // проверяем, что удалось открыть
            if (fileIn)
            {
                // получаем длину содержимого файла
                fileIn.seekg(0, std::ios::end);
                size_t size = fileIn.tellg();

                // резервируем память достаточного размера
                char * tmpBuff = new char [size + 1];

                // записываем в буфер
                fileIn.seekg (0, std::ios::beg);
                fileIn.read(tmpBuff, size); 
                tmpBuff[size] = '\0';

                // преобразуем в string и чистим буфер
                fileData = tmpBuff;
                delete[] tmpBuff;
                tmpBuff = nullptr;

                isEmpty = false;        // ставим флаг
                fileName = filename;    // ставим имя

                fileIn.close();
            }
            else 
            {
                throw logic_error("file not found");
            }
        }
        else 
        {
            throw logic_error("file is not empty. clear before load new data");
        }

    }

    // очистить содержимое файла
    void File::clear()                        
    {
        // если есть данные
        if (!empty()) 
        {
            // чистим данные и ставим флаг
            fileData.clear();
            fileName.clear();
            isEmpty = true;
        }
    }

    // получить содержимое файла
    String File::getData()                        
    {
        if (empty())
        {
            throw logic_error("file not uploaded yet");
        }

        return fileData;
    }

    // получить имя файла
    String const& File::getFileName() const noexcept
    {
        return fileName;
    }
}