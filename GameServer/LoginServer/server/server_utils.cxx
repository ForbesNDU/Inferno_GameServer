#include "server_utils.h"

void* get_in_addr(struct sockaddr *sa) {
        if(sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

std::string hash_SHA_256( std::string input ) {

	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, input.c_str(), input.size());
	SHA256_Final(hash, &ctx);

	std::stringstream ss;

	int i;
	for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}	

	return ss.str();

}

std::string timestamp() {

        time_t current_time;
        struct tm* timeinfo;
        char buffer[100];

        time(&current_time);
        timeinfo = localtime(&current_time);

        strftime(buffer, 100, "[%d-%m-%Y %I:%M:%S CET]", timeinfo);
        std::string str(buffer);

        return str;

}
