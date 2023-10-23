#ifndef __CMD_PARSER_H
#define __CMD_PARSER_H

void cmd_parser(int maxCommandLength, int maxCommands, char *input, char **commandsList, int *numCommands, bool *bgArray);

#endif