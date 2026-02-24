# AGENTS.md - Network Implementation

## Directory Structure

```
src/network/
├── socket.c                # Socket creation
├── socketpair.c            # Socket pair creation
├── bind.c                  # Bind socket to address
├── listen.c                # Listen for connections
├── accept.c                # Accept incoming connection
├── accept4.c               # Accept with flags
├── connect.c               # Connect to socket
├── send.c                  # Send data
├── sendto.c                # Send to address
├── sendmsg.c               # Send message
├── sendmmsg.c              # Send multiple messages
├── recv.c                  # Receive data
├── recvfrom.c              # Receive from address
├── recvmsg.c               # Receive message
├── recvmmsg.c              # Receive multiple messages
├── getsockopt.c            # Get socket options
├── setsockopt.c            # Set socket options
├── getsockname.c           # Get socket name
├── getpeername.c           # Get peer name
├── shutdown.c              # Shutdown socket
├── sockatmark.c            # Check for out-of-band mark
├── getaddrinfo.c           # Address translation
├── getnameinfo.c           # Name translation
├── freeaddrinfo.c          # Free address info
├── gai_strerror.c          # Get error message
├── gethostbyname.c         # Host by name (legacy)
├── gethostbyname_r.c       # Reentrant host by name
├── gethostbyname2.c        # Host by name (IPv6)
├── gethostbyname2_r.c      # Reentrant host by name (IPv6)
├── gethostbyaddr.c         # Host by address
├── gethostbyaddr_r.c       # Reentrant host by address
├── getservbyname.c         # Service by name
├── getservbyname_r.c       # Reentrant service by name
├── getservbyport.c         # Service by port
├── getservbyport_r.c       # Reentrant service by port
├── getifaddrs.c            # Get interface addresses
├── if_freenameindex.c      # Free interface index
├── if_indextoname.c        # Index to name
├── if_nametoindex.c        # Name to index
├── if_nameindex.c          # Get interface index
├── inet_ntoa.c             # IPv4 to string
├── inet_aton.c             # String to IPv4
├── inet_addr.c             # String to IPv4 (legacy)
├── inet_ntop.c             # IP to string
├── inet_pton.c             # String to IP
├── inet_legacy.c            # Legacy inet functions
├── htonl.c                 # Host to network long
├── htons.c                 # Host to network short
├── ntohl.c                 # Network to host long
├── ntohs.c                 # Network to host short
├── res_init.c              # Resolver initialization
├── res_query.c             # Resolver query
├── res_querydomain.c       # Resolver query domain
├── res_send.c              # Resolver send
├── res_mkquery.c           # Make resolver query
├── res_msend.c             # Multiple resolver sends
├── res_state.c             # Resolver state
├── resolvconf.c            # Parse resolv.conf
├── res_cache.c             # DNS caching
├── dn_comp.c               # Domain name compression
├── dn_expand.c             # Domain name expansion
├── dn_skipname.c           # Skip domain name
├── ns_parse.c              # Parse name server
├── dns_parse.c             # Parse DNS response
├── lookup.h                # Lookup infrastructure
├── lookup_name.c           # Name lookup
├── lookup_ipliteral.c      # IP literal lookup
├── lookup_serv.c           # Service lookup
├── netlink.c               # Netlink socket operations
├── netlink.h               # Netlink definitions
├── ent.c                   # Entity parsing
├── serv.c                  # Service parsing
├── proto.c                 # Protocol parsing
├── netname.c               # Network name utilities
├── ether.c                 # Ethernet address operations
├── h_errno.c               # h_errno variable
├── herror.c                # Print h_errno error
├── hstrerror.c             # h_errno to string
├── in6addr_any.c           # IPv6 any address
├── in6addr_loopback.c      # IPv6 loopback address
├── __cmsg_nxthdr.c        # Next cmsg helper
├── linux/                  # Linux-specific implementations
└── liteos_a/              # LiteOS-specific implementations
```

---

## 1. Build Commands

### Building the Project
```bash
# Configure for target architecture
./configure --target=aarch64-linux-musl --prefix=/usr/local/musl

# Build all components
make

# Clean build artifacts
make clean
make distclean
```

