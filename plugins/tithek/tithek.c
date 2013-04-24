#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "tithek_header.h"
#include "tithek.h"
#include "tithek_settings.h"
#include "putlocker.h"
#include "filenuke.h"
#include "youtube.h"
#include "kinox.h"
#include "myvideo.h"
#include "rtl2now.h"
#include "streamcloud.h"
#include "flashx.h"
#include "vidstream.h"
#include "xvidstage.h"
#include "nowvideo.h"
#include "beeg.h"
#include "solarmovie.h"

char pluginname[] = "TiTan Mediathek";
char plugindesc[] = "TiTan Mediathek";
char pluginpic[] = "%pluginpath%/tithek/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/tithek/skin.xml", 0, 0);
	readscreen(tmpstr, 195, 1);
	free(tmpstr); tmpstr = NULL;

	mkdir("/tmp/tithek", 777);
	pluginaktiv = 1;
	debug(10, "TiThek Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(195);
	delallfiles("/tmp/tithek", NULL);
	pluginaktiv = 0;
	debug(10, "TiThek Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	if(checkbox("ATEMIO510") == 0 && checkbox("ATEMIO7600") == 0 && checkbox("UFS912") == 0 && checkbox("ATEVIO700") == 0 && checkbox("ATEVIO7000") == 0 && checkbox("WHITEBOX") == 0 && checkbox("ATEMIO520") == 0)
	{
		if(file_exist("/var/bin/audio.elf") || file_exist("/var/swap/bin/audio.elf"))
			textbox(_("Message"), _("Alternativ Audio Firmware not working korrekt with all videos (DTSDOWNMIX)!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
	}

	int aktplayerbuffersize = getconfigint("playerbuffersize", NULL);
	status.hangtime = 99999;
	tithekdownloadrun = 0;
	tithekdownloadcount = 0;
	tithekrun = 1;
	tithekexit = 0;
	if(checkbox("WHITEBOX") == 1)
		screentithekplay("http://atemio.dyndns.tv/mediathek/mainmenu.crenova.list", "Operator - Mainmenu", 1);
	else
		screentithekplay("http://atemio.dyndns.tv/mediathek/mainmenu.list", "Tithek - Mainmenu", 1);
	
	tithekrun = 0;
	addconfigint("playerbuffersize", aktplayerbuffersize);
	status.hangtime = getconfigint("hangtime", NULL);
}
