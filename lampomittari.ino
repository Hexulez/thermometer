#define BLYNK_TEMPLATE_ID "TMPL4_zFtp-aI"
#define BLYNK_TEMPLATE_NAME "SalaisiaAsioita"
#define BLYNK_AUTH_TOKEN "esXlcGXWQJBrXpgVQUbVAF7RUkxZglzV"

#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

// Blynk-yhteyden tiedot
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Hacknet";
char pass[] = "KailaVaiEi";
//char ssid[] = "4G-Gateway-E6F8";
//char pass[] = "EE5NTMT4TGB";

BlynkTimer ajastin;

int buttonState = 0; // Muuttuja Blynkin napin tilalle

// LED-pinnit
const int redPin = 5;      // Punainen LED (pin 5)
const int yellowPin = 4;   // Keltainen LED (pin 4)
const int greenPin = 3;    // Vihreä LED (pin 3)

void setup() {
  // LED-pinien määrittely ulostuloiksi
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // Alusta kaikki LEDit pois päältä
  digitalWrite(redPin, 0);
  digitalWrite(yellowPin, 0);
  digitalWrite(greenPin, 0);

  // Alusta Blynk-yhteys
  Blynk.begin(auth, ssid, pass);

  // Aseta ajastin kutsumaan 'sendSensorData' funktiota sekunnin välein
  ajastin.setInterval(1000L, sendSensorData);

  // Määritä analogisen lukemisen resoluutio, jos käytössä (esim. Arduino Due)
  analogReadResolution(12);
}

void loop() {
  Blynk.run();
  ajastin.run();
}

// Funktio lämpötilan lukemiseen ja tietojen lähettämiseen Blynkiin
double readTemperatureCelsius() {
  double analogValue = analogRead(A1);
  double V_out = 3.3 * (double)analogValue / 4095.0;
  double T_A = V_out / 0.010 - 273.15; // Muunna jännite Celsius-asteiksi
  return T_A;
}

double convertToFahrenheit(double celsius) {
  return (celsius * 9.0 / 5.0) + 32.0;
}

void sendSensorData() {
  double celsius = readTemperatureCelsius();
  double fahrenheit = convertToFahrenheit(celsius);

  // Lähetä lämpötila Blynkiin
  Blynk.virtualWrite(V5, celsius);      // Celsius
  Blynk.virtualWrite(V4, fahrenheit);   // Fahrenheit

  // Ohjaa LED-valoja lämpötilan perusteella
  if (buttonState == 0) { // Jos nappia ei ole painettu
    if (celsius >= 25 && celsius < 26 ) {
      digitalWrite(redPin, 1);      // Sytytä punainen LED (pin 5)
      digitalWrite(yellowPin, 0);
      digitalWrite(greenPin, 0);
    }
    else if (celsius >= 26 && celsius < 27) {
      digitalWrite(redPin, 1);
      digitalWrite(yellowPin, 1);   // Sytytä keltainen LED (pin 4)
      digitalWrite(greenPin, 0);
    }
    else if (celsius >= 27 )  {
      digitalWrite(redPin, 1);
      digitalWrite(yellowPin, 1);
      digitalWrite(greenPin, 1);    // Sytytä vihreä LED (pin 3)
    }
    else {
      digitalWrite(redPin, 0);
      digitalWrite(yellowPin, 0);
      digitalWrite(greenPin, 0); 
    }
  }
}

// Käsittele Blynkin nappia (V0)
BLYNK_WRITE(V0) {
  buttonState = param.asInt(); // 1 = nappi painettu, 0 = nappi vapaa

  if (buttonState == 1) {
    // Sytytä kaikki LEDit
    digitalWrite(redPin, 1);
    digitalWrite(yellowPin, 1);
    digitalWrite(greenPin, 1);
  }
  else {
    // Sammuta kaikki LEDit
    digitalWrite(redPin, 0);
    digitalWrite(yellowPin, 0);
    digitalWrite(greenPin, 0);
  }
}
