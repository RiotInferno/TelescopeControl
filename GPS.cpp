#include "GPS.h"
#include <SoftwareSerial.h>

GPS::GPS() {
  Serial1.begin(GPSBaud);
}

GeoData GPS::GetGeoData() {
    if (Serial1.available() > 0 
        && gps.encode(Serial1.read())
        && gps.location.isValid()
        && gps.location.isUpdated())
    {
        return GeoData(gps.location.lat(), gps.location.lng());
    }
    return GeoData();
}

long GPS::GetLocalSiderealTime()
{

  double M, Y, D, MN, H, S;
  double A, B, C, E, F;

  M = (int)gps.date.month();
  Y = (int)gps.date.year();
  D = (int)gps.date.day();
  MN = (int)gps.time.minute();
  H = (int)gps.time.hour();// Hours must be GMT/UT.
  S = (int)gps.time.second();

  if (M < 3) {
    M += 12;
    Y -= 1;
  }

  A = (long)Y / 100;
  B = (long)A / 4;
  C = (long)2 - A + B;
  E = (long)(365.25 * (Y + 4716));
  F = (long)(30.6001 * (M + 1));

  double LastJDN = C + D + E + F - 1524.5; // Julian day last midnight UT
  double Current_d = LastJDN - 2451545.0; //Days since Year 2000
  long Current_T = Current_d / 36525; // Julian centuries since Year 2000
  long NewJDN = LastJDN + H / 24; // Julian day today

  Serial.println();
  Serial.print("Julian Day Number..: ");
  Serial.print(NewJDN); // Julian day today

  double Term1 = 6.697374558; // this line must be a double!
  double Term2 = 0.06570982441908 * Current_d;

  H = H + ((double)MN / 60) + ((double)S / 3600);

  float GMST;
  float Term3;

  Term3 = 0.00273790935 * H;
  Term3 += H;
  GMST = Term1 + Term2 + Term3; // Terms are found at http://aa.usno.navy.mil/faq/docs/GAST.php

  //add on longitude divided by 15 to get LST

  double LST = GMST + (gps.location.lng()/ 15); // longitude as hour angle.

    return LST;
  //reduce it to 24 format
/*
  int LSTint;
  LSTint = (int)LST;
  LSTint /= 24;
  LST = LST - (double)LSTint * 24;

  int LST_H = (int)LST;
  int LST_M = ((LST - LST_H) * 60);
  int LST_S = (((LST - LST_H) * 60) - LST_M) * 60;
  */
}