#ifndef BUFFER_FRAME_H
#define BUFFER_FRAME_H

#include "XAllocator.h"

#include <deque>


typedef struct tag_RTPHEADER_S
{
#if(BYTE_ORDER == LITTLE_ENDIAN)
	unsigned short cc      :4;   /*CSRC count*/
	unsigned short x       :1;   /*header extension flag*/
	unsigned short p       :1;   /*padding flag*/
	unsigned short version :2;   /*protocol version*/
	unsigned short pt      :7;   /*payload type*/
	unsigned short marker  :1;   /*marker bit*/
#elif(BYTE_ORDER == BIG_ENDIAN)
	unsigned short version :2;   /*protocol version*/
	unsigned short p       :1;   /*padding flag*/
	unsigned short x       :1;   /*header extension flag*/
	unsigned short cc      :4;   /*CSRC count*/
	unsigned short marker  :1;   /*marker bit*/
	unsigned short pt      :7;   /*payload type*/
#else
#error YOU MUST DEFINE BYTE_ORDER == LITTLE_ENDIAN OR BIG_ENDIAN !  
#endif
	unsigned short seqno  :16;   /*sequence number*/
	int lTimeSec;      /*timestamp*/
	unsigned int ssrc;         /*synchronization source*/
	unsigned short usProfile;
	unsigned short usExtLen;
	unsigned int uiDecoderID;
	//unsigned int uiPadding;
	unsigned char ucFrameNum;    /*帧内分包数量*/
	unsigned char ucFrameSeq;    /*帧内分包序号,从0开始*/
	unsigned short usPtLen;    /*有效数据长度*/
} RTPHEADER_S;

#ifndef RTP_HEADER_LENGTH
#define RTP_HEADER_LENGTH  sizeof(RTPHEADER_S)
#endif

class XFrameAllocator;

class BufferFrame
{
public:
	explicit BufferFrame(XFrameAllocator& allocator,char *ptr);
	~BufferFrame();
	
	void create(const RTPHEADER_S &hdr,unsigned char* buffer);
	void destroy();	

	const char* getData();
	int getDataLength() const;

	const char* getPtr();
	int getLength() const;	

	void addRef();
	void release();
	int  getRefCount() const;
	bool isIdle() const;	

private:
	XFrameAllocator& m_allocator;

	char* m_ptr; ///< buffer pointer	
	unsigned m_length;
	int m_refCount;
};

/////////////////////////////////////////////////////////////////////////////
typedef BufferFrame* BufferFramePtr;
typedef std::deque<BufferFramePtr>  BufferPtrQueue;
/////////////////////////////////////////////////////////////////////////////

class XFrameAllocator
{
public:
	XFrameAllocator();
	virtual ~XFrameAllocator();

	virtual BufferFramePtr allocFrame(unsigned length);  ///从池中申请一个frame
	virtual void freeFrame(BufferFramePtr pFrame);  ///释放一个frame到池中

	unsigned getPoolSize(); ///得到内存池大小

	bool create(unsigned maxLength);    ///创建池
	void destroy();   ///销毁池

private:
	/// disable assign and copy
	XFrameAllocator(const XFrameAllocator& ref);
	XFrameAllocator& operator =(const XFrameAllocator& ref);

private:
	XAllocator m_MemPool;  //采用内存管理 

};


#endif




