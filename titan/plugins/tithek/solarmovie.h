#ifndef SOLARMOVIE_H
#define SOLARMOVIE_H

char* solarmovie(char* link)
{
	debug(99, "link: %s", link);
	char* tmpstr = NULL, *tmpstr1 = NULL, *pos = NULL, *url = NULL, *streamurl = NULL, *tmphost = NULL, *tmppath = NULL;
	char* error = NULL;

	if(link == NULL || ostrncmp("http://", link, 7)) return NULL;

	tmphost = string_replace("http://", "", (char*)link, 0);

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		tmppath = pos + 1;
	}
	
	char* send = ostrcat("GET /", NULL, 0, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0); 
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "send: %s", send);

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 0);
	debug(99, "tmpstr: %s", tmpstr);
	free(send); send = NULL;

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "<h2>404</h2>") != NULL)
	{
		error = string_resub("<strong>", "</strong>", tmpstr, 0);
		string_deltags(error);
		stringreplacechar(error, '|', '\0');
		error = strstrip(error);
		if(error == NULL || strlen(error) == 0)
			error = ostrcat(_("The page is temporarily unavailable"), error, 0, 1);
		textbox(_("Message"), error, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		goto end;
	}
            
	string_strip_whitechars(tmpstr);
	if(ostrstr(tmpstr, "<div class=\"thirdPartyEmbContainer\">") != NULL)
	{
		tmpstr1 = string_resub("<div class=\"thirdPartyEmbContainer\">", "</div>", tmpstr, 0);
		stringreplacechar(tmpstr1, '\n', ' ');
		url = string_resub("<center><iframe src=\"", "\"", tmpstr1, 0);
		if(url == NULL || ostrncmp("http://", url, 7) == 1)
		{
			free(url); url == NULL;
			url = oregex(".*src=\"(http://.*)&width.*", tmpstr1);
		}
	}
	else
	{
		//char* tmpurl = oregex(".*<iframe name=\"service_frame\" class=\"service_frame\" src=\"(http://.*)\".*", tmpstr);
		//url = oregex("(http://.*)\".*", tmpurl);
		//free(tmpurl); tmpurl == NULL;
		url = string_resub("<iframe name=\"service_frame\" class=\"service_frame\" src=\"", "\"", tmpstr, 0);
		url = string_replace_all("embed", "file", url, 1);
	}

	streamurl = hoster(url);

end:

	free(error), error = NULL;
	free(url), url = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	free(tmphost), tmphost = NULL;
//	free(tmppath), tmppath = NULL;

	return streamurl;
}

int solarmovie_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1;
	int incount = 0;
		
	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* search = NULL;
	if(searchstr == NULL)
		search = textinputhist("Search", " ", "searchhist");
	else
		search = textinputhist("Search", searchstr, "searchhist");
	
	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		char* tmpstr = NULL;
		char* tmpstr1 = NULL;
		char* line = NULL;
		char* pic = NULL;
		char* title = NULL;
		char* url = NULL;
		char* ip = NULL;
		char* path = NULL;
		char* menu = NULL;
		char* file = NULL;
		char* file1 = NULL;
		search = strstrip(search);
		search = stringreplacechar(search, ' ', '+');
		debug(99, "search: %s", search);

		ip = ostrcat("www.solarmovie.so", NULL, 0, 0);
		if(flag == 0)
			path = ostrcat("movie/search/", search, 0, 0);
		else
			path = ostrcat("tv/search/", search, 0, 0);

		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		titheklog(debuglevel, "/tmp/solarmovie_tmpstr", NULL, tmpstr);

		while(ostrstr(tmpstr, "<div class=\"typicalGrey coverGroup\">") != NULL)
		{
			int rcret = waitrc(NULL, 10, 0);
			if(rcret == getrcconfigint("rcexit", NULL)) break;

			incount += 1;
			if(debuglevel == 99)
			{
				file = ostrcat("/tmp/solarmovie_tmpstr", oitoa(incount), 0, 1);
				writesys(file, tmpstr, 0);
			}
				
			tmpstr1 = string_resub("<div class=\"typicalGrey coverGroup\">", "</div>", tmpstr, 0);

			if(debuglevel == 99)
			{
				file1 = ostrcat("/tmp/solarmovie_tmpstr_resub", oitoa(incount), 0, 1);
				writesys(file1, tmpstr1, 0);
			}

			path = string_resub("href=\"", "\"", tmpstr1, 0);
			pic = string_resub("<img src=\"", "\"", tmpstr1, 0);
			title = string_resub("<a title=\"", "\"", tmpstr1, 0);
	
			url = ostrcat("http://www.solarmovie.so", path, 0, 0);
	
			debug(99, "---------------------------");
			debug(99, "pic: %s", pic);
			debug(99, "title: %s", title);
			debug(99, "url: %s", url);					
			debug(99, "---------------------------");
	
			if(url != NULL)
			{
				line = ostrcat(line, title, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, url, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, pic, 1, 0);
				line = ostrcat(line, "#solarmovie_search_", 1, 0);
				line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
				line = ostrcat(line, ".jpg#SolarMovie - Search#28\n", 1, 0);
			}
			free(url), url = NULL;
			free(path), path = NULL;
			free(title), title = NULL;
			free(pic), pic = NULL;		
			free(file), file = NULL;
			free(file1), file1 = NULL;

			tmpstr = string_replace("typicalGrey coverGroup", "", tmpstr, 1);
			free(tmpstr1), tmpstr1 = NULL;
		}

		if(line != NULL)
		{
			menu = ostrcat("/tmp/tithek/solarmovie.search.list", NULL, 0, 0);
			writesys(menu, line, 0);
			free(line); line = NULL;
			
			struct tithek* tnode = (struct tithek*)listbox->select->handle;
			createtithek(tnode, tnode->title,  menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
			ret = 0;
		}

		free(tmpstr), tmpstr = NULL;
		free(ip), ip = NULL;
	}
	free(search), search = NULL;
	return ret;
}

