#ifndef DNSTREE_H
#define DNSTREE_H

#include "DnsNode.h" // Include the header for the DnsNode class.
#include <string>

class DnsTree {
private:
	DnsNode root;	  // The root node of the DNS tree.
	std::string name; // The name of the DNS tree.

public:
	// Constructor to initialize a DnsTree with a given name.
	DnsTree(const std::string &name);

	// Insert a DNS record, mapping a domain name to an IP address.
	void insertRecord(const std::string &domainName, const std::string &ipAddress);

	// Remove a DNS record associated with the specified domain name.
	bool removeRecord(const std::string &domainName);

	// Remove a specific DNS record mapping a domain name to an IP address.
	bool removeRecord(const std::string &domainName, const std::string &ipAddress);

	// Query for an IP address associated with a given domain name.
	std::string queryDomain(const std::string &domainName);

	// Print all DNS records stored in the DNS tree, including subdomains and their IP addresses.
	void printAllRecords() const;
};

#endif
