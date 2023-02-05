#pragma once
#include "customer.h"
#include "location.h"
#include "contract.h"

class FileHandling {
public:
    static void retrieveCustomerData(std::vector<Customer>& m_customerList);
    static void retrieveLocationData(std::vector<Location>& m_locations);
    static void retrieveContractData(std::vector<Contract>& m_contracts);
    static void restoreDefaultData(std::vector<Customer>& m_customerList, std::vector<Location>& m_locations, std::vector<Contract>& m_contracts);
};
