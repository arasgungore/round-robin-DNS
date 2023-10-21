#include "DnsTree.h"
#include <iostream>

// Constructor to initialize a DnsTree with a given name.
DnsTree::DnsTree(const std::string &name) : root(), name(name) { }

// Insert a DNS record, mapping a domain name to an IP address.
void DnsTree::insertRecord(const std::string &domainName, const std::string &ipAddress) {
	std::string domainNameCopy = domainName;
	DnsNode *n = root.getNode(domainNameCopy);
	if(n != nullptr) {
		if(!n->doesIpAddressExist(ipAddress))
			n->insertIpAddress(ipAddress);
		return;
	}
	root.createNode(domainNameCopy = domainName)->insertIpAddress(ipAddress);
}

// Remove a DNS record associated with the specified domain name.
bool DnsTree::removeRecord(const std::string &domainName) {
	std::string domainNameCopy = domainName;
	return root.deleteNode(domainNameCopy);
}

// Remove a specific DNS record mapping a domain name to an IP address.
bool DnsTree::removeRecord(const std::string &domainName, const std::string &ipAddress) {
	std::string domainNameCopy = domainName;
	DnsNode *n = root.getNode(domainNameCopy);
	if(n != nullptr && n->doesIpAddressExist(ipAddress)) {
		n->deleteIpAddress(ipAddress);
		return true;
	}
	return false;
}

// Query for an IP address associated with a given domain name.
std::string DnsTree::queryDomain(const std::string &domainName) {
	std::string domainNameCopy = domainName;
	DnsNode *n = root.getNode(domainNameCopy);
	return n != nullptr ? n->getNextIpAddress() : "";
}

// Print all DNS records stored in the DNS tree, including subdomains and their IP addresses.
void DnsTree::printAllRecords() const {
	std::string domainName = "";
	std::map<std::string, std::vector<std::string>> records = root.getAllRecords(domainName);
	std::cout << "Server Info:" << std::endl;
	std::cout << "\tServer Name: " << name << std::endl;
	std::cout << "\tDomain Names:" << std::endl;
	for(auto i = records.begin(); i != records.end(); i++) {
		std::cout << std::endl << "\t\t" << i->first << ":" << std::endl;
		for(auto j = i->second.begin(); j != i->second.end(); j++)
			std::cout << "\t\t\t" << *j << std::endl;
	}
	std::cout << std::endl << std::endl;
}