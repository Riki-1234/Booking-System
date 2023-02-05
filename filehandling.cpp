#include <vector>
#include <string>
#include <fstream>
#include "filehandling.h"

void FileHandling::retrieveCustomerData(std::vector<Customer>& m_customerList) {
    std::ifstream read_customerData("customerData.txt");
    if (read_customerData.fail()) {
        std::cerr << "customerData.txt can't open";
        return;
    }

    constexpr size_t firstNameIndex = 0, lastNameIndex = 1, dateOfBirthIndex = 2, addressIndex = 3;
    std::vector<std::string> allLines;
    char character{0};
    int lineIndex{0};
    std::string line;

    do {
        character = read_customerData.get();

        if (character == '\n' || character == EOF) {
            allLines.push_back(line);

            std::vector<std::string> wordArray;
            std::string word;

            std::stringstream oneLine(allLines[lineIndex]);
            while (std::getline(oneLine, word, ',')) {
                wordArray.push_back(word);
            }

            if (wordArray.size() >= 4) {
                std::string firstName = wordArray[firstNameIndex], lastName = wordArray[lastNameIndex],
                    dateOfBirth = wordArray[dateOfBirthIndex], address = wordArray[addressIndex];

                m_customerList.emplace_back(firstName, lastName, dateOfBirth, address);
            }

            line.clear();

            lineIndex++;
        }
        else {
            line += character;
        }
    } while (character != EOF);

    read_customerData.close();
}

void FileHandling::retrieveLocationData(std::vector<Location>& m_locations) {
    std::ifstream read_locationData("locationData.txt");
    if (read_locationData.fail()) {
        std::cerr << "locationData.txt can't open";
        return;
    }

    constexpr size_t locationNameIndex = 0, addressIndex = 1, priceIndex = 2;
    std::vector<std::string> allLines;
    char character{ 0 };
    int lineIndex{ 0 };
    std::string line;

    do {
        character = read_locationData.get();

        if (character == '\n' || character == EOF) {
            allLines.push_back(line);

            std::vector<std::string> wordArray;
            std::string word;

            std::stringstream oneLine(allLines[lineIndex]);
            while (std::getline(oneLine, word, ',')) {
                wordArray.push_back(word);
            }

            if (wordArray.size() >= 3) {
                std::string locationName = wordArray[locationNameIndex], address = wordArray[addressIndex];
                uint64_t price = std::stoll(wordArray[priceIndex]);

                m_locations.emplace_back(locationName, address, price);
            }

            line.clear();

            lineIndex++;
        }
        else {
            line += character;
        }
    } while (character != EOF);

    read_locationData.close();
}

void FileHandling::retrieveContractData(std::vector<Contract>& m_contracts) {
    std::ifstream in("contractData.json");
    if (in.fail()) {
        std::cerr << "contractData.json can't open";
        return;
    }

    Json::Value root;
    Json::Reader reader;

    bool isParsing = reader.parse(in, root);
    if (!isParsing) {
        std::cerr << "contractData.json failed to parse";
        return;
    }

    std::string firstName, lastName, dateOfBirth, address;
    std::vector<Customer> customersForContract;
    Location location;

    int nContracts = root["contracts"].size();
    for (int i = 0, contractIndex = 0; i < nContracts; i++) {
        Json::Value jsonContracts = root["contracts"][i];
        int nCustomers = jsonContracts["customers"].size();

        for (int j = 0; j < nCustomers; j++) {
            firstName = jsonContracts["customers"][j]["firstName"].asString();
            lastName = jsonContracts["customers"][j]["lastName"].asString();
            dateOfBirth = jsonContracts["customers"][j]["dateOfBirth"].asString();
            address = jsonContracts["customers"][j]["address"].asString();
        }
        customersForContract.emplace_back(firstName, lastName, dateOfBirth, address);

        uint32_t price = jsonContracts["price"].asInt();

        location.m_name = jsonContracts["location"]["name"].asString();
        location.m_address = jsonContracts["location"]["address"].asString();
        location.m_price = jsonContracts["location"]["price"].asUInt64();

        m_contracts.emplace_back(customersForContract, price, location);

        m_contracts[contractIndex].setDateOfCreation(jsonContracts["dateOfCreation"].asString());
        m_contracts[contractIndex].setID(jsonContracts["ID"].asUInt());

        contractIndex++;
    }
}

void FileHandling::restoreDefaultData(std::vector<Customer>& m_customerList, std::vector<Location>& m_locations, std::vector<Contract>& m_contracts) {
    std::cout << "   -Restore Default Data-\n\n";

    std::cout << "0 - Exit\n"
        << "1 - Restore all data\n"
        << "2 - Restore contract data\n"
        << "3 - Restore customer data\n"
        << "4 - Restore location data\n\n";

    int restoreChoice = -1;
    std::cout << "Choose an option: ";
    std::cin >> restoreChoice;

    auto restoreData = [](const std::string& mainDataFile, const std::string& defaultDataFile) -> void {
        std::ofstream mainData(mainDataFile);
        std::ifstream defaultData(defaultDataFile);

        if (mainData.fail()) {
            std::cerr << mainDataFile << " can't open";
        }
        if (defaultData.fail()) {
            std::cerr << defaultDataFile << " can't open";
        }
        if (mainData.fail() || defaultData.fail()) {
            return;
        }

        std::string oneLine;
        while (std::getline(defaultData, oneLine)) {
            mainData << oneLine << std::endl;
        }
        mainData.close();
        defaultData.close();
    };

    if (restoreChoice == 0) {
        return;
    }
    else if (restoreChoice == 1) {
        restoreData("contractData.json", "contractData_default.json");
        m_contracts.clear();

        restoreData("customerData.txt", "customerData_default.txt");
        m_customerList.clear();
        retrieveCustomerData(m_customerList);

        restoreData("locationData.txt", "locationData_default.txt");
        m_locations.clear();
        retrieveLocationData(m_locations);
    }
    else if (restoreChoice == 2) {
        restoreData("contractData.json", "contractData_default.json");
        m_contracts.clear();
    }
    else if (restoreChoice == 3) {
        restoreData("customerData.txt", "customerData_default.txt");
        m_customerList.clear();
        retrieveCustomerData(m_customerList);
    }
    else if (restoreChoice == 4) {
        restoreData("locationData.txt", "locationData_default.txt");
        m_locations.clear();
        retrieveLocationData(m_locations);
    }
}
