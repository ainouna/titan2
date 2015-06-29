#ifndef PLAY_H
#define PLAY_H

extern struct screensaver* screensaver;

void screenplaypolicy()
{
	if(checkbit(status.playercan, 0) == 0) return;

	int rcret = 0;
	struct skin* playpolicy = getscreen("playpolicy");
	char* tmpstr = NULL;

start:
	changepolicy();
	tmpstr = getpolicy();
	changetext(playpolicy, _(tmpstr));
	free(tmpstr); tmpstr = NULL;
	drawscreen(playpolicy, 0, 0);

	while(1)
	{
		rcret = waitrc(playpolicy, 1000, 0);
		if(rcret == getrcconfigint("rcok", NULL))
			goto start;
		break;
	}

	clearscreen(playpolicy);
}

void screenplaybufferstatus(struct stimerthread* self)
{
	int rcret = -1, draw = 50;
	if(self == NULL) return;

	struct skin* playbufferstatus = getscreen("playbufferstatus");

	playbufferstatus->progresssize = 0;
	
	while(self->aktion != STOP)
	{
		rcret = waitrc(0, 1, 0);
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			playerstopbuffer();
			break;
		}

		if(draw == 50)
		{
			draw = 0;
			if(playergetbuffersize() > 0)
				drawscreen(playbufferstatus, 0, 0);
		}
		//usleep(10000);
		draw++;
	}
	
	if(playergetbuffersize() > 0)
	{
		clearscreen(playbufferstatus);
		drawscreen(skin, 0, 0);
	}
}

//flag = 4 ---> timeshift
//flag = 5 --> timeshift, not in play mode (only recording)
void screenplayinfobar(char* file, char* showname, int mode, int playertype, int flag)
{
	int change = 1;
	int videodevpts = 1;

	if(checkbit(status.playercan, 14) == 0) return;

	if((flag == 2) || (flag == 3))
	{
		return;
	}
	struct skin* playinfobar = getscreen("playinfobar");
	struct skin* playinfobarpic = getscreen("playinfobarpic");
	if(mode == 1)
	{
		clearscreen(playinfobar);
		clearscreen(playinfobarpic);
		drawscreen(skin, 0, 0);
		return;
	}

	struct skin* title = getscreennode(playinfobar, "title");
	struct skin* spos = getscreennode(playinfobar, "pos");
	struct skin* slen = getscreennode(playinfobar, "len");
	struct skin* sreverse = getscreennode(playinfobar, "reverse");
	struct skin* sprogress = getscreennode(playinfobar, "progress");
	char* tmpstr = NULL;
	unsigned long long pos = 0, dpos = 0, len = 0, reverse = 0;

	// show thumb cover start
	struct skin* playinfobarcover = getscreen("playinfobarcover");
	struct skin* cover = getscreennode(playinfobarcover, "cover");
	cover->hidden = YES;
	if(file != NULL)
	{
		struct mediadb* node = NULL;
		char* dname = ostrcat(file, NULL, 0, 0);
		dname = dirname(dname);
		node = getmediadb(dname, basename(file), 0);
		free(dname); dname = NULL;
		
		if(node != NULL)
		{
			if(ostrncmp("tt", node->id, 2) == 0)
			{
				tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, node->id, 1, 0);
				tmpstr = ostrcat(tmpstr, "_poster.jpg", 0, 0);
				if(file_exist(tmpstr))
				{
					changepic(cover, tmpstr);
					cover->hidden = NO;
				}
				free(tmpstr), tmpstr = NULL;
			}
		}
	}
	// show thumb cover end

	if(showname == NULL)
	{
		tmpstr = ostrcat(file, NULL, 0, 0);
		if(tmpstr != NULL) changetext(title, basename(tmpstr));
		free(tmpstr); tmpstr = NULL;
	}
	else
		changetext(title, showname);

	videodevpts = 1;
	if(playertype == 1)
	{
		int ret = 0;
		unsigned long long startpos = 0;
		if(flag == 4)
			ret = playergetinfots(&len, &startpos, NULL, &pos, NULL, 1);
		else if(flag == 5)
			ret = playergetinfots(&len, &startpos, NULL, &pos, NULL, 2);
		else
		{
			ret = playergetinfots(&len, &startpos, NULL, &pos, NULL, 0);
			if(ret == 0)
			{
				ret = videogetpts(status.aktservice->videodev, &dpos);
				if(ret == 0)
				{
					pos = dpos;
					videodevpts = 1;
				}
				else
				{
					ret = 0; 
					videodevpts = 0;
				}
			}
		}
		len = len / 90000;
		pos = (pos - startpos) / 90000;
		if(ret != 0) change = 0;
	}
	else if(playertype == 2)
	{
		pos = dvdgetpts() / 90000;
		len = dvdgetlength();
	}
	else
	{
		pos = playergetpts() / 90000;
		len = playergetlength();
	}
	if(pos < 0) pos = 0;
	reverse = len - pos;

	if(change == 1)
	{
		if(len == 0)
			sprogress->progresssize = 0;
		else
			sprogress->progresssize = pos * 100 / len;
	}

	tmpstr = convert_timesec(pos);
	if(videodevpts == 1)
		changetext(spos, tmpstr);
	else
		changetext(spos, " ");
	free(tmpstr); tmpstr = NULL;

	tmpstr = convert_timesec(len);
	changetext(slen, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = convert_timesec(reverse);
	changetext(sreverse, tmpstr);
	free(tmpstr); tmpstr = NULL;

	drawscreen(playinfobar, 0, 0);
	drawscreen(playinfobarpic, 0, 0);
	drawscreen(playinfobarcover, 0, 0);
}

