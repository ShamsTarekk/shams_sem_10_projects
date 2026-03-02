////////////////////////////////////////old non working version
////////////////////
//////#include <stdio.h>
//////#include "platform.h"
//////#include "xil_printf.h"
//////#include "xparameters.h"
//////#include "XGpio.h"
//////#include "sleep.h"
//////
////
//////////////////
//////////////////int main()
//////////////////{
//////////////////    init_platform();
//////////////////
//////////////////    int status;
//////////////////
//////////////////    int8_t cells[36] = {
//////////////////        0,10,10,10,10,0,
//////////////////        0,10,10,10,10,0,
//////////////////        0,10,10,10,10,0,
//////////////////        0,10,10,10,10,0,
//////////////////        0,10,10,10,10,0,
//////////////////        0,10,10,10,10,0
//////////////////    };
//////////////////
//////////////////    int8_t filter[9] = {1,0,-1, 1,0,-1, 1,0,-1};
//////////////////
//////////////////    int32_t pixels[4][4];
//////////////////
//////////////////    XGpio in1,in2,in3,in4,out1,out2;
//////////////////
//////////////////    status  = XGpio_Initialize(&out1,   XPAR_AXI_GPIO_0_DEVICE_ID);//data in & pixel fifo read
//////////////////    status |= XGpio_Initialize(&out2, XPAR_AXI_GPIO_1_DEVICE_ID);//write filter & write cell
//////////////////    status |= XGpio_Initialize(&in1, XPAR_AXI_GPIO_2_DEVICE_ID);//pixel out & pixel fifo out
//////////////////    status |= XGpio_Initialize(&in2,   XPAR_AXI_GPIO_3_DEVICE_ID);//write & read
//////////////////    status |= XGpio_Initialize(&in3,   XPAR_AXI_GPIO_4_DEVICE_ID);//pixel fifo empty& pixel fifo full
//////////////////    status |= XGpio_Initialize(&in4,   XPAR_AXI_GPIO_5_DEVICE_ID);//pixel VALID
//////////////////
//////////////////    if(status != XST_SUCCESS){
//////////////////        xil_printf("GPIO Init Failed!\n\r");
//////////////////        return -1;
//////////////////    }
//////////////////
//////////////////    xil_printf("Initialization successful!\n\r");
//////////////////
//////////////////    // Set directions
//////////////////    XGpio_SetDataDirection(&out1,   1, 0x00);  // DATA in
//////////////////    XGpio_SetDataDirection(&out1,   2, 0x00);  // PIXEL FIFO READ in
//////////////////
//////////////////    XGpio_SetDataDirection(&out2, 1, 0x00);  // filter_write
//////////////////    XGpio_SetDataDirection(&out2, 2, 0x00);  // cell_write
//////////////////
//////////////////    XGpio_SetDataDirection(&in1, 1, 0xFF);  // pixel_OUT
//////////////////    XGpio_SetDataDirection(&in1, 2, 0xFF);  // pixel_FIFO_OUT
//////////////////
//////////////////    XGpio_SetDataDirection(&in2, 1, 0xFF);  // WRITE
//////////////////    XGpio_SetDataDirection(&in2, 2, 0xFF);  // READ
//////////////////
//////////////////    XGpio_SetDataDirection(&in3, 1, 0xFF);  // pixel_FIFO EMPTY
//////////////////    XGpio_SetDataDirection(&in3, 2, 0xFF);  // pixel_FIFO FULL
//////////////////
//////////////////    XGpio_SetDataDirection(&in4, 1, 0xFF);  // pixel valid
//////////////////
//////////////////    xil_printf("Loading Filter...\n\r");
//////////////////
//////////////////    for(int i=0; i<9; i++)
//////////////////    {
//////////////////        XGpio_DiscreteWrite(&out1, 1, (u32)(filter[i] & 0xFF));
//////////////////
//////////////////        XGpio_DiscreteWrite(&out2, 1, 1);  // write_filter = 1
//////////////////        usleep(5);
//////////////////        XGpio_DiscreteWrite(&out2, 1, 0);  // write_filter = 0
//////////////////
//////////////////        usleep(5);
//////////////////    }
//////////////////
//////////////////    xil_printf("Loading Cells...\n\r");
//////////////////
//////////////////    for(int i=0; i<36; i++)
//////////////////    {
//////////////////        XGpio_DiscreteWrite(&out1, 1, (u32)(cells[i] & 0xFF));
//////////////////
//////////////////        XGpio_DiscreteWrite(&out2, 2, 1);  // write_cell = 1
//////////////////        usleep(5);
//////////////////        XGpio_DiscreteWrite(&out2, 2, 0);  // write_cell = 0
//////////////////
//////////////////        usleep(5);
//////////////////    }
//////////////////
//////////////////    xil_printf("Reading Convolution Output...\n\r");
//////////////////
//////////////////    for(int i=0; i<4; i++)
//////////////////    {
//////////////////        for(int j=0; j<4; j++)
//////////////////        {
//////////////////            // Wait until pixel_valid becomes 1
//////////////////            while( XGpio_DiscreteRead(&in4, 1) == 0 );
//////////////////
//////////////////            // Pulse pixel_fifo_read
//////////////////            XGpio_DiscreteWrite(&out1, 2, 1);
//////////////////            usleep(5);
//////////////////            XGpio_DiscreteWrite(&out1, 2, 0);
//////////////////
//////////////////            // Wait for data to update (VERY IMPORTANT)
//////////////////            usleep(10);
//////////////////
//////////////////            // Read pixel output
//////////////////            int32_t value = (int32_t)XGpio_DiscreteRead(&in1, 1);
//////////////////
//////////////////            pixels[i][j] = value;
//////////////////
//////////////////            xil_printf("%d ", pixels[i][j]);
//////////////////        }
//////////////////        xil_printf("\n\r");
//////////////////    }
//////////////////
////////
////// //////////////////////////////TASK 1 WITH 1 OUTPUT 3X3
//////
//////#include "platform.h"
//////#include "xil_printf.h"
//////#include "xparameters.h"
//////#include "xgpio.h"
//////
//////XGpio Gpio0, Gpio1, Gpio2, Gpio3;
//////
//////static int32_t sign_extend_20bit(uint32_t raw)
//////{
//////    raw &= 0xFFFFF;
//////    if(raw & (1u << 19)) raw |= 0xFFF00000;
//////    return (int32_t)raw;
//////}
//////
//////int main()
//////{
//////    int status;
//////    init_platform();
//////
//////    status = XGpio_Initialize(&Gpio0, XPAR_AXI_GPIO_0_DEVICE_ID);
//////    status |= XGpio_Initialize(&Gpio1, XPAR_AXI_GPIO_1_DEVICE_ID);
//////    status |= XGpio_Initialize(&Gpio2, XPAR_AXI_GPIO_2_DEVICE_ID);
//////    status |= XGpio_Initialize(&Gpio3, XPAR_AXI_GPIO_3_DEVICE_ID);
//////
//////    if(status != XST_SUCCESS){
//////        xil_printf("GPIO Init Failed\n\r");
//////        return -1;
//////    }
//////
//////    // Directions
//////    XGpio_SetDataDirection(&Gpio0,1,0x0);
//////    XGpio_SetDataDirection(&Gpio0,2,0x0);
//////    XGpio_SetDataDirection(&Gpio1,1,0x0);
//////    XGpio_SetDataDirection(&Gpio1,2,0x0);
//////    XGpio_SetDataDirection(&Gpio2,1,0x0);
//////    XGpio_SetDataDirection(&Gpio2,2,0x0);
//////    XGpio_SetDataDirection(&Gpio3,1,0xFFFFFFFF);
//////    XGpio_SetDataDirection(&Gpio3,2,0xFFFFFFFF);
//////
//////    xil_printf("Starting full feature map computation\n\r");
//////
//////    // 6x6 activation
//////    int8_t activation[6][6] = {
//////        {0,10,10,10,10,0},
//////        {0,10,10,10,10,0},
//////        {0,10,10,10,10,0},
//////        {0,10,10,10,10,0},
//////        {0,10,10,10,10,0},
//////        {0,10,10,10,10,0}
//////    };
//////
//////    // 3x3 filter
//////    int8_t filter[3][3] = {
//////        {1,0,-1},
//////        {1,0,-1},
//////        {1,0,-1}
//////    };
//////
//////    // Slide window over 6x6 input
//////    for(int row=0; row<4; row++)
//////    {
//////        for(int col=0; col<4; col++)
//////        {
//////            int index = 0;
//////
//////            // Load 3x3 window
//////            for(int i=0;i<3;i++)
//////            {
//////                for(int j=0;j<3;j++)
//////                {
//////                    XGpio_DiscreteWrite(&Gpio0,1,
//////                        activation[row+i][col+j]);
//////
//////                    XGpio_DiscreteWrite(&Gpio1,1,
//////                        filter[i][j]);
//////
//////                    XGpio_DiscreteWrite(&Gpio2,1,index);
//////
//////                    XGpio_DiscreteWrite(&Gpio0,2,1);
//////                    XGpio_DiscreteWrite(&Gpio0,2,0);
//////
//////                    index++;
//////                }
//////            }
//////
//////            // Start convolution
//////            XGpio_DiscreteWrite(&Gpio1,2,1);
//////            XGpio_DiscreteWrite(&Gpio1,2,0);
//////
//////            // Wait done
//////            while((XGpio_DiscreteRead(&Gpio3,2) & 0x1) == 0);
//////
//////            // Read result
//////            uint32_t raw = XGpio_DiscreteRead(&Gpio3,1);
//////            int32_t result = sign_extend_20bit(raw);
//////
//////            xil_printf("%6d ", result);
//////
//////            // Clear done
//////            XGpio_DiscreteWrite(&Gpio2,2,1);
//////            XGpio_DiscreteWrite(&Gpio2,2,0);
//////        }
//////
//////        xil_printf("\n\r");
//////    }
//////
//////    xil_printf("Feature map complete\n\r");
//////
//////    cleanup_platform();
//////    return 0;
//////}
//////
//////
//////
/////////////////////////////////////TASK 1 WITH FULL FEATURE MAP OUTPUT 3X3
////////
//////////////#include "platform.h"
//////////////#include "xil_printf.h"
//////////////#include "xparameters.h"
//////////////#include "xgpio.h"
//////////////
//////////////// GPIO handles
//////////////XGpio Gpio0, Gpio1, Gpio2, Gpio3;
//////////////
//////////////// Sign-extend 20-bit result (if needed)
//////////////static int32_t sign_extend_20bit(uint32_t raw)
//////////////{
//////////////    raw &= 0xFFFFF;
//////////////    if(raw & (1u << 19)) raw |= 0xFFF00000;
//////////////    return (int32_t)raw;
//////////////}
//////////////
//////////////int main() {
//////////////    int status;
//////////////    init_platform();
//////////////
//////////////    // ------------------------
//////////////    // Initialize GPIOs
//////////////    // ------------------------
//////////////    status = XGpio_Initialize(&Gpio0, XPAR_AXI_GPIO_0_DEVICE_ID);
//////////////    status |= XGpio_Initialize(&Gpio1, XPAR_AXI_GPIO_1_DEVICE_ID);
//////////////    status |= XGpio_Initialize(&Gpio2, XPAR_AXI_GPIO_2_DEVICE_ID);
//////////////    status |= XGpio_Initialize(&Gpio3, XPAR_AXI_GPIO_3_DEVICE_ID);
//////////////
//////////////    if(status != XST_SUCCESS){
//////////////        xil_printf("GPIO Initialization Failed!\n\r");
//////////////        cleanup_platform();
//////////////        return -1;
//////////////    }
//////////////    xil_printf("GPIO Initialization Successful!\n\r");
//////////////
//////////////    // ------------------------
//////////////    // Set GPIO Directions
//////////////    // ------------------------
//////////////    XGpio_SetDataDirection(&Gpio0, 1, 0x0); // activation OUT
//////////////    XGpio_SetDataDirection(&Gpio0, 2, 0x0); // load_enable OUT
//////////////    XGpio_SetDataDirection(&Gpio1, 1, 0x0); // weights OUT
//////////////    XGpio_SetDataDirection(&Gpio1, 2, 0x0); // start OUT
//////////////    XGpio_SetDataDirection(&Gpio2, 1, 0x0); // load_index OUT
//////////////    XGpio_SetDataDirection(&Gpio2, 2, 0x0); // clear_done OUT
//////////////    XGpio_SetDataDirection(&Gpio3, 1, 0xFFFFFFFF); // result IN
//////////////    XGpio_SetDataDirection(&Gpio3, 2, 0xFFFFFFFF); // done IN
//////////////
//////////////    // ------------------------
//////////////    // Load cells and weights
//////////////    // ------------------------
//////////////    int8_t cells[9]   = {0,10,10,10,10,0,0,10,10};
//////////////    int8_t weights[9] = {1,0,-1,1,0,-1,1,0,-1};
//////////////
//////////////    for(int i=0; i<9; i++){
//////////////        XGpio_DiscreteWrite(&Gpio0, 1, (uint32_t)cells[i]);    // activation
//////////////        XGpio_DiscreteWrite(&Gpio2, 1, i);                     // load_index
//////////////        XGpio_DiscreteWrite(&Gpio1, 1, (uint32_t)weights[i]);  // weights
//////////////        XGpio_DiscreteWrite(&Gpio0, 2, 1); // pulse load_enable
//////////////        XGpio_DiscreteWrite(&Gpio0, 2, 0);
//////////////    }
//////////////
//////////////    // ------------------------
//////////////    // Start convolution
//////////////    // ------------------------
//////////////    XGpio_DiscreteWrite(&Gpio1, 2, 1); // start pulse
//////////////    XGpio_DiscreteWrite(&Gpio1, 2, 0);
//////////////
//////////////    // ------------------------
//////////////    // Wait for done
//////////////    // ------------------------
//////////////    while((XGpio_DiscreteRead(&Gpio3, 2) & 0x1) == 0);
//////////////
//////////////    // ------------------------
//////////////    // Read and print result
//////////////    // ------------------------
//////////////    uint32_t raw = XGpio_DiscreteRead(&Gpio3, 1);
//////////////    int32_t val = sign_extend_20bit(raw);
//////////////    xil_printf("Convolution result: %d\n\r", val);
//////////////
//////////////    // ------------------------
//////////////    // Clear done
//////////////    // ------------------------
//////////////    XGpio_DiscreteWrite(&Gpio2, 2, 1);
//////////////    XGpio_DiscreteWrite(&Gpio2, 2, 0);
//////////////
//////////////    cleanup_platform();
//////////////    return 0;
//////////////}
////////////
////////////
////////////
////////////
///////////////////////////////////TASK 1 WITH FULL FEATURE MAP OUTPUT 10X4
////////////
////////////#include "platform.h"
////////////#include "xil_printf.h"
////////////#include "xparameters.h"
////////////#include "xgpio.h"
////////////
////////////// GPIO handles
////////////XGpio Gpio0, Gpio1, Gpio2, Gpio3;
////////////
////////////// Sign-extend 20-bit result (if needed)
////////////static int32_t sign_extend_20bit(uint32_t raw)
////////////{
////////////    raw &= 0xFFFFF;
////////////    if(raw & (1u << 19)) raw |= 0xFFF00000;
////////////    return (int32_t)raw;
////////////}
////////////
////////////int main() {
////////////    int status;
////////////    init_platform();
////////////
////////////    // ------------------------
////////////    // Initialize GPIOs
////////////    // ------------------------
////////////    status = XGpio_Initialize(&Gpio0, XPAR_AXI_GPIO_0_DEVICE_ID);
////////////    status |= XGpio_Initialize(&Gpio1, XPAR_AXI_GPIO_1_DEVICE_ID);
////////////    status |= XGpio_Initialize(&Gpio2, XPAR_AXI_GPIO_2_DEVICE_ID);
////////////    status |= XGpio_Initialize(&Gpio3, XPAR_AXI_GPIO_3_DEVICE_ID);
////////////
////////////    if(status != XST_SUCCESS){
////////////        xil_printf("GPIO Initialization Failed!\n\r");
////////////        cleanup_platform();
////////////        return -1;
////////////    }
////////////    xil_printf("GPIO Initialization Successful!\n\r");
////////////
////////////    // ------------------------
////////////    // Set GPIO Directions
////////////    // ------------------------
////////////    XGpio_SetDataDirection(&Gpio0, 1, 0x0); // activation OUT
////////////    XGpio_SetDataDirection(&Gpio0, 2, 0x0); // load_enable OUT
////////////    XGpio_SetDataDirection(&Gpio1, 1, 0x0); // weights OUT
////////////    XGpio_SetDataDirection(&Gpio1, 2, 0x0); // start OUT
////////////    XGpio_SetDataDirection(&Gpio2, 1, 0x0); // load_index OUT
////////////    XGpio_SetDataDirection(&Gpio2, 2, 0x0); // clear_done OUT
////////////    XGpio_SetDataDirection(&Gpio3, 1, 0xFFFFFFFF); // result IN
////////////    XGpio_SetDataDirection(&Gpio3, 2, 0xFFFFFFFF); // done IN
////////////
////////////    // ------------------------
////////////    // 12x6 Input Activation
////////////    // ------------------------
////////////    int8_t input[12][6] = {
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0},
////////////        {0,10,10,10,10,0}
////////////    };
////////////
////////////    // ------------------------
////////////    // 10x4 Filter
////////////    // ------------------------
////////////    int8_t filter[10][4] = {
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1},
////////////        {1,0,-1,1}
////////////    };
////////////
////////////    // ------------------------
////////////    // Sliding window convolution
////////////    // ------------------------
////////////    int out_h = 12 - 10 + 1; // 3
////////////    int out_w = 6 - 4 + 1;   // 3
////////////
////////////    xil_printf("Starting full feature map computation (10x4 filter)\n\r");
////////////
////////////    for(int row = 0; row < out_h; row++){
////////////        for(int col = 0; col < out_w; col++){
////////////            int index = 0;
////////////
////////////            for(int i = 0; i < 10; i++){     // filter height
////////////                for(int j = 0; j < 4; j++){  // filter width
////////////                    XGpio_DiscreteWrite(&Gpio0, 1, (uint32_t)input[row+i][col+j]); // activation
////////////                    XGpio_DiscreteWrite(&Gpio1, 1, (uint32_t)filter[i][j]);        // weight
////////////                    XGpio_DiscreteWrite(&Gpio2, 1, index);                         // load_index
////////////
////////////                    // Pulse load_enable
////////////                    XGpio_DiscreteWrite(&Gpio0, 2, 1);
////////////                    XGpio_DiscreteWrite(&Gpio0, 2, 0);
////////////
////////////                    index++;
////////////                }
////////////            }
////////////
////////////            // Start convolution
////////////            XGpio_DiscreteWrite(&Gpio1, 2, 1);
////////////            XGpio_DiscreteWrite(&Gpio1, 2, 0);
////////////
////////////            // Wait done
////////////            while((XGpio_DiscreteRead(&Gpio3, 2) & 0x1) == 0);
////////////
////////////            // Read result
////////////            uint32_t raw = XGpio_DiscreteRead(&Gpio3, 1);
////////////            int32_t result = sign_extend_20bit(raw);
////////////            xil_printf("%6d ", result);
////////////
////////////            // Clear done
////////////            XGpio_DiscreteWrite(&Gpio2, 2, 1);
////////////            XGpio_DiscreteWrite(&Gpio2, 2, 0);
////////////        }
////////////        xil_printf("\n\r");
////////////    }
////////////
////////////    xil_printf("Feature map complete\n\r");
////////////
////////////    cleanup_platform();
////////////    return 0;
////////////}
////////////
////////////
////////
/////////////////////////////////TASK 2 WITH SOFTWARE CONV 1 AND CONV 2 10x4
////////
////////////#include <stdio.h>
////////////#include <stdint.h>
////////////#include <stddef.h>
////////////#include <string.h>
////////////#include <limits.h>
////////////
////////////#include "platform.h"   // from Vitis Hello World template
////////////
////////////#include "conv1_weights.h"
////////////#include "conv1_bias.h"
////////////#include "conv2_weights.h"
////////////#include "conv2_bias.h"
////////////#include "dense1_weights.h"
////////////#include "dense1_bias.h"
////////////#include "dense2_weights.h"
////////////#include "dense2_bias.h"
////////////
/////////////* ---------------------- Model dimensions (from your headers) ---------------------- */
////////////#define IN_H    49
////////////#define IN_W    10
////////////#define IN_C    1
////////////
////////////#define C1_KH   10
////////////#define C1_KW   4
////////////#define C1_OC   28
////////////#define C1_SH   1
////////////#define C1_SW   1
////////////#define C1_OH   (((IN_H - C1_KH)/C1_SH) + 1)   /* 40 */
////////////#define C1_OW   (((IN_W - C1_KW)/C1_SW) + 1)   /* 7  */
////////////
////////////#define C2_KH   10
////////////#define C2_KW   4
////////////#define C2_IC   C1_OC
////////////#define C2_OC   30
////////////#define C2_SH   2
////////////#define C2_SW   1
////////////#define C2_OH   (((C1_OH - C2_KH)/C2_SH) + 1)  /* 16 */
////////////#define C2_OW   (((C1_OW - C2_KW)/C2_SW) + 1)  /* 4  */
////////////
////////////#define P_KH    2
////////////#define P_KW    4
////////////#define P_SH    2
////////////#define P_SW    4
////////////#define P_OH    (((C2_OH - P_KH)/P_SH) + 1)    /* 8 */
////////////#define P_OW    (((C2_OW - P_KW)/P_SW) + 1)    /* 1 */
////////////
////////////#define FLAT_LEN   (P_OH * P_OW * C2_OC)       /* 240 */
////////////#define D1_OUT     128
////////////#define D2_OUT     12
////////////#define NUM_CLASSES D2_OUT
////////////
/////////////* ---------------------- Activation scaling (tune if needed) ---------------------- */
/////////////* These are approximate requantization shifts since quant params are not exported. */
////////////#define CONV1_SHIFT   7
////////////#define CONV2_SHIFT   8
////////////#define DENSE1_SHIFT  7
////////////
////////////#define LEN(arr) ((int)(sizeof(arr)/sizeof((arr)[0])))
////////////
/////////////* Compile-time size checks */
////////////typedef char assert_conv1_w[(LEN(conv1_w) == (C1_OC * C1_KH * C1_KW * IN_C)) ? 1 : -1];
////////////typedef char assert_conv2_w[(LEN(conv2_w) == (C2_OC * C2_KH * C2_KW * C2_IC)) ? 1 : -1];
////////////typedef char assert_dense1_w[(LEN(dense1_w) == (FLAT_LEN * D1_OUT)) ? 1 : -1];
////////////typedef char assert_dense2_w[(LEN(dense2_w) == (D1_OUT * D2_OUT)) ? 1 : -1];
////////////typedef char assert_dense2_b[(LEN(dense2_b) == D2_OUT) ? 1 : -1];
////////////
/////////////* ---------------------- Buffers ---------------------- */
////////////static int8_t  input_x[IN_H][IN_W];               /* 49x10 input feature map (MFCC/log-mel style) */
////////////static int8_t  act1[C1_OH][C1_OW][C1_OC];         /* 40x7x28 */
////////////static int8_t  act2[C2_OH][C2_OW][C2_OC];         /* 16x4x30 */
////////////static int8_t  actp[P_OH][P_OW][C2_OC];           /* 8x1x30 */
////////////static int8_t  flat[FLAT_LEN];                    /* 240 */
////////////static int8_t  d1_out[D1_OUT];                    /* 128 */
////////////static int32_t logits[D2_OUT];                    /* 12 final scores */
////////////static int32_t dense1_b_fixed[D1_OUT];            /* padded bias if header is incomplete */
////////////
/////////////* Common Speech Commands (12-class) labels. Replace if your model uses different labels. */
////////////static const char *labels[NUM_CLASSES] = {
////////////    "silence", "unknown", "yes", "no", "up", "down",
////////////    "left", "right", "on", "off", "stop", "go"
////////////};
////////////
/////////////* ---------------------- Helpers ---------------------- */
////////////static inline int8_t sat_i8_relu_shift(int32_t x, int shift)
////////////{
////////////    if (x < 0) x = 0;             /* ReLU */
////////////    if (shift > 0) x >>= shift;   /* approximate requantization */
////////////    if (x > 127) x = 127;
////////////    return (int8_t)x;
////////////}
////////////
////////////static void fix_dense1_bias(void)
////////////{
////////////    int i;
////////////    for (i = 0; i < D1_OUT; i++) dense1_b_fixed[i] = 0;
////////////
////////////    /* Your dense1_bias.h currently has only 16 values; copy what exists, pad rest with 0 */
////////////    int n = LEN(dense1_b);
////////////    if (n > D1_OUT) n = D1_OUT;
////////////    for (i = 0; i < n; i++) dense1_b_fixed[i] = dense1_b[i];
////////////}
////////////
/////////////* Demo input so the program runs immediately.
//////////// * Replace this with your real 49x10 features exported from Python. */
////////////static void load_demo_input(void)
////////////{
////////////    int h, w;
////////////    for (h = 0; h < IN_H; h++) {
////////////        for (w = 0; w < IN_W; w++) {
////////////            int v = (h * 3 + w * 7) % 40;   /* deterministic pattern */
////////////            input_x[h][w] = (int8_t)(v - 20);
////////////        }
////////////    }
////////////}
////////////
/////////////* Optional: paste your real feature matrix here
//////////// * static void load_real_input(void) {
//////////// *     int8_t sample[49][10] = {
//////////// *         {...}, {...}, ...
//////////// *     };
//////////// *     memcpy(input_x, sample, sizeof(sample));
//////////// * }
//////////// */
////////////
////////////static void conv1_layer(void)
////////////{
////////////    int oh, ow, oc, kh, kw;
////////////    for (oh = 0; oh < C1_OH; ++oh) {
////////////        for (ow = 0; ow < C1_OW; ++ow) {
////////////            for (oc = 0; oc < C1_OC; ++oc) {
////////////                int32_t acc = conv1_b[oc];
////////////
////////////                for (kh = 0; kh < C1_KH; ++kh) {
////////////                    for (kw = 0; kw < C1_KW; ++kw) {
////////////                        int ih = oh * C1_SH + kh;
////////////                        int iw = ow * C1_SW + kw;
////////////
////////////                        /* conv1_w layout assumed [out][kh][kw][in=1] */
////////////                        int widx = (((oc * C1_KH) + kh) * C1_KW) + kw;
////////////
////////////                        acc += (int32_t)input_x[ih][iw] * (int32_t)conv1_w[widx];
////////////                    }
////////////                }
////////////
////////////                act1[oh][ow][oc] = sat_i8_relu_shift(acc, CONV1_SHIFT);
////////////            }
////////////        }
////////////    }
////////////}
////////////
////////////static void conv2_layer(void)
////////////{
////////////    int oh, ow, oc, kh, kw, ic;
////////////    for (oh = 0; oh < C2_OH; ++oh) {
////////////        for (ow = 0; ow < C2_OW; ++ow) {
////////////            for (oc = 0; oc < C2_OC; ++oc) {
////////////                int32_t acc = conv2_b[oc];
////////////
////////////                for (kh = 0; kh < C2_KH; ++kh) {
////////////                    for (kw = 0; kw < C2_KW; ++kw) {
////////////                        int ih = oh * C2_SH + kh;
////////////                        int iw = ow * C2_SW + kw;
////////////
////////////                        for (ic = 0; ic < C2_IC; ++ic) {
////////////                            /* conv2_w layout assumed [out][kh][kw][in] */
////////////                            int widx = ((((oc * C2_KH) + kh) * C2_KW + kw) * C2_IC) + ic;
////////////
////////////                            acc += (int32_t)act1[ih][iw][ic] * (int32_t)conv2_w[widx];
////////////                        }
////////////                    }
////////////                }
////////////
////////////                act2[oh][ow][oc] = sat_i8_relu_shift(acc, CONV2_SHIFT);
////////////            }
////////////        }
////////////    }
////////////}
////////////
////////////static void maxpool_2x4_s2x4(void)
////////////{
////////////    int oh, ow, c, kh, kw;
////////////    for (oh = 0; oh < P_OH; ++oh) {
////////////        for (ow = 0; ow < P_OW; ++ow) {
////////////            for (c = 0; c < C2_OC; ++c) {
////////////                int maxv = -128;
////////////
////////////                for (kh = 0; kh < P_KH; ++kh) {
////////////                    for (kw = 0; kw < P_KW; ++kw) {
////////////                        int ih = oh * P_SH + kh;
////////////                        int iw = ow * P_SW + kw;
////////////                        int v  = act2[ih][iw][c];
////////////                        if (v > maxv) maxv = v;
////////////                    }
////////////                }
////////////
////////////                actp[oh][ow][c] = (int8_t)maxv;
////////////            }
////////////        }
////////////    }
////////////}
////////////
////////////static void flatten_pooled(void)
////////////{
////////////    int h, w, c, idx = 0;
////////////    for (h = 0; h < P_OH; ++h) {
////////////        for (w = 0; w < P_OW; ++w) {
////////////            for (c = 0; c < C2_OC; ++c) {
////////////                flat[idx++] = actp[h][w][c];
////////////            }
////////////        }
////////////    }
////////////}
////////////
////////////static void dense1_layer(void)
////////////{
////////////    int o, i;
////////////    for (o = 0; o < D1_OUT; ++o) {
////////////        int32_t acc = dense1_b_fixed[o];
////////////
////////////        for (i = 0; i < FLAT_LEN; ++i) {
////////////            /* dense1_w layout assumed [out][in] */
////////////            int widx = o * FLAT_LEN + i;
////////////            acc += (int32_t)flat[i] * (int32_t)dense1_w[widx];
////////////        }
////////////
////////////        d1_out[o] = sat_i8_relu_shift(acc, DENSE1_SHIFT);
////////////    }
////////////}
////////////
////////////static void dense2_layer(void)
////////////{
////////////    int o, i;
////////////    for (o = 0; o < D2_OUT; ++o) {
////////////        int32_t acc = dense2_b[o];
////////////
////////////        for (i = 0; i < D1_OUT; ++i) {
////////////            /* dense2_w layout assumed [out][in] */
////////////            int widx = o * D1_OUT + i;
////////////            acc += (int32_t)d1_out[i] * (int32_t)dense2_w[widx];
////////////        }
////////////
////////////        logits[o] = acc;
////////////    }
////////////}
////////////
////////////static int argmax_logits(void)
////////////{
////////////    int i, best_i = 0;
////////////    int32_t best_v = logits[0];
////////////
////////////    for (i = 1; i < D2_OUT; ++i) {
////////////        if (logits[i] > best_v) {
////////////            best_v = logits[i];
////////////            best_i = i;
////////////        }
////////////    }
////////////    return best_i;
////////////}
////////////
////////////static void print_logits(void)
////////////{
////////////    int i;
////////////    printf("Logits:\r\n");
////////////    for (i = 0; i < D2_OUT; ++i) {
////////////        printf("  [%2d] %-8s : %ld\r\n", i, labels[i], (long)logits[i]);
////////////    }
////////////}
////////////
////////////int main(void)
////////////{
////////////    init_platform();
////////////
////////////    printf("\r\n=== Task 2: KWS CNN Inference (Vitis) ===\r\n");
////////////    printf("Input      : %dx%dx%d\r\n", IN_H, IN_W, IN_C);
////////////    printf("Conv1 out   : %dx%dx%d\r\n", C1_OH, C1_OW, C1_OC);
////////////    printf("Conv2 out   : %dx%dx%d\r\n", C2_OH, C2_OW, C2_OC);
////////////    printf("Pool out    : %dx%dx%d\r\n", P_OH, P_OW, C2_OC);
////////////    printf("Flatten     : %d\r\n", FLAT_LEN);
////////////    printf("Dense1 out  : %d\r\n", D1_OUT);
////////////    printf("Dense2 out  : %d\r\n", D2_OUT);
////////////
////////////    fix_dense1_bias();
////////////
////////////    if (LEN(dense1_b) != D1_OUT) {
////////////        printf("WARNING: dense1_bias.h has %d entries, expected %d. Padding remaining with zeros.\r\n",
////////////               LEN(dense1_b), D1_OUT);
////////////    }
////////////
////////////    /* For now run with a demo input so everything compiles/runs immediately */
////////////    load_demo_input();
////////////
////////////    /* If you have Python-exported 49x10 test features, use that instead:
////////////       load_real_input();
////////////    */
////////////
////////////    conv1_layer();
////////////    conv2_layer();
////////////    maxpool_2x4_s2x4();
////////////    flatten_pooled();
////////////    dense1_layer();
////////////    dense2_layer();
////////////
////////////    int pred = argmax_logits();
////////////
////////////    printf("\r\nPrediction index = %d\r\n", pred);
////////////    if (pred >= 0 && pred < NUM_CLASSES) {
////////////        printf("Predicted label  = %s\r\n", labels[pred]);
////////////    }
////////////
////////////    print_logits();
////////////    printf("=== Done ===\r\n");
////////////
////////////    cleanup_platform();
////////////    return 0;
////////////}
////////////
////////////
////////////
////////////
////////////
//////////
////////
/////////////////////////////////TASK 2 WITH RTL CONV 1 AND CONV 2 10x4
////
////#include <stdio.h>
////#include <stdint.h>
////#include <stddef.h>
////#include <string.h>
////#include <limits.h>
////
////#include "platform.h"
////#include "xil_printf.h"
////#include "xparameters.h"
////#include "xgpio.h"
////
////#include "conv1_weights.h"
////#include "conv1_bias.h"
////#include "conv2_weights.h"
////#include "conv2_bias.h"
////#include "dense1_weights.h"
////#include "dense1_bias.h"
////#include "dense2_weights.h"
////#include "dense2_bias.h"
////
/////* ---------------------- Model dimensions ---------------------- */
////#define IN_H    49
////#define IN_W    10
////#define IN_C    1
////
////#define C1_KH   10
////#define C1_KW   4
////#define C1_OC   28
////#define C1_SH   1
////#define C1_SW   1
////#define C1_OH   (((IN_H - C1_KH)/C1_SH) + 1)
////#define C1_OW   (((IN_W - C1_KW)/C1_SW) + 1)
////
////#define C2_KH   10
////#define C2_KW   4
////#define C2_IC   C1_OC
////#define C2_OC   30
////#define C2_SH   2
////#define C2_SW   1
////#define C2_OH   (((C1_OH - C2_KH)/C2_SH) + 1)
////#define C2_OW   (((C1_OW - C2_KW)/C2_SW) + 1)
////
////#define P_KH    2
////#define P_KW    4
////#define P_SH    2
////#define P_SW    4
////#define P_OH    (((C2_OH - P_KH)/P_SH) + 1)
////#define P_OW    (((C2_OW - P_KW)/P_SW) + 1)
////
////#define FLAT_LEN   (P_OH * P_OW * C2_OC)
////#define D1_OUT     128
////#define D2_OUT     12
////#define NUM_CLASSES D2_OUT
////
////#define CONV1_SHIFT   7
////#define CONV2_SHIFT   8
////#define DENSE1_SHIFT  7
////
////#define LEN(arr) ((int)(sizeof(arr)/sizeof((arr)[0])))
////
/////* ---------------------- Buffers ---------------------- */
////static int8_t  input_x[IN_H][IN_W];
////static int8_t  act1[C1_OH][C1_OW][C1_OC];
////static int8_t  act2[C2_OH][C2_OW][C2_OC];
////static int8_t  actp[P_OH][P_OW][C2_OC];
////static int8_t  flat[FLAT_LEN];
////static int8_t  d1_out[D1_OUT];
////static int32_t logits[D2_OUT];
////static int32_t dense1_b_fixed[D1_OUT];
////
/////* ---------------------- GPIOs for hardware conv ---------------------- */
////XGpio Gpio0, Gpio1, Gpio2, Gpio3;
////
/////* ---------------------- Labels ---------------------- */
////static const char *labels[NUM_CLASSES] = {
////    "silence", "unknown", "yes", "no", "up", "down",
////    "left", "right", "on", "off", "stop", "go"
////};
////
/////* ---------------------- Helpers ---------------------- */
////static inline int8_t sat_i8_relu_shift(int32_t x, int shift)
////{
////    if(x<0) x=0;
////    if(shift>0) x>>=shift;
////    if(x>127) x=127;
////    return (int8_t)x;
////}
////
////static void fix_dense1_bias(void)
////{
////    int i;
////    for(i=0;i<D1_OUT;i++) dense1_b_fixed[i]=0;
////    int n = LEN(dense1_b);
////    if(n>D1_OUT) n=D1_OUT;
////    for(i=0;i<n;i++) dense1_b_fixed[i]=dense1_b[i];
////}
////
////static void load_demo_input(void)
////{
////    int h,w;
////    for(h=0;h<IN_H;h++)
////        for(w=0;w<IN_W;w++)
////            input_x[h][w] = (int8_t)(((h*3+w*7)%40)-20);
////}
////
/////* ---------------------- Hardware Conv1 ---------------------- */
////static void conv1_layer_hw(void)
////{
////    int oh, ow, oc, kh, kw, index;
////    xil_printf("Running Conv1 on HW...\r\n");
////
////    for(oh=0; oh<C1_OH; oh++){
////        for(ow=0; ow<C1_OW; ow++){
////            for(oc=0; oc<C1_OC; oc++){
////                index=0;
////                for(kh=0; kh<C1_KH; kh++){
////                    for(kw=0; kw<C1_KW; kw++){
////                        XGpio_DiscreteWrite(&Gpio0,1,(uint32_t)input_x[oh+kh][ow+kw]);
////                        XGpio_DiscreteWrite(&Gpio1,1,(uint32_t)conv1_w[index]);
////                        XGpio_DiscreteWrite(&Gpio2,1,index);
////
////                        XGpio_DiscreteWrite(&Gpio0,2,1);
////                        XGpio_DiscreteWrite(&Gpio0,2,0);
////                        index++;
////                    }
////                }
////                XGpio_DiscreteWrite(&Gpio1,2,1);
////                XGpio_DiscreteWrite(&Gpio1,2,0);
////
////                while((XGpio_DiscreteRead(&Gpio3,2)&0x1)==0);
////
////                uint32_t raw = XGpio_DiscreteRead(&Gpio3,1);
////                int32_t result = raw & 0xFFFFF;
////                if(result & (1<<19)) result |= 0xFFF00000;
////
////                act1[oh][ow][oc] = sat_i8_relu_shift(result, CONV1_SHIFT);
////
////                XGpio_DiscreteWrite(&Gpio2,2,1);
////                XGpio_DiscreteWrite(&Gpio2,2,0);
////            }
////        }
////    }
////}
////
/////* ---------------------- Hardware Conv2 ---------------------- */
////static void conv2_layer_hw(void)
////{
////    int oh, ow, oc, kh, kw, ic, index;
////    xil_printf("Running Conv2 on HW...\r\n");
////
////    for(oh=0; oh<C2_OH; oh++){
////        for(ow=0; ow<C2_OW; ow++){
////            for(oc=0; oc<C2_OC; oc++){
////                index=0;
////                for(kh=0; kh<C2_KH; kh++){
////                    for(kw=0; kw<C2_KW; kw++){
////                        for(ic=0; ic<C2_IC; ic++){
////                            XGpio_DiscreteWrite(&Gpio0,1,(uint32_t)act1[oh* C2_SH + kh][ow*C2_SW + kw]);
////                            XGpio_DiscreteWrite(&Gpio1,1,(uint32_t)conv2_w[index]);
////                            XGpio_DiscreteWrite(&Gpio2,1,index);
////
////                            XGpio_DiscreteWrite(&Gpio0,2,1);
////                            XGpio_DiscreteWrite(&Gpio0,2,0);
////                            index++;
////                        }
////                    }
////                }
////                XGpio_DiscreteWrite(&Gpio1,2,1);
////                XGpio_DiscreteWrite(&Gpio1,2,0);
////
////                while((XGpio_DiscreteRead(&Gpio3,2)&0x1)==0);
////
////                uint32_t raw = XGpio_DiscreteRead(&Gpio3,1);
////                int32_t result = raw & 0xFFFFF;
////                if(result & (1<<19)) result |= 0xFFF00000;
////
////                act2[oh][ow][oc] = sat_i8_relu_shift(result, CONV2_SHIFT);
////
////                XGpio_DiscreteWrite(&Gpio2,2,1);
////                XGpio_DiscreteWrite(&Gpio2,2,0);
////            }
////        }
////    }
////}
////
/////* ---------------------- Remaining layers (software) ---------------------- */
////static void maxpool_2x4_s2x4(void)
////{
////    int oh, ow, c, kh, kw;
////    for(oh=0; oh<P_OH; oh++)
////        for(ow=0; ow<P_OW; ow++)
////            for(c=0; c<C2_OC; c++){
////                int maxv=-128;
////                for(kh=0; kh<P_KH; kh++)
////                    for(kw=0; kw<P_KW; kw++){
////                        int ih = oh*P_SH+kh;
////                        int iw = ow*P_SW+kw;
////                        if(act2[ih][iw][c]>maxv) maxv=act2[ih][iw][c];
////                    }
////                actp[oh][ow][c]=(int8_t)maxv;
////            }
////}
////
////static void flatten_pooled(void)
////{
////    int idx=0,h,w,c;
////    for(h=0;h<P_OH;h++)
////        for(w=0;w<P_OW;w++)
////            for(c=0;c<C2_OC;c++)
////                flat[idx++]=actp[h][w][c];
////}
////
////static void dense1_layer(void)
////{
////    int o,i;
////    for(o=0;o<D1_OUT;o++){
////        int32_t acc = dense1_b_fixed[o];
////        for(i=0;i<FLAT_LEN;i++){
////            acc += flat[i]*dense1_w[o*FLAT_LEN+i];
////        }
////        d1_out[o] = sat_i8_relu_shift(acc,DENSE1_SHIFT);
////    }
////}
////
////static void dense2_layer(void)
////{
////    int o,i;
////    for(o=0;o<D2_OUT;o++){
////        int32_t acc=dense2_b[o];
////        for(i=0;i<D1_OUT;i++){
////            acc += d1_out[i]*dense2_w[o*D1_OUT+i];
////        }
////        logits[o]=acc;
////    }
////}
////
////static int argmax_logits(void)
////{
////    int i,best_i=0;
////    int32_t best_v=logits[0];
////    for(i=1;i<D2_OUT;i++)
////        if(logits[i]>best_v){best_v=logits[i];best_i=i;}
////    return best_i;
////}
////
////static void print_logits(void)
////{
////    for(int i=0;i<D2_OUT;i++)
////        printf("[%2d] %-8s : %ld\r\n", i, labels[i], (long)logits[i]);
////}
////
/////* ---------------------- Main ---------------------- */
////int main(void)
////{
////    int status;
////    init_platform();
////
////    /* Initialize GPIOs for HW convs */
////    status = XGpio_Initialize(&Gpio0,XPAR_AXI_GPIO_0_DEVICE_ID);
////    status |= XGpio_Initialize(&Gpio1,XPAR_AXI_GPIO_1_DEVICE_ID);
////    status |= XGpio_Initialize(&Gpio2,XPAR_AXI_GPIO_2_DEVICE_ID);
////    status |= XGpio_Initialize(&Gpio3,XPAR_AXI_GPIO_3_DEVICE_ID);
////    if(status!=XST_SUCCESS){ xil_printf("GPIO init failed!\r\n"); cleanup_platform(); return -1; }
////
////    /* Set directions */
////    XGpio_SetDataDirection(&Gpio0,1,0x0); XGpio_SetDataDirection(&Gpio0,2,0x0);
////    XGpio_SetDataDirection(&Gpio1,1,0x0); XGpio_SetDataDirection(&Gpio1,2,0x0);
////    XGpio_SetDataDirection(&Gpio2,1,0x0); XGpio_SetDataDirection(&Gpio2,2,0x0);
////    XGpio_SetDataDirection(&Gpio3,1,0xFFFFFFFF); XGpio_SetDataDirection(&Gpio3,2,0xFFFFFFFF);
////
////    fix_dense1_bias();
////    load_demo_input();
////
////    /* Run hardware convs */
////    conv1_layer_hw();
////    conv2_layer_hw();
////
////    maxpool_2x4_s2x4();
////    flatten_pooled();
////    dense1_layer();
////    dense2_layer();
////
////    int pred = argmax_logits();
////    printf("\r\nPrediction index = %d\r\n", pred);
////    printf("Predicted label = %s\r\n", labels[pred]);
////    print_logits();
////
////    cleanup_platform();
////    return 0;
////}
////
////
//
//
//#include <stdio.h>
//#include <stdint.h>
//#include <stddef.h>
//#include <string.h>
//#include <limits.h>
//
//#include "platform.h"
//#include "xtime_l.h"        // <<< For timing
//
//#include "conv1_weights.h"
//#include "conv1_bias.h"
//#include "conv2_weights.h"
//#include "conv2_bias.h"
//#include "dense1_weights.h"
//#include "dense1_bias.h"
//#include "dense2_weights.h"
//#include "dense2_bias.h"
//
///* ---------------------- Model dimensions ---------------------- */
//#define IN_H    49
//#define IN_W    10
//#define IN_C    1
//
//#define C1_KH   10
//#define C1_KW   4
//#define C1_OC   28
//#define C1_SH   1
//#define C1_SW   1
//#define C1_OH   (((IN_H - C1_KH)/C1_SH) + 1)
//#define C1_OW   (((IN_W - C1_KW)/C1_SW) + 1)
//
//#define C2_KH   10
//#define C2_KW   4
//#define C2_IC   C1_OC
//#define C2_OC   30
//#define C2_SH   2
//#define C2_SW   1
//#define C2_OH   (((C1_OH - C2_KH)/C2_SH) + 1)
//#define C2_OW   (((C1_OW - C2_KW)/C2_SW) + 1)
//
//#define P_KH    2
//#define P_KW    4
//#define P_SH    2
//#define P_SW    4
//#define P_OH    (((C2_OH - P_KH)/P_SH) + 1)
//#define P_OW    (((C2_OW - P_KW)/P_SW) + 1)
//
//#define FLAT_LEN   (P_OH * P_OW * C2_OC)
//#define D1_OUT     128
//#define D2_OUT     12
//#define NUM_CLASSES D2_OUT
//
//#define CONV1_SHIFT   7
//#define CONV2_SHIFT   8
//#define DENSE1_SHIFT  7
//
///* ---------------------- Buffers ---------------------- */
//static int8_t  input_x[IN_H][IN_W];
//static int8_t  act1[C1_OH][C1_OW][C1_OC];
//static int8_t  act2[C2_OH][C2_OW][C2_OC];
//static int8_t  actp[P_OH][P_OW][C2_OC];
//static int8_t  flat[FLAT_LEN];
//static int8_t  d1_out[D1_OUT];
//static int32_t logits[D2_OUT];
//static int32_t dense1_b_fixed[D1_OUT];
//
///* ---------------------- Labels ---------------------- */
//static const char *labels[NUM_CLASSES] = {
//    "silence", "unknown", "yes", "no", "up", "down",
//    "left", "right", "on", "off", "stop", "go"
//};
//
///* ---------------------- Helpers ---------------------- */
//static inline int8_t sat_i8_relu_shift(int32_t x, int shift)
//{
//    if (x < 0) x = 0;
//    if (shift > 0) x >>= shift;
//    if (x > 127) x = 127;
//    return (int8_t)x;
//}
//
//static void fix_dense1_bias(void)
//{
//    int i;
//    for (i = 0; i < D1_OUT; i++) dense1_b_fixed[i] = 0;
//    int n = sizeof(dense1_b)/sizeof(dense1_b[0]);
//    if (n > D1_OUT) n = D1_OUT;
//    for (i = 0; i < n; i++) dense1_b_fixed[i] = dense1_b[i];
//}
//
//static void load_demo_input(void)
//{
//    int h, w;
//    for (h = 0; h < IN_H; h++)
//        for (w = 0; w < IN_W; w++)
//            input_x[h][w] = (int8_t)((h * 3 + w * 7) % 40 - 20);
//}
//
///* ---------------------- CNN Layers ---------------------- */
//static void conv1_layer(void)
//{
//    int oh, ow, oc, kh, kw;
//    for (oh = 0; oh < C1_OH; ++oh)
//        for (ow = 0; ow < C1_OW; ++ow)
//            for (oc = 0; oc < C1_OC; ++oc) {
//                int32_t acc = conv1_b[oc];
//                for (kh = 0; kh < C1_KH; ++kh)
//                    for (kw = 0; kw < C1_KW; ++kw) {
//                        int ih = oh + kh;
//                        int iw = ow + kw;
//                        int widx = (((oc * C1_KH) + kh) * C1_KW) + kw;
//                        acc += input_x[ih][iw] * conv1_w[widx];
//                    }
//                act1[oh][ow][oc] = sat_i8_relu_shift(acc, CONV1_SHIFT);
//            }
//}
//
//static void conv2_layer(void)
//{
//    int oh, ow, oc, kh, kw, ic;
//    for (oh = 0; oh < C2_OH; ++oh)
//        for (ow = 0; ow < C2_OW; ++ow)
//            for (oc = 0; oc < C2_OC; ++oc) {
//                int32_t acc = conv2_b[oc];
//                for (kh = 0; kh < C2_KH; ++kh)
//                    for (kw = 0; kw < C2_KW; ++kw)
//                        for (ic = 0; ic < C2_IC; ++ic) {
//                            int ih = oh * C2_SH + kh;
//                            int iw = ow + kw;
//                            int widx = ((((oc * C2_KH) + kh) * C2_KW + kw) * C2_IC) + ic;
//                            acc += act1[ih][iw][ic] * conv2_w[widx];
//                        }
//                act2[oh][ow][oc] = sat_i8_relu_shift(acc, CONV2_SHIFT);
//            }
//}
//
//static void maxpool_2x4_s2x4(void)
//{
//    int oh, ow, c, kh, kw;
//    for (oh = 0; oh < P_OH; ++oh)
//        for (ow = 0; ow < P_OW; ++ow)
//            for (c = 0; c < C2_OC; ++c) {
//                int maxv = -128;
//                for (kh = 0; kh < P_KH; ++kh)
//                    for (kw = 0; kw < P_KW; ++kw) {
//                        int ih = oh * P_SH + kh;
//                        int iw = ow * P_SW + kw;
//                        int v = act2[ih][iw][c];
//                        if (v > maxv) maxv = v;
//                    }
//                actp[oh][ow][c] = (int8_t)maxv;
//            }
//}
//
//static void flatten_pooled(void)
//{
//    int h, w, c, idx = 0;
//    for (h = 0; h < P_OH; ++h)
//        for (w = 0; w < P_OW; ++w)
//            for (c = 0; c < C2_OC; ++c)
//                flat[idx++] = actp[h][w][c];
//}
//
//static void dense1_layer(void)
//{
//    int o, i;
//    for (o = 0; o < D1_OUT; ++o) {
//        int32_t acc = dense1_b_fixed[o];
//        for (i = 0; i < FLAT_LEN; ++i)
//            acc += flat[i] * dense1_w[o * FLAT_LEN + i];
//        d1_out[o] = sat_i8_relu_shift(acc, DENSE1_SHIFT);
//    }
//}
//
//static void dense2_layer(void)
//{
//    int o, i;
//    for (o = 0; o < D2_OUT; ++o) {
//        int32_t acc = dense2_b[o];
//        for (i = 0; i < D1_OUT; ++i)
//            acc += d1_out[i] * dense2_w[o * D1_OUT + i];
//        logits[o] = acc;
//    }
//}
//
//static int argmax_logits(void)
//{
//    int i, best = 0;
//    for (i = 1; i < D2_OUT; ++i)
//        if (logits[i] > logits[best])
//            best = i;
//    return best;
//}
//
//static void print_logits(void)
//{
//    int i;
//    printf("Logits:\r\n");
//    for (i = 0; i < D2_OUT; ++i)
//        printf(" [%2d] %-8s : %ld\r\n", i, labels[i], (long)logits[i]);
//}
//
///* ========================= MAIN ========================= */
//
//int main(void)
//{
//    init_platform();
//
//    XTime tStart, tEnd;
//    u64 cycles;
//    double time_ms;
//
//    printf("\r\n=== Task 2: KWS CNN Inference ===\r\n");
//
//    fix_dense1_bias();
//    load_demo_input();
//
//    /* ===== START TIMING ===== */
//    XTime_GetTime(&tStart);
//
//    conv1_layer();
//    conv2_layer();
//    maxpool_2x4_s2x4();
//    flatten_pooled();
//    dense1_layer();
//    dense2_layer();
//
//    XTime_GetTime(&tEnd);
//    /* ===== END TIMING ===== */
//
//    cycles = (u64)(tEnd - tStart);
//    time_ms = (double)cycles * 1000.0 / COUNTS_PER_SECOND;
//
//    int pred = argmax_logits();
//
//    printf("\r\nPrediction index = %d\r\n", pred);
//    if (pred >= 0 && pred < NUM_CLASSES)
//        printf("Predicted label  = %s\r\n", labels[pred]);
//
////    print_logits();
////    printf("CPU cycles       = %llu\r\n", cycles);
//    printf("Processing Time  = %.6f ms\r\n", time_ms);
//
//    cleanup_platform();
//    return 0;
//}

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xtime_l.h"   // For timing

