
// Define our light controller argument structure
struct lightControllerArgs {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    char action[140];
    uint16_t delay;
};

// Define our callback type
typedef void (*Callback)(lightControllerArgs);

// Declare our scope's strip
Adafruit_WS2801 strip = NULL;

Ultrasonic ultrasounds[] = {
  Ultrasonic(7,8)
};

// Define the OMR logo's "m" pixel location as a range
uint8_t m_pixels[6] = { 0, 1, 2, 3, 4, 5 };

// Define the OMR sign's "background" pixel location as a range
uint16_t b_pixels[8] = { 6, 7, 8, 9, 10, 11, 12, 13 };
// Define an animation callback
Callback currentAnimation = NULL;

// Define our callback's args
lightControllerArgs currentAnimationArgs;

uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

// Allow us to set our strip in this controller from another scope
void setLightStrip(Adafruit_WS2801 &lightStrip) {
    strip = lightStrip;
}

// Set our current animation callback
void setCurrentAnimation(Callback animationLoopCallback) {
    currentAnimation = animationLoopCallback;
}

// Set our current animation callback's arguments
void setCurrentAnimationArgs(lightControllerArgs args) {
    currentAnimationArgs = args;
}

// Initialize our light strip
void initLightStrip(Adafruit_WS2801 &lightStrip) {
    // Set our scope's light strip
    setLightStrip(lightStrip);

    // Clear the light strip back to "black/blank"
    lightStrip.show();
}


/**
 * Light/Color functions go here
 */

// Change the color of the sign's background pixels
void changeColorBackgroundPixels(uint8_t R, uint8_t G, uint8_t B) {
    // Create our color from our RGB values
    uint32_t color = Color(R, G, B);

    // Get the length of the b_pixels array
    uint8_t b_pixels_length = sizeof(b_pixels)/sizeof(b_pixels[0]);

    // Loop through each pixel
    for (uint8_t i = 0; i < b_pixels_length; i++) {
        strip.setPixelColor(b_pixels[i], color);
    }

    // Show the things!
    strip.show();
}

// Change the color of the strip's pixels in the M (of the OMR logo)
void changeColorMPixels(uint8_t R, uint8_t G, uint8_t B) {
    // Create our color from our RGB values
    uint32_t color = Color(R, G, B);

    // Get the length of the m_pixels array
    uint8_t m_pixels_length = sizeof(m_pixels)/sizeof(m_pixels[0]);

    // Loop through each pixel in the "m"
    for (uint8_t i = 0; i < m_pixels_length; i++) {
        strip.setPixelColor(m_pixels[i], color);
    }

    // Show the things!
    strip.show();
}

