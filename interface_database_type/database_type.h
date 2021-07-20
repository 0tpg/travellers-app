#ifndef DATABASE_TYPE_H
#define DATABASE_TYPE_H
#include <string>
/// DataType е интерфейс, използван от видовете бази данни в приложението - тази пазеща всички регистрирани потребители, както и техните персонални.
class DataType {
public:
    virtual ~DataType() = default;

    virtual void save(const std::string&) const = 0;
    virtual void load(const std::string&) = 0; 
};
#endif