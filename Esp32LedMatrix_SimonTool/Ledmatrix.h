#define R1_PIN  25
#define G1_PIN  26
#define B1_PIN 27
#define R2_PIN  14
#define G2_PIN  12
#define B2_PIN  13

#define A_PIN  23
#define B_PIN  19
#define C_PIN  5
#define D_PIN 32
#define E_PIN  -1 // IMPORTANT: Change to a valid pin if using a 64x64px panel.

#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 33


#define PANEL_RES_X 64 // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32 // Number of pixels tall of each INDIVIDUAL panel module.

#define NUM_ROWS 1 // Number of rows of chained INDIVIDUAL PANELS
#define NUM_COLS 4 // Number of INDIVIDUAL PANELS per ROW


/******************************************************************************
   Create physical DMA panel class AND virtual (chained) display class.
 ******************************************************************************/
#include <ESP32-VirtualMatrixPanel-I2S-DMA.h>
RGB64x32MatrixPanel_I2S_DMA dma_display;
VirtualMatrixPanel          virtualDisp(dma_display, NUM_ROWS, NUM_COLS, PANEL_RES_X, PANEL_RES_Y, true);


/******************************************************************************
   Setup!
 ******************************************************************************/
void InitLedMatrix() {
  Serial.println("*****************************************************");
  Serial.println(" Led matrix Init !");
  Serial.println("*****************************************************");

#ifdef USE_CUSTOM_PINS
  dma_display.begin(R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN );  // setup the LED matrix
#else
  dma_display.begin();
#endif



  // Sanity checks
  if (NUM_ROWS <= 1) {
    Serial.println(F("There is no reason to use the VirtualDisplay class for a single horizontal chain and row!"));
  }

  if (dma_display.width() != NUM_ROWS * NUM_COLS * PANEL_RES_X )
  {
    Serial.println(F("\r\nERROR: MATRIX_WIDTH and/or MATRIX_HEIGHT in 'ESP32-RGB64x32MatrixPanel-I2S-DMA.h'\r\nis not configured correctly for the requested VirtualMatrixPanel dimensions!\r\n"));
    Serial.printf("WIDTH according dma_display is %d, but should be %d. Is your NUM_ROWS and NUM_COLS correct?\r\n", dma_display.width(), NUM_ROWS * NUM_COLS * PANEL_RES_X);
    return;
  }

  // So far so good, so continue
  //  virtualDisp.fillScreen(virtualDisp.color444(0, 0, 0));
  //  virtualDisp.drawDisplayTest(); // draw text numbering on each screen to check connectivity

  //  delay(30000);

  //  Serial.println("Chain of 64x32 panels for this example:");
  //  Serial.println("+--------+---------+");
  //  Serial.println("|   4    |    3    |");
  //  Serial.println("|        |         |");
  //  Serial.println("+--------+---------+");
  //  Serial.println("|   1    |    2    |");
  //  Serial.println("| (ESP)  |         |");
  //  Serial.println("+--------+---------+");

  //  virtualDisp.setFont(&FreeSansBold12pt7b);

  //    virtualDisp.drawRect(0, 0, dma_display.width(), dma_display.height(), dma_display.color444(15, 15, 0));

  //  virtualDisp.setTextSize(1);

  //     virtualDisp.setCursor(0, 1);
  //   virtualDisp.setTextColor(dma_display.color444(0, 0, 15));
  //     virtualDisp.println(" GATE BUS|INFORMATION ");
  //     virtualDisp.setCursor(0, 9);
  //     virtualDisp.setTextColor(dma_display.color444(15, 0, 0));
  //     virtualDisp.println(" START: ");
  //     virtualDisp.setTextColor(dma_display.color444(15, 0, 15));
  //     virtualDisp.setCursor(43, 9);
  //     virtualDisp.println("8H50P ");
  //   //  //  virtualDisp.println("123456789123456789123456789123456789123456") ; //4567891234567
  //     virtualDisp.setTextColor(dma_display.color444(15, 0, 0));
  //     virtualDisp.setCursor(0, 17);
  //     virtualDisp.println(" THANK YOU VERY MUCH   * DESIGN BY HAIMD *");

  //    virtualDisp.setCursor(64, 25);
  //    virtualDisp.println("***Design by HaiMD ***");

  //    uint8_t w =  MATRIX_WIDTH;
  //    uint8_t h =  MATRIX_HEIGHT;
  //      Serial.println(w);
  //      Serial.println(h);
  //      // Draw test
  //            for (int j = 0; j <= h-1; j++) { // giong tu 8-> 15 // h-1
  //              for (int i = 0; i <= 255; i++) { //w -1
  //                dma_display.drawPixel(i, j, dma_display.color444(15, 0, 15));
  //                Serial.printf("(%d,%d)", i, j);
  //                Serial.println("");
  //                delay(10);
  //                dma_display.drawPixel(i, j, dma_display.color444(0, 0, 0));
  //              }
  //            }


  //          dma_display.drawPixel(219, 31, dma_display.color444(15, 15, 0));
  //          dma_display.drawPixel(147, 0, dma_display.color444(15, 15, 0));
  //          dma_display.drawPixel(250, 31, dma_display.color444(15, 15, 15));
  //          dma_display.drawPixel(255, 31, dma_display.color444(15, 15, 15));


  //  Serial.printf("virtualDisp.width = %d ;virtualDisp.height= %d ", virtualDisp.width(), virtualDisp.height());



}
void ViewInfor(String msg ) {
  virtualDisp.setCursor(0, 17);
  virtualDisp.fillScreen(dma_display.color444(0, 0, 0));
  virtualDisp.setTextColor(dma_display.color444(15, 15, 0));
  virtualDisp.println(msg);
}
void TestView() {
  Serial.println("Test view");
  virtualDisp.setCursor(0, 1);
  virtualDisp.setTextColor(dma_display.color444(0, 0, 15));
  virtualDisp.println(" GATE BUS|INFORMATION ");
  virtualDisp.setCursor(0, 9);
  virtualDisp.setTextColor(dma_display.color444(15, 0, 0));
  virtualDisp.println(" START AT: ");
  virtualDisp.setTextColor(dma_display.color444(15, 0, 15));
  virtualDisp.setCursor(59, 9);
  virtualDisp.println("8H50P ");
  //  //  virtualDisp.println("123456789123456789123456789123456789123456") ; //4567891234567
  //    virtualDisp.setTextColor(dma_display.color444(15, 15, 0));
  //    virtualDisp.setCursor(0, 17);
  //    virtualDisp.println(" THANK YOU VERY MUCH   * DESIGN BY HAIMD *");
}
