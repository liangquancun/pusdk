#include "BufferFrame.h"
#include "NetMedia.h"


BufferFrame::BufferFrame(XFrameAllocator& allocator,char *ptr)
	:m_allocator(allocator),
	 m_ptr(ptr),
	 m_length(0),
	 m_refCount(0)
{	
}

BufferFrame::~BufferFrame()
{
	 destroy();
}

void BufferFrame::create(const RTPHEADER_S &hdr,unsigned char* buffer)
{
	RTPHEADER_S* pHeader = (RTPHEADER_S*) m_ptr;
	*pHeader = hdr;

	m_length = ntohs(hdr.usPtLen) + RTP_HEADER_LENGTH;
	memcpy(m_ptr + RTP_HEADER_LENGTH, buffer, ntohs(hdr.usPtLen));
}

void BufferFrame::destroy()
{
	if (m_ptr != NULL)
	{        
		m_ptr = NULL;       
		m_length = 0;
		m_refCount = 0;
	}
}


const char* BufferFrame::getData()
{
	char* pData = m_ptr;
	pData += RTP_HEADER_LENGTH;
	return pData;
}

int BufferFrame::getDataLength() const
{
	return m_length - RTP_HEADER_LENGTH;
}

const char* BufferFrame::getPtr()
{
	return m_ptr;
}

int BufferFrame::getLength() const
{
	return m_length;
}


void BufferFrame::addRef()
{
	++m_refCount;
}

void BufferFrame::release()
{
	--m_refCount;
	if (m_refCount <= 0)
	{
        
		m_allocator.freeFrame(this);
		m_refCount = 0;
	}
}

int BufferFrame::getRefCount() const
{
	return m_refCount;
}

bool BufferFrame::isIdle() const
{
	return (m_refCount == 0);
}


XFrameAllocator::XFrameAllocator()
{	
}

XFrameAllocator::~XFrameAllocator()
{
	destroy();
}

bool XFrameAllocator::create(unsigned maxLength)
{
	return m_MemPool.CreatePool(maxLength);
}

void XFrameAllocator::destroy()
{
	m_MemPool.DestroyPool();
}

BufferFramePtr XFrameAllocator::allocFrame(unsigned length)
{	
	unsigned reqLength = length + sizeof(BufferFrame);
	char *ptr = (char *)m_MemPool.Malloc(reqLength); 
	if (ptr == NULL)    ///no space
	{
		return NULL;
	}	

	///placement new
	BufferFramePtr pFrame = 
		new(ptr)BufferFrame(*this,ptr + sizeof(BufferFrame));

	return pFrame;
}


void XFrameAllocator::freeFrame(BufferFramePtr pFrame)
{
	if( pFrame == NULL )
	{
		return;
	}

	pFrame->~BufferFrame();  

	m_MemPool.Free(pFrame);  /// ·µ»ØÄÚ´æµ½³Ø
}



unsigned XFrameAllocator::getPoolSize()
{
	return m_MemPool.GetPoolSize();
}

