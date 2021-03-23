// This #include statement was automatically added by the Particle IDE.
#include <MQTT.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_DHT.h>



#define DHTPIN 2                             // DHT Sensor connected to digital pin 2.
#define DHTTYPE DHT11
#define LIGHTPIN A0  

char mqttUserName[] = "TSPhotonMQTTDemo";    // Use any name.
char mqttPass[] = "E8QS5V7GEXL6NNRU";        // Change this to your MQTT API key from Account > MyProfile.
char writeAPIKey[] = "R8JVJ19MZT8QGMUW";     // Change this to your channel write API key.
long channelID = 1330736;                     // Change this to your channel number.
char server[] = "mqtt.thingspeak.com";       // Define the ThingSpeak MQTT broker.
static const char alphanum[] ="0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";                // This variable is used for random generation of the client ID.

void callback(char* topic, byte* payload, unsigned int length) {
 }

DHT dht(DHTPIN, DHTTYPE);                  // Initialize the DHT sensor.
MQTT client(server, 1883, callback);       // Initialize the MQTT client.

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 20L * 1000L; // Post data every 20 seconds. 

void setup() {
    
    pinMode(DHTPIN, INPUT);
    
}

void loop() {
    
    // If MQTT client is not connected then reconnect.
    if (!client.isConnected())
    {
      reconnect();
    }
    
    client.loop();  // Call the loop continuously to establish connection to the server.
    
    if (millis() - lastConnectionTime > postingInterval)
    {
        mqttpublish();
    }
}

void reconnect(){
    
     char clientID[9];
     
     Serial.println("Attempting MQTT connection");
        // Generate ClientID
        for (int i = 0; i < 8; i++) {
            clientID[i] = alphanum[random(51)];
        }
        clientID[8]='\0';
        
        // Connect to the ThingSpeak MQTT broker.
        if (client.connect(clientID,mqttUserName,mqttPass))  {
            Particle.publish("Conn:"+ String(server) + " cl: " + String(clientID)+ " Uname:" + String(mqttUserName));
        } else
        {
            Particle.publish("Failed to connect. Trying to reconnect in 5 seconds");
            delay(5000);
        } 
}

void mqttpublish() {
    
    float t = dht.getTempFarenheit(); // Read temperature from DHT sensor.
    float h = dht.getHumidity();  // Read humidity from DHT sensor.
    int lightLevel = analogRead(LIGHTPIN); // Read voltage from light sensor.
    
    // Create a data string to send data to ThingSpeak.
    // Use these lines to publish to a channel feed,
    // which allows multiple fields to be updated simultaneously.
    // Comment these lines and use the next two to publish to a single channel field directly.
    String data = String("field1=" + String(t) + "&field2=" + String(h) + "&field3=" + String(lightLevel));
    String topic = String("channels/"+String(channelID)+ "/publish/"+String(writeAPIKey));
    
    //String data = String(t);
    //String topic = String("channels/"+String(channelID)+ "/publish/fields/field1/"+String(writeAPIKey));
    
    client.publish(topic,data);
    
    lastConnectionTime = millis();
}