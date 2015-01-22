/*
 *  Main File for froute daemon
 *
 *  (c) 2015
 *
 */

#include<iostream>
#include<string>

#include "../../lib/constants.hh"

enum cmd_params {
	CMD_ERROR, CMD_DEBUG, CMD_INFO, CMD_OK
};
/* return values for command types */

cmd_params parse_cmd_options(int argc, char **argv) {
	std::string usage_msg = "\n**** froute IP Router ****\n(c) 2015\n"
			"Usage:\nfroute [OPTIONS]\n\n"
			"OPTIONS:\n"
			"\t--bgp\n"
			"\t--static\n"
			"\t--debug\n" "\t-h  Displays this text\n"
			"\t-v Displays installed version\n";

	if (argc < 2) {
		std::cout << "Bad Parameters...!\n";
		std::cout << usage_msg;
		return (CMD_ERROR);
	}

	for (int i = 1; i < argc; ++i) {
		char c1 = argv[i][0];
		if (c1 == '-') {
			char c2 = argv[i][1];
			switch (c2) {
			case 'v':
				std::cout << VERSION << std::endl;
				return (CMD_INFO);
				break;
			case 'h':
				std::cout << usage_msg;
				return (CMD_INFO);
				break;
				//check options
			case '-':
				std::string option = argv[i];
				if (!option.compare("--debug")) {
					return (CMD_DEBUG);
				}
			}
		}
	}

	return (CMD_OK);
}

// main
int main(int argc, char **argv) {

//start by parsing the cmd params
	cmd_params cmd_result = parse_cmd_options(argc, argv);

	int isDebugging;
	if (cmd_result == CMD_ERROR) {
		return (-1);
	} else if (cmd_result == CMD_INFO) {
		return (0);
	} else if (cmd_result == CMD_DEBUG) {
		isDebugging = 1;
	}

//parse configuration file
	return (0);
}
