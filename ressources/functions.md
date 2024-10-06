## EXECVE

The `execve` function is a standard C function that replaces the current process image with a new process image.

- ***Function Description***: `execve` replaces the current process image with a new process image specified by filename. The `argv` is an array of argument strings passed to the new program. The `envp` is an array of strings, conventionally of the form `key=value`, which are passed as environment to the new program. It does not return on success, and -1 on error.

- ***Prototype***: `int execve(const char *filename, char *const argv[], char *const envp[]);`

- ***Library***:
```c
#include <unistd.h>
```

Example:

```c
#include <stdio.h>
#include <unistd.h>

int main() {
	char *argv[] = {"/bin/ls", "-l", NULL};
	char *envp[] = {NULL};
	if (execve("/bin/ls", argv, envp) == -1) {
		printf("Failed to execute /bin/ls.\n");
	}
	return 0;  // This line will not be executed if execve succeeds
}
```

*In this example, the program replaces its own process image with the `/bin/ls` program using `execve`. If `execve` fails, it prints "Failed to execute /bin/ls.".*
*If `execve` succeeds, the rest of the program will not be executed because the process image has been replaced.*


[Back to Summary](#summary)

---------------------------------------------------------------------------------

---------------------------------------------------------------------------------
## DUP

The `dup` function is a standard C function that creates a copy of the file descriptor.

- ***Function Description***: `dup` creates a new file descriptor that is a copy of the old file descriptor. The new file descriptor is the lowest-numbered unused descriptor. It returns the new file descriptor on success, and -1 on error.

- ***Prototype***: `int dup(int oldfd);`

- ***Library***:
```c
#include <unistd.h>
```

Example:

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd = open("/tmp/file", O_RDONLY);
	if (fd != -1) {
		int newfd = dup(fd);
		if (newfd != -1) {
			printf("Duplicated file descriptor: %d\n", newfd);
		} else {
			printf("Failed to duplicate file descriptor.\n");
		}
		close(fd);
	} else {
		printf("Failed to open /tmp/file.\n");
	}
	return 0;
}
```

*In this example, the program opens the file `/tmp/file` and duplicates its file descriptor using `dup`, storing the new file descriptor in `newfd`. If `dup` succeeds, it prints "Duplicated file descriptor: " followed by the new file descriptor.*
*If `dup` fails, it prints "Failed to duplicate file descriptor.". If the file fails to open, it prints "Failed to open /tmp/file.".*


[Back to Summary](#summary)

---------------------------------------------------------------------------------
## DUP2

The `dup2` function is a standard C function that duplicates a file descriptor to a specified file descriptor.

- ***Function Description***: `dup2` duplicates the file descriptor oldfd to newfd. If `newfd` was previously open, it is silently closed before being reused. If `oldfd` is not a valid file descriptor, then the call fails, and `newfd` is not closed. It returns the new file descriptor on success, and -1 on error.

- ***Prototype***: `int dup2(int oldfd, int newfd);`

- ***Library***:
```c
#include <unistd.h>
```

Example:

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd = open("/tmp/file", O_RDONLY);
	if (fd != -1) {
		int newfd = dup2(fd, 10);
		if (newfd != -1) {
			printf("Duplicated file descriptor to: %d\n", newfd);
		} else {
			printf("Failed to duplicate file descriptor.\n");
		}
		close(fd);
	} else {
		printf("Failed to open /tmp/file.\n");
	}
	return 0;
}
```

*In this example, the program opens the file `/tmp/file` and duplicates its file descriptor to descriptor 10 using `dup2`, storing the new file descriptor in `newfd`. If `dup2` succeeds, it prints "Duplicated file descriptor to: " followed by the new file descriptor. If `dup2` fails, it prints "Failed to duplicate file descriptor.".*
*If the file fails to open, it prints "Failed to open /tmp/file.".*


