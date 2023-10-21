#ifndef CLIENT_H
#define CLIENT_H

#include "DnsTree.h"
#include <string>
#include <vector>

#define SIZE_OF_CACHE_LIST 10

class Client {
private:
	class CachedContent {
	public:
		std::string domainName, ipAddress;
		int hitNo;
		CachedContent(const std::string &domainName, const std::string &ipAddress);
	};
	DnsTree root;
	std::string name, ipAddress;
	std::vector<CachedContent> cacheList;

	void addToCache(const std::string &domainName, const std::string &ipAddress);
public:
	Client(const std::string &name, const std::string &ipAddress, const DnsTree &root);
	std::string sendRequest(const std::string &domainName);
	void clearCache();
	void printClientInfo() const;
};

#endif
