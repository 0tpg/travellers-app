#ifndef LOGIN_INFO_H
#define LOGIN_INFO_H
#include "../interface_database_type/database_type.h"
#include "../interface_profile_component/profile_component.h"
#include <fstream>
/// LoginInfo е клас съдържащ информацията, която потребителите въвеждат при регистрация или вход във вече съществуващ профил.
///
/// Член-променливите са потребителското име, паролата и имейла на даден потребител. Тъй като класът няма динамично заделени елементи канонично представяне
/// не е имплементирано. Самата структура на класа е сравнително семпла - съдържа нужните селектори за отделните елементи, параметричен конструктор,
/// с който извършваме самата регистрация, локални за класа имплементации на наследените в интерфейсите функции, предефинирани оператори за сравнение, които се използват в
/// проверки, както и виртуален деструктор, тъй като този клас е базов за класа Profile. Информацията за индивидуалните потребители, пазена в базата данни от всички
/// регистрирани в приложението е от този тип.
class LoginInfo: public DataType, public Component {

    std::string m_username;
    std::string m_password;
    std::string m_email;

    void clear() override;
    void swap(LoginInfo&);

    void save_string_member(std::ofstream&, const std::string&) const override;
    void load_string_member(std::ifstream&, std::string&) override;

public:
    LoginInfo() = default;
    LoginInfo(const std::string&, const std::string&, const std::string&);
    virtual ~LoginInfo();

    const std::string get_username() const;
    const std::string get_password() const;
    const std::string get_email() const;

    void save(const std::string&) const override;
    void load(const std::string&) override;

    void save_info(std::ofstream&) const override;
    void load_info(std::ifstream&) override;
    void print() const override;

    bool operator==(const LoginInfo&) const;
    bool operator!=(const LoginInfo&) const;
    LoginInfo& operator=(const LoginInfo&);
    friend std::ostream& operator<<(std::ostream&, const LoginInfo&);
};
std::ostream& operator<<(std::ostream&, const LoginInfo&);
#endif