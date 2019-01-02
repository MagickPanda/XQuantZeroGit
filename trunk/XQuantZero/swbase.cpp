#include "swbase.h"

void DoubleToString(double val, string &ret) {
	stringstream ss;
	ss.clear();  ss.str("");;
	ss << val;
	ss >> ret;
}

string sw_getDate() {
	char date[36];

	time_t t = time(NULL);
	tm* tm = localtime(&t);

	sprintf(date, "%d-%02d-%02d", (tm->tm_year + 1900), tm->tm_mon + 1, tm->tm_mday);

	return string(date);
}

string sw_getTime() {
	char date[36];

	time_t t = time(NULL);
	tm* tm = localtime(&t);

	sprintf(date, "%02d::%02d::%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

	return string(date);
}

string sw_getFullTime() {
	char date[36];

	time_t t = time(NULL);
	tm* tm = localtime(&t);

	sprintf(date, "%d-%02d-%02d %02d::%02d::%02d", (tm->tm_year + 1900), tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

	return string(date);
}

string orderToRef(int orderID) {
	char temp[13];
	sprintf(temp, "%.12d", orderID);
	return string(temp);
}