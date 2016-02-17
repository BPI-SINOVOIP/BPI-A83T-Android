#include <CdxTypes.h>
#include <CdxParser.h>
#include <CdxStream.h>
#include <CdxMemory.h>
#include <CdxId3Parser.h>

static int Id3Init(CdxParserT *id3_impl)
{
    cdx_int32 ret = 0;
    cdx_int64 offset=0;
    cdx_int32 tmpFd = 0;
    unsigned char headbuf[10] = {0};    
    struct Id3ParserImplS *impl          = NULL;
	    
    impl = (Id3ParserImplS*)id3_impl;
	CdxStreamRead(impl->stream,headbuf,10);
	impl->file_offset = 10;
    if(!memcmp(headbuf,ID3TAG,3))//for id3
	{
        CDX_LOGE("ID3 SKIPING!");
		impl->Id3v2len = ((cdx_int32)(headbuf[6]&0x7f))<<7 | ((cdx_int32)(headbuf[7]&0x7f));
		impl->Id3v2len = impl->Id3v2len<<7 | ((cdx_int32)(headbuf[8]&0x7f));
		impl->Id3v2len = impl->Id3v2len<<7 | ((cdx_int32)(headbuf[9]&0x7f));
		offset = impl->Id3v2len;
		impl->Id3v2len +=10;
	}
    ret = CdxStreamSeek(impl->stream, offset, SEEK_CUR);
	if(ret==-1){
		CDX_LOGE("Skip id3 byte error!");
		goto OPENFAILURE;
	}
	
	impl->file_offset += offset;
	//reopen parser
	CdxStreamGetMetaData(impl->stream,METEDATAKEY,(void **)&impl->keyinfo);
	ret = sscanf(impl->keyinfo, "fd://%d?offset=%lld&length=%lld", &tmpFd, &impl->fdoffset, &impl->file_size);
    
	
    ret = sprintf(impl->newurl,"fd://%d?offset=%lld&length=%lld",tmpFd,impl->file_offset,impl->file_size - impl->file_offset);

	memset(&impl->cdxDataSource,0x00,sizeof(impl->cdxDataSource));
	impl->cdxDataSource.uri = impl->newurl;

	ret = CdxParserPrepare(&impl->cdxDataSource, NO_NEED_DURATION, &impl->lock, &impl->forceStop,
		&impl->child, &impl->childStream, NULL, NULL);
	if(ret < 0)
	{
		CDX_LOGE("CdxParserPrepare fail");
		goto OPENFAILURE;
	}

    impl->mErrno = PSR_OK;
    return 0;
OPENFAILURE:
    CDX_LOGE("Id3OpenThread fail!!!");
    impl->mErrno = PSR_OPEN_FAIL;
    return -1;
}

static cdx_int32 __Id3ParserControl(CdxParserT *parser, cdx_int32 cmd, void *param)
{
    struct Id3ParserImplS *impl = NULL; 
    impl = (Id3ParserImplS*)parser;
	(void)param;
	if(!impl->child) 
		return CDX_SUCCESS;
    switch (cmd)
    {
    case CDX_PSR_CMD_DISABLE_AUDIO:
    case CDX_PSR_CMD_DISABLE_VIDEO:
    case CDX_PSR_CMD_SWITCH_AUDIO:
    	break;
    case CDX_PSR_CMD_SET_FORCESTOP:
    	CdxParserForceStop(impl->child);
      break;
    case CDX_PSR_CMD_CLR_FORCESTOP:
    	CdxParserClrForceStop(impl->child);
    	break;
    default :
        CDX_LOGW("not implement...(%d)", cmd);
        break;
    }
    impl->flags = cmd;
    return CDX_SUCCESS;
}

static cdx_int32 __Id3ParserPrefetch(CdxParserT *parser, CdxPacketT *pkt)
{
    cdx_int32 ret = CDX_FAILURE;
    struct Id3ParserImplS *impl = NULL;
    impl = CdxContainerOf(parser, struct Id3ParserImplS, base);
	if(!impl->child)
		return ret;
	ret = CdxParserPrefetch(impl->child,pkt);
	impl->mErrno = CdxParserGetStatus(impl->child);
    return ret;
}

