// The ledmatrix-max7219 SPI driver ported to Sparkcore https://github.com/digitalwizards/SparkCore-LEDMatrix-MAX7219-MAX7221
#include "application.h"
#include "ledmatrix-max7219-max7221.h"
#include "ClickButton.h"
#include "Timer.h"


// Some bit-level instruction that usually come with Arduino but not included with Sparkcore by default.
#ifndef bitRead
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#endif

// Control cloudconn
SYSTEM_MODE(MANUAL);

/* Function prototypes -------------------------------------------------------*/
int scrollText(String text);
void draw64(uint64_t image);
void debug(String text);
void slack(String slackstring);

/* Constants  ----------------------------------------------------------------*/
LEDMatrix *led;
int fontWidth = 5, space = 1;
static const uint64_t NUMBERS[] = {
  0x3c66666e76663c00,
  0x7e1818181c181800,
  0x7e060c3060663c00,
  0x3c66603860663c00,
  0x30307e3234383000,
  0x3c6660603e067e00,
  0x3c66663e06663c00,
  0x1818183030667e00,
  0x3c66663c66663c00,
  0x3c66607c66663c00
};

static const uint64_t question = 0x080008081022221c;

const uint64_t IMAGES[] = {
  0x183c7effffff6600,   //Heart
  0x000042ff2a1c0000,   //Car
  0x2424181a3c585840,   //Human
  0x010a0c1e3870e0c0,   //Sword
  0x7e4242ff7e3c1800,   //House
  0x3c4299a581a5423c,    //Smiley
  0x3c3c3c3cff7e3c18,    //Arrow
  0x060e0c0808281800,    //Music1
  0x066eecc88898f000,     //Music2
  0x0000663cff3c6600,     //Star
  0x001c27e4271c0000     //AND
};

NewTimer t;
int currenttimer = 0;
String notifymessage="";

// The button
const int buttonPin1 = 4;
ClickButton button1(buttonPin1, LOW, CLICKBTN_PULLUP);
int clickmode = 0;

void setup() {
    WiFi.off();
    // Turn LED to red to not cause distraction at night. We can turn it off fully if we want.
    //RGB.control(true);
    //RGB.color(50, 0, 0);

    // Set our button to INPUT_PULLUP - Pin is grounded when click.
    pinMode(D4, INPUT_PULLUP);
    button1.debounceTime   = 20;   // Debounce timer in ms
    button1.multiclickTime = 250;  // Time limit for multi clicks
    button1.longClickTime  = 1000; // time until "held-down clicks" register

    // create new library instance with one display
    led = new LEDMatrix();
    // Our debug USB Serial is here.
    Serial.begin(9600);
    // register display on the position [0;0] without rotatation or mirroring
    led->addMatrix(0, 0);

    // set brightness (0-15)
    led->setIntensity(7);

    // redraw display
    led->flush();

    // Declare IoT functions we can call from interweb.
    Particle.function("scrolltext", scrollText);
    debug("setup finished");
}

// drawText will use Adafruit_GFX method to draw a string.
void drawText(String s, int x)
{
  int y = 0;
  for(unsigned int i = 0; i < s.length(); i++) {
    // Adafruit_GFX method
    led->drawChar(x + i*(fontWidth+space), y, s[i], true, false, 1);
  }
}

int genrand(int minRand, int maxRand)
{
return rand() % (maxRand-minRand+1) + minRand;
}

// scrollText uses to scroll a string from left to right.
int scrollText(String text){
  debug("scrollText initiated");
  debug(text);
  int bitmapWidth = 8;
  int textLength = text.length();
  int textX = bitmapWidth;
  // text animation is ending when the whole text is outside the bitmap
  while(textX > textLength*(fontWidth+space)*(-1)){
    drawText(text, textX--);
    // draw text
    led->flush();
    delay(120);
  }
  return 1;
}

// draw a 8x8 matrix image we generated using editor here: https://xantorohara.github.io/led-matrix-editor
void draw64(uint64_t image) {
  debug("drawing images");
  led->fillScreen(false);
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      led->bitmap->setPixel(j,i, bitRead(row, j));
    }
  }
  led->flush();
}

// Send slack message to our channel.
void slack(String slackstring){
  WiFi.on();
  WiFi.setCredentials("WIFIESSID", "WIFI_PASSWORD");
  WiFi.connect();
  while(!WiFi.ready()){};
  debug("Connecting to cloud!");
  Particle.connect();
  waitUntil(Particle.connected);
  debug("Connected");
  Particle.process();
  delay(5000);
  if(Particle.publish("slackbutton", slackstring,60,PRIVATE)) debug("Message sent");
  Particle.process();
  //Particle.disconnect();
  WiFi.off();
}

// Send debut message.
void debug(String text){
  Serial.println(text);
}

void notify(void){
  debug("Notifying slack!");
  slack(notifymessage);
}

//Button pressed
void singleclick(void){
  debug("Single click");
  scrollText("Pee Pee!");
  int jackpot=genrand(0,11);
  for( int a = 0; a < 4 ; a++ ){
      draw64(IMAGES[jackpot]);
      delay(400);
      led->fillScreen(false);
      led->flush();
      delay(100);
  }
  notifymessage="OI! Jeremy did a WEE 10 mins ago, REMIND him NOW!";
  debug("Timer start!");
  t.stop(currenttimer);
  currenttimer=t.after(1000*60*10,notify);
  led->fillScreen(false);
  led->flush();
}

//Button pressed
void doubleclick(void){
  debug("Double click ");
  scrollText("Poo Poo!");
  int jackpot=random(0,11);
  for( int a = 0; a < 3 ; a++ ){
      draw64(IMAGES[jackpot]);
      delay(200);
      led->fillScreen(false);
      led->flush();
      delay(100);
  }
    debug("Timer start!");
    notifymessage="OI! Jeremy did a POO 10 mins ago, REMIND him NOW!";
    t.stop(currenttimer);
    currenttimer=t.after(1000*60*10,notify);
    led->fillScreen(false);
    led->flush();
}


// Our main loop
void loop(){
    t.update();
    button1.Update();
    if (button1.clicks != 0) clickmode = button1.clicks;

    if(clickmode == 1) singleclick();

    if(clickmode == 2) doubleclick();

    if(clickmode == 3) Serial.println("TRIPLE click");

    if(clickmode == -1) Serial.println("Long press");

    if(clickmode == -2) Serial.println("DOUBLE LONG click");

    if(clickmode == -3) Serial.println("TRIPLE LONG click");

    clickmode = 0;
    delay(5);
}
