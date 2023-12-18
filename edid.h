/*
   edid.S: EDID data template API

   Copyright (C) 2012 Carsten Emde <C.Emde@osadl.org>
   Copyright (C) 2023 Mubashshir   <ahmubashshir@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
*/


#ifndef __EDID_H__
#define __EDID_H__

#include "edid-private.h"
#define XY_RATIO_16_10	0b00
#define XY_RATIO_4_3	0b01
#define XY_RATIO_5_4	0b10
#define XY_RATIO_16_9	0b11

/* Manufacturer */

/* EDID 1.3 standard definitions */
/* Serial number. 32 bits, little endian. */
/* Week of manufacture */
/* Year of manufacture, less 1990. (1990-2245)
   If week=255, it is the model year instead */

#define BEGIN(\
	VER, REV, \
	M, F, G, \
	YEAR, WEEK, \
	PROD, SERIAL \
)	PROCESS(BEGIN, \
	VER, REV, \
	M, F, G, \
	YEAR, WEEK, \
	PROD, SERIAL \
)

#define VIDEO_DIGITAL_DEPTH6	0b00010000
#define VIDEO_DIGITAL_DEPTH8	0b00100000
#define VIDEO_DIGITAL_DEPTH10	0b00110000
#define VIDEO_DIGITAL_DEPTH12	0b01000000
#define VIDEO_DIGITAL_DEPTH14	0b01010000
#define VIDEO_DIGITAL_DEPTH16	0b01100000
#define VIDEO_DIGITAL_RDEPTH	0b01110000
#define VIDEO_DIGITAL_DVI		0b00000001
#define VIDEO_DIGITAL_HDMIa		0b00000010
#define VIDEO_DIGITAL_HDMIb		0b00000011
#define VIDEO_DIGITAL_MDDI		0b00000100
#define VIDEO_DIGITAL_DP		0b00000101

#define VIDEO_ANALOG_P100N040V	0b01000000
#define VIDEO_ANALOG_P071N028V	0b00100000
#define VIDEO_ANALOG_BLANK		0b00010000
#define VIDEO_ANALOG_SYNC_SEP	0b00001000
#define VIDEO_ANALOG_COMP_SYNC	0b00000100
#define VIDEO_ANALOG_GREEN_SYNC 0b00000010
#define VIDEO_ANALOG_SERR_VSYNC 0b00000001
#define VIDEO_ANALOG_P070N000V	VIDEO_ANALOG_P100N040V | VIDEO_ANALOG_P071N028V
/* If Bit 7=1	Digital input. If set, the following bit definitions apply:
     Bits 6-1	Reserved, must be 0
     Bit 0	Signal is compatible with VESA DFP 1.x TMDS CRGB,
		  1 pixel per clock, up to 8 bits per color, MSB aligned,
   If Bit 7=0	Analog input. If clear, the following bit definitions apply:
     Bits 6-5	Video white and sync levels, relative to blank
		  00=+0.7/-0.3 V; 01=+0.714/-0.286 V;
		  10=+1.0/-0.4 V; 11=+0.7/0 V
   Bit 4	Blank-to-black setup (pedestal) expected
   Bit 3	Separate sync supported
   Bit 2	Composite sync (on HSync) supported
   Bit 1	Sync on green supportedp
   Bit 0	VSync pulse must be serrated when somposite or
		  sync-on-green is used. */
#define VIDEO(TYPE, FLAGS)		PROCESS(VIDEO, TYPE, FLAGS)
#define VIDEO_ANALOG(FLAGS)		PROCESS(VIDEO, ANALOG, FLAGS)
#define VIDEO_DIGITAL(FLAGS)	PROCESS(VIDEO, DIGITAL, FLAGS)

/* Maximum horizontal image size, in centimetres
   (max 292 cm/115 in at 16:9 aspect ratio) */
#define MAX_SIZE(XPIX, YPIX, DPI) PROCESS(MAX_SIZE, XPIX, YPIX, DPI)

