/*
 * C128 6581/8580 SID.
 *
 * Copyright (c) Steven P. Goldsmith. All rights reserved.
 */

#ifndef _SID_H
#define _SID_H

#define sidVoice1   0xd400 /* Voices */
#define sidVoice2   0xd407
#define sidVoice3   0xd40E
#define sidCutoffLo 0xd415 /* Cutoff filter */
#define sidCutoffHi 0xd416
#define sidResCtrl  0xd417 /* Resonance control */
#define sidVolume   0xd418 /* Master volume and filter select */
#define sidPotX     0xd419 /* Paddle X */
#define sidPotY     0xd41A /* Paddle Y */
#define sidEnvGen3  0xd41C

#define sidGate 0x01 /* Waveforms */
#define sidSync 0x02
#define sidRing 0x04
#define sidTest 0x08
#define sidTri  0x10
#define sidSaw  0x20
#define sidSqu  0x40
#define sidNoi  0x80

#define sidLowPass   0x10 /* Filter select settings */
#define sidBandPass  0x20
#define sidHighPass  0x40
#define sidVoice3Off 0x80

#define sidFilter1   0x01 /* Filter resonance output settings */
#define sidFilter2   0x02
#define sidFilter3   0x04
#define sidFilterExt 0x08

extern void clearSid();
extern void __LIB__ setSidVol(unsigned char amp, unsigned char filter);
extern void __LIB__ setSidEnv(unsigned int voice, unsigned char attack, unsigned char decay, unsigned char sustain,
unsigned char release);
extern void __LIB__ setSidRel(unsigned int Voice, unsigned char Waveform);
extern void __LIB__ setSidFreq(unsigned int voice, unsigned int freq);
extern void __LIB__ setSidAtt(unsigned int voice, unsigned char waveform);
extern void __LIB__ setSidPulWav(unsigned int voice, unsigned int width);
extern void __LIB__ readSidPots(unsigned char *x1, unsigned char *y1, unsigned char *x2, unsigned char *y2);
extern void __LIB__ readMouseSid(unsigned char *x1, unsigned char *y1, unsigned char *x2, unsigned char *y2);

#endif