#include "conv1_weights.h"
#include "conv1_bias.h"
#include "conv2_weights.h"
#include "conv2_bias.h"
#include "dense1_weights.h"
#include "dense1_bias.h"
#include "dense2_weights.h"
#include "dense2_bias.h"

/* ---------------------- Model dimensions ---------------------- */
#define IN_H    49
#define IN_W    10
#define IN_C    1

#define C1_KH   10
#define C1_KW   4
#define C1_OC   28
#define C1_SH   1
#define C1_SW   1
#define C1_OH   (((IN_H - C1_KH)/C1_SH) + 1)
#define C1_OW   (((IN_W - C1_KW)/C1_SW) + 1)

#define C2_KH   10
#define C2_KW   4
#define C2_IC   C1_OC
#define C2_OC   30
#define C2_SH   2
#define C2_SW   1
#define C2_OH   (((C1_OH - C2_KH)/C2_SH) + 1)
#define C2_OW   (((C1_OW - C2_KW)/C2_SW) + 1)

#define P_KH    2
#define P_KW    4
#define P_SH    2
#define P_SW    4
#define P_OH    (((C2_OH - P_KH)/P_SH) + 1)
#define P_OW    (((C2_OW - P_KW)/P_SW) + 1)

#define FLAT_LEN   (P_OH * P_OW * C2_OC)
#define D1_OUT     128
#define D2_OUT     12
#define NUM_CLASSES D2_OUT