[Back to Summary](#summary)

---------------------------------------------------------------------------------
## PIPE

The `pipe` function is a standard C function that creates a pipe, which is a unidirectional data channel that can be used for interprocess communication.

- ***Function Description***: `pipe` creates a pipe and places two file descriptors, one for reading and one for writing, into the array pointed to by `pipefd`. `pipefd[0]` is for reading, `pipefd[1]` is for writing. It returns 0 on success, and -1 on error.

- ***Prototype***: `int pipe(int pipefd[2]);`

- ***Library***:
```c
#include <unistd.h>
```

Example:

```c
#include <stdio.h>
#include <unistd.h>

int main() {
	int pipefd[2];
	if (pipe(pipefd) == 0) {
		printf("Created a pipe. Read fd: %d, Write fd: %d\n", pipefd[0], pipefd[1]);
	} else {
		printf("Failed to create a pipe.\n");
	}
	return 0;
}
```

*In this example, the program creates a pipe using `pipe` and stores the file descriptors in `pipefd`. If `pipe` succeeds, it prints "Created a pipe. Read fd: ", followed by the read file descriptor, ", Write fd: ", followed by the write file descriptor.*
*If `pipe` fails, it prints "Failed to create a pipe.".*


[Back to Summary](#summary)

---------------------------------------------------------------------------------

## STRERROR

The `strerror` function is a standard C function that returns a string describing the error code passed in the argument errno.

- ***Function Description***: `strerror` returns a pointer to a string that describes the error code passed in the argument errnum. The string is actually a static buffer that is overwritten by each call to `strerror`.

- ***Prototype***: `char *strerror(int errnum);`

- ***Library***:
```c
#include <string.h>
```

Example:

```c
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
	FILE *file = fopen("/nonexistent", "r");
	if (!file) {
		printf("Error: %s\n", strerror(errno));
	}
	return 0;
}
```

*In this example, the program tries to open a non-existent file. When the `fopen` call fails, it prints the error message associated with the current `errno` value using `strerror`.*


[Back to Summary](#summary)

---------------------------------------------------------------------------------


## GAI_STRERROR

The `gai_strerror` function is used to translate an error code returned by `getaddrinfo` into a human-readable string describing the error.

- ***Function Description***: `gai_strerror` returns a string describing the error code returned by `getaddrinfo`, which is useful for debugging network address translation issues.

- ***Prototype***: `const char *gai_strerror(int errcode);`

- ***Library***:

```c
#include <netdb.h>
```

Example:

```c
#include <stdio.h>
#include <netdb.h>

int main() {
    int errcode = EAI_NONAME;  // Example error code
    const char *errstr = gai_strerror(errcode);
    
    printf("Error message: %s\n", errstr);
    return 0;
}
```

*In this example, the program uses an error code (EAI_NONAME) returned by getaddrinfo and passes it to gai_strerror to get a human-readable error message, which is then printed to the console.*

---------------------------------------------------------------------------------

## FORK

The `fork` function is a standard C function that creates a new process.

- ***Function Description***: `fork` creates a new process by duplicating the existing process. The new process, called the child, is an exact copy of the calling process, called the parent, except for a few values changed, including the process ID and parent process ID. It returns the process ID of the child process to the parent, and 0 to the child. If it fails, it returns -1.

- ***Prototype***: `pid_t fork(void);`

- ***Library***:
```c
#include <unistd.h>
```

Example:

```c

#include <stdio.h>
#include <unistd.h>

int main() {
	pid_t pid = fork();

	if (pid < 0) {
		printf("Failed to fork.\n");
	} else if (pid == 0) {
		printf("This is the child process.\n");
	} else {
		printf("This is the parent process, child PID is %d.\n", pid);
	}
	return 0;
}
```

*In this example, the program creates a new process using `fork`. If `fork` fails, it prints "Failed to fork.". If `fork` succeeds, the parent process prints its own process ID and the child's process ID, and the child process prints "This is the child process.".*


[Back to Summary](#summary)

---------------------------------------------------------------------------------

## SOCKETPAIR

The `socketpair` function is used to create a pair of connected sockets, which can be used for inter-process communication (IPC).

- ***Function Description***: `socketpair` creates two connected sockets, usually in the UNIX domain, which can be used to send and receive data between two processes or threads.

- ***Prototype***: `int socketpair(int domain, int type, int protocol, int sv[2]);`

- ***Library***:

```cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
```

``` cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

int main() {
    int sv[2];  // Array to hold the pair of sockets

    // Create the socket pair
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        return 1;
    }

    // Write a message to the first socket
    const char* message = "Hello, world!";
    write(sv[0], message, 13);

    // Buffer to hold the message from the second socket
    char buf[100] = {0};
    read(sv[1], buf, sizeof(buf));

    // Output the message received from the second socket
    std::cout << "Received message: " << buf << std::endl;

    return 0;
}

```

*In this C++ example, `socketpair` creates two connected sockets (sv[0] and sv[1]). A message is written to the first socket (sv[0]), and the message is read from the second socket (sv[1]). The program then prints the received message using `std::cout` to demonstrate IPC between the two sockets.*


---------------------------------------------------------------------------------

## HTONS

The `htons` function is used to convert a 16-bit number from host byte order to network byte order.

- ***Function Description***: `htons` (host to network short) converts a 16-bit integer (short) from the host machine's byte order (which may be little-endian or big-endian) to network byte order (big-endian), which is required for network communication.

- ***Prototype***: `uint16_t htons(uint16_t hostshort);`

- ***Library***:

```cpp
#include <arpa/inet.h>
#include <iostream>
```

```cpp
#include <arpa/inet.h>
#include <iostream>

int main() {
    uint16_t host_port = 8080;  // Example port number in host byte order
    uint16_t network_port = htons(host_port);  // Convert to network byte order

    std::cout << "Host order: " << host_port << std::endl;
    std::cout << "Network order: " << network_port << std::endl;

    return 0;
}

```

*In this C++ example, `htons` is used to convert the port number 8080 from host byte order to network byte order. The original host order and the converted network order are then printed using `std::cout` to illustrate the byte order conversion.*

---------------------------------------------------------------------------------

## HTONL

The `htonl` function is used to convert a 32-bit number from host byte order to network byte order.

- ***Function Description***: `htonl` (host to network long) converts a 32-bit integer from the host machine's byte order (which may be little-endian or big-endian) to network byte order (big-endian), required for sending data over the network.

- ***Prototype***: `uint32_t htonl(uint32_t hostlong);`

- ***Library***:

```cpp
#include <arpa/inet.h>
#include <iostream>
```

```cpp
#include <arpa/inet.h>
#include <iostream>

int main() {
    uint32_t host_address = 0xC0A80001;  // Example IP address in host byte order (192.168.0.1)
    uint32_t network_address = htonl(host_address);  // Convert to network byte order

    std::cout << "Host order: " << std::hex << host_address << std::endl;
    std::cout << "Network order: " << std::hex << network_address << std::endl;

    return 0;
}

```

*In this C++ example, `htonl` converts the IP address 192.168.0.1 (represented as 0xC0A80001 in hex) from host byte order to network byte order. The original host and network order values are printed using `std::cout` in hexadecimal format to illustrate the conversion.*

---------------------------------------------------------------------------------

## NTOHS

The `ntohs` function is used to convert a 16-bit number from network byte order to host byte order.

- ***Function Description***: `ntohs` (network to host short) converts a 16-bit integer from network byte order (big-endian) to the host machine's byte order (which may be little-endian or big-endian), allowing for proper interpretation of data received over the network.

- ***Prototype***: `uint16_t ntohs(uint16_t netshort);`

- ***Library***:

```cpp
#include <arpa/inet.h>
#include <iostream>
```

```cpp
#include <arpa/inet.h>
#include <iostream>

int main() {
    uint16_t network_port = 0x1F90;  // Example port number in network byte order (8080 in decimal)
    uint16_t host_port = ntohs(network_port);  // Convert to host byte order

    std::cout << "Network order: " << network_port << std::endl;
    std::cout << "Host order: " << host_port << std::endl;

    return 0;
}

```

*In this C++ example, `ntohs` is used to convert the network byte order port number 0x1F90 (which is 8080 in decimal) to host byte order. The original network order and the converted host order values are printed using `std::cout` to demonstrate the byte order conversion.*

---------------------------------------------------------------------------------

## NTOHL

The `ntohl` function is used to convert a 32-bit number from network byte order to host byte order.

- ***Function Description***: `ntohl` (network to host long) converts a 32-bit integer from network byte order (big-endian) to the host machine's byte order (which may be little-endian or big-endian), allowing for proper interpretation of data received over the network.

- ***Prototype***: `uint32_t ntohl(uint32_t netlong);`

- ***Library***:

```cpp
#include <arpa/inet.h>
#include <iostream>
```

```cpp
#include <arpa/inet.h>
#include <iostream>

int main() {
    uint32_t network_address = 0xC0A80001;  // Example IP address in network byte order (192.168.0.1)
    uint32_t host_address = ntohl(network_address);  // Convert to host byte order

    std::cout << "Network order: " << std::hex << network_address << std::endl;
    std::cout << "Host order: " << std::hex << host_address << std::endl;

    return 0;
}

```

*In this C++ example, `ntohl` is used to convert the IP address `0xC0A80001` (which corresponds to 192.168.0.1 in decimal) from network byte order to host byte order. The original network and converted host order values are printed using `std::cout` in hexadecimal format to illustrate the conversion.*

---------------------------------------------------------------------------------

## SELECT

The `select` function is used to monitor multiple file descriptors to see if they are ready for reading, writing, or if an error has occurred.

- ***Function Description***: `select` allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation (e.g., reading or writing). This is useful for implementing multiplexing I/O.

- ***Prototype***: `int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);`

- ***Library***:

```cpp
#include <sys/select.h>
#include <unistd.h>
#include <iostream>
```


```cpp

```

**


---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

---------------------------------------------------------------------------------


```


```cpp

```

**

