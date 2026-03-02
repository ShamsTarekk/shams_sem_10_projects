#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#include "platform.h"
#include "xtime_l.h"        // <<< For timing

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

/* ---------------------- Buffers ---------------------- */
static int8_t  input_x[IN_H][IN_W];
static int8_t  act1[C1_OH][C1_OW][C1_OC];
static int8_t  act2[C2_OH][C2_OW][C2_OC];
static int8_t  actp[P_OH][P_OW][C2_OC];
static int8_t  flat[FLAT_LEN];
static int8_t  d1_out[D1_OUT];
static int32_t logits[D2_OUT];
static int32_t dense1_b_fixed[D1_OUT];

/* ---------------------- Labels ---------------------- */
static const char *labels[NUM_CLASSES] = {
   "silence", "unknown", "yes", "no", "up", "down",
   "left", "right", "on", "off", "stop", "go"
};

/* ---------------------- Helpers ---------------------- */
static inline int8_t sat_i8_relu_shift(int32_t x, int shift)
{
   if (x < 0) x = 0;
   if (shift > 0) x >>= shift;
   if (x > 127) x = 127;
   return (int8_t)x;
}

static void fix_dense1_bias(void)
{
   int i;
   for (i = 0; i < D1_OUT; i++) dense1_b_fixed[i] = 0;
   int n = sizeof(dense1_b)/sizeof(dense1_b[0]);
   if (n > D1_OUT) n = D1_OUT;
   for (i = 0; i < n; i++) dense1_b_fixed[i] = dense1_b[i];
}

static void load_demo_input(void)
{
   int h, w;
   for (h = 0; h < IN_H; h++)
       for (w = 0; w < IN_W; w++)
           input_x[h][w] = (int8_t)((h * 3 + w * 7) % 40 - 20);
}

/* ---------------------- CNN Layers ---------------------- */
static void conv1_layer(void)
{
   int oh, ow, oc, kh, kw;
   for (oh = 0; oh < C1_OH; ++oh)
       for (ow = 0; ow < C1_OW; ++ow)
           for (oc = 0; oc < C1_OC; ++oc) {
               int32_t acc = conv1_b[oc];
               for (kh = 0; kh < C1_KH; ++kh)
                   for (kw = 0; kw < C1_KW; ++kw) {
                       int ih = oh + kh;
                       int iw = ow + kw;
                       int widx = (((oc * C1_KH) + kh) * C1_KW) + kw;
                       acc += input_x[ih][iw] * conv1_w[widx];
                   }
               act1[oh][ow][oc] = sat_i8_relu_shift(acc, CONV1_SHIFT);
           }
}

static void conv2_layer(void)
{
   int oh, ow, oc, kh, kw, ic;
   for (oh = 0; oh < C2_OH; ++oh)
       for (ow = 0; ow < C2_OW; ++ow)
           for (oc = 0; oc < C2_OC; ++oc) {
               int32_t acc = conv2_b[oc];
               for (kh = 0; kh < C2_KH; ++kh)
                   for (kw = 0; kw < C2_KW; ++kw)
                       for (ic = 0; ic < C2_IC; ++ic) {
                           int ih = oh * C2_SH + kh;
                           int iw = ow + kw;
                           int widx = ((((oc * C2_KH) + kh) * C2_KW + kw) * C2_IC) + ic;
                           acc += act1[ih][iw][ic] * conv2_w[widx];
                       }
               act2[oh][ow][oc] = sat_i8_relu_shift(acc, CONV2_SHIFT);
           }
}

static void maxpool_2x4_s2x4(void)
{
   int oh, ow, c, kh, kw;
   for (oh = 0; oh < P_OH; ++oh)
       for (ow = 0; ow < P_OW; ++ow)
           for (c = 0; c < C2_OC; ++c) {
               int maxv = -128;
               for (kh = 0; kh < P_KH; ++kh)
                   for (kw = 0; kw < P_KW; ++kw) {
                       int ih = oh * P_SH + kh;
                       int iw = ow * P_SW + kw;
                       int v = act2[ih][iw][c];
                       if (v > maxv) maxv = v;
                   }
               actp[oh][ow][c] = (int8_t)maxv;
           }
}

static void flatten_pooled(void)
{
   int h, w, c, idx = 0;
   for (h = 0; h < P_OH; ++h)
       for (w = 0; w < P_OW; ++w)
           for (c = 0; c < C2_OC; ++c)
               flat[idx++] = actp[h][w][c];
}

static void dense1_layer(void)
{
   int o, i;
   for (o = 0; o < D1_OUT; ++o) {
       int32_t acc = dense1_b_fixed[o];
       for (i = 0; i < FLAT_LEN; ++i)
           acc += flat[i] * dense1_w[o * FLAT_LEN + i];
       d1_out[o] = sat_i8_relu_shift(acc, DENSE1_SHIFT);
   }
}

static void dense2_layer(void)
{
   int o, i;
   for (o = 0; o < D2_OUT; ++o) {
       int32_t acc = dense2_b[o];
       for (i = 0; i < D1_OUT; ++i)
           acc += d1_out[i] * dense2_w[o * D1_OUT + i];
       logits[o] = acc;
   }
}

static int argmax_logits(void)
{
   int i, best = 0;
   for (i = 1; i < D2_OUT; ++i)
       if (logits[i] > logits[best])
           best = i;
   return best;
}

static void print_logits(void)
{
   int i;
   printf("Logits:\r\n");
   for (i = 0; i < D2_OUT; ++i)
       printf(" [%2d] %-8s : %ld\r\n", i, labels[i], (long)logits[i]);
}

/* ========================= MAIN ========================= */

int main(void)
{
   init_platform();

   XTime tStart, tEnd;
   u64 cycles;
   double time_ms;

   printf("\r\n=== Task 2: KWS CNN Inference ===\r\n");

   fix_dense1_bias();
   load_demo_input();

   /* ===== START TIMING ===== */
   XTime_GetTime(&tStart);

   conv1_layer();
   conv2_layer();
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
   if (pred >= 0 && pred < NUM_CLASSES)
       printf("Predicted label  = %s\r\n", labels[pred]);

//    print_logits();
//    printf("CPU cycles       = %llu\r\n", cycles);
   printf("Processing Time  = %.6f ms\r\n", time_ms);

   cleanup_platform();
   return 0;
}
