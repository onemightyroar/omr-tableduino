/* Define Constants */
#define WEBDUINO_AUTH_REALM             "OMRTable"
#define WEBDUINO_FAIL_MESSAGE           ""
#define WEBDUINO_AUTH_MESSAGE           ""
#define WEBDUINO_SERVER_ERROR_MESSAGE   ""
#define WEBDUINO_FAVICON_DATA           ""


/* Include our external libraries */
#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"
#include "Adafruit_WS2801.h"
#include "Ultrasonic.h"


/**
 * Define our app credentials
 *
 * Should be base64 encoded in the HTTP Basic standard
 * ( see http://en.wikipedia.org/wiki/Basic_access_authentication#Client_side )
 */
#define HTTP_BASIC_CREDENTIALS          "dGVzdDp0ZXN0"


/* Include our OMRSign app code */
#include "OMRTable.h"


// Define our network addresses
static uint8_t mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xCC, 0x4F }; // Mac
static uint8_t ip[] = { 10, 1, 10, 151 }; // IP


// Create our webserver
WebServer webserver("", 80); // At root, port 80 (HTTP default)

// Create our LED light strip
Adafruit_WS2801 lightStrip = Adafruit_WS2801(18, 2, 3);

// Initializer
void setup()
{
    // Configure the Ethernet library with our network addresses
    Ethernet.begin(mac, ip);

    // Register the webserver's commands
    setupServerCommands(webserver);

    // Start the server
    webserver.begin();

    // Start the light strip
    lightStrip.begin();

    // Initialize our light strip's controller
    initLightStrip(lightStrip);
    
}


// Event loop (runs the lifetime of the device's power)
void loop()
{
    // Process each incoming connection
    webserver.processConnection();

    if (currentAnimation != NULL) {
        currentAnimation(currentAnimationArgs);
    }
}
