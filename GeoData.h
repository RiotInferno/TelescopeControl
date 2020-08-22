class DMS {
  public:
    int degree = -1;
    int minutes = -1;
    int seconds = -1;

    DMS(double degrees){
      degree = (int)degrees;
      double remainder = degrees - degree;
      double temp_minutes = 60 * remainder;
      minutes = (int)temp_minutes;
      remainder = temp_minutes - minutes;
      double temp_seconds = 60 * remainder;
      seconds = (int)temp_seconds;
    }

    DMS(){}
};

class GeoData {
  public:
    DMS LatDms;
    DMS LngDms;
    double Latitude = -1;
    double Longitude = -1;
    double CosPhi;
    double SinPhi;
  
    GeoData(double latitude, double longitude){
      LatDms = DMS(latitude);
      LngDms = DMS(longitude);
      Latitude = latitude;
      Longitude = longitude;
      const double phi = Latitude * PI / 180.0; 
      CosPhi = cos(phi);
      SinPhi = sin(phi);
    }

    GeoData(){}
};
