#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <json/json.h>
#include "bookingsystem.h"

BookingSystem::BookingSystem() {
    FileHandling::retrieveCustomerData(m_customerList);
    FileHandling::retrieveLocationData(m_locations);
    FileHandling::retrieveContractData(m_contracts);

    printMainMenu();
}

void BookingSystem::printMainMenu() {
    int chooseOption = -1;
    while (chooseOption != 9) {
        system("cls");
        std::cout << "   -Booking System-\n"
                     "1 - Create a new contract\n"
                     "2 - Cancel a contract\n"
                     "3 - Contracts\n"
                     "4 - Travel Locations\n"
                     "5 - Adjust prices\n"
                     "6 - Edit Customers\n"
                     "7 - Edit Locations\n"
                     "8 - Restore default data\n"
                     "9 - Exit\n\n"
                     "Choose an option: ";
        std::cin >> chooseOption;

        system("cls");
        switch (chooseOption) {
        case 1:
            createContract();
            break;
        case 2:
            deleteContract();
            break;
        case 3:
            contractList();
            break;
        case 4:
            locationList();
            break;
        case 5:
            adjustPrices();
            break;
        case 6:
            editCustomers();
            break;
        case 7:
            editLocations();
            break;
        case 8:
            FileHandling::restoreDefaultData(m_customerList, m_locations, m_contracts);
            break;
        }
    }
}

