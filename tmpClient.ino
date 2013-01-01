#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>
#define DHTPIN 2
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

// Your MAC address
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x25, 0x76 };

// Your IP here
IPAddress ip(192,168,1,32);

// fill in your Domain Name Server address here:
IPAddress myDns(8,8,8,8);

// initialize the library instance:
EthernetClient client;

// Your Device ID
String device =  ""; 
// API Server IP
char server[] = "188.141.4.12";

float temp = 0.0;
float humid = 0.0;


void setup() {
  // start serial port:
  Serial.begin(9600);
  // Set up the temp sensor
  dht.begin();
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, myDns);
}

void loop() {
  // if there's incoming data from the net connection. send it out the serial port.
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  readTemp();
  printTemp();

  // if you're not connected then connect again and send data:
  if(!client.connected()) {
    httpRequest();
  }

// Delay between data send
  delay(20000);
}

// this method makes a HTTP connection to the server:
void httpRequest() {
    
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");

    // send the HTTP PUT request:
    String get = "GET http://weatherbyte.com/?device=" + device;

    char tmp[10];
    char hum[10];
    dtostrf(temp,1,2,tmp);
    dtostrf(humid,1,2,hum);

    get = get + "&temperature=" + tmp + "&humidity=" + hum;

    Serial.println(get);
    client.println(get);
    client.stop();
  } 
  else {
    client.stop();
  }
}

void readTemp(){
  // Read the tempature 
  temp = dht.readTemperature();
  humid = dht.readHumidity();
}

void printTemp(){
  Serial.print("Humidity: "); 
  Serial.print(humid);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(temp);
  Serial.println(" *C");
}



