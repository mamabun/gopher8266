#include <ESP8266WiFi.h>

//TODO: uSD card support
//      Credentials in file instead of hardcoded(needs uSD card support)

// Your network credentials
const char* ssid = "SSID";
const char* password = "WIFI_PASSWORD";

// Gopher protocol uses port 70
WiFiServer server(70);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Gopher server started on port 70.");
}

void loop() {
  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected.");

    // Read the client's request
    String request = "";
    while (client.connected() && client.available()) {
      char c = client.read();
      if (c != '\r' && c != '\n') {
        request += c;
      }
      if (c == '\n') {
        break;
      }
    }

    Serial.print("Client requested: ");
    Serial.println(request);

    //This will be replaced once uSD card support is added
    //This is a very messy way to do things but it works

    // Serve content based on the request
    if (request == "") {
      // Root directory request
      sendDirectory(client);
    } else if (request == "about.txt") {
      // File request for "about.txt"
      sendFile(client, "This Gopher server is running on an ESP8266.\n\nA clone of the LoLin node MCU to be exact.\n\nTODO:\nAdd uSD card support\nSupport credentials on uSD\nUpload source to github.");
    } 
      else if (request =="hello.txt") {
      // Modify this section to include a multiline string
      sendFile(client, "Hello, Gopher user!\n\nThis is a a small ESP8266 based server.\n\nNot much is here but enjoy exploring this tiny Gopher hole.");
  }
      else {
      // Handle "not found" or other requests
      sendError(client, "Item not found.");
    }

    // Give the client time to receive the data
    delay(1);

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  }
}

void sendDirectory(WiFiClient client) {
  // Send the directory listing
  String hostIP = WiFi.localIP().toString();
    client.println("iWelcome to my ESP8266 Gopher server.\t.\t" + hostIP + "\t70");
    client.println("0About this server\tabout.txt\t" + hostIP + "\t70");
    client.println("0Hello Gopher\thello.txt\t" + hostIP + "\t70");
    client.println("1QUUX Gopher Server\t\tgopher.quux.org\t70");
    client.println("1Gopher Project\t\tgopherproject.org\t70");
    client.println("."); // End of directory
}

void sendFile(WiFiClient client, String content) {
  // Send the content of a file
  client.println(content);
  client.println("."); // End of file
}

void sendError(WiFiClient client, String message) {
  // Send a Gopher error message
  String hostIP = WiFi.localIP().toString();
  client.println("3Error: " + message + "\t.\t" + hostIP + "\t70");
  client.println("."); // End of error
}
