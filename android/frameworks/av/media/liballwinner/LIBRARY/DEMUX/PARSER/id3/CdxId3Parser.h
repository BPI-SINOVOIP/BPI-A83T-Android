#ifndef CDX_AMR_PARSER_H
#define CDX_AMR_PARSER_H

typedef struct Id3ParserImplS
{
     //audio common
     CdxParserT base;
     CdxStreamT *stream;   
     cdx_int32 mErrno; //Parser Status
     cdx_uint32 flags; //cmd
     cdx_int32  forceStop;
     //id3 base    
     CdxParserT *child;/*为子parser，下一级的媒体文件parser，如mp3 parser*/
	 cdx_char *  keyinfo;
	 cdx_char    newurl[128];
	 cdx_int64   fdoffset;
	 cdx_int64   file_size;
	 CdxDataSourceT cdxDataSource;
     cdx_int32  Id3v2len;
	 cdx_int64  file_offset;
	 CdxStreamT *childStream;
	 pthread_mutex_t lock;

}Id3ParserImplS;

#define ID3TAG "ID3"
#define METEDATAKEY    "uri"

#endif
