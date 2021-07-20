#ifndef PROFILE_H
#define PROFILE_H
#include "../login_info/login_info.h"
#include "../trip_info/trip_info.h"
#include <fstream>
#include <vector>
/// Profile е класът съдържащ цялата информация за съответен регистриран профил
///
/// От LoginInfo Profile получава името, паролата и имейла асоцииран с дадена профилна инстанция, а в своите член-данни той пази списък с
/// всички разходки и списък с всички приятели на потребителя. В private има редица помощни функции свързани с валидирането на всички нови данни,
/// които потребителя въвежда. В профила се извършват както промяната на списъка със снимките за конкретна разходка, така и проверката дали
/// приятел е посетил някоя дестинация, защото тук е мястото в което потребителят има достъп до тези данни. Профил може да бъде конструиран, както директно чрез
/// LoginInfo, така и поотделно с потребителско име, парола и имейл. Отново имаме както селектори така и функционалностите на Component и DataType,
/// наследени заедно с LoginInfo.
class Profile: public LoginInfo {

    std::vector<Trip> m_trips;
    std::vector<LoginInfo> m_friends;

    void clear();
    void swap(Profile&);

    void save_string_member(std::ofstream&, const std::string&) const override;
    void load_string_member(std::ifstream&, std::string&) override;

    bool trip_exists(const Trip&) const;
    bool friend_exists(const LoginInfo&) const;
    bool friend_has_visited(const Profile&, const Trip&) const;
    void friend_trip_print(const Profile&, const Trip&) const;
    
    void print_trips() const ;
    void print_friends() const ;

    void edit_delegation(const std::string&, Trip&);
    void edit(const std::string&, const Trip&);
    
public:
    Profile() = default;
    Profile(const LoginInfo&);
    Profile(const std::string&, const std::string&, const std::string&);
    ~Profile();

    void add_trip(const Trip&);
    void remove_trip(const Trip&);
    void edit_trip(const Trip&);

    void add_friend(const LoginInfo&);
    void remove_friend(const LoginInfo&);
    void friend_destination_visit(const LoginInfo&, const Trip&) const;

    void save(const std::string&) const override;
    void load(const std::string&) override;

    void save_info(std::ofstream&) const override;
    void load_info(std::ifstream&) override;
    void print() const override;

    const std::vector<Trip> get_trips() const;

    bool operator==(const Profile&) const;
    bool operator!=(const Profile&) const;
    Profile& operator=(const Profile&);
    friend std::ostream& operator<<(std::ostream&, const Profile&);
};
std::ostream& operator<<(std::ostream&, const Profile&);
#endif