#include <TinyGPS++.h>
#include "GeoData.h"

class GPS 
{
    private:
        static const uint32_t GPSBaud = 9600;
        TinyGPSPlus gps;

    public:
        GPS(); 
        GeoData GetGeoData();
        long GetLocalSiderealTime();
        TinyGPSDate GetDate() { gps.date; }
        TinyGPSTime GetTime() { gps.time; }
};