// Animate the color of the sign's background pixels
void animateBackgroundPixels(char action[], uint8_t R, uint8_t G, uint8_t B, uint16_t delayTime = 0) {
    // Create our color from our RGB values
    uint32_t color = Color(R, G, B);

    // Create some reusable colors
    uint32_t blackColor = Color(0, 0, 0);

    // Get the length of the b_pixels array
    uint8_t b_pixels_length = sizeof(b_pixels)/sizeof(b_pixels[0]);

    // Turn the action into a String object (lower cased)
    String str_action = String(action);
    str_action.toLowerCase();

    // Holy hell, I hate C. Make sure to ALWAYS append a "\0" character to your "strings" (character arrays)
    if (str_action.equals("line-fill\0")) {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 50;
        }

        // Loop through each pixel in the "m"
        for (int16_t i = (b_pixels_length - 1); i >= 0; i--) {
            strip.setPixelColor(b_pixels[i], color);
            strip.show();
            delay(delayTime);
        }
        for (int16_t i = (b_pixels_length - 1); i >= 0; i--) {
            strip.setPixelColor(b_pixels[i], blackColor);
            strip.show();
            delay(delayTime);
        }

        for (uint16_t i = 0; i < b_pixels_length; i++) {
            strip.setPixelColor(b_pixels[i], color);
            strip.show();
            delay(delayTime);
        }
        for (uint16_t i = 0; i < b_pixels_length; i++) {
            strip.setPixelColor(b_pixels[i], blackColor);
            strip.show();
            delay(delayTime);
        }

    } else if (str_action.equals("snake\0")) {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 160;
        }

        // Loop through each pixel in the "m"
        for (int16_t i = b_pixels_length; i >= 0; i -= 2) {
            if (i != (b_pixels_length - 1)) {
                strip.setPixelColor(b_pixels[i], blackColor);
            }

            strip.setPixelColor(b_pixels[i - 1], color);
        }

        strip.show();
        delay(delayTime);

        for (int16_t i = (b_pixels_length - 1); i >= 0; i -= 2) {
            strip.setPixelColor(b_pixels[i], blackColor);
            strip.setPixelColor(b_pixels[i - 1], color);
        }

        strip.show();
        delay(delayTime);

    } else if (str_action.equals("pulse\0")) {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 1;
        }

        uint8_t red = R;
        uint8_t green = G;
        uint8_t blue = B;

        // Fade out
        while (red > 0 || green > 0 || blue > 0) {
            // Create our color from our RGB values
            uint32_t color = Color(red, green, blue);

            // Loop through each pixel
            for (uint8_t i = 0; i < b_pixels_length; i++) {
                strip.setPixelColor(b_pixels[i], color);
            }

            strip.show();

            // Decrement our values
            if (red > 0) {
                red--;
            }
            if (green > 0) {
                green--;
            }
            if (blue > 0) {
                blue--;
            }

            delay(delayTime);
        }

        // Fade in
        while (red < R || green < G || blue < B) {
            // Create our color from our RGB values
            uint32_t color = Color(red, green, blue);

            // Loop through each pixel
            for (uint8_t i = 0; i < b_pixels_length; i++) {
                strip.setPixelColor(b_pixels[i], color);
            }

            strip.show();

            // Decrement our values
            if (red < R) {
                red++;
            }
            if (green < G) {
                green++;
            }
            if (blue < B) {
                blue++;
            }

            delay(delayTime);
        }

    } else if (str_action.equals("sensor-mode\0")) {

        if (delayTime == 0) {
            delayTime = 1;
        }
        //Grab our first sensor
        Ultrasonic ultrasonic1 = ultrasounds[0];
        
        float cmMsec1;
        long microsec1 = ultrasonic1.timing();
        
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        
        const int numReadings = 10;

        int red_avg[numReadings];      
        int green_avg[numReadings];
        
        int index = 0;                  
        
        int total_red = 0;                  
        int total_green = 0;
        
        int average_red = 0;        
        int average_green = 0;
        
        for (int thisReading = 0; thisReading < numReadings; thisReading++){

          red_avg[thisReading] = 0;
          green_avg[thisReading] = 0;

        }
        
        total_red = (total_red - red_avg[index]);
        total_green = (total_green - green_avg[index]);

        cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM);
      
        if (cmMsec1 > 50.0) {
          red_avg[index] = 255;
          green_avg[index] = 0;
          blue = 0;
        } else if (cmMsec1 < 50.0 && cmMsec1 > 20.0) {
            red_avg[index] = (255/((cmMsec1/255)*cmMsec1));
            green_avg[index] = 255 - (255/((cmMsec1/255)*cmMsec1));
            blue = 0;
        } else {
            red_avg[index] = 0;
            green_avg[index] = (255/((cmMsec1/255)*cmMsec1));
            blue = 255 - (255/((cmMsec1/255)*cmMsec1));
         }
         
        total_red= total_red + red_avg[index];
        total_green= total_green + green_avg[index];
        
        index = index + 1;
        
        if (index >= numReadings)    {
        
        index = 0;
        
        }
        
        average_red = total_red / numReadings;
        average_green = total_green / numReadings;
        
        uint32_t color = Color(average_red, average_green, blue);
        
        for (uint8_t i = 0; i < b_pixels_length; i++) {
            strip.setPixelColor(b_pixels[i], color);
        }

        strip.show();
        delay(delayTime);

    } else if (str_action.equals("detect-mode\0")) {
        if (delayTime == 0) {
            delayTime = 1;
        }

        //Grab our first sensor
        Ultrasonic ultrasonic1 = ultrasounds[0];
        
        float cmMsec1;
        long microsec1 = ultrasonic1.timing();

        cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM);
      
        if (cmMsec1 < 20.0) {
            uint8_t red = R;
            uint8_t green = G;
            uint8_t blue = B;

            // Fade out
            while (red > 0 || green > 0 || blue > 0) {
                // Create our color from our RGB values
                uint32_t color = Color(red, green, blue);

                // Only light up one
                strip.setPixelColor(b_pixels[0], color);

                strip.show();

                // Decrement our values
                if (red > 0) {
                    red--;
                }
                if (green > 0) {
                    green--;
                }
                if (blue > 0) {
                    blue--;
                }

                delay(delayTime);
            }

            // Fade in
            while (red < R || green < G || blue < B) {
                // Create our color from our RGB values
                uint32_t color = Color(red, green, blue);

                // Only light up one
                strip.setPixelColor(b_pixels[0], color);

                strip.show();

                // Decrement our values
                if (red < R) {
                    red++;
                }
                if (green < G) {
                    green++;
                }
                if (blue < B) {
                    blue++;
                }

                delay(delayTime);
            }
        } else {
          
            uint32_t color = Color(0, 0, 0);

            // turn off
            strip.setPixelColor(b_pixels[0], color);
            
            strip.show();
            delay(delayTime);
        }

    } else {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 160;
        }

        // Setup our colors
        uint32_t redColor = Color(255, 0, 0);

        // Loop through each pixel in the "m"
        for (uint8_t j = 0; j < 2; j++) {
            for (int16_t i = (b_pixels_length - 1); i >= 0; i--) {
                strip.setPixelColor(b_pixels[i], redColor);
            }

            strip.show();
            delay(delayTime);

            for (int16_t i = (b_pixels_length - 1); i >= 0; i--) {
                strip.setPixelColor(b_pixels[i], blackColor);
            }

            strip.show();
            delay(delayTime);
        }
    }
}

