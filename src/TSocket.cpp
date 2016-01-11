/*    file: TSocket.cpp
 *    desc:
 * 
 * created: 2009-08-19 14:57:58
 *  author: 
 * version: 1.0
 * company: 
 */

#include "TSocket.h"
#include <cstdio>

#include <assert.h>
#include <sstream>

#ifdef WIN32

typedef int socklen_t;
#include <mstcpip.h>
#else
#include <signal.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>

#ifndef MAX_PATH
#define	MAX_PATH	256
#endif //MAX_PATH
#endif // WIN32


#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4966 )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4018 )
#endif


//////////////////////////////////////////////////////////////////////////////
bool TSocketLib::init()
{
#ifdef WIN32
    WORD versionWanted = MAKEWORD(2,2);
    WSADATA wsaData;
    return ( WSAStartup(versionWanted, &wsaData) != 0 );
#else //assume linux/unix
	/*
    // SIGPIPE is generated when a remote socket is closed
    void (*handler)(int);
    handler = signal(SIGPIPE, SIG_IGN);
    if (handler != SIG_DFL)
    {
        signal(SIGPIPE, handler);
    }
    return true;*/
#endif
}

void TSocketLib::unInit()
{
#ifdef WIN32                                   
    if(WSACleanup() == SOCKET_ERROR)
    {
        if(WSAGetLastError() == WSAEINPROGRESS)
        {
            WSACancelBlockingCall();
            WSACleanup();
        }
    }
#else //assume linux/unix                      
    // Restore the SIGPIPE handler             
	/*
    void (*handler)(int);
    handler = signal(SIGPIPE, SIG_DFL);
    if (handler != SIG_IGN)
    {
        signal(SIGPIPE, handler);
    }
	*/
#endif                                         
}

int TSocketLib::getLastError()
{
#ifdef WIN32
    return ::WSAGetLastError();
#else
    return errno;
#endif
}

void TSocketLib::clearLastError()
{
#if defined( WIN32 ) || defined(__CYGWIN__)
    WSASetLastError(0);
#else
    errno = 0;
#endif
}

bool TSocketLib::isErrorIntr()
{
    return (ERR_INTR == getLastError());
}

bool TSocketLib::isWouldBlock(int err)
{

#ifdef WIN32
    return ( WSAEWOULDBLOCK == err );
#else
    return (EINPROGRESS == err || EWOULDBLOCK == err);
#endif//WIN32
}

bool TSocketLib::isWouldBlock()
{
    return isWouldBlock(getLastError());
}

std::string TSocketLib::getHostIP()
{
    std::string ip;
    char buff[MAX_PATH];
    int ret = ::gethostname(buff, MAX_PATH);
    if (0 == ret)
    {
        hostent *host = ::gethostbyname(buff);
        if (host != NULL)
        {
            ip = inet_ntoa(*(in_addr*) host->h_addr_list[0]);
        }
    }
    return ip;
}

std::string TSocketLib::getLocalIP()
{
#ifdef WIN32
    return getHostIP();
#else
    int ret = 0;
    int inet_sock;
    struct ifreq ifr;
    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
    //eth0涓烘帴鍙ｅ埌鍚嶇О
    strcpy(ifr.ifr_name, "eth0");
    //SIOCGIFADDR鏍囧織浠ｈ〃鑾峰彇鎺ュ彛鍦板潃
    if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0)
    {
        strcpy(ifr.ifr_name, "eth1");
        ret = ioctl(inet_sock, SIOCGIFADDR, &ifr);
    }

    if (0 == ret)
    {
        return inet_ntoa(((struct sockaddr_in*) &(ifr.ifr_addr))->sin_addr);
    }
    else
    {
        return "127.0.0.1";
    }

#endif //WIN32
}

int TSocketLib::getAddr(const char *hostname, struct in_addr *addr)
{
    int ret = 0;
    struct hostent *h;

    h = ::gethostbyname(hostname);
    if (h != 0)
    {
        assert( h->h_addrtype == AF_INET );
        *addr = *(struct in_addr*) (h->h_addr);
    }
    else
    {
        ret = SOCKET_ERROR;
    }
    return ret;
}

