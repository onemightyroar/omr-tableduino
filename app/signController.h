

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_WS2801(18, 2, 3);
uint8_t delay_time = 10;

void signInit(){
  strip.begin();
  strip.show();
}

void changeColorAll(uint8_t R, uint8_t G, uint8_t B) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(R,G,B));
  }
      strip.show();
    delay(delay_time);
}