// Animate the color of the strip's pixels in the M (of the OMR logo)
void animateMPixels(char action[], uint8_t R, uint8_t G, uint8_t B, uint16_t delayTime = 0) {
    // Create our color from our RGB values
    uint32_t color = Color(R, G, B);

    // Create some reusable colors
    uint32_t blackColor = Color(0, 0, 0);

    // Get the length of the m_pixels array
    uint8_t m_pixels_length = sizeof(m_pixels)/sizeof(m_pixels[0]);

    // Turn the action into a String object (lower cased)
    String str_action = String(action);
    str_action.toLowerCase();

    // Holy hell, I hate C. Make sure to ALWAYS append a "\0" character to your "strings" (character arrays)
    if (str_action.equals("line-fill\0")) {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 50;
        }

        // Loop through each pixel in the "m"
        for (int8_t i = (m_pixels_length - 1); i >= 0; i--) {
            strip.setPixelColor(m_pixels[i], color);
            strip.show();
            delay(delayTime);
        }
        for (int8_t i = (m_pixels_length - 1); i >= 0; i--) {
            strip.setPixelColor(m_pixels[i], blackColor);
            strip.show();
            delay(delayTime);
        }

        for (int8_t i = 0; i < m_pixels_length; i++) {
            strip.setPixelColor(m_pixels[i], color);
            strip.show();
            delay(delayTime);
        }
        for (int8_t i = 0; i < m_pixels_length; i++) {
            strip.setPixelColor(m_pixels[i], blackColor);
            strip.show();
            delay(delayTime);
        }

    } else if (str_action.equals("snake\0")) {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 160;
        }

        // Loop through each pixel in the "m"
        for (int8_t i = m_pixels_length; i >= 0; i -= 2) {
            if (i != m_pixels_length) {
                strip.setPixelColor(m_pixels[i], blackColor);
            }

            strip.setPixelColor(m_pixels[i - 1], color);
        }

        strip.show();
        delay(delayTime);

        for (int8_t i = (m_pixels_length - 1); i >= 0; i -= 2) {
            strip.setPixelColor(m_pixels[i], blackColor);

            if (i != 0) {
                strip.setPixelColor(m_pixels[i - 1], color);
            }
        }

        strip.show();
        delay(delayTime);

    } else if (str_action.equals("pulse\0")) {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 1;
        }

        uint8_t red = R;
        uint8_t green = G;
        uint8_t blue = B;

        // Fade out
        while (red > 0 || green > 0 || blue > 0) {
            // Create our color from our RGB values
            uint32_t color = Color(red, green, blue);

            // Loop through each pixel
            for (uint8_t i = 0; i < m_pixels_length; i++) {
                strip.setPixelColor(m_pixels[i], color);
            }

            strip.show();

            // Decrement our values
            if (red > 0) {
                red--;
            }
            if (green > 0) {
                green--;
            }
            if (blue > 0) {
                blue--;
            }

            delay(delayTime);
        }

        // Fade in
        while (red < R || green < G || blue < B) {
            // Create our color from our RGB values
            uint32_t color = Color(red, green, blue);

            // Loop through each pixel
            for (uint8_t i = 0; i < m_pixels_length; i++) {
                strip.setPixelColor(m_pixels[i], color);
            }

            strip.show();

            // Decrement our values
            if (red < R) {
                red++;
            }
            if (green < G) {
                green++;
            }
            if (blue < B) {
                blue++;
            }

            delay(delayTime);
        }

    } 
    else if (str_action.equals("sensor-mode\0")) {
        if (delayTime == 0) {
            delayTime = 1;
        }
        //Grab our first sensor
        Ultrasonic ultrasonic1 = ultrasounds[0];
        
        float cmMsec1;
        long microsec1 = ultrasonic1.timing();
        
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        
        const int numReadings = 10;

        int red_avg[numReadings];      
        int green_avg[numReadings];
        
        int index = 0;                  
        
        int total_red = 0;                  
        int total_green = 0;
        
        int average_red = 0;        
        int average_green = 0;
        
        for (int thisReading = 0; thisReading < numReadings; thisReading++){

          red_avg[thisReading] = 0;
          green_avg[thisReading] = 0;

        }
        
        total_red = (total_red - red_avg[index]);
        total_green = (total_green - green_avg[index]);

        cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM);
      
        if (cmMsec1 > 50.0) {
          red_avg[index] = 255;
          green_avg[index] = 0;
          blue = 0;
        } else if (cmMsec1 < 50.0 && cmMsec1 > 20.0) {
            red_avg[index] = (255/((cmMsec1/255)*cmMsec1));
            green_avg[index] = 255 - (255/((cmMsec1/255)*cmMsec1));
            blue = 0;
        } else {
            red_avg[index] = 0;
            green_avg[index] = (255/((cmMsec1/255)*cmMsec1));
            blue = 255 - (255/((cmMsec1/255)*cmMsec1));
         }
         
        total_red= total_red + red_avg[index];
        total_green= total_green + green_avg[index];
        
        index = index + 1;
        
        if (index >= numReadings)    {
        
        index = 0;
        
        }
        
        average_red = total_red / numReadings;
        average_green = total_green / numReadings;
        
        uint32_t color = Color(average_red, average_green, blue);
        
        for (uint8_t i = 0; i < m_pixels_length; i++) {
            strip.setPixelColor(m_pixels[i], color);
        }

        strip.show();
        delay(delayTime);

    } else if (str_action.equals("detect-mode\0")) {
        if (delayTime == 0) {
            delayTime = 1;
        }

        //Grab our first sensor
        Ultrasonic ultrasonic1 = ultrasounds[0];
        
        float cmMsec1;
        long microsec1 = ultrasonic1.timing();

        cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM);
      
        if (cmMsec1 < 40.0) {
            uint8_t red = R;
            uint8_t green = G;
            uint8_t blue = B;

            // Fade out
            while (red > 0 || green > 0 || blue > 0) {
                // Create our color from our RGB values
                uint32_t color = Color(red, green, blue);

                // Loop through each pixel
                for (uint8_t i = 0; i < m_pixels_length; i++) {
                    strip.setPixelColor(m_pixels[i], color);
                }

                strip.show();

                // Decrement our values
                if (red > 0) {
                    red--;
                }
                if (green > 0) {
                    green--;
                }
                if (blue > 0) {
                    blue--;
                }

                delay(delayTime);
            }

            // Fade in
            while (red < R || green < G || blue < B) {
                // Create our color from our RGB values
                uint32_t color = Color(red, green, blue);

                // Loop through each pixel
                for (uint8_t i = 0; i < m_pixels_length; i++) {
                    strip.setPixelColor(m_pixels[i], color);
                }

                strip.show();

                // Decrement our values
                if (red < R) {
                    red++;
                }
                if (green < G) {
                    green++;
                }
                if (blue < B) {
                    blue++;
                }

                delay(delayTime);
            }
        } else {
          
            uint32_t color = Color(0, 0, 0);

            // Turn off
            for (uint8_t i = 0; i < m_pixels_length; i++) {
                strip.setPixelColor(m_pixels[i], color);
                
                strip.show();
                delay(delayTime);
            }
        }

    } else {
        // Set a sensible default
        if (delayTime == 0) {
            delayTime = 160;
        }

        // Setup our colors
        uint32_t redColor = Color(255, 0, 0);

        // Loop through each pixel in the "m"
        for (uint8_t j = 0; j < 2; j++) {
            for (int8_t i = (m_pixels_length - 1); i >= 0; i--) {
                strip.setPixelColor(m_pixels[i], redColor);
            }

            strip.show();
            delay(delayTime);

            for (int8_t i = (m_pixels_length - 1); i >= 0; i--) {
                strip.setPixelColor(m_pixels[i], blackColor);
            }

            strip.show();
            delay(delayTime);
        }
    }
}

