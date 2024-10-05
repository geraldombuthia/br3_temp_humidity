#ifndef NTP_TIME_H
#define NTP_TIME_H

#include "time.h"
#include <Arduino.h>

#define NAIROBI_GMT_OFFSET 3 * 60 * 60 //sec. GMT+3
#define DAYLIGHT_OFFSET_SEC 0

struct timeInfo {
    int year;       // Years since 1900
    int month;      // Months since January - [0,11]
    int day;        // Day of the month - [1,31]
    int hours;      // hours since midnight - [0,23]
    int minutes;    // minutes after the hour - [0,59]
    int seconds;    // seconds after the minute - [0,59]

};
enum ntp_error_codes: unsigned int {
    NTP_CONFIG_OK,
    NTP_CONFIG_ERROR
};
/**
 * @brief NTP object that provides a synchronized mechanism to set internal RTC of ESP32
 * It is noted that the ESp32 has a wild time drift of upto 10-15 minutes within 8 hrs.
 * 
 */
class NTP {
    public:
    /**
     * @brief Constructor
     * @param ntpServer Server from which our ntp time is fetched
     * @param gmtOffset_sec Greenwich Mean Time Offset in Seconds
     * @param daylightOffset_sec Daylight saving time setting in seconds
     * 
     */
    NTP(const char* ntpServer ="pool.ntp.org", long gmtOffset_sec = NAIROBI_GMT_OFFSET, int daylightOffset_sec = DAYLIGHT_OFFSET_SEC);

    /**
     * @brief Configures the NTP server and sets the GMT offset and daylight offset
     * @return NTP_CONFIG_OK if successful or NTP_CONFIG_ERROR if not
     */
    ntp_error_codes configureNTP();

    /**
     * @brief Gets the epoch time from the NTP server
     * @param epoch_time variable to store the epoch time
     * @return NTP_CONFIG_OK if successful or NTP_CONFIG_ERROR if not
     */
    ntp_error_codes getEpochTime(long &epoch_time);

    /**
     * @brief Gets the current time from the NTP server
     * @param time variable to store the time
     * @return NTP_CONFIG_OK if successful or NTP_CONFIG_ERROR if not
     */
    ntp_error_codes getCurrentTime(String &time);

    /**
     * @brief Gets the current date from the NTP server
     * @param date variable to store the date
     * @return NTP_CONFIG_OK if successful or NTP_CONFIG_ERROR if not
     */
    ntp_error_codes getCurrentDate(String &date);

    /**
     * @brief Gets the full date and time from the NTP server
     * @param full_date_time variable to store the full date and time
     * @return NTP_CONFIG_OK if successful or NTP_CONFIG_ERROR if not
     */
    ntp_error_codes getFullDateTime(String &full_date_time);

    /**
     * @brief Prints the local time to the serial monitor
     * @return NTP_CONFIG_OK if successful or NTP_CONFIG_ERROR if not
     */
    ntp_error_codes printLocalTime(struct timeInfo *timeCont);

    /**
     * @brief Returns default ntp instance
     * 
     */

    static NTP* get_default_instance();
    /**
     * @brief Destructor
     * 
     */
    ~NTP();

    private:
    struct timeInfo timeInfo;
    time_t _epoch_time; // Stores last fetched epoch time
    const char* _ntpServer = ""; //NTP server to use
    long _gmtOffset_sec = 0;     //GMT offset in seconds
    int _daylightOffset_sec = 0;  //Daylight saving time offset in seconds

};

#endif // NTP_TIME_H