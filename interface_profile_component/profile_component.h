#ifndef PROFILE_COMPONENT_H
#define PROFILE_COMPONENT_H
#include <fstream>
/// Component е интерфейс използван от LoginInfo, TripInfo, Profile и Destination.
///
/// В себе си той събира функционалностите, от които класовете се нуждаят за да сериализират информацията, която съдържат, както и някои други, които се срещат в отделните класове.
class Component {

    virtual void clear() = 0;
    virtual void save_string_member(std::ofstream&, const std::string&) const = 0;
    virtual void load_string_member(std::ifstream&, std::string&) = 0;

public:
    virtual ~Component() = default;

    virtual void save_info(std::ofstream&) const = 0;
    virtual void load_info(std::ifstream&) = 0; 
    virtual void print() const = 0;
};
#endif