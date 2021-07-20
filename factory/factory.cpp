#include "factory.h"
#include <iostream>
#include <vector>
#include <ctime>

const std::string Factory::current_date_iso() {
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    
    std::string date{std::to_string(now->tm_year + 1900)};

    date.append("-");
    if((now->tm_mon + 1) < 10) {
        date.append("0");
    }
    date.append(std::to_string(now->tm_mon + 1));

    date.append("-");
    if((now->tm_mday) < 10) {
        date.append("0");
    }
    date.append(std::to_string(now->tm_mday));
    
    return date;
}

bool Factory::date_validation(const std::string& date) {
    bool is_valid{date >= "1970-01-01" &&
                  date <= current_date_iso() &&
                  date.substr(5, 2) <= "12" &&
                  date.substr(5, 2) != "00" &&
                  date.substr(8, 2) <= "31" &&
                  date.substr(8, 2) != "00"};

    if(is_valid) {
        if(date.substr(5, 2) == "02") {
            if(date.substr(8, 2) <= "29") {
                return true;

            } else {
                return false;
            }
        } else {
            return true;
        }
    }
    return false;
}

void Factory::string_input(const std::regex& reg, const std::string& message, std::string& str) {
    do {
        std::cout << message << " $ ";
        std::getline(std::cin, str);

        try {

            if(!std::regex_match(str, reg)) {
                throw std::runtime_error("invalid input: please try again $\n");
            }

        } catch (std::runtime_error error) {
            std::cout << error.what();
        }

    } while (!std::regex_match(str, reg));
}

void Factory::grade_input(unsigned short int& num) {
    do {
        std::cout << "GRADE $ ";
        std::cin >> num;

        try {

            if(num > 5) {
                throw std::runtime_error("invalid input: please try again $\n");
            }

        } catch (std::runtime_error error) {
            std::cout << error.what();
        }

    } while (num > 5);
    std::cin.ignore();
}

bool Factory::trip_has_photos() {
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
            trip_has_photos();
    }

    return false;
}

void Factory::transfer_photos(std::vector<std::string>& photos) {
    size_t photo_quantity{0};
    std::cout << "How many photos would you like to upload? $\n"
              << "\n$ ";

    std::cin >> photo_quantity;
    std::cin.ignore();

    std::cout << "\n";

    for(size_t i = 0; i < photo_quantity; i++) {
        photos.push_back(make_photo("upload photo"));
    }
}

LoginInfo Factory::make_user(const std::string &input) {
    const std::regex name_pass("\\w+");
    const std::regex mail("(\\w+)(\\.|_)?(\\w*)@(.*)(\\.(\\w+))+");

    std::string username, password, email;

    if (input == "register" || input == "login") {

        string_input(mail, "EMAIL", email);
        string_input(name_pass, "USERNAME", username);
        string_input(name_pass, "PASSWORD", password);

    } else if (input == "add friend" || input == "unfriend") {

        string_input(mail, "EMAIL", email);
        string_input(name_pass, "USERNAME", username);
        password = "";

    } else if (input == "check friend") {

        string_input(mail, "FRIEND'S EMAIL", email);
        string_input(name_pass, "FRIEND'S USERNAME", username);
        password = "";

    }

    std::cout << std::endl;

    LoginInfo new_element{username, password, email};
    return new_element;
}

Trip Factory::make_trip(const std::string &input) {
    const std::regex dest("[A-Z][a-z]+(\\,)(\\s)[A-Z][a-z]+");
    const std::regex date("[0-9]{4}-[0-9]{2}-[0-9]{2}");
    const std::regex comm("([A-Z][A-Za-z,;'-/\"\\s]+[.?!](\\s)?)*");

    std::string destination, start_date, end_date, comment;
    unsigned short int grade;
    std::vector<std::string> photos;
    bool has_photos{false};
    
    if (input == "add trip") {
        string_input(dest, "DESTINATION", destination);

        do {
            string_input(date, "START", start_date);

            try {

                if(!date_validation(start_date)) {
                    throw std::runtime_error("invalid input: please try again $\n");
                }

            } catch (std::runtime_error error) {
                std::cout << error.what();
            }
        } while(!date_validation(start_date));

        do {
            string_input(date, "END", end_date);
            try {

                if(end_date < start_date || !date_validation(end_date)) {
                    throw std::runtime_error("invalid input: please try again $\n");
                }

            } catch (std::runtime_error error) {
                std::cout << error.what();
            }
        } while(end_date < start_date || !date_validation(end_date));

        grade_input(grade);
        string_input(comm, "COMMENT", comment); 

        std::cout << "\nWould you like to upload photos to the trip? $\n"
                  << "Press y or n to continue $\n"
                  << "\n$ ";

        has_photos = trip_has_photos();

    } else if (input == "remove trip" || input == "edit trip") {
        string_input(dest, "DESTINATION", destination);
        
        do {
            string_input(date, "START", start_date);

            try {

                if(!date_validation(start_date)) {
                    throw std::runtime_error("invalid input: please try again $\n");
                }

            } catch (std::runtime_error error) {
                std::cout << error.what();
            }
        } while(!date_validation(start_date));

        do {
            string_input(date, "END", end_date);

            try {

                if(end_date < start_date || !date_validation(end_date)) {
                    throw std::runtime_error("invalid input: please try again $\n");
                }

            } catch (std::runtime_error error) {
                std::cout << error.what();
            }
        } while(end_date < start_date || !date_validation(end_date));

        grade = 0;
        comment = "";

    } else if (input == "check friend") {

        string_input(dest, "DESTINATION", destination);
        start_date = "";
        end_date = "";
        grade = 0;
        comment = "";

    }

    std::cout << std::endl;

    Trip new_element;

    if(has_photos) {
        transfer_photos(photos);
        new_element = Trip{destination, start_date, end_date, grade, comment, photos};    
    } else {
        new_element = Trip{destination, start_date, end_date, grade, comment};
    }

    return new_element;
}

Destination Factory::make_destination(const std::string& input) {
    const std::regex dest("[A-Z][a-z]+(\\,)(\\s)[A-Z][a-z]+");

    std::string destination;

    if(input == "check destination") {
        string_input(dest, "DESTINATION",destination);
    }

    std::cout << std::endl;

    Destination new_element{destination};
    return new_element;
}

std::string Factory::make_photo(const std::string &input) {
    const std::regex pic("[A-Za-z]+((\\_)*[A-Za-z]*)*(\\.)((jpeg)|(png))");

    std::string new_element;

    if (input == "upload photo" || input == "delete photo" || input == "rename photo" || std::regex_match(input, pic)) {
        string_input(pic, "FILENAME", new_element);
    }
    
    std::cout << std::endl;
    return new_element;
} 