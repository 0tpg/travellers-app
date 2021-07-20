#include "command_prompt.h"
#include <iostream>
#include <fstream>

void Prompt::clear() {
    if(!m_input.empty()) {
        m_input.clear();
    }
}

void Prompt::user_input() {
    std::cout << "Welcome to 'Traveller's app'! $\n"
              << "To exit the application please enter 'quit'. $\n\n";

    do
    {
        clear();
        std::cout << "Please log into your account to continue or register if you still don't have one! $\n"
                  << "\n$ ";

        std::getline(std::cin, m_input);
        user_delegation();

        std::cout << std::endl;
    } while (m_input != "quit");
}

void Prompt::func_input(Profile& profile) {
    std::cout << "Welcome to your profile, " << profile.get_username() << "! $\n"
              << "To go back to the login menu please enter 'logout'. $\n\n";

    do
    {
        clear();
        std::cout << "Please enter the desired operation! Check the info panel for reference. $\n"
                  << "\n$ ";

        std::getline(std::cin, m_input);
        func_delegation(profile);

        std::cout << std::endl;
    } while (m_input != "logout");
}

void Prompt::user_delegation() {
    LoginInfo new_user{Factory::make_user(m_input)};

    std::string filepath{".\\db_files\\personal_databases\\"};
    filepath.append(new_user.get_username());
    filepath.append(".db");

    try{

        if(m_input == "register") {
            registration_sequence(filepath, new_user);
        } else if(m_input == "login") {
            login_sequence(filepath, new_user);
        } else if(m_input == "quit") {
            export_users();
            export_destinations();
        } else {
            throw std::runtime_error("invalid input: please enter a known command $\n");
        }

    } catch (std::runtime_error error) {
        std::cout << error.what();
    }
}       

void Prompt::func_delegation(Profile& profile) {
    std::string filepath{".\\db_files\\personal_databases\\"};
    filepath.append(profile.get_username());
    filepath.append(".db");
    
    try{

        if(m_input.find("friend") != std::string::npos) {
            friend_sequence(profile);
        } else if(m_input.find("trip") != std::string::npos) {
            trip_sequence(profile);
        } else if(m_input == "check destination") {
            check_destination(Factory::make_destination(m_input));
        } else if(m_input == "info") {
            profile.print();
        } else if(m_input != "logout") {
            throw std::runtime_error("invalid input: please enter a known command $\n");
        }

    } catch (std::runtime_error error) {
            std::cout << error.what();
    }

    profile.save(filepath);
}

void Prompt::export_destinations() const {
    std::ofstream destinations_db(".\\db_files\\destinations\\destinations.db", std::ios::binary | std::ios::out);

    size_t current_size{m_destinations.size()};
    destinations_db.write((char*)&current_size, sizeof(size_t));

    for(Destination current : m_destinations) {
        current.save_info(destinations_db);
    }

    destinations_db.close();
}

void Prompt::import_destinations() {
    if(!m_destinations.empty()) {
        m_destinations.clear();
    }

    std::ifstream destinations_db(".\\db_files\\destinations\\destinations.db", std::ios::binary | std::ios::in);

    size_t current_size{0};
    destinations_db.read((char*)&current_size, sizeof(size_t));

    for(size_t i = 0; i < current_size; i++) {
        Destination temp;
        temp.load_info(destinations_db);
        m_destinations.push_back(temp);
    }

    destinations_db.close();
}

bool Prompt::destination_exists(const Destination& destination) const {
    for(Destination current : m_destinations) {
        if(current == destination) {
            return true;
        }
    }

    return false;
}

void Prompt::check_destination(const Destination& destination) const {
    if(destination_exists(destination)) {
        for(Destination current : m_destinations) {
            if(current == destination) {
                current.print();
                return;
            }
        }
    } else {
        throw std::runtime_error("check failed: destination doesn't exist $\n");
    } 
}

void Prompt::export_users() const {
    std::ofstream users_db(".\\db_files\\users\\users.db", std::ios::binary | std::ios::out);

    size_t current_size{m_users.size()};
    users_db.write((char*)&current_size, sizeof(size_t));

    for(LoginInfo current : m_users) {
        current.save_info(users_db);
    }

    users_db.close();
}

void Prompt::import_users() {
    if(!m_users.empty()) {
        m_users.clear();
    }

    std::ifstream users_db(".\\db_files\\users\\users.db", std::ios::binary | std::ios::in);

    size_t current_size{0};
    users_db.read((char*)&current_size, sizeof(size_t));

    for(size_t i = 0; i < current_size; i++) {
        LoginInfo temp;
        temp.load_info(users_db);
        m_users.push_back(temp);
    }

    users_db.close();
}

