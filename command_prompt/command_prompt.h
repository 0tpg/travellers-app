#ifndef COMMAND_PROMPT_H
#define COMMAND_PROMPT_H
#include "../login_info/login_info.h"
#include "../trip_info/trip_info.h"
#include "../profile/profile.h"
#include "../factory/factory.h"
#include "../destination/destination.h"
#include <vector>
/// Prompt е класа унифициращ всички команди, които могат да бъдат изпълнени в приложението
///
/// От тук, спрямо входа в терминала се делегира цялата функционалност на програмата. При стартиране се зареждат списъците с познатите дестинации
/// и потребители, за да може при регистрация/добавяне на непозната дестинация новият потребител/дестинация да бъдат добавени към съответната
/// база данни, както и информацията за някой потребител да бъде достъпна и лесна за манипулация в случай, че той влезе в профила си подредством login
/// функцията. Тъй като тук е мястото, в което боравим с базата данни от дестинации, тук са добавени и допълнителни помощни функции, с които
/// можем да контролираме нейната коректност и изправност. Обектът е направен callable и е даден списък с инструкции за ползване при включване
/// на приложението. 
class Prompt {
    std::string m_input;
    std::vector<LoginInfo> m_users;
    std::vector<Destination> m_destinations;

    void clear();

    void user_input();
    void func_input(Profile&);

    void user_delegation();
    void func_delegation(Profile&);

    void export_destinations() const;
    void import_destinations();
    void update_destinations(Profile&, const Trip&);
    bool destination_exists(const Destination&) const;
    void check_destination(const Destination&) const;

    void export_users() const;
    void import_users();
    bool user_exists(const LoginInfo&) const;

    void registration_sequence(const std::string&, const LoginInfo&);
    void login_sequence(const std::string&, const LoginInfo&);

    void friend_sequence(Profile&);
    void trip_sequence(Profile&);

    void print_instructions() const;

public:
    Prompt() = default;
    ~Prompt();

    void operator()();
};
#endif