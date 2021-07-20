#ifndef DESTINATION_H
#define DESTINATION_H
#include "../login_info/login_info.h"
#include "../interface_profile_component/profile_component.h"
#include <vector>
/// Класът Destination описва всяка уникална дестинация посетена от потребителите в приложението
///
/// В член-променливите си класа има името на дестинацията, списък потребителите които са посещавали съответното място, както и списък с 
/// получените оценки и средноаритметичната оценка. Възможната стойност на оценка варира между 0 и 5, като тя е цяло число. След добавянето
/// на нов рейтинг, средната оценка се обновява. Във функционалността на класа освен добавяне на нов рейтинг са включени и нужните селектори,
/// както и функциите наследени от Component.
class Destination: public Component {

    std::string m_destination;
    std::vector<LoginInfo> m_visitors;
    std::vector<unsigned short int> m_grades;
    double m_average;

    void clear() override;
    void swap(Destination&);

    void save_string_member(std::ofstream&, const std::string&) const override;
    void load_string_member(std::ifstream&, std::string&) override;

    bool visitor_exists(const LoginInfo&) const;
    void update_avg();

public:
    Destination() = default;
    Destination(const std::string&);
    ~Destination();

    const std::string get_destination() const;
    const double get_average() const;

    void add_rating(const LoginInfo&, const unsigned short int&);

    void save_info(std::ofstream&) const override;
    void load_info(std::ifstream&) override; 
    void print() const override;

    bool operator==(const Destination&) const;
    bool operator!=(const Destination&) const;
    Destination& operator=(const Destination&);
    friend std::ostream& operator<<(std::ostream&, const Destination&);
};
std::ostream& operator<<(std::ostream&, const Destination&);
#endif