#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "wins3.h"

char pluginname[] = "3 Wins";
char plugindesc[] = "3 Wins Game";
char pluginpic[] = "%pluginpath%/wins3/plugin.png";

int pluginaktiv = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/wins3/skin.xml", 0, 0);
	readscreen(tmpstr, 196, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "3 Wins Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(196);
	pluginaktiv = 0;
	debug(10, "3 Wins Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screewins3();
}
