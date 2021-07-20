#ifndef TRIP_INFO_H
#define TRIP_INFO_H
#include "../interface_profile_component/profile_component.h"
#include <vector>
/// Trip е клас съдържат в себе си за всяка една разходка, която потребителите документират в приложението.
///
/// Той съдържа цялата информация за разходката - дестинацията, периода от време, в който тя се е състояла, оценка, коментар и контейнер
/// със снимки, в който потребителя може да добавя, премахва и преименува снимки.
/// Съдържа 2 на брой параметрични конструктора, като единия позволява при създаване на нова разходка в системата към нея да се прибавят снимки
/// още при качването, а другия - по-късно. Имплементирани са нужните селектори, както и функциите, получени при наследяването на Component.
class Trip: public Component {

    std::string m_destination;
    std::string m_start_date;
    std::string m_end_date;
    unsigned short int m_grade;
    std::string m_comment;
    std::vector<std::string> m_photos; 

    void clear() override;
    void swap(Trip&);

    void save_string_member(std::ofstream&, const std::string&) const override;
    void load_string_member(std::ifstream&, std::string&) override;

    bool are_you_sure() const;

    bool photo_exists(const std::string&) const;
    void print_photos() const ;

public:
    Trip() = default;
    Trip(const std::string&, const std::string&, const std::string&, const unsigned short int&, const std::string&);
    Trip(const std::string&, const std::string&, const std::string&, const unsigned short int&, const std::string&, const std::vector<std::string>&);
    ~Trip();

    const std::string get_destination() const;
    const std::string get_start_date() const;
    const std::string get_end_date() const;
    const unsigned short int get_grade() const;
    const std::string get_comment() const;
    const std::vector<std::string> get_photos() const;

    void add_photo(const std::string&);
    void delete_photo(const std::string&);
    void rename_photo(const std::string&);

    void save_info(std::ofstream&) const override;
    void load_info(std::ifstream&) override;
    void print() const override;

    bool operator==(const Trip&) const;
    bool operator!=(const Trip&) const;
    Trip& operator=(const Trip&);
    friend std::ostream& operator<<(std::ostream&, const Trip&);
};
std::ostream& operator<<(std::ostream&, const Trip&);
#endif