static cdx_int32 __Id3ParserRead(CdxParserT *parser, CdxPacketT *pkt)
{
	cdx_int32 ret = CDX_FAILURE;
	struct Id3ParserImplS *impl = NULL;
    impl = CdxContainerOf(parser, struct Id3ParserImplS, base);
	if(!impl->child)
		return ret;
	ret = CdxParserRead(impl->child,pkt);
	impl->mErrno = CdxParserGetStatus(impl->child);
    return ret;
}

static cdx_int32 __Id3ParserGetMediaInfo(CdxParserT *parser, CdxMediaInfoT *mediaInfo)
{
    cdx_int32 ret = CDX_FAILURE;
    struct Id3ParserImplS *impl = NULL;
    impl = CdxContainerOf(parser, struct Id3ParserImplS, base);
	if(!impl->child)
		return ret;
	ret = CdxParserGetMediaInfo(impl->child,mediaInfo);
	impl->mErrno = CdxParserGetStatus(impl->child);
    return ret;
}

static cdx_int32 __Id3ParserSeekTo(CdxParserT *parser, cdx_int64 timeUs)
{ 
    cdx_int32 ret = CDX_FAILURE;
    struct Id3ParserImplS *impl = NULL;
    impl = CdxContainerOf(parser, struct Id3ParserImplS, base);
	if(!impl->child)
		return ret;
	ret = CdxParserSeekTo(impl->child,timeUs); 
	impl->mErrno = CdxParserGetStatus(impl->child);
    return ret;
}

static cdx_uint32 __Id3ParserAttribute(CdxParserT *parser)
{
    struct Id3ParserImplS *impl = NULL;
    impl = CdxContainerOf(parser, struct Id3ParserImplS, base);
    return CdxParserAttribute(impl->child);
}
#if 0
static cdx_int32 __Id3ParserForceStop(CdxParserT *parser)
{
    struct Id3ParserImplS *impl = NULL;
	impl = CdxContainerOf(parser, struct Id3ParserImplS, base);
    return CdxParserForceStop(impl->child);
}
#endif
static cdx_int32 __Id3ParserGetStatus(CdxParserT *parser)
{
    struct Id3ParserImplS *impl = NULL;
	impl = CdxContainerOf(parser, struct Id3ParserImplS, base);

   	return impl->child?CdxParserGetStatus(impl->child):impl->mErrno;
}

static cdx_int32 __Id3ParserClose(CdxParserT *parser)
{
	struct Id3ParserImplS *impl = NULL;
	impl = CdxContainerOf(parser, struct Id3ParserImplS, base);
	if(impl->child)
		CdxParserClose(impl->child);
    pthread_mutex_destroy(&impl->lock);
    CdxStreamClose(impl->stream);    CdxFree(impl);    return CDX_SUCCESS;
}

static struct CdxParserOpsS id3ParserOps =
{
    .control = __Id3ParserControl,
    .prefetch = __Id3ParserPrefetch,
    .read = __Id3ParserRead,
    .getMediaInfo = __Id3ParserGetMediaInfo,
    .seekTo = __Id3ParserSeekTo,
    .attribute = __Id3ParserAttribute,
    .getStatus = __Id3ParserGetStatus,
    .close = __Id3ParserClose,
    .init = Id3Init
};

static cdx_uint32 __Id3ParserProbe(CdxStreamProbeDataT *probeData)
{
    CDX_CHECK(probeData);
    if(probeData->len < 10)
    {
        CDX_LOGE("Probe ID3_header data is not enough.");
        return 0;
    }
    
    if(memcmp(probeData->buf,ID3TAG,3))
    {
        CDX_LOGE("id3 probe failed.");
        return 0;
    }
    return 100;
}

static CdxParserT *__Id3ParserOpen(CdxStreamT *stream, cdx_uint32 flags)
{
    cdx_int32 ret = 0;
    struct Id3ParserImplS *impl;
    impl = CdxMalloc(sizeof(*impl));

    memset(impl, 0x00, sizeof(*impl));
    ret = pthread_mutex_init(&impl->lock, NULL);
    CDX_FORCE_CHECK(ret == 0);
    impl->stream = stream;
    impl->base.ops = &id3ParserOps;
	(void)flags;
    //ret = pthread_create(&impl->openTid, NULL, Id3OpenThread, (void*)impl);
    //CDX_FORCE_CHECK(!ret);
    impl->mErrno = PSR_INVALID;
    
    return &impl->base;
}

struct CdxParserCreatorS id3ParserCtor =
{
    .probe = __Id3ParserProbe,
    .create = __Id3ParserOpen
};
