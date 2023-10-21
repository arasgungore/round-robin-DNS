#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>


#define SIZE_OF_CACHE_LIST 10

class DnsNode;
class DnsTree;
class Client;


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


DnsNode::DnsNode() : validDomain(false) {	}

DnsNode *DnsNode::getNode(std::string &domainName)
{
    if (domainName.find('.') == std::string::npos)
    {
        std::map<std::string, DnsNode>::iterator i = childNodeList.find(domainName);
        return i == childNodeList.end() ? nullptr : &i->second;
    }
    const size_t found = domainName.find_last_of('.');
    const std::string subDomain = domainName.substr(found + 1);
    std::map<std::string, DnsNode>::iterator i = childNodeList.find(subDomain);
    if(i == childNodeList.end())
        return nullptr;
	DnsNode* subDomainNode = &i->second;
	domainName.erase(found);
	return subDomainNode->getNode(domainName);
}

DnsNode *DnsNode::createNode(std::string &domainName) {
    if(domainName.find('.') == std::string::npos)
        return &childNodeList.emplace(domainName, DnsNode()).first->second;
    const size_t found = domainName.find_last_of('.');
    const std::string subDomain = domainName.substr(found + 1);
    std::map<std::string, DnsNode>::iterator i = childNodeList.find(subDomain);
    DnsNode* subDomainNode = i == childNodeList.end() ? &childNodeList.emplace(subDomain, DnsNode()).first->second : &i->second;
	domainName.erase(found);
	return subDomainNode->createNode(domainName);
}

bool DnsNode::deleteNode(std::string &domainName) {
    if(domainName.find('.') == std::string::npos) {
        const bool deleted = childNodeList.find(domainName) != childNodeList.end();
		if(deleted)
			childNodeList.erase(domainName);
		return deleted;
    }
    const size_t found = domainName.find_last_of('.');
    const std::string subDomain = domainName.substr(found + 1);
    std::map<std::string, DnsNode>::iterator i = childNodeList.find(subDomain);
    if(i == childNodeList.end())
		return false;
	DnsNode* subDomainNode = &i->second;
	domainName.erase(found);
	return subDomainNode->deleteNode(domainName);
}

std::string DnsNode::getNextIpAddress() {
    if(ipAddresses.empty())
		return "";
	rotate(ipAddresses.begin(), ipAddresses.begin() + 1, ipAddresses.end());
	return ipAddresses.back();
}

void DnsNode::insertIpAddress(const std::string &ipAddress) {
    if(ipAddresses.empty())
        validDomain = true;
	ipAddresses.push_back(ipAddress);
}

void DnsNode::deleteIpAddress(const std::string &ipAddress) {
    ipAddresses.erase(find(ipAddresses.begin(), ipAddresses.end(), ipAddress));
	if(ipAddresses.empty())
        validDomain = false;
}

bool DnsNode::doesIpAddressExist(const std::string &ipAddress) const {
    return find(ipAddresses.begin(), ipAddresses.end(), ipAddress) != ipAddresses.end();
}

std::map<std::string, std::vector<std::string>> DnsNode::getAllRecords(std::string &prevDomain) const {
    std::map<std::string, std::vector<std::string>> domainAndIPs;
    for(auto i = childNodeList.begin(); i != childNodeList.end(); i++) {
		prevDomain.insert(0, "." + i->first);
		if(i->second.validDomain)
			domainAndIPs.emplace(prevDomain.substr(1), i->second.ipAddresses);
        std::map<std::string, std::vector<std::string>> temp = i->second.getAllRecords(prevDomain);
        domainAndIPs.insert(temp.begin(), temp.end());
		prevDomain.erase(0, i->first.length() + 1);
	}
	return domainAndIPs;
}

DnsTree::DnsTree(const std::string &name) : root(), name(name) { }

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
		for(auto j = i->second.begin() ; j != i->second.end(); j++)
			std::cout << "\t\t\t" << *j << std::endl;
	}
	std::cout << std::endl << std::endl;
}


Client::CachedContent::CachedContent(const std::string &domainName = "", const std::string &ipAddress = "") : domainName(domainName), ipAddress(ipAddress), hitNo(0) { }

Client::Client(const std::string &name, const std::string &ipAddress, const DnsTree &root) : root(root), name(name), ipAddress(ipAddress) { }

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

void Client::clearCache() {
    cacheList.clear();
}

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


int main() {
	DnsTree internet("International Network");
	internet.insertRecord("bbc.co.uk", "7.7.7.7");
	internet.insertRecord("bbc.co.uk", "31.69.14.88");
	internet.insertRecord("cambridge.ac.uk", "8.8.8.8");
	internet.insertRecord("google.com", "3.3.3.3");
	internet.printAllRecords();

	internet.insertRecord("mail.google.com", "4.4.4.4");
	internet.insertRecord("twitter.com", "5.5.5.5");
	internet.insertRecord("developer.twitter.com", "6.6.6.6");
	internet.insertRecord("boun.edu.tr", "1.1.1.1");
	internet.insertRecord("cmpe.boun.edu.tr", "2.2.2.2");
	internet.insertRecord("ee.boun.edu.tr", "3.3.33.33");
	internet.insertRecord("metu.edu.tr", "1.1.1.1");
	internet.printAllRecords();

	internet.removeRecord("bbc.co.uk");
	internet.removeRecord("google.com", "3.3.3.3");
	internet.removeRecord("ee.boun.edu.tr", "3.3.33.33");
	internet.removeRecord("cmpe.boun.edu.tr", "2.1.1.2");
	internet.removeRecord("twitter.com", "5.5.5.5");
	internet.insertRecord("github.com", "3.1.3.1");
    internet.insertRecord("github.com", "3.1.6.9");
    internet.insertRecord("github.com", "3.6.9.1");
    internet.insertRecord("turkiye.gov.tr", "38.0.0.0");
	internet.printAllRecords();


	Client aras("ArasDasDas", "42.23.11.9", internet);
	aras.sendRequest("boun.edu.tr");
	aras.printClientInfo();

	aras.sendRequest("boun.edu.tr");
	aras.sendRequest("boun.edu.tr");
	aras.sendRequest("boun.edu.tr");
	aras.printClientInfo();

    aras.sendRequest("github.com");
    aras.sendRequest("github.com");
    aras.sendRequest("ee.boun.edu.tr");
	aras.sendRequest("dedeleregeldik.org");
	aras.sendRequest("twitter.com");
	aras.sendRequest("developer.twitter.com");
    aras.sendRequest("github.com");
    aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.printClientInfo();

	internet.removeRecord("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.printClientInfo();

	aras.sendRequest("mail.google.com");
	aras.sendRequest("twitter.com");
	aras.sendRequest("developer.twitter.com");
	aras.sendRequest("boun.edu.tr");
	aras.sendRequest("cmpe.boun.edu.tr");
	aras.sendRequest("ee.boun.edu.tr");
	aras.sendRequest("metu.edu.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.printClientInfo();

	return 0;
}
