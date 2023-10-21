#ifndef DNSNODE_H
#define DNSNODE_H

#include <string>
#include <vector>
#include <map>

class DnsNode {
private:
	// A map to store child nodes, where the key is a domain name and the value is another DnsNode.
	std::map<std::string, DnsNode> childNodeList;

	// A boolean flag to indicate whether this node represents a valid domain.
	bool validDomain;

	// A list of IP addresses associated with this domain.
	std::vector<std::string> ipAddresses;

public:
	// Constructor to initialize a DnsNode.
	DnsNode();

	// Get a child node based on a domain name.
	DnsNode *getNode(std::string &domainName);

	// Create a new child node with the given domain name.
	DnsNode *createNode(std::string &domainName);

	// Delete a child node with the specified domain name.
	bool deleteNode(std::string &domainName);

	// Get the next IP address in the list (round-robin style).
	std::string getNextIpAddress();

	// Insert an IP address into the list for this domain.
	void insertIpAddress(const std::string &ipAddress);

	// Delete a specific IP address from the list.
	void deleteIpAddress(const std::string &ipAddress);

	// Check if a given IP address exists in the list.
	bool doesIpAddressExist(const std::string &ipAddress) const;

	// Retrieve all DNS records under this node and its children, starting from the prevDomain.
	std::map<std::string, std::vector<std::string>> getAllRecords(std::string &prevDomain) const;
};

#endif
