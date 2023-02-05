#pragma once
#include <vector>
#include "customer.h"
#include "location.h"
#include "contract.h"
#include "filehandling.h"

class BookingSystem {
public:
    BookingSystem();
    void printMainMenu();
    void createContract();
    void deleteContract();
    void contractList();
    void locationList();
    void adjustPrices();
    void editCustomers();
    void editLocations();
private:
    std::vector<Customer> m_customerList;
    std::vector<Location> m_locations;
    std::vector<Contract> m_contracts;
};