int TSocketLib::lookup(const char *hostname, struct sockaddr_in *sin)
{
    int ret = getAddr(hostname, &sin->sin_addr);
    sin->sin_family = AF_INET;
    return ret;
}

bool TSocketLib::isDotstring(const std::string& host)
{
    bool ok = true;
    for (size_t i = 0; i < host.size(); ++i)
    {
        if (host[i] != '.')
        {
            if (host[i] < '0' || host[i] > '9')
            {
                ok = false;
                break;
            }
        }
    }
    return ok;
}

std::string TSocketLib::host2DotString(const std::string& host)
{
    if (isDotstring(host))
        return host;
    else
    {
        sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        lookup(host.c_str(), &sin);

        struct
        {
            unsigned char s_b1, s_b2, s_b3, s_b4;
        } tmp;
        memcpy(&tmp, &sin.sin_addr, sizeof(tmp));

        static char buff[100];
        sprintf(buff, "%u.%u.%u.%u", tmp.s_b1, tmp.s_b2, tmp.s_b3, tmp.s_b4);
        return std::string(buff);
    }
}
//////////////////////////////////////////////////////////////////////////////
TSockAddr::TSockAddr(const std::string& host, unsigned short port)
{
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));
    m_socklen = (unsigned int) sizeof(m_sockaddr);
    set(host.c_str(), port);
}

TSockAddr::TSockAddr(const char * pHost, unsigned short port)
{
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));
    m_socklen = (unsigned int) sizeof(m_sockaddr);
    set(pHost, port);
}

void TSockAddr::set(const char * pHost, unsigned short port)
{
    std::string dotstring;
    if (!pHost)
    {
        pHost = "0.0.0.0";
    }
    else if (!TSocketLib::isDotstring(pHost))
    {
        dotstring = TSocketLib::host2DotString(pHost);
        pHost = dotstring.c_str();
    }

    //lint -e644 -e516 -e529 -e530
    m_sockaddr.sin_family = AF_INET;
    m_sockaddr.sin_addr.s_addr = inet_addr(pHost);
    m_sockaddr.sin_port = htons(port);
    m_socklen = (unsigned int) sizeof(m_sockaddr);
}

void TSockAddr::set(unsigned int addr, unsigned short port)
{
    m_sockaddr.sin_family = AF_INET;
    m_sockaddr.sin_addr.s_addr = htonl(addr);
    m_sockaddr.sin_port = htons(port);
    m_socklen = (unsigned int) sizeof(m_sockaddr);
}

void TSockAddr::setPort(unsigned short port)
{
    m_sockaddr.sin_port = htons(port);
}

const char * TSockAddr::getIPAddr() const
{
    return inet_ntoa(m_sockaddr.sin_addr);
}

unsigned int TSockAddr::getAddr() const
{
    return ntohl(m_sockaddr.sin_addr.s_addr);
}

unsigned short TSockAddr::getPort() const
{
    return ntohs(m_sockaddr.sin_port);
}
;

std::string TSockAddr::toString() const
{
    std::ostringstream ss;
    ss << getIPAddr();
    ss << ":";
    ss << getPort();
    return ss.str();
}
//////////////////////////////////////////////////////////////////////////////


TSocket::TSocket()
{
    m_socket = INVALID_SOCKET;
}

TSocket::~TSocket()
{

}

TSocket::TSocket(socket_t sock)
{
    m_socket = sock;
}

void TSocket::attach(const TSocket& sock)
{
    m_socket = sock.m_socket;
}

void TSocket::attach(socket_t sock)
{
    m_socket = sock;
}

socket_t TSocket::detach()
{
    socket_t theSocket = m_socket;
    m_socket = INVALID_SOCKET;
    return theSocket;
}

bool TSocket::open(int type)
{
    assert( !isOpen() );

    m_socket = ::socket(AF_INET, type, 0);
    return (INVALID_SOCKET != m_socket);
}