#define CONV1_SHIFT   7
#define CONV2_SHIFT   8
#define DENSE1_SHIFT  7

#define LEN(arr) ((int)(sizeof(arr)/sizeof((arr)[0])))

/* ---------------------- Buffers ---------------------- */
static int8_t  input_x[IN_H][IN_W];
static int8_t  act1[C1_OH][C1_OW][C1_OC];
static int8_t  act2[C2_OH][C2_OW][C2_OC];
static int8_t  actp[P_OH][P_OW][C2_OC];
static int8_t  flat[FLAT_LEN];
static int8_t  d1_out[D1_OUT];
static int32_t logits[D2_OUT];
static int32_t dense1_b_fixed[D1_OUT];

/* ---------------------- GPIOs for hardware conv ---------------------- */
XGpio Gpio0, Gpio1, Gpio2, Gpio3;

/* ---------------------- Labels ---------------------- */
static const char *labels[NUM_CLASSES] = {
    "silence", "unknown", "yes", "no", "up", "down",
    "left", "right", "on", "off", "stop", "go"
};

/* ---------------------- Helpers ---------------------- */
static inline int8_t sat_i8_relu_shift(int32_t x, int shift)
{
    if(x<0) x=0;
    if(shift>0) x>>=shift;
    if(x>127) x=127;
    return (int8_t)x;
}

