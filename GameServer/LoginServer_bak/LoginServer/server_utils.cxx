#include "server_utils.h"

void* get_in_addr(struct sockaddr *sa) {
        if(sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
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
