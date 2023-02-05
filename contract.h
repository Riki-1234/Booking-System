#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <json/json.h>
#include "customer.h"
#include "location.h"

class Contract {
public:
	Contract(const std::vector<Customer>& customers, uint32_t price, const Location& location);

	const std::vector<Customer>& getCustomers() const;
	const Location& getLocation() const;
	uint32_t getPrice() const;
	const std::string& getDateOfCreation() const;
	void setDateOfCreation(const std::string& newDateOfCreation);
	uint16_t getID() const;
	void setID(const uint16_t newID);

	const std::string& currentDate();
	void serialize(Json::Value& jsonContract, const std::vector<Contract>& m_contracts, int contractIndex) const;

private:
	std::vector<Customer> m_customers;
	uint32_t m_price;
	Location m_location;
	uint16_t m_ID;
	std::string m_dateOfCreation;

	static uint16_t m_instanceCounter;
};