### Testing Commands
```bash
# Build libc-test suite
cd libc-test
make -f Makefile config.mak
make -f Makefile

# Run specific network tests
./src/common/runtest.exe -w '' socket
./src/common/runtest.exe -w '' connect
./src/common/runtest.exe -w '' accept
./src/common/runtest.exe -w '' getaddrinfo
./src/common/runtest.exe -w '' getnameinfo
```

### Building Single Object
```bash
# Rebuild specific network object file
make obj/src/network/socket.o
make obj/src/network/accept.o
make obj/src/network/getaddrinfo.o
```

---

## 2. Socket API Categories

### 2.1 Basic Socket Operations

| File | Function | Description |
|------|----------|-------------|
| `socket.c` | `socket()` | Create socket |
| `socketpair.c` | `socketpair()` | Create socket pair |
| `bind.c` | `bind()` | Bind socket to address |
| `listen.c` | `listen()` | Listen for connections |
| `accept.c` | `accept()` | Accept connection |
| `accept4.c` | `accept4()` | Accept with flags |
| `connect.c` | `connect()` | Connect to socket |
| `shutdown.c` | `shutdown()` | Shutdown socket |

### 2.2 Data Transfer

| File | Function | Description |
|------|----------|-------------|
| `send.c` | `send()` | Send data |
| `sendto.c` | `sendto()` | Send to address |
| `sendmsg.c` | `sendmsg()` | Send message |
| `sendmmsg.c` | `sendmmsg()` | Send multiple messages |
| `recv.c` | `recv()` | Receive data |
| `recvfrom.c` | `recvfrom()` | Receive from address |
| `recvmsg.c` | `recvmsg()` | Receive message |
| `recvmmsg.c` | `recvmmsg()` | Receive multiple messages |

### 2.3 Socket Options

| File | Function | Description |
|------|----------|-------------|
| `getsockopt.c` | `getsockopt()` | Get socket option |
| `setsockopt.c` | `setsockopt()` | Set socket option |
| `getsockname.c` | `getsockname()` | Get socket name |
| `getpeername.c` | `getpeername()` | Get peer name |
| `sockatmark.c` | `sockatmark()` | Check OOB mark |

### 2.4 Address Translation (Modern)

| File | Function | Description |
|------|----------|-------------|
| `getaddrinfo.c` | `getaddrinfo()` | Address translation |
| `getnameinfo.c` | `getnameinfo()` | Name translation |
| `freeaddrinfo.c` | `freeaddrinfo()` | Free address info |
| `gai_strerror.c` | `gai_strerror()` | Get error message |

### 2.5 Address Translation (Legacy)

| File | Function | Description |
|------|----------|-------------|
| `gethostbyname.c` | `gethostbyname()` | Host by name |
| `gethostbyname_r.c` | `gethostbyname_r()` | Reentrant version |
| `gethostbyaddr.c` | `gethostbyaddr()` | Host by address |
| `gethostbyaddr_r.c` | `gethostbyaddr_r()` | Reentrant version |
| `getservbyname.c` | `getservbyname()` | Service by name |
| `getservbyname_r.c` | `getservbyname_r()` | Reentrant version |
| `getservbyport.c` | `getservbyport()` | Service by port |
| `getservbyport_r.c` | `getservbyport_r()` | Reentrant version |

### 2.6 Interface Operations

| File | Function | Description |
|------|----------|-------------|
| `getifaddrs.c` | `getifaddrs()` | Get interface addresses |
| `if_freenameindex.c` | `if_freenameindex()` | Free interface list |
| `if_indextoname.c` | `if_indextoname()` | Index to name |
| `if_nametoindex.c` | `if_nametoindex()` | Name to index |
| `if_nameindex.c` | `if_nameindex()` | Get interface index |

### 2.7 IP Address Conversion

