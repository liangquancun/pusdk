/*    file: TSocket.h
 *    desc:
 *   
 * created: 2009-08-19 14:47:20
 *  author: 
 * version: 1.0
 * company: 
 */

#if !defined TSOCKET__
#define TSOCKET__

////////////////////////////////////////////////////////////////////////////
#include <string>
#include <exception>

#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#endif // WIN32
//////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif // WIN32
//////////////////////////////////////////////////////////////////////////////

class TSockAddr;
class TSocket;

//////////////////////////////////////////////////////////////////////////////
class TSocketLib
{
public:
    static bool init();
    static void unInit();

    static int getLastError();
    static void clearLastError();
    static bool isErrorIntr();

    static bool isWouldBlock(int err);
    static bool isWouldBlock();

    static std::string getHostIP();

    static std::string getLocalIP();

    static int getAddr(const char *hostname, struct in_addr *addr);
    static int lookup(const char *hostname, struct sockaddr_in *sin);
    static bool isDotstring(const std::string& host);
    static std::string host2DotString(const std::string& host);

};

//////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
enum
{
    ERR_INTR = WSAEINTR,
    ERR_WOULDBLOCK = WSAEWOULDBLOCK
};
#else
#define	INVALID_SOCKET	(-1)
#define SOCKET_ERROR	(-1)

enum
{
    ERR_INTR = EINTR, ERR_WOULDBLOCK = EWOULDBLOCK
};

#endif //WIN32
//////////////////////////////////////////////////////////////////////////////

/// socket address
class TSockAddr
{
public:
    struct sockaddr_in m_sockaddr;
    unsigned int m_socklen;

    TSockAddr(const char * pHost = 0, unsigned short port = 0);
    TSockAddr(const std::string& host, unsigned short port = 0);

    void set(const char * pHost = 0, unsigned short port = 0);

    void set(const std::string& host, unsigned short port = 0)
    {
        set(host.c_str(), port);
    }

    /**
     * set address
     * @param addr can be host byte order and special addresses(INADDR_ANY, INADDR_BROADCAST ...)
     * @param port
     */
    void set(unsigned int addr, unsigned short port);

    void set(int addr, unsigned short port)
    {
        set((unsigned int) addr, port);
    }

    void setPort(unsigned short port);

    unsigned int getAddr() const;

    unsigned short getPort() const;

    const char * getIPAddr() const;

    std::string toString() const;

};

////////////////////////////////////////////////////////////////////////////

/// socket wrapper
class TSocket
{
public:
    TSocket();
    ~TSocket();

    explicit TSocket(socket_t sock);

    /// attach socket fd
    void attach(socket_t sock);

    void attach(const TSocket& sock);

    /// detach and return socket fd
    socket_t detach();

    socket_t getSocket() const
    {
        return m_socket;
    }

    /// return true if the socket is valid
    bool isOpen() const
    {
        return (INVALID_SOCKET != m_socket);
    }

    /**
     * @see isOpen()
     */
    operator bool() const
    {
        return isOpen();
    }

    /**
     * open socket.
     * @param type SOCK_STREAM for TCP and SOCK_DGRAM for UDP, TCP is default
     */
    bool open(int type = SOCK_STREAM);

    void close();

    /**
     * bind address.
     * @return on success, 0 is returned
     */
    int bind(const TSockAddr& addr);

    /**
     * connect to server.
     * @return 0 on success
     */
    int connect(const TSockAddr& addr);
    int connect(const char * hostName, unsigned short port);

    /**
     * accept connection.
     * @param addr is address of remote peer
     * @return socket
     */
    TSocket accept(TSockAddr& addr);

    /**
     * listen.
     * @return 0 on success
     */
    int listen(int backlog = 1024);

    /**
     * receive data.
     * @param buf
     * @param size
     * @param flag
     * @ return length of received data
     */
    int receive(char * buf, int size, int flag = 0);

    /**
     * send data.
     * @param buf
     * @param size
     * @param flag
     * @return length of sent data
     */
    int send(const char * buf, int size, int flag = 0);

    int receiveFrom(char * buf, int size, int flag, TSockAddr& addr);
    int sendTo(const char * buf, int size, int flag, const TSockAddr& addr);

    /// shut down
    int shutDown(int flag);

    /// enable broadcast
    bool setBroadcast(bool enable);

    bool setNoLinger(bool enable);

    /// keep alive
    bool setKeepAlive(bool enable, int keepIdle, int keepInterval,
            int keepCount);

    int getSendBufferSize();
    int getRecvBufferSize();
    bool setSendBufferSize(int size);
    bool setRecvBufferSize(int size);

    /// get address of remote peer
    TSockAddr getPeerName();

    /// get local address
    TSockAddr getSockName();

    /// set none block
    bool setNonblock(bool enable);

    /// set reuse
    bool setReuse();

    /**
     * set timeout.
     * @param timeout milliseconds
     */
    bool setSendTimeout(int milliseconds);

    bool setRecvTimeout(int milliseconds);

    /**
     * check readable.
     * @param millsec timeout in millisecond, if millsec < 0, socket will be blocked until it is readable
     * @return true if the socket is readable
     */
    bool checkReadable(long millsec);

    /**
     * check writable
     * @param millsec timeout in millisecond, if millsec < 0, socket will be blocked until it is writable
     * @return true if the socket is writable
     */
    bool checkWritable(long millsec);

    /**
     * send data block.
     * send all of the data except error occurs. used for TCP
     * @return length of sent data
     */
    int sendBlock(const char * buf, int len);

    /**
     * receive data block
     * receive all of the data, except error occurs. used for TCP
     * @return length of received data
     */
    int receiveBlock(char* buf, int len);

    /**
     * check write and exception
     * @param millsec timeout in millisecond, if millsec < 0, socket will be blocked until it is writable
     * @return 0 on timeout, 1 on writable, -1 on exception
     */
    int checkWriteAndException(long millsec);

    /**
     * check write and exception
     * @param millsec timeout in millisecond, if millsec < 0, socket will be blocked until it is writable
     * @return 0 on timeout, 1 on writable, -1 on exception
     */
    int checkConnect(long millsec);
    int readnInTime(void *buffer, int length,int msec);

    int recvFrame(char *buf,int inLen,char chBegin,char chEnd,int wantReadLen = 0);

protected:
    socket_t m_socket;

};

////////////////////////////////////////////////////////////////////////////
#endif //TSOCKET__