void Prompt::update_destinations(Profile& profile, const Trip& trip) {
    Destination new_dest{trip.get_destination()};

    if(!destination_exists(new_dest)) {
        m_destinations.push_back(new_dest);
        m_destinations[m_destinations.size() - 1].add_rating(profile, trip.get_grade());

        std::cout << "destination added successfully! $\n";
    } else {

        for(Destination& current : m_destinations) {
            if(current == new_dest) {
                current.add_rating(profile, trip.get_grade());

                return;
            }
        }
    }
}

bool Prompt::user_exists(const LoginInfo& user) const {
    for(LoginInfo current : m_users) {
        if(current == user) {
            return true;
        }
    }

    return false;
}

void Prompt::registration_sequence(const std::string& name, const LoginInfo& user) {
    if(user_exists(user)) {
        throw std::runtime_error("registration failed: user already exists $\n");
    } else {
        Profile user_profile{user};
        user_profile.save(name);

        m_users.push_back(user);
        std::cout << "registration successful! $\n"; 
        return;                                      
    }
}

void Prompt::login_sequence(const std::string& name, const LoginInfo& user) {
    if(!user_exists(user)) {
        throw std::runtime_error("login failed: user doesn't exist $\n");
    } else {
        Profile logged_user{user};
        Profile middleman;
        middleman.load(name.c_str());

        if(logged_user == middleman && logged_user.get_password() == middleman.get_password()) {
            
            logged_user = middleman;
            std::cout << "login successful! $\n\n";
            func_input(logged_user);

            return;
        } else {
            throw std::runtime_error("login failed: invalid password or email $\n");
        }
    }
}

void Prompt::friend_sequence(Profile& profile) {
    if(m_input == "add friend") {
        LoginInfo friend_info{Factory::make_user(m_input)};

        if(user_exists(friend_info)) {
            profile.add_friend(friend_info);
        } else {
            throw std::runtime_error("add friend failed: user doesn't exist $\n");
        }

    } else if(m_input == "unfriend") {
        LoginInfo friend_info{Factory::make_user(m_input)};

        if(user_exists(friend_info)) {
            profile.remove_friend(friend_info);
        } else {
            throw std::runtime_error("remove friend failed: user doesn't exist $\n");
        }

    } else if(m_input == "check friend") {
        profile.friend_destination_visit(Factory::make_user(m_input), Factory::make_trip(m_input));
    }
}

void Prompt::trip_sequence(Profile& profile) {
    if(m_input == "add trip") {
        Trip new_trip{Factory::make_trip(m_input)};   
        profile.add_trip(new_trip);

        update_destinations(profile, new_trip);

    } else if(m_input == "remove trip") {
        profile.remove_trip(Factory::make_trip(m_input));
    } else if(m_input == "edit trip") {
        profile.edit_trip(Factory::make_trip(m_input));
    }
}

void Prompt::print_instructions() const {
    std::cout << "              Instructions\n"
              << "------------------------------------------\n"
              << "-                                        -\n"
              << "- register/login                         -\n"
              << "- <email>                                -\n"
              << "- <username>                             -\n"
              << "- <password>                             -\n"
              << "-                                        -\n"
              << "- once registered and/or logged into the -\n"
              << "- you can do one of the following actions-\n"
              << "-                                        -\n"
              << "- add friend/unfriend                    -\n"
              << "- <email>                                -\n"
              << "- <username>                             -\n"
              << "-                                        -\n"
              << "- add trip                               -\n"
              << "- <destination>                          -\n"
              << "- <starting date>                        -\n"
              << "- <ending date>                          -\n"
              << "- <grade>                                -\n"
              << "- <comment>                              -\n"
              << "-                                        -\n"
              << "- remove trip/edit trip                  -\n"
              << "- <destination>                          -\n"
              << "- <starting date>                        -\n"
              << "- <ending date>                          -\n"
              << "-                                        -\n"
              << "- check destination                      -\n"
              << "- <destination>                          -\n"
              << "-                                        -\n"
              << "- check friend                           -\n"
              << "- <destination>                          -\n"
              << "- <email>                                -\n"
              << "- <username>                             -\n"
              << "-                                        -\n"
              << "- info                                   -\n"
              << "-                                        -\n"
              << "- upon entering edit trip you will be    -\n"
              << "- able to do the following               -\n"
              << "-                                        -\n"
              << "- upload/delete/rename photo             -\n"
              << "- <file name>                            -\n"
              << "-                                        -\n"
              << "------------------------------------------\n\n";
}

Prompt::~Prompt() {
    if(!m_destinations.empty()) {
        m_destinations.clear();
    }
    if(!m_users.empty()) {
        m_users.clear();
    }
    if(!m_input.empty()) {
        m_input.clear();
    }
}

void Prompt::operator()() {
    import_users();
    import_destinations();
    print_instructions();
    user_input();
}