static void fix_dense1_bias(void)
{
    int i;
    for(i=0;i<D1_OUT;i++) dense1_b_fixed[i]=0;
    int n = LEN(dense1_b);
    if(n>D1_OUT) n=D1_OUT;
    for(i=0;i<n;i++) dense1_b_fixed[i]=dense1_b[i];
}

static void load_demo_input(void)
{
    int h,w;
    for(h=0;h<IN_H;h++)
        for(w=0;w<IN_W;w++)
            input_x[h][w] = (int8_t)(((h*3+w*7)%40)-20);
}

/* ---------------------- Hardware Conv1 ---------------------- */
static void conv1_layer_hw(void)
{
    int oh, ow, oc, kh, kw, index;
    xil_printf("Running Conv1 on HW...\r\n");

    for(oh=0; oh<C1_OH; oh++){
        for(ow=0; ow<C1_OW; ow++){
            for(oc=0; oc<C1_OC; oc++){
                index=0;
                for(kh=0; kh<C1_KH; kh++){
                    for(kw=0; kw<C1_KW; kw++){
                        XGpio_DiscreteWrite(&Gpio0,1,(uint32_t)input_x[oh+kh][ow+kw]);
                        XGpio_DiscreteWrite(&Gpio1,1,(uint32_t)conv1_w[index]);
                        XGpio_DiscreteWrite(&Gpio2,1,index);

                        XGpio_DiscreteWrite(&Gpio0,2,1);
                        XGpio_DiscreteWrite(&Gpio0,2,0);
                        index++;
                    }
                }
                XGpio_DiscreteWrite(&Gpio1,2,1);
                XGpio_DiscreteWrite(&Gpio1,2,0);

                while((XGpio_DiscreteRead(&Gpio3,2)&0x1)==0);

                uint32_t raw = XGpio_DiscreteRead(&Gpio3,1);
                int32_t result = raw & 0xFFFFF;
                if(result & (1<<19)) result |= 0xFFF00000;

                act1[oh][ow][oc] = sat_i8_relu_shift(result, CONV1_SHIFT);

                XGpio_DiscreteWrite(&Gpio2,2,1);
                XGpio_DiscreteWrite(&Gpio2,2,0);
            }
        }
    }
}

