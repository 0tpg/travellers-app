#include "profile.h"
#include "../factory/factory.h"
#include <iostream>

void Profile::clear() {
    if(!m_trips.empty()) {
        m_trips.clear();
    }
    if(!m_friends.empty()) {
        m_friends.clear();
    }
}

void Profile::swap(Profile& other) {
    std::swap(m_trips, other.m_trips);
    std::swap(m_friends, other.m_friends);
}

void Profile::save_string_member(std::ofstream& out, const std::string& member) const {
    size_t current_size{member.size() + 1};
    char* transformed{new char[current_size]};
    strcpy(transformed, member.c_str());

    out.write((char*)&current_size, sizeof(size_t));
    out.write(transformed, current_size);
    delete[] transformed;
}

void Profile::load_string_member(std::ifstream& in, std::string& member) {
    size_t current_size;
    in.read((char*)&current_size, sizeof(size_t));
    char* transformed{new char[current_size]};
    in.read(transformed, current_size);

    member.assign(transformed);
    delete[] transformed;
}

bool Profile::trip_exists(const Trip& trip) const {
    for(Trip current : m_trips) {
        if(current == trip) {
            return true;
        }
    }

    return false;
}

bool Profile::friend_exists(const LoginInfo& user) const {
    for(LoginInfo current : m_friends) {
        if(current == user) {
            return true;
        }
    }

    return false;
}

bool Profile::friend_has_visited(const Profile& user, const Trip& trip) const {
    for(Trip current : user.get_trips()) {
        if(current.get_destination() == trip.get_destination()) {
            return true;
        }
    }

    return false;
}

void Profile::friend_trip_print(const Profile& fr, const Trip& trip) const {
    for(Trip current : fr.get_trips()) {
        if(current.get_destination() == trip.get_destination()) {
            std::cout << current.get_destination() << ": $\n"
                      << "'" << current.get_comment() << "' $\n"
                      << " - ";
                      fr.LoginInfo::print();
            std::cout << "\n";
        }
    }
}

void Profile::print_trips() const  {
    for(Trip current : m_trips) {
        std::cout << current << "\n";
    }
}

void Profile::print_friends() const  {
    for(LoginInfo current : m_friends) {
        std::cout << current << "\n";
    }
}

void Profile::edit_delegation(const std::string& input, Trip& trip) {
    try {

        if(input == "upload photo") {
            trip.add_photo(Factory::make_photo(input));
        } else if(input == "delete photo") {
            trip.delete_photo(Factory::make_photo(input));
        } else if(input == "rename photo") {
            std::cout << "CURRENT ";
            trip.rename_photo(Factory::make_photo(input));
        } else if(input != "save changes") {
            throw std::runtime_error("invalid input: please enter a known command $\n");
        }

    } catch (std::runtime_error error) {
        std::cout << error.what();
    }
}

void Profile::edit(const std::string& input, const Trip& trip) {
    for(Trip& current : m_trips) {
        if(current == trip) {
            edit_delegation(input, current);

            return;
        }
    }
}

Profile::Profile(const LoginInfo& user)
    : LoginInfo{user} {}

Profile::Profile(const std::string& username, const std::string& password, const std::string& email)
    : LoginInfo{username, password, email} {}

Profile::~Profile() {
    clear();
}

void Profile::add_trip(const Trip& trip) {
    if(!trip_exists(trip)) {
        m_trips.push_back(trip);
        std::cout << "added trip successfully! $\n";
    } else {
        throw std::runtime_error("addition failed: trip already exists $\n");
    }
}

void Profile::remove_trip(const Trip& trip) {
    if(trip_exists(trip)) {
        for(size_t i = 0; i < m_trips.size(); i++) {
            if(m_trips[i] == trip) {
                m_trips.erase(m_trips.begin() + i);
                std::cout << "removed trip successfully! $\n";

                return;
            }
        }
    } else {
        throw std::runtime_error("removal failed: trip doesn't exist $\n");
    }
}

