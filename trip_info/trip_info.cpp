#include "trip_info.h"
#include "../factory/factory.h"
#include <iostream>
#include <cstring>

void Trip::clear() {
    if(!m_photos.empty()) {
        m_photos.clear();
    }
    if(!m_comment.empty()) {
        m_comment.clear();
    }
    m_grade = 0;
    if(!m_end_date.empty()) {
        m_end_date.clear();
    }
    if(!m_start_date.empty()) {
        m_start_date.clear();
    }
    if(!m_destination.empty()) {
        m_destination.clear();
    }
}

void Trip::swap(Trip& other) {
    std::swap(m_destination, other.m_destination);
    std::swap(m_start_date, other.m_start_date);
    std::swap(m_end_date, other.m_end_date);
    std::swap(m_grade, other.m_grade);
    std::swap(m_comment, other.m_comment);
    std::swap(m_photos, other.m_photos);
}

void Trip::save_string_member(std::ofstream& out, const std::string& member) const {
    size_t current_size{member.size() + 1};
    char* transformed{new char[current_size]};
    strcpy(transformed, member.c_str());

    out.write((char*)&current_size, sizeof(size_t));
    out.write(transformed, current_size);
    delete[] transformed;
}

void Trip::load_string_member(std::ifstream& in, std::string& member) {
    size_t current_size;
    in.read((char*)&current_size, sizeof(size_t));
    char* transformed{new char[current_size]};
    in.read(transformed, current_size);

    member.assign(transformed);
    delete[] transformed;
}

bool Trip::are_you_sure() const {
    char option;
    std::cin.get(option);
    std::cin.ignore();
    
    switch(option) {
        case 'y':
            return true;
        case 'n':
            return false;
        default: 
            std::cout << "\ninvalid input: please enter a known command $\n"
                      << "\n$ ";
            are_you_sure();
    }

    return false;
}

bool Trip::photo_exists(const std::string& photo) const {
    for(std::string current : m_photos) {
        if(current == photo) {
            return true;
        }
    }

    return false;
}

void Trip::print_photos() const  {
    for(std::string current : m_photos) {
        std::cout << current << " $\n";
    }
}

Trip::Trip(const std::string& destination, const std::string&start_date, const std::string& end_date, const unsigned short int& grade, const std::string& comment) 
    : m_destination{destination}, m_start_date{start_date}, m_end_date{end_date}, m_grade{grade}, m_comment{comment} {}

Trip::Trip(const std::string& destination, const std::string&start_date, const std::string& end_date, const unsigned short int& grade, const std::string& comment, const std::vector<std::string>& photos) 
    : m_destination{destination}, m_start_date{start_date}, m_end_date{end_date}, m_grade{grade}, m_comment{comment}, m_photos{photos} {}

Trip::~Trip() {
    clear();
}

const std::string Trip::get_destination() const  {
    return m_destination;
}

const std::string Trip::get_start_date() const  {
    return m_start_date;
}

const std::string Trip::get_end_date() const  {
    return m_end_date;
}

const unsigned short int Trip::get_grade() const  {
    return m_grade;
}

const std::string Trip::get_comment() const  {
    return m_comment;
}

const std::vector<std::string> Trip::get_photos() const  {
    return m_photos;
}

void Trip::add_photo(const std::string& photo) {
    if(!photo_exists(photo)) {

        m_photos.push_back(photo);
        std::cout << "photo uploaded successfully! $\n";

    } else {
        throw std::runtime_error("upload failed: photo with this name already exists $\n");
    }
}
void Trip::delete_photo(const std::string& photo) {
    if(photo_exists(photo)) {
        for(size_t i = 0; i < m_photos.size(); i++) {
            if(m_photos[i] == photo) {

                std::cout << "Are you sure you want to delete " << photo << "? $\n"
                          << "Press y or n to continue $\n"
                          << "\n$ ";

                if(are_you_sure()) {

                    m_photos.erase(m_photos.begin() + i);
                    std::cout << "\nphoto deleted successfully! $\n";

                } else {
                    throw std::runtime_error("\ndeletion canceled $\n");
                }

                return;
            }
        }
    } else {
        throw std::runtime_error("deletion failed: photo with this name doesn't exist $\n");
    }
}
void Trip::rename_photo(const std::string& photo) {
    if(photo_exists(photo)) {
        for(size_t i = 0; i < m_photos.size(); i++) {
            if(m_photos[i] == photo) {

                std::cout << "Are you sure you want to rename " << photo << "? $\n"
                          << "Press y or n to continue $\n"
                          << "\n$ ";

                if(are_you_sure()) {

                    std::cout << "\nNEW ";
                    m_photos[i] = Factory::make_photo(photo);
                    std::cout << "photo renamed successfully! $\n";

                } else {
                    throw std::runtime_error("\nrenaming canceled $\n");
                }

                return;
            }
        }
    } else {
        throw std::runtime_error("rename failed: photo with this name doesn't exist $\n");
    }
}

void Trip::save_info(std::ofstream& out) const {
    save_string_member(out, m_destination);
    save_string_member(out, m_start_date);
    save_string_member(out, m_end_date);

    out.write((char*)&m_grade, sizeof(unsigned short int));

    save_string_member(out, m_comment);

    size_t current_size{m_photos.size()};
    out.write((char*)&current_size, sizeof(size_t));

    for(std::string current : m_photos) {
        save_string_member(out, current);
    }
}

void Trip::load_info(std::ifstream& in) {
    load_string_member(in, m_destination);
    load_string_member(in, m_start_date);
    load_string_member(in, m_end_date);

    in.read((char*)&m_grade, sizeof(unsigned short int));

    load_string_member(in, m_comment);

    size_t current_size{0};
    in.read((char*)&current_size, sizeof(size_t));

    for(size_t i = 0; i < current_size; i++) {
        std::string temp;
        load_string_member(in, temp);
        m_photos.push_back(temp);
    }
}

void Trip::print() const  {
    std::cout << m_destination << ": $\n"
              << "("  << m_start_date << " - " << m_end_date << ") $\n"
              << m_grade << "/5" << " " << m_comment << " $\n";

    print_photos();
}

bool Trip::operator==(const Trip& other) const {
    return m_destination == other.m_destination && m_start_date == other.m_start_date && m_end_date == other.m_end_date;
}

bool Trip::operator!=(const Trip& other) const {
    return m_destination != other.m_destination && m_start_date != other.m_start_date && m_end_date != other.m_end_date;
}

Trip& Trip::operator=(const Trip& other) {
    Trip temp(other);
    swap(temp);
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Trip& trip) {
    out << trip.m_destination << ": $\n"
        << "("  << trip.m_start_date << " - " << trip.m_end_date << ") $\n"
        << trip.m_grade << "/5" << " " << trip.m_comment << " $\n";

    for(std::string current : trip.m_photos) {
        out << current << " $\n";
    }
    
    return out;
}