/* ---------------------- Hardware Conv2 ---------------------- */
static void conv2_layer_hw(void)
{
    int oh, ow, oc, kh, kw, ic, index;
    xil_printf("Running Conv2 on HW...\r\n");

    for(oh=0; oh<C2_OH; oh++){
        for(ow=0; ow<C2_OW; ow++){
            for(oc=0; oc<C2_OC; oc++){
                index=0;
                for(kh=0; kh<C2_KH; kh++){
                    for(kw=0; kw<C2_KW; kw++){
                        for(ic=0; ic<C2_IC; ic++){
                            XGpio_DiscreteWrite(&Gpio0,1,(uint32_t)act1[oh* C2_SH + kh][ow*C2_SW + kw]);
                            XGpio_DiscreteWrite(&Gpio1,1,(uint32_t)conv2_w[index]);
                            XGpio_DiscreteWrite(&Gpio2,1,index);

                            XGpio_DiscreteWrite(&Gpio0,2,1);
                            XGpio_DiscreteWrite(&Gpio0,2,0);
                            index++;
                        }
                    }
                }
                XGpio_DiscreteWrite(&Gpio1,2,1);
                XGpio_DiscreteWrite(&Gpio1,2,0);

                while((XGpio_DiscreteRead(&Gpio3,2)&0x1)==0);

                uint32_t raw = XGpio_DiscreteRead(&Gpio3,1);
                int32_t result = raw & 0xFFFFF;
                if(result & (1<<19)) result |= 0xFFF00000;

                act2[oh][ow][oc] = sat_i8_relu_shift(result, CONV2_SHIFT);

                XGpio_DiscreteWrite(&Gpio2,2,1);
                XGpio_DiscreteWrite(&Gpio2,2,0);
            }
        }
    }
}

