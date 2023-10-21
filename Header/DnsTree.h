#ifndef DNSTREE_H
#define DNSTREE_H

#include "DnsNode.h"
#include <string>

class DnsTree {
private:
	DnsNode root;
	std::string name;
public:
	DnsTree(const std::string &name);
	void insertRecord(const std::string &domainName, const std::string &ipAddress);
	bool removeRecord(const std::string &domainName);
	bool removeRecord(const std::string &domainName, const std::string &ipAddress);
	std::string queryDomain(const std::string &domainName);
	void printAllRecords() const;
};

#endif
