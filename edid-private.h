/*
   edid-private.h: EDID data template

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


#ifndef __EDID_PRIVATE_H__
#define __EDID_PRIVATE_H__
edid_start:	.data
/* Fixed header pattern */
header:		.byte	0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00

#define __swap16(v1) ((v1>>8)+((v1&0xff)<<8))
#define __msbs2(v1,v2) ((((v1>>8)&0x0f)<<4)+((v2>>8)&0x0f))
#define __msbs4(v1,v2,v3,v4) \
	(((v1&0x03)>>2)+((v2&0x03)>>4)+((v3&0x03)>>6)+((v4&0x03)>>8))
#define __pixdpi2mm(pix,dpi) ((pix*25)/dpi)
#define __count_impl(X) X
#define __count(X) __count_impl(X)
#define count() __count(__COUNTER__)
#define PROCESS(NAME, ...)			___COUNT(count(), NAME, ## __VA_ARGS__)
#define ___COUNT(num, NAME, ...)	__IMPL_ ## NAME(num, ## __VA_ARGS__)
#define __label(pref, mid, suff) pref ## mid ## suff:
#define __required_symbol(section) .ifndef section; .err; .endif


#define __IMPL_BEGIN(num, \
	VER, REV, \
	M, F, G, \
	YEAR, WEEK, \
	PROD, SERIAL \
)\
mfg_id:		.hword	__swap16(((((M-'@')&0x1f)<<10)+(((F-'@')&0x1f)<<5)+((G-'@')&0x1f))); \
prod_code:	.hword	PROD; \
serial_number:	.long	SERIAL; \
week:		.byte	WEEK; \
year:		.byte	YEAR-1990; \
.if VER != 1; .err; .endif; \
version:	.byte	VER; \
.if REV < 0 || REV > 4; .err; .endif; \
revision:	.byte	REV


#define __VIDEO_DIGITAL(X)		0b10000000 | X
#define __VIDEO_ANALOG(X)		0b00000000 | X
#define __IMPL_VIDEO(num, TYPE, MODE) \
video_parms:	.byte	__VIDEO_##TYPE(MODE)

#define __IMPL_MAX_SIZE(num, X, Y, DPI) \
max_size:	.byte	(__pixdpi2mm(X, DPI)/10) , (__pixdpi2mm(Y, DPI)/10)

#define __IMPL_GAMMA(num, X) \
gamma:		.byte	X

#define __IMPL_DSP(num, X) \
dsp_features:	.byte	X

#define __IMPL_LSB(num, color_pair, v1x, v1y, v2x, v2y) \
color_pair ## _lsb:	.byte	\
		  ((v1x & 0b11)<<6) \
		| ((v1y & 0b11)<<4) \
		| ((v2x & 0b11)<<2) \
		| ((v2y & 0b11)<<0)

#define __IMPL_MSB(num, color, x, y) \
color ## _x_y_msb:	.byte	x, y

#define __IMPL_EST_TIMING(num, B7, B6, B5, B4, B3, B2, B1, B0) \
etimings_##num:	.byte	\
	(B7<<7) | \
	(B6<<6) | \
	(B5<<5) | \
	(B4<<4) | \
	(B3<<3) | \
	(B2<<2) | \
	(B1<<1) | \
	(B0<<0)

#define __IMPL_STD_RES(num, type, ...) \
std_res_ ## num: __STD_RES_##type(num, ## __VA_ARGS__)

#define __STD_RES_PAD(num)\
	.fill	1,2,0x0101

/* Standard timing */
/* X resolution, less 31, divided by 8 (256-2288 pixels) */
/* Y resolution, X:Y pixel ratio
   Bits 7-6	X:Y pixel ratio: 00=16:10; 01=4:3; 10=5:4; 11=16:9.
   Bits 5-0	Vertical frequency, less 60 (60-123 Hz) */
#define __STD_RES_RATIO(num, X, RATIO, FREQ)\
	.byte	(X/8)-31, (RATIO<<6)+FREQ-60

#define __IMPL_DSC(num, type, ...) \
dsc ## num: __DSC_##type(num, ## __VA_ARGS__)

#define __DSC_DUMMY(num) \
	.byte	0, 0, 0, 0x10; \
	.fill	12,1,0x01; \
	.byte 0x0a, 0x20

#define __DSC_DTD(num, CLK, DPI, FEATURES, \
			XPIX, YPIX, \
			XOFFSET, YOFFSET, \
			XPULSE, YPULSE, \
			XBLANK, YBLANK, \
			XBORDER, YBORDER) \
/* Pixel clock in 10 kHz units. (0.-655.35 MHz, little-endian) */ \
			.hword	CLK/10; \
/* Horizontal active pixels 8 lsbits (0-4095) */ \
			.byte	XPIX&0xff; \