/* Maximum vertical image size, in centimetres.
   If either byte is 0, undefined (e.g. projector) */

/* Display gamma, minus 1, times 100 (range 1.00-3.5 */
#define GAMMA(VAL) PROCESS(GAMMA, VAL)

/* Bit 7	DPMS standby supported
   Bit 6	DPMS suspend supported
   Bit 5	DPMS active-off supported
   Bits 4-3	Display type: 00=monochrome; 01=RGB colour;
		  10=non-RGB multicolour; 11=undefined
   Bit 2	Standard sRGB colour space. Bytes 25-34 must contain
		  sRGB standard values.
   Bit 1	Preferred timing mode specified in descriptor block 1.
   Bit 0	GTF supported with default parameter values. */
#define DSP_STANDBY		0b10000000
#define DSP_SUSPEND		0b01000000
#define DSP_ACTIVEOFF	0b00100000
#define DSP_COLOR		0b00010000
#define DSP_RGB_COLOR	0b00001000
#define DSP_STD_SRGB	0b00000100
#define DSP_PREFER_DT1	0b00000010
#define DSP_DEFAULTGTF	0b00000001
#define DSP_NONE		0b00000000
#define DSP(FLAGS)		PROCESS(DSP, FLAGS)
/* Chromaticity coordinates. */
#define LSB(color_pair, v1x, v1y, v2x, v2y) PROCESS(LSB, color_pair, v1x, v1y, v2x, v2y)
/* Red and green least-significant bits
   Bits 7-6	Red x value least-significant 2 bits
   Bits 5-4	Red y value least-significant 2 bits
   Bits 3-2	Green x value lst-significant 2 bits
   Bits 1-0	Green y value least-significant 2 bits */
#define LSB_RED_GREEN(v1x, v1y, v2x, v2y) PROCESS(LSB, red_green, v1x, v1y, v2x, v2y)
/* Blue and white least-significant 2 bits */
#define LSB_BLUE_WHITE(v1x, v1y, v2x, v2y) PROCESS(LSB, blue_white, v1x, v1y, v2x, v2y)

/* 0-255 encodes 0-0.996 (255/256); 0-0.999 (1023/1024) with lsbits */
/* Color(R,G,B,W) x and y value most significant 8 bits */
#define MSB(color, x, y) PROCESS(MSB, color, x, y)

#define MSB_RED(x, y)	PROCESS(MSB, red, x, y)
#define MSB_GREEN(x, y)	PROCESS(MSB, green, x, y)
#define MSB_BLUE(x, y)	PROCESS(MSB, blue, x, y)
#define MSB_WHITE(x, y)	PROCESS(MSB, white, x, y)

/* Established timings */
/* Bit 0,7	720x400 @ 70 Hz
   Bit 0,6	720x400 @ 88 Hz
   Bit 0,5	640x480 @ 60 Hz
   Bit 0,4	640x480 @ 67 Hz
   Bit 0,3	640x480 @ 72 Hz
   Bit 0,2	640x480 @ 75 Hz
   Bit 0,1	800x600 @ 56 Hz
   Bit 0,0	800x600 @ 60 Hz
   Bit 1,7	800x600 @ 72 Hz
   Bit 1,6	800x600 @ 75 Hz
   Bit 1,5	832x624 @ 75 Hz
   Bit 1,4	1024x768 @ 87 Hz, interlaced (1024x768)
   Bit 1,3	1024x768 @ 60 Hz
   Bit 1,2	1024x768 @ 72 Hz
   Bit 1,1	1024x768 @ 75 Hz
   Bit 1,0	1280x1024 @ 75 Hz
   Bit 2,7	1152x870 @ 75 Hz (Apple Macintosh II)
   Bits 2,6-0 	Other manufacturer-specific display mod */
#define EST_TIMING(B7, B6, B5, B4, B3, B2, B1, B0) PROCESS(EST_TIMING, B7, B6, B5, B4, B3, B2, B1, B0)

