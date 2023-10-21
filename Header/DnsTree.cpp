#include "DnsTree.h"
#include <iostream>

DnsTree::DnsTree(const std::string &name) : root(), name(name) {}

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

bool DnsTree::removeRecord(const std::string &domainName) {
	std::string domainNameCopy = domainName;
	return root.deleteNode(domainNameCopy);
}

bool DnsTree::removeRecord(const std::string &domainName, const std::string &ipAddress) {
	std::string domainNameCopy = domainName;
	DnsNode *n = root.getNode(domainNameCopy);
	if(n != nullptr && n->doesIpAddressExist(ipAddress)) {
		n->deleteIpAddress(ipAddress);
		return true;
	}
	return false;
}

std::string DnsTree::queryDomain(const std::string &domainName) {
	std::string domainNameCopy = domainName;
	DnsNode *n = root.getNode(domainNameCopy);
	return n != nullptr ? n->getNextIpAddress() : "";
}

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