void TSocket::close()
{
    if (isOpen())
    {
        ::shutdown(m_socket, 0x02);
#if defined(WIN32) && !defined(__CYGWIN32__)

        ::closesocket( m_socket );
#else
        ::close(m_socket);
#endif
        m_socket = INVALID_SOCKET;
    }
}

int TSocket::bind(const TSockAddr& addr)
{
    return ::bind(m_socket, (sockaddr*) &addr.m_sockaddr, addr.m_socklen);
}

int TSocket::connect(const TSockAddr& addr)
{
    return ::connect(m_socket, (sockaddr*) &addr.m_sockaddr, addr.m_socklen);
}

int TSocket::connect(const char * hostName, unsigned short port)
{
    TSockAddr addr(hostName, port);
    return connect(addr);
}

TSocket TSocket::accept(TSockAddr& addr)
{
    socklen_t len = sizeof(addr);
    socket_t sock = ::accept(m_socket, (sockaddr*) &addr.m_sockaddr, &len);
    return TSocket(sock);
}

int TSocket::listen(int backlog)
{
    return ::listen(m_socket, backlog);
}

int TSocket::receive(char * buf, int size, int flag)
{
    return ::recv(m_socket, buf, size, flag);
}

int TSocket::send(const char * buf, int size, int flag)
{
    return ::send(m_socket, buf, size, flag);
}


int TSocket::receiveFrom(char* buf, int size, int flag, TSockAddr& addr)
{
    socklen_t len = sizeof(addr);
    return ::recvfrom(m_socket, buf, size, flag, (sockaddr*) &addr.m_sockaddr,
            &len);
}

int TSocket::sendTo(const char* buf, int size, int flag, const TSockAddr& addr)
{
    return ::sendto(m_socket, buf, size, flag, (sockaddr*) &addr.m_sockaddr,
            (int) addr.m_socklen);
}

int TSocket::shutDown(int flag)
{
    return ::shutdown(m_socket, flag);
}

bool TSocket::setBroadcast(bool enable)
{
    int val = enable ? 1 : 0;
    int ret = ::setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char *) &val,
            sizeof(val));
    return (0 == ret);
}

bool TSocket::setNoLinger(bool enable)
{
    struct linger l;
    l.l_onoff = enable ? 1 : 0;
    l.l_linger = 0;
    int ret = ::setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char *) &l,
            sizeof(l));
    return (0 == ret);
}

bool TSocket::setKeepAlive(bool enable, int keepIdle, int keepInterval,
        int keepCount)
{
    int val = enable ? 1 : 0;
    int ret = ::setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char *) &val,
            sizeof(val));
#ifdef WIN32
    tcp_keepalive keepalive;
    keepalive.onoff = enable ? 1 : 0;
    keepalive.keepalivetime = keepIdle * 1000;
    keepalive.keepaliveinterval = keepInterval * 1000;

    DWORD dwReturn = 0;
    ret = ::WSAIoctl( m_socket, SIO_KEEPALIVE_VALS, &keepalive, sizeof(keepalive),
            NULL, 0, &dwReturn, NULL, NULL );

#else
    ::setsockopt(m_socket, SOL_TCP, TCP_KEEPIDLE, (void *) &keepIdle,
            sizeof(keepIdle));
    ::setsockopt(m_socket, SOL_TCP, TCP_KEEPINTVL, (void *) &keepInterval,
            sizeof(keepInterval));
    ::setsockopt(m_socket, SOL_TCP, TCP_KEEPCNT, (void *) &keepCount,
            sizeof(keepCount));
#endif //WIN32
    return (0 == ret);
}

int TSocket::getSendBufferSize()
{
    int len = 0;
    socklen_t sizeInt = (int) sizeof(len);
    int ret = ::getsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char *) &len,
            &sizeInt);
    if (0 != ret)
    {
        len = -1;
    }
    return len;
}

int TSocket::getRecvBufferSize()
{
    int len = 0;
    socklen_t sizeInt = sizeof(len);
    int ret = ::getsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char *) &len,
            &sizeInt);
    if (0 != ret)
    {
        len = -1;
    }
    return len;
}

