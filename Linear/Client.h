#ifndef CLIENT_H
#define CLIENT_H

class Client {
	public:
		Client(char*, char*);
		~Client();
		const char* getAddress();
		const char* getUsername();

	private:
		const char* address;
		const char *username;		

};

#endif