| File | Function | Description |
|------|----------|-------------|
| `inet_ntoa.c` | `inet_ntoa()` | IPv4 to string |
| `inet_aton.c` | `inet_aton()` | String to IPv4 |
| `inet_addr.c`` | `inet_addr()` | String to IPv4 (legacy) |
| `inet_ntop.c` | `inet_ntop()` | IP to string |
| `inet_pton.c` | `inet_pton()` | String to IP |

### 2.8 Byte Order Conversion

| File | Function | Description |
|------|----------|-------------|
| `htonl.c` | `htonl()` | Host to network long |
| `htons.c` | `htons()` | Host to network short |
| `ntohl.c` | `ntohl()` | Network to host long |
| `ntohs.c` | `ntohs()` | Network to host short |

### 2.9 DNS Resolver

| File | Function | Description |
|------|----------|-------------|
| `res_init.c` | `res_init()` | Initialize resolver |
| `res_query.c` | `res_query()` | Query DNS server |
| `res_querydomain.c` | `res_querydomain()` | Query domain |
| `res_send.c` | `res_send()` | Send query |
| `res_mkquery.c` | `res_mkquery()` | Make query packet |
| `res_msend.c` | `res_msend()` | Send multiple queries |
| `resolvconf.c` | `__res_maybe_rc()` | Parse resolv.conf |
| `res_cache.c` | DNS caching infrastructure | |

---

## 3. Code Style Guidelines

### File Structure and Headers
- Apache 2.0 license header in all source files
- Include `"syscall.h"` for system call wrappers
- Include `"lookup.h"` for DNS lookup infrastructure
- Include `<sys/socket.h>` for socket definitions

### Imports and Includes
- System headers first: `#include <sys/socket.h>`, `#include <netinet/in.h>`, etc.
- Internal headers second: `#include "syscall.h"`, `#include "lookup.h"`, etc.
- Alphabetical order within groups

### Naming Conventions
- **Functions**: POSIX names (e.g., `socket`, `connect`, `getaddrinfo`)
- **Internal functions**: `__res_*` for resolver internals
- **Variables**: `snake_case` for local variables
- **Constants**: `AF_*`, `SOCK_*`, `SOL_*`, `IPPROTO_*`

### Type Usage
- Use `int` for socket descriptors
- Use `socklen_t` for socket address lengths
- Use `struct sockaddr *` for generic addresses
- Use `struct sockaddr_in` for IPv4
- Use `struct sockaddr_in6` for IPv6

### Error Handling
- Return -1 on failure, appropriate value on success
- Set `errno` to indicate specific error
- For getaddrinfo, return error code (not errno)
- Check return values of system calls

### Thread Safety
- Use `socketcall_cp()` for cancellation-point calls
- DNS resolver uses internal locking
- Legacy functions are not thread-safe (use _r versions)

### Memory Management
- Use `malloc()`/`free()` for dynamic allocations
- Use `freeaddrinfo()` to free getaddrinfo results
- Use `freeifaddrs()` to free getifaddrs results

### DNS Resolution
- Parse `/etc/resolv.conf` for nameserver configuration
- Support multiple nameservers
- Implement DNS caching for performance
- Handle both IPv4 (A) and IPv6 (AAAA) queries

---

## 4. Key Implementation Details

### 4.1 Socket Creation
- Uses `socket()` system call
- FD tracking with `OHOS_FDTRACK_HOOK_ENABLE`
- Supports `SOCK_CLOEXEC` and `SOCK_NONBLOCK` flags

### 4.2 DNS Resolution
- UDP queries to nameservers on port 53
- Retry logic for failed queries
- Timeout handling for slow responses
- OpenHarmony: netsys cache integration

### 4.3 Address Translation
- `getaddrinfo()` is the modern, thread-safe API
- Supports IPv4, IPv6, and dual-stack configurations
- Handles service names via `/etc/services`
- Supports AI_NUMERICHOST, AI_CANONNAME, AI_PASSIVE flags

### 4.4 Legacy APIs
- `gethostbyname()` uses static storage (not thread-safe)
- `_r` versions provide reentrant alternatives
- Deprecated in favor of getaddrinfo()
- Still maintained for compatibility

---

## 5. Related Resources

- [POSIX Sockets Specification](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html)
- [RFC 3493 - getaddrinfo](https://datatracker.ietf.org/doc/html/rfc3493)
- [RFC 1035 - DNS](https://datatracker.ietf.org/doc/html/rfc1035)