void screenplaytracklist(int mode, int playertype, int flag)
{
	//mode 1 = audiotracks
	//mode 2 = subtitle tracks

	if(mode == 1 && checkbit(status.playercan, 1) == 0) return;
	if(mode == 2 && checkbit(status.playercan, 2) == 0) return;

	if(playertype == 1)
	{
		screenplayinfobar(NULL, NULL, 1, playertype, flag);
		if(mode == 1)
			playerchangeaudiotrackts();
		else if(mode == 2)
			playerchangesubtitletrackts();
		blitfb(0);
		return;
	}

	int i = 0;
	int rcret = 0, curtrackid = 0;
	struct skin* track = NULL;
	if(mode == 1)
		track = getscreen("audiotrack");
	else if(mode == 2)
		track = getscreen("subtitle");
	struct skin* listbox = getscreennode(track, "listbox");
	struct skin* tmp = NULL;
	char** tracklist = NULL;
	char* curtrackencoding = NULL, *curtrackname = NULL;
	char* tmpstr = NULL;

	playergetcurtrac(mode, &curtrackid, &curtrackencoding, &curtrackname);
	tracklist = playergettracklist(mode);

	if(tracklist != NULL)
	{
		while(tracklist[i] != NULL)
		{
			tmp = addlistbox(track, listbox, tmp, 1);
			if(tmp != NULL)
			{
				if(ostrcmp(tracklist[i], "und") == 0)
					tmpstr = ostrcat(tmpstr, _("undefined"), 1, 0);
				else
					tmpstr = ostrcat(tmpstr, _(tracklist[i]), 1, 0);
					
				if(tracklist[i + 1] != NULL && strlen(tracklist[i + 1]) > 0)
				{	
					tmpstr = ostrcat(tmpstr, " (", 1, 0);
					tmpstr = ostrcat(tmpstr, tracklist[i + 1], 1, 0);
					tmpstr = ostrcat(tmpstr, ")", 1, 0);
				}
				
				
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
				tmp->type = CHOICEBOX;
				tmp->del = 1;
				if(tracklist[i] != NULL && atoi(tracklist[i]) != 0)
					tmp->handle = (char*)atoi(tracklist[i]);
				else
					tmp->handle = (char*)(i / 2);
				
#ifdef MIPSEL	
				if(curtrackid == (i / 2))
				{
					tmp->handle1 = "running";
					changeinput(tmp, _("running"));
				}
				else
					changeinput(tmp, "");
#else
				if((ostrcmp(curtrackname, tracklist[i]) == 0 && ostrcmp(curtrackencoding, tracklist[i + 1]) == 0) || (tracklist[i] != NULL && curtrackid == atoi(tracklist[i])))
				{
					tmp->handle1 = "running";
					changeinput(tmp, _("running"));
				}
				else
					changeinput(tmp, "");
#endif
			}
			i += 2;
		}
	}

	free(curtrackencoding); curtrackencoding = NULL;
	free(curtrackname); curtrackname = NULL;
	playerfreetracklist(tracklist);
	tracklist = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	screenplayinfobar(NULL, NULL, 1, playertype, flag);
	drawscreen(track, 0, 0);
	addscreenrc(track, listbox);

	while(1)
	{
		rcret = waitrc(track, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcyellow", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				if(mode == 1) {
					playerchangeaudiotrack((int)listbox->select->handle);
#ifdef MIPSEL
					if(playertype == 0)
						playerseek(-1);
#endif
				}
				else if(mode == 2)
				{
					if(listbox->select->handle1 != NULL)
						playerstopsubtitletrack();
					else	
						playerchangesubtitletrack((int)listbox->select->handle);
				}
			}
			break;
		}
	}

	delmarkedscreennodes(track, 1);
	delownerrc(track);
	clearscreen(track);
	blitfb(0);
	
	if(rcret == getrcconfigint("rcyellow", NULL))
		screenplaytracklist(2, playertype, flag);
}

void playrcyellow(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 1) == 0) return;

	screenplaytracklist(1, playertype, flag);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);
}

void playrctext(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 2) == 0) return;

	screenplaytracklist(2, playertype, flag);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);
}

void playrcgreen(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 3) == 0) return;

	screenplayinfobar(file, showname, 1, playertype, flag);
	if(playertype == 2)
		screenvideomode(2);
	else
		screenvideomode(1);
	drawscreen(skin, 0, 0);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);
}

void playrcblue(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
	if(checkbit(status.playercan, 4) == 0) return;

	screenplayinfobar(file, showname, 1, playertype, flag);
	screenpowerofftimer();
	drawscreen(skin, 0, 0);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);
}

void playrcok(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
//	if(checkbit(status.playercan, 0) == 0) return;

	free(status.playfile); status.playfile = NULL;
	status.playfile = ostrcat(file, NULL, 0, 0);
	screenplaypolicy(file, 1);
	drawscreen(skin, 0, 0);
	if(playinfobarstatus > 0)
		screenplayinfobar(file, showname, 0, playertype, flag);
}

