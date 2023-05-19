#define NETTHREADS 2
#define GRAPHTTREADS 2

#define RTREENODES 16
#define DEBUG true
#define DATADEBUG false			//Debug connection data flow
#define GRAPHDEBUG false		//standalone graph test
#define TOONEDEBUG false		//ONETOONE path
#define TOALLDEBUG false			//ONETOALL path
#define USERCU false


#define GRIDWIDTH 500 //141//.55339059 //707.1067812	//1m diagonal
#define MAXDIST 500

#if DEBUG == true || TOONEDEBUG == true || TOALLDEBUG == true
#include <iostream>
#endif