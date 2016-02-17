#ifndef _CDX_MP3_PARSER_H_
#define _CDX_MP3_PARSER_H_

#define ENABLE_INFO_DEBUG       1
#define ENABLE_FILE_DEBUG       0
#define AV_TIME_BASE            1000000

#define MP3_PROBE_SIZE          10 * 1024
#define MP3_PACKET_SIZE         1024
#define AVPROBE_SCORE_MAX       100

#define ID3v2_HEADER_SIZE       10
#define MPA_STEREO              0
#define MPA_JSTEREO             1
#define MPA_DUAL                2
#define MPA_MONO                3

#define	INFLEN	1024*8
#define ID3TAGNUM 8
#define SKIPLEN 1024


#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))

const uint16_t FFMpaFreqTab[3] = { 44100, 48000, 32000 };

const uint16_t FFMpaBitrateTab[2][3][15] = {
    { {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 },
      {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384 },
      {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 } },
    { {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},
      {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
      {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}
    }
};

#define MPADECODEHEADER \
    int mFrameSize; \
    int errorProtection; \
    int layer; \
    int mSampleRate; \
    int mSampleRateIndex; /* between 0 and 8 */ \
    int mBitRate; \
    int nChannels; \
    int mode; \
    int modeExt; \
    int lsf;

typedef struct MPADecodeHeader {
  MPADECODEHEADER
} MPADecodeHeader;
#if 0
static int FFId3v2Match(const cdx_uint8 *buf)
{
    return  buf[0]         ==  'I' &&
            buf[1]         ==  'D' &&
            buf[2]         ==  '3' &&
            buf[3]         != 0xff &&
            buf[4]         != 0xff &&
           (buf[6] & 0x80) ==    0 &&
           (buf[7] & 0x80) ==    0 &&
           (buf[8] & 0x80) ==    0 &&
           (buf[9] & 0x80) ==    0;
}

static int FFId3v2TagLen(const uint8_t * buf)
{
    int len = ((buf[6] & 0x7f) << 21) +
              ((buf[7] & 0x7f) << 14) +
              ((buf[8] & 0x7f) << 7) +
               (buf[9] & 0x7f) +
              ID3v2_HEADER_SIZE;
    if (buf[5] & 0x10)
        len += ID3v2_HEADER_SIZE;
    return len;
}
#endif

typedef struct __language_coding
{
	cdx_int8 language[4];
	cdx_int32  coding;
}anguage_coding_s;


typedef enum
{
    IMG_FORMAT_BMP =0,
    IMG_FORMAT_JPG,
    IMG_FORMAT_GIF,
    IMG_FORMAT_PNG,
    IMG_FORMAT_UNSUPPORT = -1
}img_format_t;

typedef struct CDX_ID3_IMAGE_INFO
{
    int               length;         //image length
    int               FileLocation;   //image location
    img_format_t      img_format;     
    int               pic_type;       //picture type;
    int               img_high;      
    int               img_width;     
    int               otherdata;      

}cdx_id3_image_info_t;


typedef struct XINGSeeker {
    int64_t mFirstFramePos;
    int64_t mDurationUs;
    int32_t mSizeBytes;
    int32_t mEncoderDelay;
    int32_t mEncoderPadding;

    // TOC entries in XING header. Skip the first one since it's always 0.
    unsigned char mTOC[99];
    cdx_bool mTOCValid;
}XINGSeeker;

typedef struct VBRISeeker {
    int64_t     mBasePos;
    int64_t     mDurationUs;
	cdx_uint32  mSegmentsize;
    cdx_uint32  mSegments[1024];
}VBRISeeker;

typedef enum CDX_A_AUDIO_FONTTYPE
{
	  A_AUDIO_FONTTYPE_ISOIEC8859_1  = 0,       //ISO/IEC 8859-1
	  A_AUDIO_FONTTYPE_UTF_16LE,//
	  A_AUDIO_FONTTYPE_UTF_16BE,
	  A_AUDIO_FONTTYPE_UTF_8,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_2,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_3,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_4,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_5,//
	  A_AUDIO_FONTTYPE_ISOIEC8859_6,
	  A_AUDIO_FONTTYPE_ISOIEC8859_7,
	  A_AUDIO_FONTTYPE_ISOIEC8859_8,
	  A_AUDIO_FONTTYPE_ISOIEC8859_9,
	  A_AUDIO_FONTTYPE_ISOIEC8859_10,
	  A_AUDIO_FONTTYPE_ISOIEC8859_11,
	  A_AUDIO_FONTTYPE_ISOIEC8859_12,
	  A_AUDIO_FONTTYPE_ISOIEC8859_13,
	  A_AUDIO_FONTTYPE_ISOIEC8859_14,
	  A_AUDIO_FONTTYPE_ISOIEC8859_15,
	  A_AUDIO_FONTTYPE_ISOIEC8859_16,
	  A_AUDIO_FONTTYPE_WINDOWS_1250,
	  A_AUDIO_FONTTYPE_WINDOWS_1251,//
	  A_AUDIO_FONTTYPE_WINDOWS_1252,
	  A_AUDIO_FONTTYPE_WINDOWS_1253,
	  A_AUDIO_FONTTYPE_WINDOWS_1254,
	  A_AUDIO_FONTTYPE_WINDOWS_1255,
	  A_AUDIO_FONTTYPE_WINDOWS_1256,
	  A_AUDIO_FONTTYPE_WINDOWS_1257,
	  A_AUDIO_FONTTYPE_WINDOWS_1258,
	  A_AUDIO_FONTTYPE_KOI8_R,
	  A_AUDIO_FONTTYPE_KOI8_U,
	  A_AUDIO_FONTTYPE_GB2312,
	  A_AUDIO_FONTTYPE_GBK,
	  A_AUDIO_FONTTYPE_BIG5,
		
	  
	  A_AUDIO_FONTTYPE_ = -1
}cdx_audio_fonttype_e;

typedef struct MP3ParserImpl{
    // Cdx Struct
    CdxParserT      base;
    CdxStreamT      *stream;
    
    int             mStatus;  
    int             mErrno;    // errno
    int             exitFlag;

	pthread_cond_t  cond;

    XINGSeeker      *mXINGSeeker;
    VBRISeeker      *mVBRISeeker;

    cdx_int64       mFileSize;
    cdx_uint64      mFirstFramePos;
    cdx_uint32      mFixedHeader;
    int64_t         mCurrentPos;
    cdx_uint16      mChannels;
    cdx_uint32      mSampleRate;
    cdx_int32       mBitRate;
    cdx_uint32      mFrameSize;
    cdx_int32       readPacketSize;
    /*Duration*/
    cdx_uint64      mDuration;

    cdx_uint32      mSeeking;
    cdx_int64       mSeekingTime;
    int             teeFd;
	/*ID3*/
    cdx_int32					mGenre_sz;             // 流派
    cdx_int8					*mGenre;               // pop soft...
    cdx_audio_fonttype_e	    mGenreCharEncode;

    cdx_int32					mtitle_sz;             // 歌曲名
    cdx_int8					*mtitle;
    cdx_audio_fonttype_e		mtitleCharEncode;

    cdx_int32					mauthor_sz;            // 演唱者
    cdx_int8					*mauthor;
    cdx_audio_fonttype_e	    mauthorCharEncode;

    cdx_int32					mAlbum_sz;             // 专集
    cdx_int8					*mAlbum;
    cdx_audio_fonttype_e	    mAlbumCharEncode;

    cdx_int32					mYear_sz;              // 出品年代
    cdx_int8					*mYear;
    cdx_audio_fonttype_e	    mYearCharEncode;

    cdx_int32					mAPic_sz;             // attached picture
    cdx_id3_image_info_t		*mAPic;
    cdx_audio_fonttype_e	    mAPicCharEncode;

    cdx_int32					mInforBufLeftLength;
    cdx_int8					*mInforBuf;            
	cdx_int8                    *mInforBuftemp;
	cdx_uint8					*pAlbumArtBuf;
	cdx_int32					mAlbumArtBufSize;
} MP3ParserImpl;

#endif
