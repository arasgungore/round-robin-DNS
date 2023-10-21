#include "Header/DnsTree.h"
#include "Header/Client.h"

int main() {
	// Create a DNS tree named "internet" for the International Network.
	DnsTree internet("International Network");

	// Insert DNS records mapping domain names to IP addresses.
	internet.insertRecord("bbc.co.uk", "7.7.7.7");
	internet.insertRecord("bbc.co.uk", "31.69.14.88");
	internet.insertRecord("cambridge.ac.uk", "8.8.8.8");
	internet.insertRecord("google.com", "3.3.3.3");

	// Print all DNS records in the DNS tree.
	internet.printAllRecords();

	// Continue to insert more DNS records.
	internet.insertRecord("mail.google.com", "4.4.4.4");
	internet.insertRecord("twitter.com", "5.5.5.5");
	internet.insertRecord("developer.twitter.com", "6.6.6.6");
	internet.insertRecord("boun.edu.tr", "1.1.1.1");
	internet.insertRecord("cmpe.boun.edu.tr", "2.2.2.2");
	internet.insertRecord("ee.boun.edu.tr", "3.3.33.33");
	internet.insertRecord("metu.edu.tr", "1.1.1.1");

	// Print all DNS records again after adding more entries.
	internet.printAllRecords();

	// Remove specific DNS records.
	internet.removeRecord("bbc.co.uk");
	internet.removeRecord("google.com", "3.3.3.3");
	internet.removeRecord("ee.boun.edu.tr", "3.3.33.33");
	internet.removeRecord("cmpe.boun.edu.tr", "2.1.1.2");
	internet.removeRecord("twitter.com", "5.5.5.5");

	// Add more DNS records and print them.
	internet.insertRecord("github.com", "3.1.3.1");
	internet.insertRecord("github.com", "3.1.6.9");
	internet.insertRecord("github.com", "3.6.9.1");
	internet.insertRecord("turkiye.gov.tr", "38.0.0.0");
	internet.printAllRecords();

	// Create a client named "aras" with an IP address and the DNS tree.
	Client aras("ArasDasDas", "42.23.11.9", internet);

	// Send DNS requests, cache responses, and print client information.
	aras.sendRequest("boun.edu.tr");
	aras.printClientInfo();

	// Send more requests, including repeated requests, and observe caching behavior.
	aras.sendRequest("boun.edu.tr");
	aras.sendRequest("boun.edu.tr");
	aras.sendRequest("boun.edu.tr");
	aras.printClientInfo();

	// Send additional requests and print client information.
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

	// Remove a DNS record and observe the client's behavior.
	internet.removeRecord("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.sendRequest("turkiye.gov.tr");
	aras.printClientInfo();

	// Send more DNS requests and print the client's information.
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