/* ---------------------- Software remaining layers ---------------------- */
static void maxpool_2x4_s2x4(void)
{
    int oh, ow, c, kh, kw;
    for(oh=0; oh<P_OH; oh++)
        for(ow=0; ow<P_OW; ow++)
            for(c=0; c<C2_OC; c++){
                int maxv=-128;
                for(kh=0; kh<P_KH; kh++)
                    for(kw=0; kw<P_KW; kw++){
                        int ih = oh*P_SH+kh;
                        int iw = ow*P_SW+kw;
                        if(act2[ih][iw][c]>maxv) maxv=act2[ih][iw][c];
                    }
                actp[oh][ow][c]=(int8_t)maxv;
            }
}

static void flatten_pooled(void)
{
    int idx=0,h,w,c;
    for(h=0;h<P_OH;h++)
        for(w=0;w<P_OW;w++)
            for(c=0;c<C2_OC;c++)
                flat[idx++]=actp[h][w][c];
}

static void dense1_layer(void)
{
    int o,i;
    for(o=0;o<D1_OUT;o++){
        int32_t acc = dense1_b_fixed[o];
        for(i=0;i<FLAT_LEN;i++){
            acc += flat[i]*dense1_w[o*FLAT_LEN+i];
        }
        d1_out[o] = sat_i8_relu_shift(acc,DENSE1_SHIFT);
    }
}

