#include "DnsNode.h"
#include <algorithm>

// Constructor to initialize a DnsNode.
DnsNode::DnsNode() : validDomain(false) { }

// Get a child node based on a domain name.
DnsNode *DnsNode::getNode(std::string &domainName) {
    if(domainName.find('.') == std::string::npos) {
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

// Create a new child node with the given domain name.
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

// Delete a child node with the specified domain name.
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

// Get the next IP address in the list (round-robin style).
std::string DnsNode::getNextIpAddress() {
    if(ipAddresses.empty())
		return "";
	rotate(ipAddresses.begin(), ipAddresses.begin() + 1, ipAddresses.end());
	return ipAddresses.back();
}

// Insert an IP address into the list for this domain.
void DnsNode::insertIpAddress(const std::string &ipAddress) {
    if(ipAddresses.empty())
        validDomain = true;
	ipAddresses.push_back(ipAddress);
}

// Delete a specific IP address from the list.
void DnsNode::deleteIpAddress(const std::string &ipAddress) {
    ipAddresses.erase(find(ipAddresses.begin(), ipAddresses.end(), ipAddress));
	if(ipAddresses.empty())
        validDomain = false;
}

// Check if a given IP address exists in the list.
bool DnsNode::doesIpAddressExist(const std::string &ipAddress) const {
    return find(ipAddresses.begin(), ipAddresses.end(), ipAddress) != ipAddresses.end();
}

// Retrieve all DNS records under this node and its children, starting from the prevDomain.
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