// wrapper
#define STD_RES(TYPE, ...) PROCESS(STD_RES, TYPE, ## __VA_ARGS__)
/* Standard timing
   @param XRES
   @param STD_RATIO
   @param FREQ
   X resolution, less 31, divided by 8 (256-2288 pixels)
   Y resolution, X:Y pixel ratio
   Bits 7-6	X:Y pixel ratio: 00=16:10; 01=4:3; 10=5:4; 11=16:9.
   Bits 5-0	Vertical frequency, less 60 (60-123 Hz) */
// declare standard ratio based resolution
#define STD_RES_RATIO(XRES, STD_RATIO, FREQ) \
	PROCESS(STD_RES, RATIO, XRES, STD_RATIO, FREQ)
// add padding
#define	STD_RES_PAD() \
	PROCESS(STD_RES, PAD)

#define DTD_INTERLACED 0x1000000
#define DTD_STERIO(X, Y)    (X<<6) | (Y<<5)
#define DTD_SYNC_TYPE(X, Y) (X<<4) | (Y<<3)
#define DTD_SYNC_DIR(X, Y)  (X<<2) | (Y<<1)
#define DTD_LINE_INTERLEAVED 1


#define DSC(TYPE, ...) \
	PROCESS(DSC, TYPE, ## __VA_ARGS__)
// Dummy descriptor, used for padding
#define DSC_DUMMY() \
	PROCESS(DSC, DUMMY)
// Alphanumeric data
#define DSC_ALNUM(data) \
	PROCESS(DSC, ALNUM, data)

// DSC_SERIAL()
#define DSC_SERIAL(data) \
	PROCESS(DSC, RAW, 0xff, .ascii data)

// Detailed Timing Descriptor
#define DSC_DTD(CLK, DPI, FEATURES, \
			XPIX, YPIX, \
			XOFFSET, YOFFSET, \
			XPULSE, YPULSE, \
			XBLANK, YBLANK, \
			XBORDER, YBORDER) \
	PROCESS(DSC, DTD, \
			CLK, DPI, FEATURES, \
			XPIX, YPIX, \
			XOFFSET, YOFFSET, \
			XPULSE, YPULSE, \
			XBLANK, YBLANK, \
			XBORDER, YBORDER)

#define EXTENSIONS(x) PROCESS(EXTENSIONS, x)

#define END() PROCESS(END)
#elif ! defined(__EDID_H_REPEAT__)
#define __EDID_H_REPEAT__
	__required_symbol_count(count(), 28)
	__required_symbol(edid_start)
	__required_symbol(header)
	__required_symbol(mfg_id)
	__required_symbol(prod_code)
	__required_symbol(serial_number)
	__required_symbol(week)
	__required_symbol(year)
	__required_symbol(version)
	__required_symbol(revision)
	__required_symbol(video_parms)
	__required_symbol(max_size)
	__required_symbol(gamma)
	__required_symbol(dsp_features)
	__required_symbol(red_green_lsb)
	__required_symbol(blue_white_lsb)
	__required_symbol(red_x_y_msb)
	__required_symbol(green_x_y_msb)
	__required_symbol(blue_x_y_msb)
	__required_symbol(white_x_y_msb)
	__required_symbol(etimings_11)
	__required_symbol(etimings_12)
	__required_symbol(etimings_13)
	__required_symbol(std_res_14)
	__required_symbol(std_res_15)
	__required_symbol(std_res_16)
	__required_symbol(std_res_17)
	__required_symbol(std_res_18)
	__required_symbol(std_res_19)
	__required_symbol(std_res_20)
	__required_symbol(std_res_21)
	__required_symbol(dsc22)
	__required_symbol(dsc23)
	__required_symbol(dsc24)
	__required_symbol(dsc25)
	__required_symbol(extensions)
	__required_symbol(checksum)
#endif /* __EDID_H__ */