// Change the color of ALL of the strip's LEDs
void changeColorAll(uint8_t R, uint8_t G, uint8_t B) {
    // Call our separate functions
    changeColorBackgroundPixels(R, G, B);
    changeColorMPixels(R, G, B);
}

// Animate ALL of the strip's LEDs
void animateAll(char action[], uint8_t R, uint8_t G, uint8_t B, uint16_t delayTime = 0) {
    // Call our separate functions
    animateBackgroundPixels(action, R, G, B, delayTime);
    animateMPixels(action, R, G, B, delayTime);
}

// Get the color values as a string
char *getColorString(uint8_t R, uint8_t G, uint8_t B) {
    char colorString[100];

    sprintf(
        colorString,
        "Red: %d (%x)\r\nGreen: %d (%x)\r\nBlue: %d (%x)",
        R, R,
        G, G,
        B, B
    );

    return colorString;
}


/**
 * Struct arg versions go here
 */

void animateMPixels(lightControllerArgs args) {
    animateMPixels(args.action, args.red, args.green, args.blue, args.delay);
}

void animateBackgroundPixels(lightControllerArgs args) {
    animateBackgroundPixels(args.action, args.red, args.green, args.blue, args.delay);
}

void animateAll(lightControllerArgs args) {
    animateAll(args.action, args.red, args.green, args.blue, args.delay);
}