/* Horizontal blanking pixels 8 lsbits (0-4095)
   End of active to start of next active. */ \
			.byte	XBLANK&0xff; \
/* Bits 7-4 	Horizontal active pixels 4 msbits
   Bits 3-0	Horizontal blanking pixels 4 msbits */ \
			.byte	__msbs2(XPIX,XBLANK); \
/* Vertical active lines 8 lsbits (0-4095) */ \
			.byte	YPIX&0xff; \
/* Vertical blanking lines 8 lsbits (0-4095) */ \
			.byte	YBLANK&0xff; \
/* Bits 7-4 	Vertical active lines 4 msbits
   Bits 3-0 	Vertical blanking lines 4 msbits */ \
			.byte	__msbs2(YPIX,YBLANK); \
/* Horizontal sync offset pixels 8 lsbits (0-1023) From blanking start */\
			.byte	XOFFSET&0xff; \
/* Horizontal sync pulse width pixels 8 lsbits (0-1023) */ \
			.byte	XPULSE&0xff; \
/* Bits 7-4 	Vertical sync offset lines 4 lsbits -63)
   Bits 3-0 	Vertical sync pulse width lines 4 lsbits -63) */ \
			.byte	((YOFFSET-63)<<4)+(YPULSE-63); \
/* Bits 7-6 	Horizontal sync offset pixels 2 msbits
   Bits 5-4 	Horizontal sync pulse width pixels 2 msbits
   Bits 3-2 	Vertical sync offset lines 2 msbits
   Bits 1-0 	Vertical sync pulse width lines 2 msbits */ \
			.byte	__msbs4(XOFFSET,XPULSE,YOFFSET,YPULSE); \
/* Horizontal display size, mm, 8 lsbits (0-4095 mm, 161 in) */ \
			.byte	__pixdpi2mm(XPIX, DPI)&0xff; \
/* Vertical display size, mm, 8 lsbits (0-4095 mm, 161 in) */\
			.byte	__pixdpi2mm(YPIX, DPI)&0xff; \
/* Bits 7-4 	Horizontal display size, mm, 4 msbits
   Bits 3-0 	Vertical display size, mm, 4 msbits */ \
			.byte	__msbs2(__pixdpi2mm(XPIX, DPI),__pixdpi2mm(YPIX, DPI)); \
/* Horizontal border pixels (each side; total is twice this) */ \
			.byte	XBORDER; \
/* Vertical border lines (each side; total is twice this) */ \
			.byte	YBORDER; \
/* Bit 7 	Interlaced
   Bits 6-5 	Stereo mode: 00=No stereo; other values depend on bit 0:
   Bit 0=0: 01=Field sequential, sync=1 during right; 10=similar,
     sync=1 during left; 11=4-way interleaved stereo
   Bit 0=1 2-way interleaved stereo: 01=Right image on even lines;
     10=Left image on even lines; 11=side-by-side
   Bits 4-3 	Sync type: 00=Analog composite; 01=Bipolar analog composite;
     10=Digital composite (on HSync); 11=Digital separate
   Bit 2 	If digital separate: Vertical sync polarity (1=positive)
   Other types: VSync serrated (HSync during VSync)
   Bit 1 	If analog sync: Sync on all 3 RGB lines (else green only)
   Digital: HSync polarity (1=positive)
   Bit 0 	2-way line-interleaved stereo, if bits 4-3 are not 00. */ \
			.byte	FEATURES

#define __DSC_ALNUM(num, data) \
	.byte	0,0; /* Not a detailed timing descriptor */ \
	.byte	0;	 /* Must be zero */\
	.byte	0xfe;/* Descriptor is monitor range limits */\
	.byte	0;	/* Must be zero */\
dsc##num##_start:\
	.ascii data; \
dsc##num##_end:\
	.byte	0x0a;	/* End marker */\
	.fill	12-((dsc##num##_end)-(dsc##num##_start)), 1, 0x20 /* Padded spaces */

#define __DSC_RAW(num, type, data) \
	.byte	0,0; /* Not a detailed timing descriptor */ \
	.byte	0;	 /* Must be zero */\
	.byte	type;/* Descriptor is monitor range limits */\
	.byte	0;	/* Must be zero */\
dsc##num##_start:\
	data; \
dsc##num##_end:\
	.byte	0x0a;	/* End marker */\
	.fill	12-((dsc##num##_end)-(dsc##num##_start)), 1, 0x20 /* Padded spaces */

#define __IMPL_EXTENSIONS(num, x) \
extensions:	.byte	x	/* Number of extensions to follow */

#define __IMPL_END(num) \
checksum:	.byte	CRC /* Sum of all bytes must be 0 (mod 256) */

#define __required_symbol_count(num, req) \
.if num != req; \
	.err; \
.endif;
#endif /* __EDID_PRIVATE_H__ */
