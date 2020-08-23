#include <TinyGPS++.h>
#include "GeoData.h"

class GPS 
{
    private:
        TinyGPSPlus gps;
        GeoData lastGoodData;
        TinyGPSTime _time;
        TinyGPSDate _date;

    public:
        GPS(); 
        void Setup();
        GeoData GetGeoData();
        long GetLocalSiderealTime();
        TinyGPSDate GetDate() { return _date; }
        TinyGPSTime GetTime() { return _time; }
        static const uint32_t GPSBaud = 9600;
};