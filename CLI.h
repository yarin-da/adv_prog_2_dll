/* 
 * CLI.h
 *
 * Author: 316383298 Yarin Dado
 */


#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
public:
	CLI(DefaultIO* dio);
	void start();
    void stop();
	virtual ~CLI();
};

#endif /* CLI_H_ */
