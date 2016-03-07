#ifndef CLIENT_H
#define CLIENT_H

class Client {
	public:
		Client(int);
		~Client();
		int getELO();

	private:
		int ELO;

};

#endif
