#include "contract.h"

Contract::Contract(const std::vector<Customer>& customers, uint32_t price, const Location& location) :
	m_customers(customers),
	m_price(price),
	m_location(location),
	m_ID(m_instanceCounter),
	m_dateOfCreation(currentDate())
{
	m_instanceCounter++;
}

uint16_t Contract::m_instanceCounter = 1;

const std::vector<Customer>& Contract::getCustomers() const {
	return m_customers;
}

uint32_t Contract::getPrice() const {
	return m_price;
}

const Location& Contract::getLocation() const {
	return m_location;
}

const std::string& Contract::getDateOfCreation() const {
	return m_dateOfCreation;
}

void Contract::setDateOfCreation(const std::string& newDateOfCreation) {
	m_dateOfCreation = newDateOfCreation;
}

uint16_t Contract::getID() const {
	return m_ID;
}

void Contract::setID(const uint16_t newID) {
	m_ID = newID;
}

const std::string& Contract::currentDate() {
	int yearOffset = 1900, monthOffset = 1;

	std::string dateOfCreation;

	time_t currentTime = time(0);
	tm localTime;

	localtime_s(&localTime, &currentTime);

	dateOfCreation = std::to_string(localTime.tm_mday) + "/" + std::to_string(localTime.tm_mon + monthOffset)
		+ "/" + std::to_string(localTime.tm_year + yearOffset);

	return dateOfCreation;
}

void Contract::serialize(Json::Value& jsonContract, const std::vector<Contract>& m_contracts, int contractIndex) const {
	int customerIndex = 0;
	for (const auto& customer : m_contracts[contractIndex].getCustomers()) {
		jsonContract["customers"][customerIndex]["firstName"] = customer.m_firstName;
		jsonContract["customers"][customerIndex]["lastName"] = customer.m_lastName;
		jsonContract["customers"][customerIndex]["dateOfBirth"] = customer.m_dateOfBirth;
		jsonContract["customers"][customerIndex]["address"] = customer.m_address;

		customerIndex++;
	}

	jsonContract["price"] = m_contracts[contractIndex].getPrice();
	jsonContract["location"]["name"] = m_contracts[contractIndex].getLocation().m_name;
	jsonContract["location"]["address"] = m_contracts[contractIndex].getLocation().m_address;
	jsonContract["location"]["price"] = m_contracts[contractIndex].getLocation().m_price;
	jsonContract["dateOfCreation"] = m_contracts[contractIndex].getDateOfCreation();
	jsonContract["ID"] = m_contracts[contractIndex].getID();
}
