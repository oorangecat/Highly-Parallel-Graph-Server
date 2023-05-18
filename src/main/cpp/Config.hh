#define NETTHREADS 4
#define GRAPHTTREADS 4

#define RTREENODES 16
#define DEBUG false
#define GRAPHDEBUG false
#define TOONEDEBUG false
#define TOALLDEBUG true
#define USERCU false

#define GRIDWIDTH 869		//500mm radius

#if DEBUG == true || TOONEDEBUG == true || TOALLDEBUG == true
#include <iostream>
#endif