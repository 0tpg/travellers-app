#ifndef FACTORY_H
#define FACTORY_H
#include "../login_info/login_info.h"
#include "../trip_info/trip_info.h"
#include "../destination/destination.h"
#include <regex>
/// Factory класа позволява да създаваме нови потребители, разходки, дестинации и снимки
///
/// В съответствие на подадения от потребителя вход и с помощта на функциите в private, Factory създава съответен елемент само след пълна
/// валидация на получената от потребителя информация. Това става чрез регулярни изрази, които принуждават стила на написаното от потребителя
/// да е коректен спрямо условието и константен. Съществува допълнителна проверка на коректността на датите, както и опция да бъдат създавани разходки
/// със и без първоначални снимки - по желание на потребителя.
class Factory {

    static const std::string current_date_iso();
    static bool date_validation(const std::string&);

    static void string_input(const std::regex&, const std::string&, std::string&);
    static void grade_input(unsigned short int&);

    static bool trip_has_photos();
    static void transfer_photos(std::vector<std::string>&);

public:
    static LoginInfo make_user(const std::string &);
    static Trip make_trip(const std::string &);
    static Destination make_destination(const std::string&);
    static std::string make_photo(const std::string &);
};
#endif