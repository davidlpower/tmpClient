#include <DHT.h>

#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN 2
#define DHTTYPE DHT22 

DHT dht(DHTPIN, DHTTYPE);

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x25, 0x76 };

// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192,168,1,32);

// fill in your Domain Name Server address here:
IPAddress myDns(89,101,160,5);

// initialize the library instance:
EthernetClient client;

// Put your device ID here
String device =  "";
// Address to server
char server[] = "http://www.weatherbyte.com";

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
  
  // Delay before next reading and save
  delay(20000);
}

// this method makes a HTTP connection to the server:
void httpRequest() {

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");

    // send the HTTP PUT request:
    String get = "GET" + server + "/?device=" + device;

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




