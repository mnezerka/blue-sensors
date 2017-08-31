#ifndef TIMEPROVIDER_H
#define TIMEPROVIDER_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"

// Singleton that provides methods for getting current time
class TimeProvider
{
    private:

        WiFiUDP *udp;

        // By default 'time.nist.gov' is used with 60 seconds update
        // interval and no offset
        NTPClient *timeClient;

        TimeProvider()
        {
            udp = new WiFiUDP();
            timeClient = new NTPClient(*udp, TIME_ZONE_OFFSET);
        }

    public:

        static TimeProvider& getInstance()
        {
            static TimeProvider instance; // Guaranteed to be destroyed.
                                          // Instantiated on first use.
            return instance;
        }

        // technique of deleting the methods we don't want.
        TimeProvider(TimeProvider const&) = delete;
        void operator=(TimeProvider const&)  = delete;
      
    void begin()
    {
        timeClient->begin();
    }

    void update()
    {
        timeClient->update();
        //Serial.println(timeClient->getFormattedTime());
    }

    String getTimeStr()
    {
        unsigned long rawTime = timeClient->getEpochTime();
        unsigned long hours = (rawTime % 86400L) / 3600;
        String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

        unsigned long minutes = (rawTime % 3600) / 60;
        String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

        return hoursStr + ":" + minuteStr;
    }

    unsigned long getUnixTimestamp()
    {
        return timeClient->getEpochTime();
    }

};

#endif
