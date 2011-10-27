#ifndef SCART_H
#define SCART_H

void screenscart()
{
	debug(1000, "in");
	int rcret = 0;
	char* tmpstr = NULL;

	tmpstr = getvideomode();
	if(ostrncmp("576i", tmpstr, 4) == 0 || ostrcmp("pal", tmpstr) == 0)
	{
		textbox(_("Message"), _("Change to scart"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
		setinput("scart");
		while(1)
		{
			rcret = waitrc(NULL, 0, 0);
			if(rcret == getrcconfigint("rcback", NULL)) break;
		}
	}
	else
		textbox(_("Message"), _("Videomode must be 576i for Scart"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
	free(tmpstr); tmpstr = NULL;
	debug(1000, "out");
	return;
}

#endif
