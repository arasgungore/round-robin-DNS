#ifndef DNSNODE_H
#define DNSNODE_H

#include <string>
#include <vector>
#include <map>

class DnsNode {
private:
	std::map<std::string, DnsNode> childNodeList;
	bool validDomain;
	std::vector<std::string> ipAddresses;
public:
	DnsNode();
	DnsNode *getNode(std::string &domainName);
	DnsNode *createNode(std::string &domainName);
	bool deleteNode(std::string &domainName);
	std::string getNextIpAddress();
	void insertIpAddress(const std::string &ipAddress);
	void deleteIpAddress(const std::string &ipAddress);
	bool doesIpAddressExist(const std::string &ipAddress) const;
	std::map<std::string, std::vector<std::string>> getAllRecords(std::string &prevDomain) const;
};

#endif
