#ifndef _SERIAL_MODEM_GPS__H_
#define _SERIAL_MODEM_GPS__H_

class SerialModemGPS {
public:
  SerialModemGPS();

  void requestPosition();

private:

  bool _configured;
};

#endif // _SERIAL_MODEM_GPS__H_