void BookingSystem::createContract() {
    system("cls");

    std::vector<Customer> customersForContract;

    std::cout << "   -Create a new contract-\n\n";

    std::cout << "List of customers:\n\n";
    std::cout << "0 - Selection finished\n\n";
    for (int i = 1; i <= m_customerList.size(); i++) {
        std::cout << i << " - " << m_customerList[i - 1].m_firstName << " " << m_customerList[i - 1].m_lastName << std::endl;
    }
    std::cout << std::endl;

    int userChoice = -1, customerCounter = 1;
    std::vector<size_t> customerChoices;
    while (userChoice != 0) {

        std::cout << "Choose " << customerCounter << ". customer: ";
        std::cin >> userChoice;

        auto isDuplicate = [&customerChoices = std::as_const(customerChoices), userChoice]() -> bool {
            return (std::find(customerChoices.begin(), customerChoices.end(), userChoice) != customerChoices.end());
        };

        if (isDuplicate()) {
            std::cout << "You can't choose the same customer again.\n\n";
        }
        else if (userChoice > 0 && userChoice <= m_customerList.size()) {
            customersForContract.push_back(m_customerList[userChoice - 1]);
            customerCounter++;
        }
        else if (customersForContract.size() == 0) {
            return;
        }

        customerChoices.push_back(userChoice);
    }

    std::cout << std::endl;

    std::cout << "List of locations:\n\n";
    for (int i = 1; i <= m_locations.size(); i++) {
        std::cout << i << " - " << m_locations[i - 1].m_name
                       << " " << m_locations[i - 1].m_address
                       << " " << m_locations[i - 1].m_price << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Choose an option: ";
    int locationChoice;
    std::cin >> locationChoice;
    int locationIndex = locationChoice - 1;

    uint64_t finalPrice = m_locations[locationIndex].m_price * customersForContract.size();
    m_contracts.emplace_back(customersForContract, finalPrice, m_locations[locationIndex]);

    auto saveContractToFile = [=]() -> void {
        std::ofstream out;
        out.open("contractData.json");
        if (out.fail()) {
            std::cerr << "contractData.json can't open";
            return;
        }

        Json::Value jsonContracts, root;
        int contractIndex = 0;
        for (const auto& contract : m_contracts) {
            Json::Value jsonContract;
            contract.serialize(jsonContract, m_contracts, contractIndex);
            jsonContracts.append(jsonContract);

            root["contracts"][contractIndex] = jsonContracts[contractIndex];

            contractIndex++;
        }

        Json::StyledStreamWriter writer;
        writer.write(out, root);

        out.close();
    };

    saveContractToFile();
}


void BookingSystem::deleteContract() {
    std::cout << "   -Cancel a contract-\n\n";

    std::cout << "0 - Exit" << std::endl;
    for (int i = 0; i < m_contracts.size(); i++) {
        std::cout << i + 1 << " - " << i+1 << ". contract\n";
    }
    std::cout << std::endl;

    std::cout << "Choose an option: ";
    int deleteChoice;
    std::cin >> deleteChoice;

    if (deleteChoice == 0) {
        return;
    }

    m_contracts.erase(m_contracts.begin() + (deleteChoice - 1));
}

void BookingSystem::contractList() {
    std::cout << "   -Contract list-\n\n";

    for (int i = 0; i < m_contracts.size(); i++) {
        std::cout << "-----------------------------\n";
        std::cout << i + 1 << ". contract\n\n";

        std::cout << "Customers:\n";
        std::vector<Customer> customers = m_contracts[i].getCustomers();
        for (int j = 0; j < customers.size(); j++) {
            std::cout << customers[j].m_firstName << " " << customers[j].m_lastName << " " << customers[j].m_dateOfBirth << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Location: " << m_contracts[i].getLocation().m_name << std::endl
                  << "Address: " << m_contracts[i].getLocation().m_address << std::endl
                  << "Price: " << m_contracts[i].getPrice() << std::endl << std::endl
                  << "ID: " << m_contracts[i].getID() << std::endl
                  << "Date of Creation: " << m_contracts[i].getDateOfCreation() << std::endl
                  << "-----------------------------\n\n";
    }
        std::cout << std::endl;

        std::cout << "\nPress Enter to continue." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
}

void BookingSystem::locationList() {
    std::cout << "   -Location list-\n\n";

    for (int i = 1; i <= m_locations.size(); i++) {
        std::cout << i << " - " << m_locations[i-1].m_name
                       << " " << m_locations[i-1].m_address
                       << " " << m_locations[i-1].m_price << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "\nPress Enter to continue." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void BookingSystem::adjustPrices() {
    std::cout << "   -Adjust prices-\n\n";
    
    int locationChoice = -1;
    std::cout << "0 - Exit\n";
    for (int i = 1; i <= m_locations.size(); i++) {
        std::cout << i << " - " << m_locations[i - 1].m_name
                       << " " << m_locations[i - 1].m_address
                       << " " << m_locations[i - 1].m_price << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Choose an option: ";
    std::cin >> locationChoice;

    if (locationChoice == 0) {
        return;
    }
    std::cout << std::endl;

    unsigned int newPrice;
    std::cout << "Enter new price: ";
    std::cin >> newPrice;

    m_locations[locationChoice - 1].m_name = newPrice;
}

void BookingSystem::editCustomers() {
    std::cout << "   -Edit Customers-\n\n";

    std::cout << "0 - Exit\n"
              << "1 - Add a customer\n"
              << "2 - Remove a customer\n\n";
    
    int editChoice = -1;
    std::cout << "Choose an option: ";
    std::cin >> editChoice;

    std::cout << std::endl;

    if (editChoice == 0) {
        return;
    }
    else if (editChoice == 1) {
        std::string firstName, lastName, dateOfBirth, address;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "First name: ";
        std::getline(std::cin, firstName);
        std::cout << "Last name: ";
        std::getline(std::cin, lastName);
        std::cout << "Date of birth: ";
        std::getline(std::cin, dateOfBirth);
        std::cout << "Address: ";
        std::getline(std::cin, address);

        m_customerList.emplace_back(firstName, lastName, dateOfBirth, address);

        std::ofstream out;
        out.open("customerData.txt");

        if (out.fail()) {
            std::cerr << "customerData.txt can't open";
            return;
        }

        for (const auto& customer : m_customerList) {
            out << customer.m_firstName << ','
                << customer.m_lastName << ','
                << customer.m_dateOfBirth << ','
                << customer.m_address << std::endl;
        }

        out.close();
    }
    else if (editChoice == 2) {
        std::cout << "List of customers:\n\n";
        for (int i = 1; i <= m_customerList.size(); i++) {
            std::cout << i << " - " << m_customerList[i - 1].m_firstName << " " << m_customerList[i - 1].m_lastName << std::endl;
        }
        std::cout << std::endl;

        int removeCustomerChoice = -1;
        std::cout << "Choose an option: ";
        std::cin >> removeCustomerChoice;

        m_customerList.erase(m_customerList.begin() + (removeCustomerChoice - 1));

        std::ofstream out;

        out.open("customerData.txt");

        if (out.fail()) {
            std::cerr << "customerData.txt can't open";
            return;
        }

        for (const auto& customer : m_customerList) {
            out << customer.m_firstName << ',' 
                          << customer.m_lastName << ',' 
                          << customer.m_dateOfBirth << ',' 
                          << customer.m_address << std::endl;
        }

        out.close();
    }
}

void BookingSystem::editLocations() {
    std::cout << "   -Edit Locations-\n\n";

    std::cout << "0 - Exit\n"
        << "1 - Add a location\n"
        << "2 - Remove a location\n\n";

    int editChoice = -1;
    std::cout << "Choose an option: ";
    std::cin >> editChoice;

    std::cout << std::endl;

    if (editChoice == 0) {
        return;
    }
    else if (editChoice == 1) {
        std::string name, address;
        uint64_t price;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Location name: ";
        std::getline(std::cin, name);
        std::cout << "Address: ";
        std::getline(std::cin, address);
        std::cout << "Price: ";
        std::cin >> price;

        m_locations.emplace_back(name, address, price);

        std::ofstream out;
        out.open("locationData.txt");

        if (out.fail()) {
            std::cerr << "locationData.txt can't open";
            return;
        }

        for (const auto& location : m_locations) {
            out << location.m_name << ','
                << location.m_address << ','
                << location.m_price << std::endl;
        }

        out.close();
    }
    else if (editChoice == 2) {
        std::cout << "List of locations:\n\n";
        for (int i = 1; i <= m_locations.size(); i++) {
            std::cout << i << " - " << m_locations[i - 1].m_name << " " << m_locations[i - 1].m_address  
                           << " " << m_locations[i - 1].m_price << std::endl;
        }
        std::cout << std::endl;

        int removeLocationChoice = -1;
        std::cout << "Choose an option: ";
        std::cin >> removeLocationChoice;

        m_locations.erase(m_locations.begin() + (removeLocationChoice - 1));

        std::ofstream out;

        out.open("locationData.txt");

        if (out.fail()) {
            std::cerr << "locationData.txt can't open";
            return;
        }

        for (const auto& location : m_locations) {
            out << location.m_name << ','
                << location.m_address << ','
                << location.m_price << std::endl;
        }

        out.close();
    }
}
