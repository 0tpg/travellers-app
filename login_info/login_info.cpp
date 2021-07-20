#include "login_info.h"
#include <iostream>
#include <cstring>

void LoginInfo::clear() {
    if(!m_email.empty()) {
        m_email.clear();
    }
    if(!m_password.empty()) {
        m_password.clear();
    }
    if(!m_email.empty()) {
        m_email.clear();
    }
}

void LoginInfo::swap(LoginInfo& other) {
    std::swap(m_username, other.m_username);
    std::swap(m_password, other.m_password);
    std::swap(m_email, other.m_email);
}

void LoginInfo::save_string_member(std::ofstream& out, const std::string& member) const {
    size_t current_size{member.size() + 1};
    char* transformed{new char[current_size]};
    strcpy(transformed, member.c_str());

    out.write((char*)&current_size, sizeof(size_t));
    out.write(transformed, current_size);
    delete[] transformed;
}

void LoginInfo::load_string_member(std::ifstream& in, std::string& member) {
    size_t current_size;
    in.read((char*)&current_size, sizeof(size_t));
    char* transformed{new char[current_size]};
    in.read(transformed, current_size);

    member.assign(transformed);
    delete[] transformed;
}

LoginInfo::LoginInfo(const std::string& username, const std::string& password, const std::string& email)
    : m_username{username}, m_password{password}, m_email{email} {}

LoginInfo::~LoginInfo() {
    clear();
}

const std::string LoginInfo::get_username() const  {
    return m_username;
}

const std::string LoginInfo::get_password() const  {
    return m_password;
}

const std::string LoginInfo::get_email() const  {
    return m_email;
}

void LoginInfo::save(const std::string& filepath) const {

    std::ofstream file;
    file.open(filepath.c_str(), std::ios::binary | std::ios::out);

    save_info(file);

    file.close();
}

void LoginInfo::load(const std::string& filepath) {
    clear();

    std::ifstream file;
    file.open(filepath.c_str(), std::ios::binary | std::ios::in);

    load_info(file);
    
    file.close();
}

void LoginInfo::save_info(std::ofstream& out) const {
    save_string_member(out, m_username);
    save_string_member(out, m_password);
    save_string_member(out, m_email);
}

void LoginInfo::load_info(std::ifstream& in) {
    load_string_member(in, m_username);
    load_string_member(in, m_password);
    load_string_member(in, m_email);
}

void LoginInfo::print() const  {
    std::cout << m_username << " : " << m_email << " $\n";
}

bool LoginInfo::operator==(const LoginInfo& other) const {
    return m_username == other.m_username && m_email == other.m_email;
}

bool LoginInfo::operator!=(const LoginInfo& other) const {
    return m_username != other.m_username && m_email != other.m_email;
}

LoginInfo& LoginInfo::operator=(const LoginInfo& other) {
    LoginInfo temp(other);
    swap(temp);
    return *this;
}

std::ostream& operator<<(std::ostream& out, const LoginInfo& user) {
    out << user.m_username << " : " << user.m_email << " $\n";
    return out;
}