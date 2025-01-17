#ifndef OSAMD_DHT_READER_H_
#define OSAMD_DHT_READER_H_

#include <Arduino.h>
#include "Config.h"

#ifdef DHTTYPE
  #ifdef ESP32DEVKIT
    #include <DHTesp.h> // -> DHT sensor library for ESPx (by beegee_tokyo)
  #else
    #include <DHT.h> // -> DHT sensor library (by Adafruit)
  #endif
#endif

namespace s72 {

  // Vorwärtsdeklaration, weil sonst der Compiler meckert, dass er DHTEvent nicht kennt
  class DHTEvent;

    // Klassen, die auf DHT-Events reagieren möchten, müssen von DHTListener erben (und die Methoden implementieren)
  class DHTListener {
  public:
    DHTListener() {}
    virtual ~DHTListener() {}
    virtual void onEvent(DHTEvent &event) = 0;  // pure virtual, abstrakte Klasse
  };

  // Ein Event, der die ausgelesenen Daten des DHT-Sensors enthält
  class DHTEvent
  {
    friend class DHTReader; // nur der DHTReader darf neue Instanzen der Klasse erzeugen

  private:
    static DHTListener *listener[2]; // ACHTUNG! maximal zwei Listener (spart RAM)
    static void raise(byte const pin, float const temperature,float const humidity);

    DHTEvent(byte const pin, float const temperature, float const humidity) : pin(pin), temperature(temperature), humidity(humidity) {}
    DHTEvent() = default;

  public:
    static void registerListener(DHTListener &listener);
    //static void unregisterListener(DHTListener *listener);
    
    byte const pin;
    float const temperature;
    float const humidity;

    DHTEvent(const DHTEvent &) = delete; // no copying
    DHTEvent &operator=(const DHTEvent &) = delete;


  };

  // liest den DHT-Sensor aus und interpretiert dessen Werte
  class DHTReader
  {
  private:
    #ifdef DHTTYPE
      #ifdef ESP32DEVKIT
        DHTesp *dht;
      #else
        DHT *dht;
      #endif
    #endif
    DHTReader() = default;

    unsigned long lastReadTime = 0;
    float prev_temperature = 0.0;
    float prev_humidity = 0.0;

  public:

    static unsigned long const READ_DELAY_MS = 10000;  // der DHT22 darf nicht öfter als alle 2 Sekunden ausgelesen werden!

    byte const pin;
    unsigned long const readDelayMs;

    DHTReader(byte const pin);
    DHTReader(byte const pin, unsigned long const readDelayMs);

    DHTReader(const DHTReader &) = delete; // no copying
    DHTReader &operator=(const DHTReader &) = delete;

    void setup();
    void update();
  };

}
#endif
