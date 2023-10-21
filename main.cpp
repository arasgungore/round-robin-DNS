#include "Header/DnsTree.h"
#include "Header/Client.h"

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
