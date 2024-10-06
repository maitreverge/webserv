#pragma once

// -------------------    Libraries   -----------------------

// CPP Libraries
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <ostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

// C Libraries
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>

// Arpa
#include <arpa/inet.h>

// Netinet
#include <netinet/in.h>

// Sys Libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
// #include <sys/event.h> // ! Intellisense tells me to fuck off on this one
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>

// ------------------- Using keywords -----------------------

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::stringstream;
using std::exception;
using std::getline;
using std::map;
using std::vector;