static void dense2_layer(void)
{
    int o,i;
    for(o=0;o<D2_OUT;o++){
        int32_t acc=dense2_b[o];
        for(i=0;i<D1_OUT;i++){
            acc += d1_out[i]*dense2_w[o*D1_OUT+i];
        }
        logits[o]=acc;
    }
}

static int argmax_logits(void)
{
    int i,best_i=0;
    int32_t best_v=logits[0];
    for(i=1;i<D2_OUT;i++)
        if(logits[i]>best_v){best_v=logits[i];best_i=i;}
    return best_i;
}

static void print_logits(void)
{
    for(int i=0;i<D2_OUT;i++)
        printf("[%2d] %-8s : %ld\r\n", i, labels[i], (long)logits[i]);
}

/* ---------------------- Main ---------------------- */
int main(void)
{
    int status;
    XTime tStart, tEnd;
    u64 cycles;
    double time_ms;

    init_platform();

    /* Initialize GPIOs for HW convs */
    status = XGpio_Initialize(&Gpio0,XPAR_AXI_GPIO_0_DEVICE_ID);
    status |= XGpio_Initialize(&Gpio1,XPAR_AXI_GPIO_1_DEVICE_ID);
    status |= XGpio_Initialize(&Gpio2,XPAR_AXI_GPIO_2_DEVICE_ID);
    status |= XGpio_Initialize(&Gpio3,XPAR_AXI_GPIO_3_DEVICE_ID);
    if(status!=XST_SUCCESS){ xil_printf("GPIO init failed!\r\n"); cleanup_platform(); return -1; }

    /* Set directions */
    XGpio_SetDataDirection(&Gpio0,1,0x0); XGpio_SetDataDirection(&Gpio0,2,0x0);
    XGpio_SetDataDirection(&Gpio1,1,0x0); XGpio_SetDataDirection(&Gpio1,2,0x0);
    XGpio_SetDataDirection(&Gpio2,1,0x0); XGpio_SetDataDirection(&Gpio2,2,0x0);
    XGpio_SetDataDirection(&Gpio3,1,0xFFFFFFFF); XGpio_SetDataDirection(&Gpio3,2,0xFFFFFFFF);

    fix_dense1_bias();
    load_demo_input();

    /* ===== START TIMING ===== */
    XTime_GetTime(&tStart);

    conv1_layer_hw();
    conv2_layer_hw();
    maxpool_2x4_s2x4();
    flatten_pooled();
    dense1_layer();
    dense2_layer();

    XTime_GetTime(&tEnd);
    /* ===== END TIMING ===== */

    cycles = (u64)(tEnd - tStart);
    time_ms = (double)cycles * 1000.0 / COUNTS_PER_SECOND;

    int pred = argmax_logits();
    printf("\r\nPrediction index = %d\r\n", pred);
    printf("Predicted label  = %s\r\n", labels[pred]);

//    print_logits();
//    printf("CPU cycles       = %llu\r\n", cycles);
    printf("Processing Time  = %.6f ms\r\n", time_ms);

    cleanup_platform();
    return 0;
}
