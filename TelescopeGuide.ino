#include "config.h"

void setup()
{
  //  TWBR = 12;  // 400 kbit/sec I2C speed
  Serial.begin(115200);

  // Set up the interrupt pin, its set as active high, push-pull
  pinMode(intPin, INPUT);
  digitalWrite(intPin, LOW);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);

  // Set up for data display
  display.begin();
  display.setPowerSave(0);
  display.setFont(u8x8_font_chroma48medium8_r);
}

void DoLX200Communication()
{
  int i = 0;
  input[i++] = Serial.read();
  delay(5);
  while ((input[i++] = Serial.read()) != '#')
  {
    if(input[i-1] == 0xFFFFFFFF){
      i--;
      break;
    }
    delay(5);
  }
  input[i] = '\0';

  if (input[1] == ':' && input[2] == 'G' && input[3] == 'R' && input[4] == '#')
  {
    Serial.print(txAR);
  }

  if (input[1] == ':' && input[2] == 'G' && input[3] == 'D' && input[4] == '#')
  {
    Serial.print(txDEC);
  }
  
  memset(input,0,sizeof(input));
}

void loop()
{
  GeoData geoData = gps.GetGeoData();
  mpu9250.ReadHeading();
  long altitude = mpu9250.GetAltitude();
  long azimuth = mpu9250.GetAzimuth();
  long lst = gps.GetLocalSiderealTime();
  
  AZ_to_EQ(altitude, azimuth, lst, geoData);

  if (Serial.available() > 0)
  {
    DoLX200Communication();
  }

  display.setCursor(0,0);
  display.println(micros());

  TinyGPSDate date = gps.GetDate();
  TinyGPSTime time = gps.GetTime();

  String separator = "-";
  display.print(date.year() + separator + date.month() + separator + date.day());
  separator = ":";
  display.println(time.hour() + separator + time.minute() + separator + time.second());

  display.print("(");
  display.print(geoData.Latitude);
  display.print("°, ");
  display.print(geoData.Longitude);
  display.println("°)");
  
  display.print("(");
  display.print(altitude);
  display.print(", ");
  display.print(azimuth);
  display.println("°)");
  
  display.println(lst);
  display.refreshDisplay();
}

//===================================================================================================================
//====== Set of useful function to access acceleration. gyroscope, magnetometer, and temperature data
//======================  =============================================================================================
void AZ_to_EQ(long altitude, long azimuth, long localSiderealTime, GeoData geoData)
{
  long AR_tel_s, DEC_tel_s;
  long AR_stell_s, DEC_stell_s;
  double delta_tel, sin_h, cos_h, sin_A, cos_A, sin_DEC, cos_DEC;
  double H_telRAD, h_telRAD, A_telRAD;
  long H_tel;
  long arHH, arMM, arSS;
  long decDEG, decMM, decSS;
  char sDEC_tel;

  A_telRAD = (azimuth/ 3600.0) * PI / 180.0;
  h_telRAD = (altitude / 3600.0) * PI / 180.0;
  sin_h = sin(h_telRAD);
  cos_h = cos(h_telRAD);
  sin_A = sin(A_telRAD);
  cos_A = cos(A_telRAD);
  delta_tel = asin((geoData.SinPhi * sin_h) + (geoData.CosPhi * cos_h * cos_A));
  sin_DEC = sin(delta_tel);
  cos_DEC = cos(delta_tel);
  DEC_tel_s = long((delta_tel * 180.0 / PI) * 3600.0);

  while (DEC_tel_s >= 324000)
  {
    DEC_tel_s = DEC_tel_s - 324000;
  }
  while (DEC_tel_s <= -324000)
  {
    DEC_tel_s = DEC_tel_s + 324000;
  }

  H_telRAD = acos((sin_h - (geoData.SinPhi * sin_DEC)) / (geoData.CosPhi * cos_DEC));
  H_tel = long((H_telRAD * 180.0 / PI) * 240.0);

  if (sin_A >= 0)
  {
    H_tel = 86400 - H_tel;
  }
  AR_tel_s = localSiderealTime - H_tel;

  while (AR_tel_s >= 86400)
  {
    AR_tel_s = AR_tel_s - 86400;
  }
  while (AR_tel_s < 0)
  {
    AR_tel_s = AR_tel_s + 86400;
  }

  arHH = AR_tel_s / 3600;
  arMM = (AR_tel_s - arHH * 3600) / 60;
  arSS = (AR_tel_s - arHH * 3600) - arMM * 60;
  decDEG = abs(DEC_tel_s) / 3600;
  decMM = (abs(DEC_tel_s) - decDEG * 3600) / 60;
  decSS = (abs(DEC_tel_s) - decDEG * 3600) - decMM * 60;
  (DEC_tel_s < 0) ? sDEC_tel = 45 : sDEC_tel = 43;

  sprintf(txAR, "%02d:%02d:%02d#", int(arHH), int(arMM), int(arSS));
  sprintf(txDEC, "%c%02d%c%02d:%02d#", sDEC_tel, int(decDEG), 223, int(decMM), int(decSS));
}