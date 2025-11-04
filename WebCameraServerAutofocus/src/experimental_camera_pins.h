//Pins defined by users using OV5640 breakout boards or other boards
//Change pin-map to match your project.


#if defined(EXPERIMENTAL_CAMERA_PINS)  
       //Define the pin-map for the following sixteen (16) pins
#define RESET_GPIO_NUM   -1  //  -1 Only if not using reset
#define Y3_GPIO_NUM       9
#define Y5_GPIO_NUM      10
#define Y7_GPIO_NUM      18
#define Y9_GPIO_NUM      16
#define PCLK_GPIO_NUM    13
#define VSYNC_GPIO_NUM   6
#define SIOC_GPIO_NUM    5
#define PWDN_GPIO_NUM    15
#define Y2_GPIO_NUM      11
#define Y4_GPIO_NUM      8
#define Y6_GPIO_NUM      12
#define Y8_GPIO_NUM      17
#define XCLK_GPIO_NUM    -1  //-1 Only if using a breakout that provides an internal camera clock
#define HREF_GPIO_NUM    7
#define SIOD_GPIO_NUM    4


#endif