bool TSocket::setSendBufferSize(int size)
{
    int ret = ::setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char *) &size,
            sizeof(size));
    return (0 == ret);
}

bool TSocket::setRecvBufferSize(int size)
{
    int ret = ::setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char *) &size,
            sizeof(size));
    return (0 == ret);
}

TSockAddr TSocket::getPeerName()
{
    TSockAddr addr;
    socklen_t len = sizeof(addr);
    ::getpeername(m_socket, (sockaddr*) &addr.m_sockaddr, &len);
    return addr;
}

TSockAddr TSocket::getSockName()
{
    TSockAddr addr;
    socklen_t len = sizeof(addr);
    ::getsockname(m_socket, (sockaddr*) &addr.m_sockaddr, &len);
    return addr;
}

bool TSocket::setNonblock(bool enable)
{
    int ret = 0;
#ifdef WIN32
    unsigned long fl = enable ? 1 : 0;
    ret = ioctlsocket( m_socket, FIONBIO, &fl);
#else
    int fl = fcntl(m_socket, F_GETFL);
    if (fl >= 0)
    {
        ret = fcntl(m_socket, F_SETFL, fl | O_NONBLOCK);
    }
#endif
    return (ret >= 0);
}

bool TSocket::setReuse()
{
    int ret = 0;
    const int one = 1;

#ifdef SO_REUSEADDR
    ret = ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*) &one,
            sizeof(one));
#endif
#ifdef SO_REUSEPORT
    ret = ::setsockopt( m_socket, SOL_SOCKET, SO_REUSEPORT,
            (const void*)&one, sizeof(one) );

#endif
    return (ret >= 0);
}

bool TSocket::setSendTimeout(int milliseconds)
{
#ifdef WIN32
    int ret = setsockopt( m_socket, SOL_SOCKET, SO_SNDTIMEO,
            (const char*)&milliseconds, sizeof(int) );
#else
    timeval tv =
    { milliseconds / 1000, (milliseconds % 1000) * 1000 };
    int ret = setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*) &tv,
            sizeof(tv));
#endif //WIN32
    return (0 == ret);
}

bool TSocket::setRecvTimeout(int milliseconds)
{
#ifdef WIN32
    int ret = setsockopt( m_socket, SOL_SOCKET, SO_RCVTIMEO,
            (const char*)&milliseconds, sizeof(int) );
#else
    timeval tv =
    { milliseconds / 1000, (milliseconds % 1000) * 1000 };
    int ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv,
            sizeof(tv));
#endif //WIN32
    return (0 == ret);
}

bool TSocket::checkReadable(long millsec)
{
    fd_set rset;
    FD_ZERO( &rset );
    FD_SET( m_socket, &rset );
    timeval* ptv = NULL;
    timeval tv =
    { millsec / 1000, (millsec % 1000) * 1000 };
    ptv = (millsec >= 0) ? (&tv) : NULL;

    int ret = select((int) m_socket + 1, &rset, NULL, NULL, ptv);
    return (1 == ret);
}

bool TSocket::checkWritable(long millsec)
{
    fd_set wset;
    FD_ZERO( &wset );
    FD_SET( m_socket, &wset );
    timeval* ptv = NULL;
    timeval tv =
    { millsec / 1000, (millsec % 1000) * 1000 };
    ptv = (millsec >= 0) ? (&tv) : NULL;

    int ret = select((int) m_socket + 1, NULL, &wset, NULL, ptv);
    return (1 == ret);
}

int TSocket::sendBlock(const char * buf, int len)
{
    int length = len;
    int bytes = 0;
    do
    {
        bytes = send(buf, length, 0);
        if (bytes > 0)
        {
            length -= bytes;
            buf += bytes;
        }
        else if (SOCKET_ERROR == bytes)
        {
            if (ERR_INTR == TSocketLib::getLastError())
            {
                continue;
            }
        }
    } while (bytes > 0 && length > 0);
    return (len - length);
}