void Profile::edit_trip(const Trip& trip) {
    if(trip_exists(trip)) {
        std::cout << "Welcome to " << trip.get_destination() << " (" << trip.get_start_date() << " - " << trip.get_end_date() << ")'s settings. $\n"
                  << "To go back to your profile enter 'save changes'. $\n\n";

        std::string input;
        do
        {
            input.clear();
            std::cout << "Please enter the desired action! You can upload, rename and delete photos. $\n"
                      << "\n$ ";

            std::getline(std::cin, input);
            edit(input, trip);

            std::cout << std::endl;
        } while (input != "save changes");

    } else {
        throw std::runtime_error("edit trip failed: trip doesn't exist $\n");
    }
}

void Profile::add_friend(const LoginInfo& user) {
    if(!friend_exists(user)) {
        if(user != *this) {
            m_friends.push_back(user);
            std::cout << "added friend successfully! $\n";
        } else {
            throw std::runtime_error("addition failed: user already a friend $\n");
        }
    } else {
        throw std::runtime_error("addition failed: user already a friend $\n");
    }
    
}

void Profile::remove_friend(const LoginInfo& user) {
    if(friend_exists(user)) {
        for(size_t i = 0; i < m_friends.size(); i++) {
            if(m_friends[i] == user) {
                m_friends.erase(m_friends.begin() + i);
                std::cout << "removed friend successfully! $\n";

                return;
            }
        }
    } else {
        throw std::runtime_error("removal failed: user isn't a friend $\n");
    }
}

void Profile::friend_destination_visit(const LoginInfo& user, const Trip& trip) const {
    if(friend_exists(user)) {
        std::string filepath{".\\db_files\\personal_databases\\"};
        filepath.append(user.get_username());
        filepath.append(".db");

        Profile temp_friend;
        temp_friend.load(filepath);

        if(friend_has_visited(temp_friend, trip)) {

            friend_trip_print(temp_friend, trip);

        } else {
            throw std::runtime_error("check failed: user hasn't visited this destination $\n");
        }

    } else {
        throw std::runtime_error("check failed: user isn't a friend $\n");
    }
}

void Profile::save(const std::string& filepath) const {
    std::ofstream file;
    file.open(filepath.c_str(), std::ios::binary | std::ios::out);

    save_info(file);

    file.close();
}

void Profile::load(const std::string& filepath) {
    clear();

    std::ifstream file;
    file.open(filepath.c_str(), std::ios::binary | std::ios::in);

    load_info(file);

    file.close();
}

void Profile::save_info(std::ofstream& out) const {
    LoginInfo::save_info(out);

    size_t current_size{m_trips.size()};
    out.write((char*)&current_size, sizeof(size_t));
    
    for(Trip current : m_trips) {
        current.save_info(out);
    }

    current_size = m_friends.size();
    out.write((char*)&current_size, sizeof(size_t));

    for(LoginInfo current : m_friends) {
        current.save_info(out);
    }
}

void Profile::load_info(std::ifstream& in) {
    LoginInfo::load_info(in);

    size_t current_size{0};
    in.read((char*)&current_size, sizeof(size_t));

    for(size_t i = 0; i < current_size; i++) {
        Trip temp;
        temp.load_info(in);
        m_trips.push_back(temp);
    }

    in.read((char*)&current_size, sizeof(size_t));

    for(size_t i = 0; i < current_size; i++) {
        LoginInfo temp;
        temp.load_info(in);
        m_friends.push_back(temp);
    }
}

void Profile::print() const  {
    LoginInfo::print();
    std::cout << std::endl;
    print_trips();
    std::cout << std::endl;
    print_friends();
}

const std::vector<Trip> Profile::get_trips() const {
    return m_trips;
}

bool Profile::operator==(const Profile& other) const {
    return LoginInfo::operator==(other);
}

bool Profile::operator!=(const Profile& other) const {
    return LoginInfo::operator!=(other);
}

Profile& Profile::operator=(const Profile& other) {
    Profile temp(other);
    swap(temp);
    LoginInfo::operator=(other);
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Profile& profile) {
    out << profile.get_username() << " : " << profile.get_email() << " $\n";

    for(Trip current : profile.m_trips) {
        out << current << "\n";
    }

    for(LoginInfo current : profile.m_friends) {
        out << current << "\n";
    }

    return out;
}