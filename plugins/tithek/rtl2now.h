#ifndef RTL2NOW_H
#define RTL2NOW_H

// flag 1 = get getstreamurl http
// flag 2 = getstreamurl

char* rtl2now(char* link, char* url, char* name, int flag)
{
	debug(99, "link(%d): %s", flag, link);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *newlink = NULL, *newpath = NULL;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	char* tmpstr = NULL;
	tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
		
	if(flag == 1)
	{
		if(ostrstr(tmpstr, "<!-- 1-->") != NULL)
			textbox(_("Message"), _("Found Pay Stream visit the officle Website and Order this Stream and try this agine !" , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);

		tmpstr = string_resub("data:'", "',", tmpstr, 0);
		debug(99, "tmpstr: %s", tmpstr);

		htmldecode(tmpstr, tmpstr);
		tmpstr = ostrcat(url, tmpstr, 0, 1);
		debug(99, "streamurl: %s", tmpstr);
		streamurl = rtl2now(tmpstr, url, name, 2);
		free(tmpstr); tmpstr = NULL;
	}
	else if(flag == 2)
	{
		tmpstr = string_resub("rtmpe://", ".f4v", tmpstr, 0);
		char* tmpstr9 = NULL;
		tmpstr9 = ostrcat(tmpstr9, tmpstr, 1, 0);
		free(tmpstr), tmpstr = NULL;
		tmpstr = ostrcat("rtmpe://", tmpstr9, 0, 0);
		tmpstr = ostrcat(tmpstr, ".f4v", 1, 0);		
		free(tmpstr9), tmpstr9 = NULL;

		debug(99, "tmpstr: %s", tmpstr);

		int count = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "/", &count);
		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				if(i < 3)
				{
					if(count > i)
						newlink = ostrcat(newlink, (&ret1[i])->part, 1, 0);

					if(i == 0)
						newlink = ostrcat(newlink, "//", 1, 0);
					else
						newlink = ostrcat(newlink, "/", 1, 0);
				}
				else
				{
					if(count > i)
						newpath = ostrcat(newpath, (&ret1[i])->part, 1, 0);
					if(i != max - 1)
						newpath = ostrcat(newpath, "/", 1, 0);
				}
			}
			free(ret1), ret1 = NULL;

			debug(99, "newlink: %s", newlink);
			debug(99, "newpath: %s", newpath);
	
			streamurl = ostrcat(newlink, " swfVfy=1 playpath=mp4:", 0, 0);
			streamurl = ostrcat(streamurl, newpath, 1, 0);
			streamurl = ostrcat(streamurl, " app=", 1, 0);
			streamurl = ostrcat(streamurl, name, 1, 0);
			streamurl = ostrcat(streamurl, "/_definst_ pageUrl=", 1, 0);
			streamurl = ostrcat(streamurl, url, 1, 0);
			streamurl = ostrcat(streamurl, "/p/ tcUrl=", 1, 0);
			streamurl = ostrcat(streamurl, newlink, 1, 0);
			streamurl = ostrcat(streamurl, " swfUrl=", 1, 0);
			streamurl = ostrcat(streamurl, url, 1, 0);
			streamurl = ostrcat(streamurl, "/includes/vodplayer.swf", 1, 0);
		}
		free(tmpstr); tmpstr = NULL;
		debug(99, "streamurl: %s", streamurl);
	}

	free(ip), ip = NULL;
	free(newlink), newlink = NULL;
	free(newpath), newpath = NULL;
// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;
	
	debug(99, "streamurl2: %s", streamurl);	
	return streamurl;
}

#endif
