#include "ntpClient.h"

NTP::NTP(const char *ntpServer, long gmtOffset_sec, int daylightOffset_sec) : _ntpServer(ntpServer), _gmtOffset_sec(gmtOffset_sec), _daylightOffset_sec(daylightOffset_sec) {
                                                                              };

NTP::~NTP() {

};

struct tm cont;

ntp_error_codes NTP::configureNTP()
{
    configTime(3 * 3600, 0, "pool.ntp.org");

    struct tm cont;

    if (!getLocalTime(&cont))
    {
        Serial.println("Failed to get time");
    }

    Serial.println(&cont, "Today's Date: %A, %B %d %Y %H:%M:%S");

    Serial.println("NTP configured successfully");
    return NTP_CONFIG_OK;
};

ntp_error_codes NTP::getEpochTime(long &epoch_time)
{
    time_t now = time(nullptr);
    epoch_time = now;

    Serial.printf("Epoch time is: %ld\n", epoch_time);
    return NTP_CONFIG_OK;
};

ntp_error_codes NTP::getFullDateTime(String &full_date_time)
{

    time_t now = time(nullptr);

    struct tm *tm_local = localtime(&now); // Returns a pointer to a tm structure with the time representation of the local time eg. tm_hour, tm_min, tm_sec, tm_mday, tm_mon, tm_year, tm_wday, tm_yday, tm_isdst

    tm_local->tm_year += 1900;                      // Years since 1900
    tm_local->tm_mon += 1;                          // Months since January
    tm_local->tm_hour += NAIROBI_GMT_OFFSET / 3600; // Nairobi GMT offset

    timeInfo.year = tm_local->tm_year;
    timeInfo.month = tm_local->tm_mon;
    timeInfo.day = tm_local->tm_mday;
    timeInfo.hours = tm_local->tm_hour;
    timeInfo.minutes = tm_local->tm_min;
    timeInfo.seconds = tm_local->tm_sec;

    full_date_time = asctime(tm_local); // Converts the tm structure to a string eg. "Sun Sep 16 01:03:52 1973\n"

    Serial.printf("Time: %s\n", full_date_time.c_str());
    return ntp_error_codes::NTP_CONFIG_OK;
};

NTP *NTP::get_default_instance()
{
    static NTP ntp("pool.ntp.org", 3 * 60 * 60, 0);
    return &ntp;
}
// ntp_error_codes NTP::printLocalTime(Struct timeinfo *timeCont) {

//     Serial.printf("%d-%02d-%02d %02d:%02d:%02d\n", timeCont->year, timeCont->month, timeCont->day, timeCont->hours, timeCont->minutes, timeCont->seconds);

//     return NTP_CONFIG_OK;

// };