int solarmovie_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1, series = 0;
	char* pos = NULL, *etitle = NULL, *episode = NULL, *session = NULL, *update = NULL, *quality = NULL, *id = NULL, *line = NULL, *hname = NULL, *tmpstr = NULL, *cmd = NULL, *url = NULL, *tmpstr1 = NULL, *tmphost = NULL, *tmphname = NULL;
	char* pichname = NULL, *tmppath = NULL;
	unlink("/tmp/tithek/get");
	unlink("/tmp/tithek/get_zcat");
	unlink("/tmp/tithek/get_zcat1");

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	drawscreen(load, 0, 0);

	tmphost = string_replace("http://", "", (char*)link, 0);

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		tmppath = pos + 1;
	}

	tmpstr = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);
	
	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
//	if(debuglevel == 99) system("cp -a /tmp/tithek/get /tmp/solarmovie1_tmpstr_get");

//	tmpstr = command("cat /tmp/tithek/get");
	if(ostrstr(tmpstr, "<!DOCTYPE") == NULL)
	{
		cmd = ostrcat(cmd, "cat /tmp/tithek/get | zcat", 1, 0);
		debug(99, "cmd: %s", cmd);
		free(tmpstr), tmpstr = NULL;
		tmpstr = command(cmd);
		
		writesys("/tmp/tithek/get_zcat", tmpstr, 0);
		free(cmd), cmd = NULL;
	}
