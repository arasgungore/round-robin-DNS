#ifndef CLIENT_H
#define CLIENT_H

#include "DnsTree.h" // Include the header for the DnsTree class.
#include <string>
#include <vector>

#define SIZE_OF_CACHE_LIST 10

class Client {
private:
	// Inner class to represent cached content.
	class CachedContent {
	public:
		std::string domainName, ipAddress;
		int hitNo;
		// Constructor to initialize CachedContent.
		CachedContent(const std::string &domainName, const std::string &ipAddress);
	};

	DnsTree root;						  // The DNS tree associated with this client.
	std::string name;					  // The name of the client.
	std::string ipAddress;				  // The IP address of the client.
	std::vector<CachedContent> cacheList; // A list to store cached DNS responses.

	// Helper function to add an entry to the cache.
	void addToCache(const std::string &domainName, const std::string &ipAddress);

public:
	// Constructor to initialize a client with a name, IP address, and a reference to a DNS tree.
	Client(const std::string &name, const std::string &ipAddress, const DnsTree &root);

	// Send a DNS request and retrieve an IP address, caching the response.
	std::string sendRequest(const std::string &domainName);

	// Clear the cache of DNS responses.
	void clearCache();

	// Print client information, including the name, IP address, and cached responses.
	void printClientInfo() const;
};

#endif
