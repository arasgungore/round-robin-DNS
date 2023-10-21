#include "Client.h"
#include <iostream>

// Constructor to initialize CachedContent.
Client::CachedContent::CachedContent(const std::string &domainName = "", const std::string &ipAddress = "") : domainName(domainName), ipAddress(ipAddress), hitNo(0) { }

// Constructor to initialize a client with a name, IP address, and a reference to a DNS tree.
Client::Client(const std::string &name, const std::string &ipAddress, const DnsTree &root) : root(root), name(name), ipAddress(ipAddress) { }

// Send a DNS request and retrieve an IP address, caching the response.
std::string Client::sendRequest(const std::string &domainName) {
	for(auto i = cacheList.begin(); i != cacheList.end(); i++)
		if(!i->domainName.compare(domainName)) {
			i->hitNo++;
			addToCache(domainName, i->ipAddress);
			return i->ipAddress;
		}
	const std::string ipAddress = root.queryDomain(domainName);
	if(!ipAddress.empty())
		addToCache(domainName, ipAddress);
	return ipAddress;
}

// Helper function to add an entry to the cache.
void Client::addToCache(const std::string &domainName, const std::string &ipAddress) {
	for(auto i = cacheList.begin(); i != cacheList.end(); i++)
		if(!(i->domainName.compare(domainName) || i->ipAddress.compare(ipAddress)))
			return;
	if(cacheList.size() < SIZE_OF_CACHE_LIST) {
		cacheList.push_back(CachedContent(domainName, ipAddress));
		return;
	}
	int minHitNo = cacheList.front().hitNo;
	std::vector<CachedContent>::iterator deleted;
	for(auto i = cacheList.begin() + 1; i != cacheList.end(); i++)
		if(minHitNo > i->hitNo)
			minHitNo = (deleted = i)->hitNo;
	*deleted = CachedContent(domainName, ipAddress);
}

// Clear the cache of DNS responses.
void Client::clearCache() {
	cacheList.clear();
}

// Print client information, including the name, IP address, and cached responses.
void Client::printClientInfo() const {
	std::cout << "Client Info:" << std::endl;
	std::cout << "\tClient Name: " << name << std::endl;
	std::cout << "\tIP Address: " << ipAddress << std::endl;
	std::cout << "\tCache List:" << std::endl << std::endl;
	for(size_t i = 0; i < cacheList.size(); i++) {
		std::cout << "\t\tCache " << i + 1 << ":" << std::endl;
		std::cout << "\t\t\tDomain Name: " << cacheList[i].domainName << std::endl;
		std::cout << "\t\t\tIP Address: " << cacheList[i].ipAddress << std::endl;
		std::cout << "\t\t\tHit No: " << cacheList[i].hitNo << std::endl << std::endl;
	}
	std::cout << std::endl;
}