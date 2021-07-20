#include "destination.h"
#include <iostream>
#include <cstring>

void Destination::clear() {
    m_average = 0;
    if(!m_grades.empty()) {
        m_grades.clear();
    }
    if(!m_visitors.empty()) {
        m_visitors.clear();
    }
    if(!m_destination.empty()) {
        m_destination.clear();
    }
}

void Destination::swap(Destination& other) {
    std::swap(m_destination, other.m_destination);
    std::swap(m_visitors, other.m_visitors);
    std::swap(m_grades, other.m_grades);
    std::swap(m_average, other.m_average);
}

void Destination::save_string_member(std::ofstream& out, const std::string& member) const {
    size_t current_size{member.size() + 1};
    char* transformed{new char[current_size]};
    strcpy(transformed, member.c_str());

    out.write((char*)&current_size, sizeof(size_t));
    out.write(transformed, current_size);
    delete[] transformed;
}

void Destination::load_string_member(std::ifstream& in, std::string& member) {
    size_t current_size;
    in.read((char*)&current_size, sizeof(size_t));
    char* transformed{new char[current_size]};
    in.read(transformed, current_size);

    member.assign(transformed);
    delete[] transformed;
}

bool Destination::visitor_exists(const LoginInfo& visitor) const {
    for(LoginInfo current : m_visitors) {
        if(current == visitor) {
            return true;
        }
    }

    return false;
}

void Destination::update_avg() {
    unsigned short int sum{0};

    for(unsigned short int& current : m_grades) {
        sum += current;
    }

    m_average =  sum/m_visitors.size();
}

Destination::Destination(const std::string& destination)
    : m_destination{destination}, m_average{0} {}

Destination::~Destination() {
    clear();
}

const std::string Destination::get_destination() const {
    return m_destination;
}

const double Destination::get_average() const {
    return m_average;
}

void Destination::add_rating(const LoginInfo& visitor, const unsigned short int& grade) {
    if(!visitor_exists(visitor)){
        m_visitors.push_back(visitor);
    } 

    m_grades.push_back(grade);

    update_avg();
    std::cout << "rating added successfully! $\n";
}

void Destination::save_info(std::ofstream& out) const {
    save_string_member(out, m_destination);

    size_t current_size{m_visitors.size()};
    out.write((char*)&current_size, sizeof(size_t));

    for(LoginInfo current : m_visitors) {
        current.save_info(out);
    }

    current_size = m_grades.size();
    out.write((char*)&current_size, sizeof(size_t));

    for(unsigned short int current : m_grades) {
        out.write((char*)&current, sizeof(unsigned short int));
    }

    out.write((char*)&m_average, sizeof(double));
}

void Destination::load_info(std::ifstream& in) {
    load_string_member(in, m_destination);

    size_t current_size{0};
    in.read((char*)&current_size, sizeof(size_t));

    for(size_t i = 0; i < current_size; i++) {
        LoginInfo temp;
        temp.load_info(in);
        m_visitors.push_back(temp);
    }

    in.read((char*)&current_size, sizeof(size_t));

    for(size_t i = 0; i < current_size; i++) {
        unsigned short int temp;
        in.read((char*)&temp, sizeof(unsigned short int));
        m_grades.push_back(temp);
    }

    in.read((char*)&m_average, sizeof(double));
}

void Destination::print() const {
    std::cout << m_destination << ": $\n\n";

    for(size_t i = 0; i < m_visitors.size(); i++) {
        std::cout << m_visitors[i] << "  Rating: " << m_grades[i] << "/5 $\n\n";
    }

    std::cout << "Avg rating: " << m_average << "/5 $\n";
}

bool Destination::operator==(const Destination& other) const {
    return m_destination == other.m_destination;
}

bool Destination::operator!=(const Destination& other) const {
    return m_destination != other.m_destination;
}

Destination& Destination::operator=(const Destination& other) {
    Destination temp(other);
    swap(temp);
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Destination& destination) {
    out << destination.m_destination << ": $\n\n";

    for(size_t i = 0; i < destination.m_visitors.size(); i++) {
        out << destination.m_visitors[i] << "  Rating: " << destination.m_grades[i] << "/5 $\n";
    }

    out << "\nAvg rating: " << destination.m_average << "/5 $\n";

    return out;
}