void id3tag_info(char* file)
{
	char* tmpstr = NULL;

	if(file == NULL) return;
	if(!filelistflt(".mp3 .flac .ogg .wma .ra .wav", file))
	{
		struct id3tag* id3tag = NULL;
		int hash = gethash(file);
		char* tmphash = olutoa(hash);
				
		id3tag = getid3(file, tmphash, 1);
		free(tmphash); tmphash = NULL;
	
		if(id3tag != NULL)
		{
			tmpstr = ostrcat(tmpstr, _("Title:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->title, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			
			tmpstr = ostrcat(tmpstr, _("Artist:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->artist, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			
			tmpstr = ostrcat(tmpstr, _("Album:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->album, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			
			tmpstr = ostrcat(tmpstr, _("Year:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->year, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
			tmpstr = ostrcat(tmpstr, _("Genre:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->genretext, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);

			tmpstr = ostrcat(tmpstr, _("Comment:"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, id3tag->comment, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		}
		freeid3(id3tag); id3tag = NULL;		
	}
	else
	{
		tmpstr = ostrcat(tmpstr, _("Title:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Title"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Artist:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Artist"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Album:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Album"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Year:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Year"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	
		tmpstr = ostrcat(tmpstr, _("Genre:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Genre"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Comment:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Comment"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Track:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Track"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
		tmpstr = ostrcat(tmpstr, _("Copyright:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, playergetinfo("Copyright"), 1, 1);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		
	//	tmpstr = ostrcat(tmpstr, _("TestLibEplayer:"), 1, 0);
	//	tmpstr = ostrcat(tmpstr, " ", 1, 0);
	//	tmpstr = ostrcat(tmpstr, playergetinfo("TestLibEplayer"), 1, 1);
	//	tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	}
			
	textbox(_("iD3Tag"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 500, 100, 0);
	free(tmpstr), tmpstr = NULL;
}

void imdb_submenu(char* file, int mode)
{
	struct skin* pluginnode = NULL;
	void (*startplugin)(char*, char*, int, char*, char*);

	startplugin = NULL;
	
	if(mode == 0)
		pluginnode = getplugin("IMDb");
	else if(mode == 1)
		pluginnode = getplugin("IMDb-API");
	else if(mode == 2)
		pluginnode = getplugin("TMDb");
			
	if(pluginnode != NULL)
	{			
		if(mode == 0)
			startplugin = dlsym(pluginnode->pluginhandle, "screenimdb");
		else if(mode == 1)
			startplugin = dlsym(pluginnode->pluginhandle, "screenimdbapi");
		else if(mode == 2)
			startplugin = dlsym(pluginnode->pluginhandle, "screentmdb");

		if(startplugin != NULL)
		{
				debug(133, "file=%s", basename(file));
			if(file != NULL)
			{
				if(ostrstr(file, "/") != NULL)
				{
					//create imdb search name
	
					char* dname = ostrcat(file, NULL, 0, 0);
					dname = dirname(dname);
				
					char* shortname = ostrcat(basename(file), NULL, 0, 0);
					string_tolower(shortname);
	//				shortname = string_shortname(shortname, 1);
					shortname = string_shortname(shortname, 2);
					string_removechar(shortname);
					strstrip(shortname);
	
					debug(133, "inputfile=%s", file);
					debug(133, "shortname=%s", shortname);
					debug(133, "dname=%s", dname);
					debug(133, "file=%s", basename(file));
	
					startplugin(shortname, NULL, 2, dname, basename(file));
	
					free(shortname), shortname = NULL;
					free(dname), dname = NULL;
				}
				else
				{
					startplugin(file, NULL, 2, NULL, NULL);
				}
				
			}				
		}
	}
}

void get_mediadb_scan_info()
{
	int videocount = 0, audiocount = 0, picturecount = 0;
	getmediadbcounts(&videocount, &audiocount, &picturecount);

	char* tmpstr = NULL;
	tmpstr = ostrcat(tmpstr, "scanning (", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(videocount), 1, 1);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(status.mediadbfiles), 1, 1);
	tmpstr = ostrcat(tmpstr, ")", 1, 0);
	
	tmpstr = ostrcat(tmpstr, "\n\n ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("MediaDB directory scan started in Background !"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n  ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Delete MediaDB before scan"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t", 1, 0);
	if(ostrcmp(getconfig("mediadbscandelall", NULL), "1") == 0)
		tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);			
	tmpstr = ostrcat(tmpstr, _("Delete unused entrys before scan"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t", 1, 0);		
	if(ostrcmp(getconfig("mediadbscandelnotfound", NULL), "1") == 0)
		tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("scan Directory"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("mc_vp_path", NULL), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
	tmpstr = ostrcat(tmpstr, _("MediaDB place"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);				
	tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);
	tmpstr = ostrcat(tmpstr, _("MediaDB Debug"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);
	if(ostrcmp(getconfig("mediadbdebug", NULL), "1") == 0)
		tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
	tmpstr = ostrcat(tmpstr, _("Backdrop Download Count"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": \t\t", 1, 0);		
	if(getconfigint("mediadbbackdrop", NULL) == 0)
		tmpstr = ostrcat(tmpstr, _("all"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, oitoa(getconfigint("mediadbbackdrop", NULL)), 1, 1);

	textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 500, 10, 0);
	free(tmpstr), tmpstr = NULL;
}

// flag 100 = tithek
int playrcred(char* file, char* showname, int playinfobarstatus, int playertype, int flag)
{
	int ret = 1;
//	if(checkbit(status.playercan, 5) == 0) return;
	if(status.play == 1)
		screenplayinfobar(file, showname, 1, playertype, flag);

	int hangtime = status.hangtime;

	struct skin* pluginnode = NULL;
	void (*startplugin)(void);
	struct skin* plugin = getscreen("plugin");
	struct skin* child = plugin->child;
	struct menulist* mlist = NULL, *mbox = NULL;
	char* skintitle = _("Menu");
		
	if(flag < 99)
	{
		if(status.play == 1)
		{
			addmenulist(&mlist, "Video Settings", _("Video Settings"), NULL, 0, 0);
			addmenulist(&mlist, "AV Settings", _("AV Settings"), NULL, 0, 0);
			addmenulist(&mlist, "iD3Tag Info", _("iD3Tag Info"), NULL, 0, 0);
		}

		addmenulist(&mlist, "MediaDB Edit", _("MediaDB Edit"), NULL, 0, 0);
	
		if(status.play == 0)
			addmenulist(&mlist, "Delete File", _("Delete File"), NULL, 0, 0);
			
		if(status.mediadbfiles > 0)
			addmenulist(&mlist, "MediaDB Scan Info", _("MediaDB Scan Info"), NULL, 0, 0);
	}
	else
	{
		addmenulist(&mlist, "Create Favorites sub folder", _("Create Favorites sub folder"), NULL, 0, 0);
		addmenulist(&mlist, "Create Favorites sub folder(PIN secured)", _("Create Favorites sub folder(PIN secured)"), NULL, 0, 0);

		if(flag < 199)
		{
			if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
			{
				addmenulist(&mlist, "Search on KinoX", _("Search on KinoX"), NULL, 0, 0);
				addmenulist(&mlist, "Search on KinoX (local)", _("Search on KinoX (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Movie4k", NULL, _("Search on Movie4k"), 0, 0);
				addmenulist(&mlist, "Search on Movie4k (local)", _("Search on Movie4k (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Solarmovies (movie)", _("Search on Solarmovies (movie)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Solarmovies (serie)", _("Search on Solarmovies (serie)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Solarmovies (local)", _("Search on Solarmovies (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Mle-HD (local)", _("Search on Mle-HD (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Beeg (local)", _("Search on Beeg (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on Xvideos (local)", _("Search on Xvideos (local)"), NULL, 0, 0);
				addmenulist(&mlist, "Search on FilmOn (local)", _("Search on FilmOn (local)"), NULL, 0, 0);
			}
			addmenulist(&mlist, "Search on Netzkino (local)", _("Search on Netzkino (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on Youtube", _("Search on Youtube"), NULL, 0, 0);
			addmenulist(&mlist, "Search on Youtube (local)", _("Search on Youtube (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on MyVideo", _("Search on MyVideo"), NULL, 0, 0);
			addmenulist(&mlist, "Search on MyVideo (local)", _("Search on MyVideo (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on ARD (local)", _("Search on ARD (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on ZDF (local)", _("Search on ZDF (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on TecTime (local)", _("Search on TecTime (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on Giga (local)", _("Search on Giga (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on NowTv (local)", _("Search on NowTv (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on InternetTv (local)", _("Search on InternetTv (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on InternetRadio (local)", _("Search on InternetRadio (local)"), NULL, 0, 0);
			addmenulist(&mlist, "Search on All (local)", _("Search on All (local)"), NULL, 0, 0);
		}
	}

	addmenulist(&mlist, "Downloads", _("Downloads"), NULL, 0, 0);

	if(flag < 199)
	{
		//add plugins
		while(child != NULL)
		{
			if(child->del == PLUGINDELMARK && (status.security >= 1 || (status.security == 0 && checkpluginskip(child->name) == 0)))
			{
				if(ostrcmp(child->name, "Streaminfo") == 0 && status.play == 1)
					addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
				else if(ostrcmp(child->name, "IMDb") == 0)
					addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
				else if(ostrcmp(child->name, "IMDb-API") == 0)
					addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
	//			else if(ostrcmp(child->name, "TMDb") == 0)
	//				addmenulist(&mlist, child->name, _(child->name), child->pic, 0, 0);
			}
			child = child->next;
		}
	}

	mbox = menulistbox(mlist, NULL, skintitle, _("Choose your Red-Menu entry from the following list"), NULL, NULL, 1, 0);
	if(mbox != NULL)
	{
		if(ostrcmp(mbox->name, "Video Settings") == 0)
			screenvideosettings();
		else if(ostrcmp(mbox->name, "AV Settings") == 0)
			screenavsettings(0);
		else if(ostrcmp(mbox->name, "IMDb") == 0)
			imdb_submenu(file, 0);
		else if(ostrcmp(mbox->name, "IMDb-API") == 0)
			imdb_submenu(file, 1);
		else if(ostrcmp(mbox->name, "TMDb") == 0)
			imdb_submenu(file, 2);
		else if(ostrcmp(mbox->name, "iD3Tag Info") == 0)
			id3tag_info(file);
		else if(ostrcmp(mbox->name, "MediaDB Scan Info") == 0)
			get_mediadb_scan_info();
		else if(ostrcmp(mbox->name, "MediaDB Edit") == 0)
			screenmediadbedit(file, 0, 0);
		else if(ostrcmp(mbox->name, "Delete File") == 0)
		{
			playcheckdirrcret(file, 1);
			ret = 9999;
		}
		else if(ostrcmp(mbox->name, "Downloads") == 0)
			screenbgdownload(1);
		else if(ostrcmp(mbox->name, "Create Sub Menu Favorite") == 0)
			ret = 0;
		else if(ostrcmp(mbox->name, "Create Sub Menu Favorite (Password)") == 0)
			ret = 1000;
		else if(ostrcmp(mbox->name, "Search on KinoX") == 0)
			ret = 2;
		else if(ostrcmp(mbox->name, "Search on KinoX (local)") == 0)
			ret = 8;
		else if(ostrcmp(mbox->name, "Search on Solarmovies (movie)") == 0)
			ret = 3;
		else if(ostrcmp(mbox->name, "Search on Solarmovies (serie)") == 0)
			ret = 4;
		else if(ostrcmp(mbox->name, "Search on Solarmovies (local)") == 0)
			ret = 9;
		else if(ostrcmp(mbox->name, "Search on Youtube") == 0)
			ret = 5;
		else if(ostrcmp(mbox->name, "Search on Youtube (local)") == 0)
			ret = 10;
		else if(ostrcmp(mbox->name, "Search on MyVideo") == 0)
			ret = 6;
		else if(ostrcmp(mbox->name, "Search on MyVideo (local)") == 0)
			ret = 11;
		else if(ostrcmp(mbox->name, "Search on Movie4k") == 0)
			ret = 7;
		else if(ostrcmp(mbox->name, "Search on Movie4k (local)") == 0)
			ret = 12;
		else if(ostrcmp(mbox->name, "Search on InternetRadio (local)") == 0)
			ret = 13;
		else if(ostrcmp(mbox->name, "Search on InternetTv (local)") == 0)
			ret = 14;
		else if(ostrcmp(mbox->name, "Search on ARD (local)") == 0)
			ret = 15;
		else if(ostrcmp(mbox->name, "Search on ZDF (local)") == 0)
			ret = 16;
		else if(ostrcmp(mbox->name, "Search on TecTime (local)") == 0)
			ret = 17;
		else if(ostrcmp(mbox->name, "Search on Giga (local)") == 0)
			ret = 18;
		else if(ostrcmp(mbox->name, "Search on Beeg (local)") == 0)
			ret = 19;
		else if(ostrcmp(mbox->name, "Search on NowTv (local)") == 0)
			ret = 20;
		else if(ostrcmp(mbox->name, "Search on All (local)") == 0)
			ret = 21;
		else if(ostrcmp(mbox->name, "Search on Xvideos (local)") == 0)
			ret = 24;
		else if(ostrcmp(mbox->name, "Search on Mle-HD (local)") == 0)
			ret = 25;
		else if(ostrcmp(mbox->name, "Search on Netzkino (local)") == 0)
			ret = 26;
		else if(ostrcmp(mbox->name, "Search on FilmOn (local)") == 0)
			ret = 27;
		else if(ostrcmp(mbox->name, "Downloads") == 0)
		{
			if(status.play == 1)
				screenbgdownload(1);
			else
				screenbgdownload(0);
		}			
		else
		{
			pluginnode = getplugin(mbox->name);

			if(pluginnode != NULL)
			{
				startplugin = dlsym(pluginnode->pluginhandle, "start");
				if(startplugin != NULL)
					startplugin();
			}
		}
	}

	freemenulist(mlist, 1); mlist = NULL;
	drawscreen(skin, 0, 0);
	resettvpic();
	if(playinfobarstatus > 0 &&	status.play == 1)
		screenplayinfobar(file, showname, 0, playertype, flag);

	status.hangtime = hangtime;
	return ret;
}

void playrcinfo(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(checkbit(status.playercan, 14) == 0) return;

	if(*playinfobarstatus == 0)
	{
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		screenplayinfobar(file, showname, 0, playertype, flag);
	}
	else if(*playinfobarstatus == 1)
	{
		*playinfobarstatus = 0;
		screenplayinfobar(NULL, NULL, 1, playertype, flag);
	}
}

void playrcstop(int playertype, int flag)
{
	if(checkbit(status.playercan, 6) == 0) return;

	free(status.playfile); status.playfile = NULL;
	
	if(playertype == 1)
		playerstopts(0, 0);
	else if(playertype == 2)
		dvdstop();
	else
		playerstop();

	writevfdmenu("Player");
	screenplayinfobar(NULL, NULL, 1, playertype, flag);
}

//type 0: only from play
//type 1: only from pause
void playrcsf(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int type, int flag)
{
	if(checkbit(status.playercan, 15) == 0) return;

	if(status.playspeed == 0 && ((type == 0 && status.pause == 0) || (type == 1 && status.pause == 1)))
	{
		status.slowspeed++;
		if(status.slowspeed > 6) status.slowspeed = 6;
		if(status.slowspeed > 0)
		{
			status.play = 0;

			if(playertype == 1)
				playerslowts((int)pow(2, status.slowspeed));
			else if(playertype == 0)
				playerslow(status.slowspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.slowspeed == 0)
		{
			status.pause = 0;
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				audioclearbuffer(status.aktservice->audiodev);
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			*playinfobarstatus = 1;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
	}
}

//type 0: only from play
//type 1: only from pause
void playrcsr(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int type, int flag)
{
	if(checkbit(status.playercan, 15) == 0) return;

	if(status.playspeed == 0 && ((type == 0 && status.pause == 0) || (type == 1 && status.pause == 1)))
	{
		status.slowspeed--;
		if(status.slowspeed < 0) status.slowspeed = 0;
		if(status.slowspeed > 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerslowts((int)pow(2, status.slowspeed));
			else if(playertype == 0)
				playerslowts(status.slowspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.slowspeed == 0)
		{
			status.pause = 0;
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				audioclearbuffer(status.aktservice->audiodev);
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			*playinfobarstatus = 1;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
	}
}

void playrcff(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.pause == 1)
	{
		playrcsf(file, showname, playinfobarstatus, playinfobarcount, playertype, 1, flag);
		return;
	}

	if(checkbit(status.playercan, 7) == 0) return;

	if(status.pause == 0 && status.slowspeed == 0)
	{
		status.playspeed++;
		if(status.playspeed > 6) status.playspeed = 6;
		if(status.playspeed > 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerffts((int)pow(2, status.playspeed));
			else if(playertype == 2)
				dvdff(status.playspeed);
			else	
				playerff(status.playspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.playspeed < 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerfrts((int)(pow(2, status.playspeed * -1) * -1), 0);
			else if(playertype == 2)
				dvdfr(status.playspeed);
			else
				playerfr(status.playspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.playspeed == 0)
		{
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			*playinfobarstatus = 1;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
	}
}

void playrcfr(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.pause == 1)
	{
		playrcsr(file, showname, playinfobarstatus, playinfobarcount, playertype, 1, flag);
		return;
	}
	
	if(checkbit(status.playercan, 8) == 0) return;

	if(status.pause == 0 && status.slowspeed == 0)
	{
		status.playspeed--;
		if(status.playspeed < -6) status.playspeed = -6;
		if(status.playspeed > 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerffts((int)pow(2, status.playspeed));
			else if(playertype == 2)
				dvdff(status.playspeed);
			else
				playerff(status.playspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.playspeed < 0)
		{
			status.play = 0;
			if(playertype == 1)
				playerfrts((int)(pow(2, status.playspeed * -1) * -1), 0);
			else if(playertype == 2)
				dvdfr(status.playspeed);
			else
				playerfr(status.playspeed);
			*playinfobarstatus = 2;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
		if(status.playspeed == 0)
		{
			status.play = 1;
			if(playertype == 1)
			{
				playerpausets();
				playercontinuets();
				playerresetts();
			}
			else if(playertype == 2)
				dvdcontinue();
			else
				playercontinue();
			*playinfobarstatus = 1;
			*playinfobarcount = 0;
			screenplayinfobar(file, showname, 0, playertype, flag);
		}
	}
}

void playrcpause(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.playspeed != 0 || status.slowspeed != 0)
	{
		playrcplay(file, showname, playinfobarstatus, playinfobarcount, playertype, flag);
		return;
	}
	
	if(checkbit(status.playercan, 9) == 0) return;

	if(status.pause == 1)
	{
		if(playertype == 1)
		{
			playercontinuets();
		}
		else if(playertype == 2)
			dvdcontinue();
		else
			playercontinue();
		status.slowspeed = 0;
		status.playspeed = 0;
		status.play = 1;
		status.pause = 0;
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		screenplayinfobar(file, showname, 0, playertype, flag);
	}
	else
	{
		status.slowspeed = 0;
		status.playspeed = 0;
		status.play = 0;
		status.pause = 1;
		if(playertype == 1)
			playerpausets();
		else if(playertype == 2)
			dvdpause();
		else
			playerpause();
		*playinfobarstatus = 2;
		*playinfobarcount = 0;
		screenplayinfobar(file, showname, 0, playertype, flag);
	}
}

void playrcplay(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(checkbit(status.playercan, 10) == 0) return;

	free(status.playfile); status.playfile = NULL;
	status.playfile = ostrcat(file, NULL, 0, 0);

	if(playertype == 1)
	{
		if(status.playspeed != 0 || status.slowspeed != 0)
			playerpausets();
		if(status.slowspeed != 0)
			audioclearbuffer(status.aktservice->audiodev);
		playercontinuets();
		if(status.playspeed != 0 || status.slowspeed != 0)
			playerresetts();
	}
	else if(playertype == 2)
		dvdcontinue();
	else
		playercontinue();
	status.slowspeed = 0;
	status.playspeed = 0;
	status.pause = 0;
	status.play = 1;
	*playinfobarstatus = 1;
	*playinfobarcount = 0;
	screenplayinfobar(file, showname, 0, playertype, flag);
}

void playrcjumpr(char* file, char* showname, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
//	if(checkbit(status.playercan, 11) == 0) return;

	unsigned long long pos = 0;
	
	if(status.pause == 0 && status.playspeed == 0 && status.slowspeed == 0)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);
		
		//a jump over the beginning of the
		//file, freez the player (type 0)
		if(playertype == 1)
			playerseekts(getservice(RECORDPLAY, 0), sec * -1, 0);
		else if(playertype == 2)
			pos = dvdgetpts() / 90000;
		else
			pos = playergetpts() / 90000;
	
		if(playertype != 1)
		{
			if(pos + 10 > sec)
			{
				if(playertype == 2)
					dvdseek(sec * -1);
				else
					playerseek(sec * -1);
			}
			else
			{
				if(playertype == 2)
				{
					dvdstop();
					dvdstart(file);
				}
				else
				{
					playerstop();
					playerstart(file);
				}
			}
		}

		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		//status.play = 0;
		clearscreen(load);
		screenplayinfobar(file, showname, 0, playertype, flag);
		//status.play = 1;
		sleep(1);
	}
}

void playrcjumpto(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	if(status.pause == 0 && status.playspeed == 0 && status.slowspeed == 0)
	{
		char* tmpstr = numinput(_("Jump To (min)"), NULL, "000", 0);
		if(tmpstr != NULL)
		{
			int jump = atoi(tmpstr) * 60;
			playrcjumpf(file, showname, jump, playinfobarstatus, playinfobarcount, playertype, flag);
		}
		free(tmpstr), tmpstr = NULL;
	}
}

void playrcjumpf(char* file, char* showname, int sec, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
//	if(checkbit(status.playercan, 12) == 0) return;

	if(status.pause == 0 && status.playspeed == 0 && status.slowspeed == 0)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);
		
		if(playertype == 1)
			playerseekts(getservice(RECORDPLAY, 0), sec, 0);
		else if(playertype == 2)
			dvdseek(sec);
		else
			playerseek(sec);
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		//status.play = 0;
		clearscreen(load);
		screenplayinfobar(file, showname, 0, playertype, flag);
		//status.play = 1;
	}
}

void playchangecodec(int playertype)
{
	if(checkbit(status.playercan, 13) == 0) return;

	char** tracklist = NULL;

	if(getconfigint("av_ac3default", NULL) == 1)
	{
		int i = 0;
		
		tracklist = playergettracklist(1);
		if(tracklist != NULL)
		{
			while(tracklist[i] != NULL)
			{
				if(ostrcmp(tracklist[i + 1], "A_AC3") == 0)
				{
					playerchangeaudiotrack(i / 2);
				}
				i += 2;
			}
		}
		playerfreetracklist(tracklist);
		tracklist = NULL;
	}
}

int playcheckdirrcret(char* file, int dirrcret)
{
	int ret = 0;
	char* epgfilename = NULL, *tmpstr = NULL;

	if(dirrcret == 4)
	{
		int sort = screendirsort();
		addconfigint("dirsort", sort);
		ret = 1;
	}
	if(dirrcret == 3)
	{
		epgfilename = changefilenameext(file, ".epg");

		tmpstr = readfiletomem(epgfilename, 0);
		if(tmpstr != NULL)
			textbox(_("EPG Info"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 2);
		else {
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".eit");
			tmpstr = readeittomem(epgfilename);
			if(tmpstr != NULL)
				textbox(_("EPG Info"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 2);
		}
		free(epgfilename); epgfilename = NULL;
		free(tmpstr); tmpstr = NULL;
		ret = 1;
	}
	if(dirrcret == 1)
	{
		if(getservicebyrecname(file, 0, 0) != NULL)
			textbox(_("Message"), _("Record in progress"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
		else if(textbox(_("Really Delete ?"), file, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
		{
			unlink(file);
			if(file_exist(file))
			{
				char* cmd = ostrcat("rm -rf \"", file, 0, 0);
				cmd = ostrcat(cmd, "\"", 1, 0);
				debug(10, "cmd: %s", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}			
			epgfilename = changefilenameext(file, ".epg");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".se");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".ma");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(file, ".as");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
		}
		ret = 1;
	}
	
	return ret;
}

void playwritevfd(char* file, char* showname)
{
	char* tmpstr = NULL;

	if(showname == NULL)
	{
		tmpstr = ostrcat(file, NULL, 0, 0);
		if(tmpstr != NULL) writevfdmenu(basename(tmpstr));
		free(tmpstr); tmpstr = NULL;
	}
	else
		writevfdmenu(showname);
}


void playstartservice()
{
	char* tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
		tmpstr = ostrcat(status.aktservice->channellist, NULL, 0, 0);
		servicecheckret(servicestart(status.aktservice->channel, tmpstr, NULL, 3), 0);
	}
	else
	{
		tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
		servicecheckret(servicestart(status.lastservice->channel, tmpstr, NULL, 0), 0);
	}
	free(tmpstr); tmpstr = NULL;
}

// flag 0 = dirlist/playing/infobar
// flag 1 = playing/infobar
// flag 2 = playing
// flag 3 = not stop/start live service
// flag 4 = playing with screensaver
// startfolder 2 = do nothing with playstop/playstart
int screenplay(char* startfile, char* showname, int startfolder, int flag)
{
	int rcret = 0, playertype = 0, ret = 0, rcwait = 1000, screensaver_delay = 0, holdselection = 0, waitofbuffer = 0;
	char* file = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	char* tmppolicy = NULL, *startdir = NULL;
	char* formats = NULL;
	struct channel* pipchannel = NULL; 
	struct skin* playinfobar = getscreen("playinfobar");
	struct skin* sprogress = getscreennode(playinfobar, "progress");
	struct skin* load = getscreen("loading");

	int oldsort = getconfigint("dirsort", NULL);
	int skip13 = getconfigint("skip13", NULL);
	int skip46 = getconfigint("skip46", NULL);
	int skip79 = getconfigint("skip79", NULL);

	if(startfolder == 0 && flag != 3)
	{
		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1) return ret;
	}

	if(status.webplayfile != NULL)
	{
		startfile = status.webplayfile;
		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1) return ret;
	}

	// allowed from atemio avi mkv mpg4 xvid mpg1 mpg2 jpeg png
	if(startfolder == 1)
		formats = ostrcat(formats, ".ts", 1, 0);
	else if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
		formats = ostrcat(formats, ".flac .ogg .mp3 .avi .dat .divx .flv .mkv .m4v .mp4 .mov .mpg .mpeg .mts .m2ts .trp .ts .vdr .vob .wmv .rm", 1, 0);
	else
		formats = ostrcat(formats, ".avi .mkv .mpg .mpeg .ts .mp3 .flac .ogg", 1, 0);

	
	status.updatevfd = PAUSE;
	tmppolicy = getpolicy();

playerstart:
	//reset timeline
	sprogress->progresssize = 0;

	if(startfolder == 0)
		startdir = getconfig("rec_moviepath", NULL);
	else
		startdir = getconfig("rec_path", NULL);

	status.playspeed = 0, status.play = 0, status.pause = 0, status.slowspeed = 0;
	int playinfobarcount = 0, playinfobarstatus = 1, dirrcret = 0;

	if(startfile == NULL)
	{
		readmediadb(getconfig("mediadbfile", NULL), 0, 0);

		tmpstr = ostrcat(file, NULL, 0, 0);
		char* tmpstartdir = startdir;
		if(file != NULL)
		{
			tmpstr1 = ostrcat(file, NULL, 0, 0);
			startdir = dirname(tmpstr1);
		}
		free(file); file = NULL;
		file = screendirreal(startdir, formats, basename(tmpstr), &dirrcret, ".epg", _("DEL"), getrcconfigint("rcred", NULL), _("SELECT"), 0, _("EPG"), getrcconfigint("rcyellow", NULL), _("SORT"), getrcconfigint("rcblue", NULL), 90, 1, 90, 1, holdselection, 64);
		holdselection = 0;
		free(tmpstr); tmpstr = NULL;
		free(tmpstr1); tmpstr1 = NULL;
		startdir = tmpstartdir;
	}
	else
		file = ostrcat(startfile, NULL, 0, 0);

	if(file == NULL)
	{
		if(playcheckdirrcret(file, dirrcret) == 1)
			goto playerstart;
	}

	if(file != NULL)
	{
		//if(getconfigint("playertype", NULL) == 1 && cmpfilenameext(file, ".ts") == 0)
		if(cmpfilenameext(file, ".ts") == 0)
			playertype = 1;

		if(startfile == NULL)
		{
			tmpstr = ostrcat(file, NULL, 0, 0);
			if(tmpstr != NULL && startfolder == 0) addconfig("rec_moviepath", dirname(tmpstr));
			free(tmpstr); tmpstr = NULL;
		
			if(playcheckdirrcret(file, dirrcret) == 1)
			{
				if(dirrcret == 1) holdselection = 1;
				goto playerstart;
			}

			if(startfolder == 1 && flag != 3)
			{
				rcret = servicestop(status.aktservice, 1, 1);
				if(rcret == 1)
				{
					free(tmppolicy);
					free(file);
					free(formats);
					addconfigint("dirsort", oldsort);
					return ret;
				}
			}
		}

		drawscreen(skin, 0, 0);
		drawscreen(load, 0, 0);
		playwritevfd(file, showname);
		if(playertype == 1)
			rcret = playerstartts(file, 0);
		else if(playertype == 2)
			rcret = playerstart(file);
		else
		{
#ifdef EPLAYER3
			if(ostrstr(file, "http://") == file)
			{
				struct stimerthread* bufferstatus = addtimer(&screenplaybufferstatus, START, 1000, 1, NULL, NULL, NULL);
				rcret = playerstart(file);
				if(bufferstatus != NULL && gettimer(bufferstatus) != NULL)
				{
					bufferstatus->aktion = STOP;
					usleep(100000);
				}
			}
			else
				rcret = playerstart(file);
#else
			rcret = playerstart(file);
#endif
		}
#ifndef SIMULATE
		if(rcret != 0)
		{
			textbox(_("Message"), _("Can't start playback !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
			writevfdmenu("Player");
			
			if(startfile == NULL)
			{
				if(startfolder == 1 && flag != 3) playstartservice();
				goto playerstart;
			}
			else
			{
				ret = 2;
				goto playerend;
			}
		}
#endif
		clearscreen(load);
		if(status.prefillbuffer == 0)
			screenplayinfobar(file, showname, 0, playertype, flag);
		else
			waitofbuffer = 1;
		
		if(flag == 4 && getconfigint("screensaver", NULL) == 1)
		{
			screensaver_delay = getconfigint("screensaver_delay", NULL);
			initscreensaver();
		}								
		
		//change codec if ac3default and video has ac3
		//deaktivate, freeze player, makes a seek -5
		//see eplayer container_ffmpeg_switch_audio
		//the seek to the beginning of the file freez
		//eplayer.
		//playchangecodec();
		free(status.playfile); status.playfile = NULL;
		status.playfile = ostrcat(file, NULL, 0, 0);
		status.play = 1;
		int count = 0;
		while(1)
		{
			while((playertype == 0 && playerisplaying()) || (playertype == 1 && playerisplayingts()))
			{
				rcret = waitrc(playinfobar, rcwait, 0);
#ifdef MIPSEL
				if(waitofbuffer == 1 &&	status.prefillbuffer == 0 && status.cleaninfobar == 1)
				{
					drawscreen(skin, 0, 0);
					screenplayinfobar(file, showname, 0, playertype, flag);
					waitofbuffer = 0;
					status.cleaninfobar = 0;
					
				}
				else if(waitofbuffer == 0 && status.prefillbuffer == 0 && status.cleaninfobar == 0)
				{
					playinfobarcount++;
					if(playinfobarstatus > 0)
						screenplayinfobar(file, showname, 0, playertype, flag);
					if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
					{
						playinfobarstatus = 0;
						screenplayinfobar(NULL, NULL, 1, playertype, flag);
					}
				}
#else
				playinfobarcount++;
				if(playinfobarstatus > 0)
					screenplayinfobar(file, showname, 0, playertype, flag);
				if(playinfobarstatus == 1 && playinfobarcount >= getconfigint("infobartimeout", NULL))
				{
					playinfobarstatus = 0;
					screenplayinfobar(NULL, NULL, 1, playertype, flag);
				}

				if(waitofbuffer == 1 &&	status.prefillbuffer == 0)
				{
					screenplayinfobar(file, showname, 0, playertype, flag);
					waitofbuffer = 0;
				}
#endif
				if(flag == 4)
				{
					if(status.play == 1 && screensaver != NULL)
						count++;
	
					if(count > screensaver_delay && screensaver != NULL)
					{
						showscreensaver();
#ifdef MIPSEL
						if(screensaver->speed < 50)
							rcwait = screensaver->speed * 10;
						else
							rcwait = screensaver->speed;
#else
						rcwait = screensaver->speed;
#endif
					}
				}
		
				if(rcret == getrcconfigint("rcpip", NULL))
				{
					if(status.pipservice->videodev == NULL)
					{
						pipchannel = status.lastservice->channel;
						printf("++++ RC: %i\n",pipstart(pipchannel, NULL, 0));
					}
					else
					{
						pipstop(status.pipservice, 0);
						pipchannel = NULL;
					}
				}				
				
				if(rcret == getrcconfigint("rcyellow", NULL))
					playrcyellow(file, showname, playinfobarstatus, playertype, flag);
				
				if(rcret == getrcconfigint("rctext", NULL) || rcret == getrcconfigint("rcsubtitel", NULL))
					playrctext(file, showname, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcgreen", NULL))
					playrcgreen(file, showname, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcblue", NULL))
					playrcblue(file, showname, playinfobarstatus, playertype, flag);
					
				if(rcret == getrcconfigint("rcok", NULL))
					playrcok(file, showname, playinfobarstatus, playertype, flag);
				
				if(rcret == getrcconfigint("rcred", NULL))
					playrcred(file, showname, playinfobarstatus, playertype, flag);

				if(rcret == getrcconfigint("rcinfo", NULL))
					playrcinfo(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcstop", NULL) || rcret == getrcconfigint("rcexit", NULL))
				{
					if(status.prefillbuffer == 1)
					{
						status.prefillbuffer = 2;
						continue;
					}
					playrcstop(playertype, flag);
					if(startfile == NULL)
					{						
						if(startfolder == 1 && flag != 3) playstartservice();
						goto playerstart;
					}
					else
					{
						ret = 1;
						goto playerend;
					}
				}
				
				if(rcret == getrcconfigint("rcsf", NULL))
					playrcsf(file, showname, &playinfobarstatus, &playinfobarcount, playertype, 0, flag);
          
        		if(rcret == getrcconfigint("rcsr", NULL))
					playrcsr(file, showname, &playinfobarstatus, &playinfobarcount, playertype, 0, flag);

				if(rcret == getrcconfigint("rcff", NULL))
					playrcff(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcfr", NULL))
					playrcfr(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcpause", NULL) || ((checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1) && rcret == getrcconfigint("rcplay", NULL) && status.pause == 0 && status.slowspeed == 0 && status.playspeed == 0 && ostrcmp(getconfig("remotecontrol", NULL), "0") == 0))
				{	
					playrcpause(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
					continue;
				}

				if(rcret == getrcconfigint("rcplay", NULL))
					playrcplay(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcleft", NULL))
					playrcjumpr(file, showname, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc1", NULL))
					playrcjumpr(file, showname, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc4", NULL))
					playrcjumpr(file, showname, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc7", NULL))
					playrcjumpr(file, showname, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rc2", NULL))
					playrcjumpto(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcright", NULL))
					playrcjumpf(file, showname, 60, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc3", NULL))
					playrcjumpf(file, showname, skip13, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc6", NULL))
					playrcjumpf(file, showname, skip46, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc9", NULL))
					playrcjumpf(file, showname, skip79, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcdown", NULL))
					playrcjumpr(file, showname, 300, &playinfobarstatus, &playinfobarcount, playertype, flag);

				if(rcret == getrcconfigint("rcup", NULL))
					playrcjumpf(file, showname, 300, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rc0", NULL))
					setmarker();
					
				if(rcret == getrcconfigint("rc5", NULL))
					screenmarker(file, showname, &playinfobarstatus, &playinfobarcount, playertype, flag);
				
				if(rcret == getrcconfigint("rcmenu", NULL)) {
					char* asfile = changefilenameext(file, ".as");
					if(status.autoseek == 0) {
						status.autoseek = 1;
						textbox(_("Message"), _("Marker autoseek is started"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
						addtimer(&markerautoseek_thread, START, 10000, 1, NULL, NULL, NULL);
						FILE* testseek = fopen(asfile, "w");
						if(testseek != NULL)
						{
							fprintf(testseek,"autoseek is started\n");
							fclose(testseek);
						}
					}
					else {
						status.autoseek = 0;
						textbox(_("Message"), _("Marker autoseek is stopped"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
						unlink(asfile);
					}
					free(asfile); asfile = NULL;
				}
			}
			//don't change this sleep, without this
			//the player stops to fast, and a last seek can
			//produce a segfault
playerend:
			sleep(1);
			if(playertype == 1)
				playerafterendts();
			else
				playerafterend();

			writevfdmenu("Player");
			screenplayinfobar(file, showname, 1, playertype, flag);

			if(startfile == NULL)
			{
				if(startfolder == 1 && flag != 3) playstartservice();
				goto playerstart;
			}
			else
				break;
		}
	}
	if(startfolder == 0 && flag != 3) playstartservice();
	status.updatevfd = START;
	
	if(status.webplayfile != NULL)
	{
		playstartservice();
		free(status.webplayfile); status.webplayfile = NULL;
	}

	if(tmppolicy != NULL)
	{
		setpolicy(tmppolicy);
		free(tmppolicy);
	}
	
	if(flag == 4)
		deinitscreensaver();

	addconfigint("dirsort", oldsort);
	free(status.playfile); status.playfile = NULL;
	status.slowspeed = 0; 
	status.playspeed = 0;
	status.pause = 0;
	status.play = 0;
	free(file);
	free(formats);

	if(status.mediadbthread == NULL)
	{
		if(status.writemediadb == 1)
			writemediadb(getconfig("mediadbfile", NULL), NULL);
		freemediadb(0);
	}

	return ret;
}

#endif