int TSocket::receiveBlock(char* buf, int len)
{
    int length = len;
    int bytes = 0;
    do
    {
        bytes = receive(buf, length);
        if (bytes > 0)
        {
            length -= bytes;
            buf += bytes;
        }
        else if (SOCKET_ERROR == bytes)
        {
            if (ERR_INTR == TSocketLib::getLastError())
            {
                continue;
            }
        }
    } while (bytes > 0 && length > 0);
    return (len - length);
}

int TSocket::checkWriteAndException(long millsec)
{
    fd_set wset;
    FD_ZERO( &wset );
    FD_SET( m_socket, &wset );

    fd_set setEx;
    FD_ZERO( &setEx );
    FD_SET( m_socket, &setEx );

    timeval* ptv = NULL;
    timeval tv =
    { millsec / 1000, (millsec % 1000) * 1000 };
    ptv = (millsec >= 0) ? (&tv) : NULL;

    int ret = select((int) m_socket + 1, NULL, &wset, &setEx, ptv);
    if (ret > 0)
    {
        if (FD_ISSET( m_socket, &setEx ))
        {
            ret = -1;
        }
    }
    return ret;
}

int TSocket::checkConnect(long millsec)
{
#ifdef WIN32 
    return checkWriteAndException( millsec );
#endif //WIN32
    fd_set rset;
    fd_set wset;
    FD_ZERO( &rset );
    FD_ZERO( &wset );
    FD_SET( m_socket, &rset );
    FD_SET( m_socket, &wset );

    timeval* ptv = NULL;
    timeval tv =
    { millsec / 1000, (millsec % 1000) * 1000 };
    ptv = (millsec >= 0) ? (&tv) : NULL;

    int ret = select((int) m_socket + 1, NULL, &wset, NULL, ptv);
    if (ret > 0)
    {
        int error = 0;
        socklen_t len = sizeof(error);
        getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*) &error, &len);
        ret = (error == 0) ? 1 : -1;
    }
    return ret;
}

int TSocket::readnInTime(void *buffer, int length,int msec)
{
    int  nleft = length;
    int  nread = 0; 	
    char *ptr = (char *)buffer;	

    while(nleft > 0)
    {
        if( ! checkReadable(msec) ) 
        {			
            break;		
        }

        nread = receive(ptr, nleft);		
        if ( nread < 0) 
        {
            if (TSocketLib::getLastError() == ERR_INTR)
            {
                nread = 0;      /* and call read() again */
            }
            else if (TSocketLib::getLastError() == ERR_WOULDBLOCK)
            {
                break;
            }
            else
            {
                return SOCKET_ERROR;
            }
        }
        else if (nread == 0)
        {
            break;  
        }

        nleft -= nread;
        ptr += nread;			
    }
    return length - nleft;	
}

int  TSocket::recvFrame(char *buf,int inLen,char chBegin,char chEnd,int wantReadLen)
{	   
    int ret = -1;
    if (buf == NULL || inLen == 0)
    {
        return ret;
    }      
   
    char *ptr = (char *) buf;

    ///过滤数据
    ret = readnInTime((char *)ptr,1,1000);	
    if (ret <= 0)
    {
        return -1;
    }

    ///不 是 开头数据,扔掉，但不能关闭连接
    if (*ptr != chBegin)
    {	
        ret = receiveBlock(&buf[ret],inLen-1);
        if (ret <= 0)
        {
            ret = -1;
        }
        return ret;
    }
   
    int readLen = ret;
    if (wantReadLen > 0)
    {
        inLen = wantReadLen;
    }

    int i = 0;
    ///过滤数据，
    for (i = readLen; (i < inLen) && (ret == 1) ; i++)
    {		
        ptr += ret;		
        ret = readnInTime((char *)ptr,1,1000);	
        if (ret > 0)
        {
            readLen += ret;
        }

        if (ret <= 0)
        {   
            readLen = -1;
            break;
        }
        if (*ptr == chEnd)
        {	            
            break;
        }
    }    

    return readLen;
}
