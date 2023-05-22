#define NETTHREADS 1
#define GRAPHTTREADS 1

#define RTREENODES 16
#define DEBUG true
#define DATADEBUG false			//Debug connection data flow
#define DISTDEBUG false
#define GRAPHDEBUG false		//standalone graph test
#define TOONEDEBUG false		//ONETOONE path
#define TOALLDEBUG false			//ONETOALL path
#define USERCU false
#define STUPIDSHORTEST false

#define QUADTREELIM 50000000


#define GRIDWIDTH 5000 //141//.55339059 //707.1067812	//1m diagonal
#define MAXDIST 500

#if DEBUG == true || TOONEDEBUG == true || TOALLDEBUG == true
#include <iostream>
#endif