//	else
//		system("cp -a /tmp/tithek/get /tmp/tithek/get_zcat");

	titheklog(debuglevel, "/tmp/solarmovie2_tmpstr_zcat", NULL, tmpstr);

	if(ostrstr(link, "/tv/") != NULL && ostrstr(link, "/season-") == NULL && ostrstr(link, "/episode-") == NULL)	
		series = 1;
	else
		series = 0;

	if(series == 0)
	{
		series = 0;
		if(tmpstr != NULL)
		{
//			free(tmpstr), tmpstr = NULL;
//			tmpstr = command("cat /tmp/tithek/get_zcat");
			char* tmpcat = string_resub("<tbody>", "</tbody>", tmpstr, 0);
	
			char* ptmpcat = ostrstr(tmpcat, "<td class=\"qualit");
			int i = 0, incount = 0;
			while(ptmpcat != NULL)
			{
				i++;
				tmpstr1 = ostrcat(tmpstr1, string_resub("<td class=\"qualit", "<div class=\"linkDetails\">", ptmpcat, 0), 1, 1);
				ptmpcat += 5;
				ptmpcat = ostrstr(ptmpcat, "<td class=\"qualit");
	
				if(ptmpcat != NULL)
				{
//					id = string_resub("<a href=\"/link/show/", "/", tmpstr1, 0);
//					id = oregex(".*/link/show/(.*)/\">.*", tmpstr1);
//					id = oregex(".*/link/show/(.*)/.*\">.*", tmpstr1);
					char* tmpid = oregex(".*<a href=\"/link/(.*)/.*", tmpstr1);
//					debug(99, "tmpid: %s", tmpid);
					id = string_resub("show/", "/", tmpid, 0);
					free(tmpid), tmpid = NULL;
//					debug(99, "id: %s", id);
//					debug(99, "#######################################################");


					quality = string_resub("yCell\">", "</td>", tmpstr1, 0);
					update = string_resub("oddCell\">", "</td>", tmpstr1, 0);
					string_remove_whitechars(quality);
					strstrip(quality);
					
//					tmphname = oregex(".*/\">(.*)</a>.*", tmpstr1);
					tmphname = oregex(".*<a href=\"/link/show/.*\">(.*)</a>.*", tmpstr1);

					string_remove_whitechars(tmphname);
					strstrip(tmphname);

					pichname = ostrcat(tmphname, NULL, 0, 0);
					string_tolower(pichname);
					pichname = stringreplacecharonce(pichname, '.', '\0');

					int type = 43;
					debug(99, "(%d/\?\?) %s id: %s quality: %s update: %s pic: %s", i, tmphname, id, quality, update, pichname);

					incount += 1;
					line = ostrcat(line, tmphname, 1, 0);
					if(quality != NULL)
					{
						line = ostrcat(line, " (", 1, 0);					
						line = ostrcat(line, quality, 1, 0);					
						line = ostrcat(line, ")", 1, 0);					
					}
					if(update != NULL)
					{
						line = ostrcat(line, " (", 1, 0);					
						line = ostrcat(line, update, 1, 0);					
						line = ostrcat(line, ")", 1, 0);					
					}					
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://www.solarmovie.so/link/play/", 1, 0);
					line = ostrcat(line, id, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
					line = ostrcat(line, pichname, 1, 0);
					line = ostrcat(line, ".jpg#solarmovie_", 1, 0);
					line = ostrcat(line, pichname, 1, 0);
					line = ostrcat(line, ".jpg#Solarmovie - ", 1, 0);
					line = ostrcat(line, title, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, oitoa(type), 1, 0);
					line = ostrcat(line, "\n", 1, 0);
					free(hname), hname = NULL;
								
					free(pichname), pichname = NULL;		
					free(tmphname), tmphname = NULL;
					free(id), id = NULL;		
				}
			}
			free(tmpcat); tmpcat = NULL;
		}	
	}
	else
	{
		series = 1;
		if(tmpstr != NULL)
		{
//writesys("/tmp/tithek/get_zcat", tmpstr, 0);

			free(tmpstr), tmpstr = NULL;
			tmpstr = command("cat /tmp/tithek/get_zcat | grep episode- | grep -v Episode | grep -v 'linkCount typicalGrey'");
			writesys("/var/usr/local/share/titan/plugins/tithek/solarmovie8_tmpstr", tmpstr, 0);

			int count = 0;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(tmpstr, "\n", &count);
			int max = count;
			int i = 0, incount = 0;
			for(i = 0; i < max; i++)
			{
//<a href="/tv/avatar-the-last-airbender-2005/season-3/episode-2/">The Headband</a>
				session = oregex(".*/season-([0-9]{1,2})/episode-.*", ret1[i].part);
				episode = oregex(".*/episode-([0-9]{1,2})/.*", ret1[i].part);
				etitle = oregex(".*>(.*)<.*", ret1[i].part);
				url = string_resub("\"", "\"", ret1[i].part, 0);

				incount += 1;
				line = ostrcat(line, _("Season"), 1, 0);
				line = ostrcat(line, " ", 1, 0);
				line = ostrcat(line, session, 1, 0);
				line = ostrcat(line, " ", 1, 0);
				line = ostrcat(line, _("Episode"), 1, 0);
				line = ostrcat(line, " ", 1, 0);
				line = ostrcat(line, episode, 1, 0);

//				line = ostrcat(line, "(s", 1, 0);
//				line = ostrcat(line, session, 1, 0);
//				line = ostrcat(line, "e", 1, 0);
//				line = ostrcat(line, episode, 1, 0);
//				line = ostrcat(line, ") ", 1, 0);
//				line = ostrcat(line, etitle, 1, 0);

				line = ostrcat(line, "#http://www.solarmovie.so", 1, 0);
				line = ostrcat(line, url, 1, 0);
//				line = ostrcat(line, ";", 1, 0);
//				line = ostrcat(line, session, 1, 0);
//				line = ostrcat(line, ";", 1, 0);
//				line = ostrcat(line, episode, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/s", 1, 0);
				line = ostrcat(line, session, 1, 0);
				line = ostrcat(line, "e", 1, 0);
				line = ostrcat(line, episode, 1, 0);
				line = ostrcat(line, ".jpg", 1, 0);																
				line = ostrcat(line, "#s", 1, 0);
				line = ostrcat(line, session, 1, 0);
				line = ostrcat(line, "e", 1, 0);
				line = ostrcat(line, episode, 1, 0);
				line = ostrcat(line, ".jpg#Solarmovie - ", 1, 0);
				line = ostrcat(line, title, 1, 0);
				line = ostrcat(line, "#28\n", 1, 0);
				free(session), session = NULL;
				free(episode), episode = NULL;
				free(url), url = NULL;	
				free(etitle), etitle = NULL;	
			}
		}
	}
	free(tmpstr), tmpstr = NULL;

	if(line != NULL)
	{
		if(series == 0)
			tmpstr = ostrcat("/tmp/tithek/solarmovie.hoster.list", NULL, 0, 0);
		else
			tmpstr = ostrcat("/tmp/tithek/solarmovie.hoster.ser.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/solarmovie8_line", NULL, line);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
end:
	free(tmphost), tmphost = NULL;
//	free(tmppath), tmppath = NULL;
		
	return ret;
}
#endif