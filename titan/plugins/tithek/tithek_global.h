#ifndef TITHEK_GLOBAL_H
#define TITHEK_GLOBAL

#include <curl/curl.h>

char* hoster(char* url)
{
	debug(99, "url: %s", url);
	char* streamurl = NULL, *tmplink = NULL;

	tmplink = ostrcat(url, NULL, 0, 0);
	string_tolower(tmplink);

	char* cmd = NULL;
	cmd = ostrcat("/tmp/localhoster/hoster.sh get \"", url, 0, 0);
	cmd = ostrcat(cmd, "\"", 1, 0);
	streamurl = command(cmd);
	streamurl = string_newline(streamurl);
	debug(99, "Streamurl1: %s", streamurl);

	free(cmd), cmd = NULL;

	if(streamurl == NULL)
	{
		if(ostrstr(tmplink, "sockshare") != NULL)
			streamurl = putlocker(url);
		else if(ostrstr(tmplink, "putlocker") != NULL)
			streamurl = firedrive(url);
		else if(ostrstr(tmplink, "filenuke") != NULL)
			streamurl = filenuke(url);
		else if(ostrstr(tmplink, "streamcloud") != NULL)
			streamurl = streamcloud(url);
		else if(ostrstr(tmplink, "vidstream") != NULL)
			streamurl = vidstream(url);
		else if(ostrstr(tmplink, "flashx_disable") != NULL)
			streamurl = flashx(url);
		else if(ostrstr(tmplink, "xvidstage") != NULL)
			streamurl = xvidstage(url);
		else if(ostrstr(tmplink, "nowvideo") != NULL)
			streamurl = nowvideo(url);
		else if(ostrstr(tmplink, "movshare") != NULL || ostrstr(tmplink, "wholecloud") != NULL)
			streamurl = movshare(url);
		else if(ostrstr(tmplink, "movreel") != NULL)
			streamurl = movreel(url);
		else if(ostrstr(tmplink, "novamov") != NULL || ostrstr(tmplink, "auroravid") != NULL)
			streamurl = novamov(url);
		else if(ostrstr(tmplink, "divxstage") != NULL || ostrstr(tmplink, "cloudtime") != NULL)
			streamurl = divxstage(url);
		else if(ostrstr(tmplink, "primeshare") != NULL)
			streamurl = primeshare(url);
		else if(ostrstr(tmplink, "faststream") != NULL || ostrstr(tmplink, "fastvideo") != NULL)
			streamurl = faststream(url);
		else if(ostrstr(tmplink, "played") != NULL)
			streamurl = played(url);
		else if(ostrstr(tmplink, "videoweed") != NULL)
			streamurl = videoweed(url);
		else if(ostrstr(tmplink, "firedrive") != NULL)
			streamurl = firedrive(url);
		else if(ostrstr(tmplink, "shared") != NULL)
			streamurl = shared(url);
		else if(ostrstr(tmplink, "thefile") != NULL)
			streamurl = thefile(url);
		else if(ostrstr(tmplink, "youtu_disable") != NULL)
			streamurl = youtube(url);
		else if(ostrstr(tmplink, "myvideo") != NULL)
			streamurl = myvideo(url);
		else if(ostrstr(tmplink, "promptfile") != NULL)
			streamurl = promptfile(url);
		else if(ostrstr(tmplink, "letwatch") != NULL || ostrstr(tmplink, "realvid") != NULL)
			streamurl = letwatch(url);
		else if(ostrstr(tmplink, "vidbull") != NULL)
			streamurl = vidbull(url);
		else if(ostrstr(tmplink, "vodlocker") != NULL)
			streamurl = vodlocker(url);
		else if(ostrstr(tmplink, "vidto") != NULL)
			streamurl = vidto(url);
		else if(ostrstr(tmplink, "amazon") != NULL)
			streamurl = amazon(url);
		else if(ostrstr(tmplink, "thevideo") != NULL)
			streamurl = thevideo(url);
		else if(ostrstr(tmplink, "mightyupload") != NULL)
			streamurl = mightyupload(url);
		else if(ostrstr(tmplink, "cloudzilla") != NULL)
			streamurl = cloudzilla(url);
		else if(ostrstr(tmplink, "vivo") != NULL)
			streamurl = vivo(url);
		else if(ostrstr(tmplink, "streamlive") != NULL)
			streamurl = streamlive(url, -1);
		else if(ostrstr(tmplink, "cricfree") != NULL)
			streamurl = cricfree(url, 0);
		else if(ostrstr(tmplink, "zerocast") != NULL)
			streamurl = zerocast(url, 0);
		else if(ostrstr(tmplink, "p2pcast") != NULL)
			streamurl = p2pcast(url);
		else if(ostrstr(tmplink, "vidzi") != NULL)
			streamurl = vidzi(url);
		else if(ostrstr(tmplink, "vid.ag") != NULL)
			streamurl = vidag(url);
		debug(99, "Streamurl2: %s", streamurl);
	}


	if(streamurl == NULL)
	{
		cmd = ostrcat("/tmp/localhoster/hoster.sh youtube_dl \"", url, 0, 0);
		cmd = ostrcat(cmd, "\"", 1, 0);
		streamurl = command(cmd);
		streamurl = string_newline(streamurl);
		debug(99, "Streamurl3: %s", streamurl);
	
		free(cmd), cmd = NULL;

		if(streamurl == NULL)
			textbox(_("Message"), _("The hoster is not yet supported !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);

	}

	debug(99, "Streamurl1: %s", streamurl);

	streamurl = string_replace_all("amp;", "", streamurl, 1);
	debug(99, "Streamurl4: %s", streamurl);

	free(tmplink), tmplink = NULL;
/*
	if(ostrncmp("http", streamurl, 4) && ostrncmp("rtmp", streamurl, 4) && ostrncmp("mms", streamurl, 3) && ostrncmp("rtsp", streamurl, 4))
	{
		printf("Streamurl3: not http*|rtmp*|mms*|rtsp* Streamurl: %s\n", streamurl); 
		free(streamurl),streamurl = NULL;
	}
*/
	return streamurl;
}

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

// flag = 0 (without header in output)
// flag = 1 (with header in output)
char* gethttps(char* url, char* localfile, char* data, char* user, char* pass, char* referer, int flag)
{
	debug(99, "url: %s", url);

	int debuglevel = getconfigint("debuglevel", NULL);

	char* tmpstr = NULL;
    FILE *fp;

	CURL *curl_handle;
	CURLcode res;
	
	struct MemoryStruct chunk;
	
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	chunk.size = 0;    /* no data at this point */
	
	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */ 
	curl_handle = curl_easy_init();
	if(curl_handle)
	{
	    if(localfile != NULL)
		    fp = fopen(localfile,"wb");
	       
		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);

		if(user != NULL && pass != NULL)
		{
			curl_easy_setopt(curl_handle, CURLOPT_USERNAME, user);
			curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, pass);
			curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		}
		if(data == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
		else
		{
			curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);

			/* example.com is redirected, so we tell libcurl to send POST on 301, 302 and
		     303 HTTP response codes */
			curl_easy_setopt(curl_handle, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
		}
		if(flag == 1)
			curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 3);
//		curl_easy_setopt(curl_handle, CURLOPT_RETURNTRANSFER, 1);

		/* send all data to this function  */
	    if(localfile == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		else
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, NULL);

		/* we pass our 'chunk' struct to the callback function */
	    if(localfile == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		else
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);

		/* some servers don't like requests that are made without a user-agent field, so we provide one */
//		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36");

		// This is occassionally required to stop CURL from verifying the peers certificate.
		// CURLOPT_SSL_VERIFYHOST may also need to be TRUE or FALSE if
		// CURLOPT_SSL_VERIFYPEER is disabled (it defaults to 2 - check the existence of a
		// common name and also verify that it matches the hostname provided)
#ifdef MIPSEL
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
#else
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
		if(debuglevel == 99)
			curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "/mnt/network/cookies");
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "/mnt/network/cookies");
		/* enable redirect following */
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
		/* allow three redirects */
		curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 3L);

		/* enable all supported built-in compressions */
		curl_easy_setopt(curl_handle, CURLOPT_ACCEPT_ENCODING, "");
  
		if(referer == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1L);
		else
			curl_easy_setopt(curl_handle, CURLOPT_REFERER, referer);

		/* get it! */
		res = curl_easy_perform(curl_handle);
		/* check for errors */
		if(res != CURLE_OK)
		{
			err("failed: %s", curl_easy_strerror(res));
			printf("curl error\n");
		}
		else
		{
			/*
			 * Now, our chunk.memory points to a memory block that is chunk.size
			 * bytes big and contains the remote file.
			 *
			 * Do something nice with it!
			 */
			printf("%lu bytes retrieved\n", (long)chunk.size);
		}
		
		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);
		if(localfile != NULL)
			fclose(fp);
	}

	tmpstr = ostrcat(chunk.memory, NULL, 0, 0);
  	free(chunk.memory);
  	/* we're done with libcurl, so clean it up */
   	curl_global_cleanup();

	if(localfile != NULL) 
		free(tmpstr), tmpstr = NULL;
	return tmpstr;
}

char* string_decode3(char* input)
{
	if(input == NULL) return input;

	input = string_replace_all("&#196;", "Ä", input, 1);
	input = string_replace_all("&#228;", "ä", input, 1);
	input = string_replace_all("&#201;", "É", input, 1);
	input = string_replace_all("&#233;", "é", input, 1);
	input = string_replace_all("&#214;", "Ö", input, 1);
	input = string_replace_all("&#246;", "ö", input, 1);
	input = string_replace_all("&#220;", "Ü", input, 1);
	input = string_replace_all("&#252;", "ü", input, 1);
	input = string_replace_all("&#223;", "ß", input, 1);
	input = string_replace_all("&#38;", "&", input, 1);

	input = string_replace_all("&Auml;", "Ä", input, 1);
	input = string_replace_all("&auml;", "ä", input, 1);
	input = string_replace_all("&Eacute;", "É", input, 1);
	input = string_replace_all("&eacute;", "é", input, 1);
	input = string_replace_all("&Ouml;", "Ö", input, 1);
	input = string_replace_all("&ouml;", "ö", input, 1);
	input = string_replace_all("&Uuml;", "Ü", input, 1);
	input = string_replace_all("&uuml;", "ü", input, 1);
	input = string_replace_all("&szlig;", "ß", input, 1);
	input = string_replace_all("&amp;", "&", input, 1);

	return input;
}

char* getfilekey(char* w, char* i, char* s, char* e)
{
	char* ret = NULL;

	if(w == NULL || i == NULL || s == NULL || e == NULL)
		return NULL;

	int a = 0, b = 0, c = 0;
	int a1 = 0, b1 = 0;

	int lw = strlen(w);
	int li = strlen(i);
	int ls = strlen(s);
	
	if(lw < 5 || li < 5 || ls < 5)
		return NULL;

	char ca[lw + li + ls - 14];
	char cb[16];

	ca[lw + li + ls - 15] = '\0';
	cb[15] = '\0';
	
	while(1)
	{
		if(a < 5)
		{
			cb[b1] = w[a];
			b1++;
		}
		else if(a < lw)
		{
			ca[a1] = w[a];
			a1++;
		}
		a++;
		
		if(b < 5)
		{
			cb[b1] = i[b];
			b1++;
		}
		else if(b < li)
		{
			ca[a1] = i[b];
			a1++;
		}
		b++;
		
		if(c < 5)
		{
			cb[b1] = s[c];
			b1++;
		}
		else if(c < ls)
		{
			ca[a1] = s[c];
			a1++;
		}
		c++;
		
		if(lw + li + ls == a1 + b1)
			break;
	}

	b = 0;
	int d = 0;
	char cc[a1 / 2 + 1];
	char casub[3] = {'\0'};
	cc[a1 / 2] = '\0';
	
	for(a = 0; a < a1; a += 2)
	{
		int c = -1;
		
		if(cb[b] % 2) c = 1;

		casub[0] = ca[a];
		casub[1] = ca[a + 1];

		cc[d] = strtol(casub, '\0', 36) - c;
		b++; d++;
		if(b >= b1) b = 0;
	}

	char* r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL;
	char* pos = ostrstr(cc, ");}('");
	if(pos != NULL)
	{
		r1 = string_resub(");}('", "'", pos, 0);
		pos = ostrstr(pos + 5, ",'");
		if(pos != NULL)
		{
			r2 = string_resub(",'", "'", pos, 0);
			pos = ostrstr(pos + 2, ",'");
			if(pos != NULL)
			{
				r3 = string_resub(",'", "'", pos, 0);
				pos = ostrstr(pos + 2, ",'");
				if(pos != NULL)
				{
					r4 = string_resub(",'", "'", pos, 0);
					ret = getfilekey(r1, r2, r3, r4);
				}
			}
		}
	}
	else
	{
		ret = string_resub("ll=\"", "\"", cc, 0);
		if(ret == NULL)
			ret = string_resub("filekey=\"", "\"", cc, 0);	
	}

  free(r1); r1 = NULL;
	free(r2); r2 = NULL;
	free(r3); r3 = NULL;
	free(r4); r4 = NULL;
					
	return ret;
}

int screenlistbox(struct skin* grid, struct skin* listbox,struct skin* countlabel, char* title, char* titheklink, int* pagecount, int* tithekexit, int* oaktpage, int* oaktline, int* ogridcol, int flag, int cflag)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;

	*oaktpage = listbox->aktpage;
	*oaktline = listbox->aktline;
	*ogridcol = listbox->gridcol;
	tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);

	if(title != NULL)
		tmpstr1 = ostrcat(title, " - ", 0, 0);
	else
		tmpstr1 = ostrcat(((struct tithek*)listbox->select->handle)->menutitle, " - ", 0, 0);	
	
	tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
	screentithekplay(tmpstr, tmpstr2, flag);
	free(tmpstr); tmpstr = NULL;
	free(tmpstr2); tmpstr2 = NULL;

	*pagecount = createtithekplay(titheklink, grid, listbox, countlabel, cflag);
//	if(pagecount == 0 || tithekexit == 1) break;
	if(*pagecount == 0 || *tithekexit == 1) return 0;
	
	listbox->aktpage = *oaktpage;
	listbox->aktline = *oaktline;
	listbox->gridcol = *ogridcol;
	addscreenrc(grid, listbox);

	return 1;
}

int all_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *line = NULL, *menu = NULL, *search = NULL;
	int ret = 1, count = 0, i = 0;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	if(searchstr == NULL)
		search = textinputhist(_("Search"), " ", "searchhist");
	else
		search = textinputhist(_("Search"), searchstr, "searchhist");

	if(search != NULL)
	{
		drawscreen(load, 0, 0);

		strstrip(search);
		string_tolower(search);

		tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/all/all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);

		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
			
				tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
				tmpstr1 = stringreplacecharonce(tmpstr1, '#', '\0');
				string_tolower(tmpstr1);

				if(ostrstr(tmpstr1, search) != NULL)
				{
					printf("found: %s\n", ret1[i].part);
					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;

					line = ostrcat(line, ret1[i].part, 1, 0);
					line = ostrcat(line, "\n", 0, 0);
				}
				free(tmpstr1), tmpstr1 = NULL;				
			}
			free(ret1), ret1 = NULL;

			if(line != NULL)
			{
				line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/all.search.list", NULL, 0, 0);
				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
				ret = 0;
			}
		}
		free(tmpstr), tmpstr = NULL;
	}
	free(search), search = NULL;
	return ret;
}

char* jsunpack(char* input)
{
	int debuglevel = getconfigint("debuglevel", NULL);

	if(input == NULL) return input;

	char* b36code = NULL, *search = NULL, *charlist = NULL, *base = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *tmpstr = NULL, *packed = NULL;

	unlink("/tmp/jsunpack1_packed");
	unlink("/tmp/jsunpack2_tmpstr");
	unlink("/tmp/jsunpack3_b36code");
	unlink("/tmp/jsunpack4_tmpstr_last");

//eval(function(p,a,c,k,e,d){while(c--)if(k[c])p=p.replace(new RegExp('\\b'+c.toString(a)+'\\b','g'),k[c]);return p}('1k.1j(\'<c 13="1i"1h="1g:1f-1e-1d-1c-1b"u="t"s="r"1a="3://h.9.g/f/19.18"><2 1="y"0="x"/><2 1="w"0="v"/><2 1="a"0="3://8.7.6.5/i/q/p.o"/><2 1="l"0="4"><2 1="m"0="4"/><2 1="k"0="j"/><2 1="n"0="4"/><2 1="12"0="3://8.7.6.5:11/d/10/b.z"/><e 13="17"16="b/9"12="3://8.7.6.5:11/d/10/b.z"y="x"w="v"u="t"s="r"a="3://8.7.6.5/i/q/p.o"n="4"m="4"a=""l="4"k="j"15="3://h.9.g/f/14/"></e></c>\');',36,57,'value|name|param|http|false|240|30|56|185|divx|previewImage|video|object||embed|plugin|com|go||Play|previewMessage|allowContextMenu|bannerEnabled|autoPlay|jpg|rezh7ogkvg6b|00004|380|height|715|width|transparent|wmode|Stage6|custommode|mkv|s4umuuk7gii5uaadrm5a76s7fd4bjlcti2r3v6bsa5d6yryt4dawmiqg|182|src|id|download|pluginspage|type|np_vid|cab|DivXBrowserPlugin|codebase|CC0F21721616|9C46|41fa|D0AB|67DABFBF|clsid|classid|ie_vid|write|document'.split('|')))

//eval(function(p,a,c,k,e,d){e=function(c){return c};if(!''.replace(/^/,String)){while(c--){d[c]=k[c]||c}k=[function(e){return d[e]}];e=function(){return'\\w+'};c=1};while(c--){if(k[c]){p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c])}}return p}('3.2(\'<0 4\'.1(\'8="\'+\'/5\'.1(\'/\'+7+\'">\\6/0>\')))',9,9,'script|concat|write|document|s|jwv|x3C|mpri_Key|rc'.split('|'),0,{}))

	while(ostrstr(input, "eval(function(p,a,c,k,e,d){") != NULL)
	{
		packed = string_resub("eval(function(p,a,c,k,e,d){", "}))", input, 0);
		if(packed == NULL)
			packed = string_resub("eval(function(p,a,c,k,e,d){", "))", input, 0);

		titheklog(debuglevel, "/tmp/jsunpack1_packed", NULL, NULL, NULL, packed);	
	
//		tmpstr = string_resub(";return p}('", ");'", packed, 0);
//		tmpstr = oregex(".*;return p}(.*)',[0-9]{2,2},[0-9]{2,2},'.*'.split.*", packed);
//		tmpstr = oregex(".*;return p}(.*)',[0-9]{1,3},[0-9]{1,3},'.*'.split.*", packed);
		tmpstr = oregex(".*return p}(.*)',[0-9]{1,3},[0-9]{1,3},'.*'.split.*", packed);

		debug(99, "tmpstr: %s", tmpstr);
		titheklog(debuglevel, "/tmp/jsunpack2_packed_tmpstr", NULL, NULL, NULL, tmpstr);
	
//		b36code = oregex(".*;',[0-9]{2,2},[0-9]{2,2},'(.*)'.split.*", packed);
		b36code = oregex(".*',[0-9]{1,3},[0-9]{1,3},'(.*)'.split.*", packed);
		
		b36code = string_replace_all("||", "| |", b36code, 1);
		debug(99, "b36code: %s", b36code);
		titheklog(debuglevel, "/tmp/jsunpack3_packed_b36code", NULL, NULL, NULL, b36code);

		if(!ostrncmp("|", b36code, 1))
			b36code = ostrcat(" ", b36code, 0, 1);

		struct splitstr* ret1 = NULL;
		int count = 0;
		int i = 0;
		ret1 = strsplit(b36code, "|", &count);
	
		charlist = ostrcat(charlist, "\"", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "'", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ".", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ";", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ":", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "=", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ",", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, " ", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "\\", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "/", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "(", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ")", 1, 0);
		charlist = ostrcat(charlist, "'", 1, 0);
		charlist = ostrcat(charlist, "%", 1, 0);
	
		for(i = 0; i < count; i++)
		{
			if(ostrstr((&ret1[i])->part, " ") != NULL)
			{
				printf("continue\n");
				continue;
			}
			char* x = oltostr(i, 36);
	
			struct splitstr* ret2 = NULL;
			int count2 = 0;
			int i2 = 0;
			tmpstr2 = ostrcat(charlist, NULL, 0, 0);
			ret2 = strsplit(tmpstr2, "|", &count2);
			for(i2 = 0; i2 < count2; i2++)
			{
				struct splitstr* ret3 = NULL;
				int count3 = 0;
				int i3 = 0;
				tmpstr3 = ostrcat(charlist, NULL, 0, 0);
				ret3 = strsplit(tmpstr3, "|", &count3);
				for(i3 = 0; i3 < count3; i3++)
				{
					debug(99, "-----------------------------------------------");
					debug(99, "replace %s%s%s <> %s%s%s",(&ret2[i2])->part, x, (&ret3[i3])->part, (&ret2[i2])->part, (&ret1[i])->part, (&ret3[i3])->part);
	
					base = ostrcat(base, (&ret2[i2])->part, 1, 0);
					base = ostrcat(base, x, 1, 0);
					base = ostrcat(base, (&ret3[i3])->part, 1, 0);		
					search = ostrcat(search, (&ret2[i2])->part, 1, 0);
					search = ostrcat(search, (&ret1[i])->part, 1, 0);
					search = ostrcat(search, (&ret3[i3])->part, 1, 0);
					tmpstr = string_replace_all(base, search, tmpstr, 1);
					free(base), base = NULL;
					free(search), search = NULL;
				}
				free(ret3), ret3 = NULL;
				free(tmpstr3), tmpstr3 = NULL;
			}
			free(ret2), ret2 = NULL;
			free(tmpstr2), tmpstr2 = NULL;
			free(x);
		}
		free(ret1), ret1 = NULL;
	
		titheklog(debuglevel, "/tmp/jsunpack4_packed_tmpstr_last", NULL, NULL, NULL, tmpstr);

		if(tmpstr == NULL)
			input = string_replace("eval(function(p,a,c,k,e,d){", "eval(function(p,a,c,k,e,d-extracted-error){", input, 1);
		else
		{
			input = string_replace("eval(function(p,a,c,k,e,d){", "eval(function(p,a,c,k,e,d-extracted){", input, 1);
			input = string_replace(packed, tmpstr, input, 1);
		}
		free(tmpstr),tmpstr = NULL;
		free(packed), packed = NULL;
		free(b36code), b36code = NULL;
		free(charlist), charlist = NULL;
	}

//eval(function(p,a,c,k,e,r){e=function(c){return c.toString(36)};if('0'.replace(0,e)==0){while(c--)r[e(c)]=k[c];k=[function(e){return r[e]||e}];e=function(){return'[2-9k-t]'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c]);return p}('k p(a){5 b="s=1",c="t=l";7(a&&a.8&&a.8.9>0)6(5 d=0;d<a.8.9;d++){5 e=a.8[d],f=["r","m"];6(5 g n f){5 h=f[g];7(e&&e[h])6(5 i=e[h],j=0;j<i.9;j++)i[j]&&i[j].2&&(i[j].2+=(-1==i[j].2.o("?")?"?":"&")+"4=3&"+b+"&"+c)}}q a}',[],30,'||file|false|direct|var|for|if|playlist|length|||||||||||function|g2wppbatyvegydjngpim4hp5rl6ammlhkneq3sdgfqh5woiym753qlis6o6737kceon73boksojlkppwz744cta5i2kpzwwdsh3hrjqq3eiwcysqmy6aerlaeszaptrregtvpdfsdaiy4idwqdlack4abotyacmth2a253x5lrvvul7dxs6kligge5dufn6eofjjxex7vzljmvrrv4dnb4xlbn3hvb67c2pgc7y|httpfallback|in|indexOf|jwConfig|return|sources|ua|vt'.split('|'),0,{}))

	while(ostrstr(input, "eval(function(p,a,c,k,e,r){") != NULL)
	{
		packed = string_resub("eval(function(p,a,c,k,e,r){", "))", input, 0);
		titheklog(debuglevel, "/tmp/jsunpack1_packed", NULL, NULL, NULL, packed);	
	
//		tmpstr = string_resub(";return p}('", ");'", packed, 0);
//		tmpstr = oregex(".*;return p}(.*)',[0-9]{2,2},[0-9]{2,2},'.*'.split.*", packed);
		tmpstr = oregex(".*;return p}(.*)',[0-9]{1,3},[0-9]{1,3},'.*'.split.*", packed);

		debug(99, "tmpstr: %s", tmpstr);
		titheklog(debuglevel, "/tmp/jsunpack2_packed_tmpstr", NULL, NULL, NULL, tmpstr);
	
//		b36code = oregex(".*;',[0-9]{2,2},[0-9]{2,2},'(.*)'.split.*", packed);
		b36code = oregex(".*',[0-9]{1,3},[0-9]{1,3},'(.*)'.split.*", packed);
		
		b36code = string_replace_all("||", "| |", b36code, 1);
		debug(99, "b36code: %s", b36code);
		titheklog(debuglevel, "/tmp/jsunpack3_packed_b36code", NULL, NULL, NULL, b36code);

		if(!ostrncmp("|", b36code, 1))
			b36code = ostrcat(" ", b36code, 0, 1);

		struct splitstr* ret1 = NULL;
		int count = 0;
		int i = 0;
		ret1 = strsplit(b36code, "|", &count);
	
		charlist = ostrcat(charlist, "\"", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "'", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ".", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ";", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ":", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "=", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ",", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, " ", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "\\", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "/", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, "(", 1, 0);
		charlist = ostrcat(charlist, "|", 1, 0);
		charlist = ostrcat(charlist, ")", 1, 0);
		charlist = ostrcat(charlist, "'", 1, 0);
		charlist = ostrcat(charlist, "%", 1, 0);
	
		for(i = 0; i < count; i++)
		{
			if(ostrstr((&ret1[i])->part, " ") != NULL)
			{
				printf("continue\n");
				continue;
			}
			char* x = oltostr(i, 36);
	
			struct splitstr* ret2 = NULL;
			int count2 = 0;
			int i2 = 0;
			tmpstr2 = ostrcat(charlist, NULL, 0, 0);
			ret2 = strsplit(tmpstr2, "|", &count2);
			for(i2 = 0; i2 < count2; i2++)
			{
				struct splitstr* ret3 = NULL;
				int count3 = 0;
				int i3 = 0;
				tmpstr3 = ostrcat(charlist, NULL, 0, 0);
				ret3 = strsplit(tmpstr3, "|", &count3);
				for(i3 = 0; i3 < count3; i3++)
				{
					debug(99, "-----------------------------------------------");
					debug(99, "replace %s%s%s <> %s%s%s",(&ret2[i2])->part, x, (&ret3[i3])->part, (&ret2[i2])->part, (&ret1[i])->part, (&ret3[i3])->part);
	
					base = ostrcat(base, (&ret2[i2])->part, 1, 0);
					base = ostrcat(base, x, 1, 0);
					base = ostrcat(base, (&ret3[i3])->part, 1, 0);		
					search = ostrcat(search, (&ret2[i2])->part, 1, 0);
					search = ostrcat(search, (&ret1[i])->part, 1, 0);
					search = ostrcat(search, (&ret3[i3])->part, 1, 0);
					tmpstr = string_replace_all(base, search, tmpstr, 1);
					free(base), base = NULL;
					free(search), search = NULL;
				}
				free(ret3), ret3 = NULL;
				free(tmpstr3), tmpstr3 = NULL;
			}
			free(ret2), ret2 = NULL;
			free(tmpstr2), tmpstr2 = NULL;
			free(x);
		}
		free(ret1), ret1 = NULL;
	
		titheklog(debuglevel, "/tmp/jsunpack4_packed_tmpstr_last", NULL, NULL, NULL, tmpstr);

		if(tmpstr == NULL)
			input = string_replace("eval(function(p,a,c,k,e,d){", "eval(function(p,a,c,k,e,d-extracted-error){", input, 1);
		else
		{
			input = string_replace("eval(function(p,a,c,k,e,d){", "eval(function(p,a,c,k,e,d-extracted){", input, 1);
			input = string_replace(packed, tmpstr, input, 1);
		}
		free(tmpstr),tmpstr = NULL;
		free(packed), packed = NULL;
		free(b36code), b36code = NULL;
		free(charlist), charlist = NULL;
	}

	return input;
}

void localparser_init(char* titheklink, char* tithekfile, int flag)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *cmd = NULL, *line = NULL, *path = NULL;
	struct skin* load = getscreen("loading");

	if(ostrcmp("http://atemio.dyndns.tv/mediathek/mainmenu.list", titheklink) == 0)
	{
		path = ostrcat("/tmp/localparser", NULL, 0, 0);
		drawscreen(load, 0, 0);

		char* titheklocalfile = ostrcat("/tmp/tithek/mainmenu.local.list", NULL, 0, 0);

		if(flag == 1)
		{
			if(!file_exist("/tmp/localhoster"))
				mkdir("/tmp/localhoster", 0777);
			if(!file_exist("/mnt/parser"))
				mkdir("/mnt/parser", 0777);
			if(!file_exist(path))
				mkdir(path, 0777);
	
			gethttp("atemio.dyndns.tv", "/mediathek/parser_free.tar", 80, "/tmp/parser.tar", HTTPAUTH, 5000, NULL, 0);
	
			cmd = ostrcat("tar -xvf /tmp/parser.tar -C ", path, 0, 0);
			system(cmd);
			free(cmd), cmd = NULL;
			unlink("/tmp/parser.tar");
	
			if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
			{
				gethttp("atemio.dyndns.tv", "/mediathek/parser_secret.tar", 80, "/tmp/parser.tar", HTTPAUTH, 5000, NULL, 0);
				cmd = ostrcat("tar -xvf /tmp/parser.tar -C ", path, 0, 0);
				system(cmd);
				free(cmd), cmd = NULL;
				unlink("/tmp/parser.tar");
			}
	
			gethttp("atemio.dyndns.tv", "/mediathek/hoster.tar", 80, "/tmp/hoster.tar", HTTPAUTH, 5000, NULL, 0);
			cmd = ostrcat("tar -xvf /tmp/hoster.tar -C ", "/tmp/localhoster", 0, 0);
			system(cmd);
			free(cmd), cmd = NULL;
			unlink("/tmp/hoster.tar");

			//dnode is freed in thread
			struct download* dnode = calloc(1, sizeof(struct download));
			if(dnode != NULL)
			{
				dnode->host = ostrcat("atemio.dyndns.tv", NULL, 0, 0);
				dnode->port = 80;
				dnode->filename = ostrcat("/tmp/python.tar", NULL, 0, 0);
				dnode->page = ostrcat("/mediathek/python.tar", NULL, 0, 0);	
					
				if(getfreespace("/mnt") / 1024 > 102400) //100mb
				{
					mkdir("/mnt/.tithek", 0777);
					if(file_exist("/mnt/.tithek"))
						dnode->filename = ostrcat("/mnt/.tithek/python.tar", NULL, 0, 0);
					dnode->page = ostrcat("/mediathek/python_full.tar", NULL, 0, 0);				
				}
				else if(file_exist("/media/hdd") && getfreespace("/media/hdd") / 1024 > 102400)
				{
					mkdir("/media/hdd/.tithek", 0777);
					if(file_exist("/media/hdd/.tithek"))
						dnode->filename = ostrcat("/media/hdd/.tithek/python.tar", NULL, 0, 0);
					dnode->page = ostrcat("/mediathek/python_full.tar", NULL, 0, 0);				
				}
				else if(file_exist("/var/swap") && getfreespace("/var/swap") / 1024 > 102400)
				{
					mkdir("/var/swap/.tithek", 0777);
					if(file_exist("/var/swap/.tithek"))
						dnode->filename = ostrcat("/var/swap/.tithek/python.tar", NULL, 0, 0);
					dnode->page = ostrcat("/mediathek/python_full.tar", NULL, 0, 0);				
				}
				else if(getfreespace("/tmp") / 1024 > 102400)
				{
					dnode->filename = ostrcat("/tmp/python.tar", NULL, 0, 0);
					dnode->page = ostrcat("/mediathek/python_full.tar", NULL, 0, 0);							
				}

				dnode->auth = ostrcat(HTTPAUTH, NULL, 0, 0);
				dnode->connfd = -1;
				dnode->ret = -1;
				dnode->timeout = 10000;
				addtimer(&tithekdownloadthread, START, 100, 1, (void*)dnode, NULL, NULL);
			}

			cmd = ostrcat("chmod -R 755 ", path, 0, 0);
			system(cmd);
			free(cmd), cmd = NULL;
	
			cmd = ostrcat("chmod -R 755 /mnt/parser", NULL, 0, 0);
			system(cmd);
			free(cmd), cmd = NULL;
	
			cmd = ostrcat("chmod -R 755 /tmp/localhoster", NULL, 0, 0);
			system(cmd);
			free(cmd), cmd = NULL;
//			}
	
			cmd = ostrcat("ls -1 ", path, 0, 0);
			cmd = ostrcat(cmd, "/*.sh", 1, 0);
	
			free(path), path = NULL;
			tmpstr1 = command("ls -1 /mnt/parser/*.sh");
			tmpstr2 = command(cmd);
			free(cmd), cmd = NULL;
			tmpstr = ostrcat(tmpstr1, "\n", 0, 0);
			tmpstr = ostrcat(tmpstr, tmpstr2, 1, 0);
	
			int count = 0, i = 0;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(tmpstr, "\n", &count);
		
			int max = count;
			for(i = 0; i < max; i++)
			{
				if(file_exist(ret1[i].part) && cmpfilenameext(ret1[i].part, ".sh") == 0)
				{
					cmd = ostrcat(ret1[i].part, " ", 0, 0);
					cmd = ostrcat(cmd, ret1[i].part, 1, 0);
					cmd = ostrcat(cmd, " init", 1, 0);
					line = command(cmd);
					debug(99, "add main menuentry: %s", line);
					writesys(titheklocalfile, line, 3);
					free(cmd), cmd = NULL;
					free(line), line = NULL;
				}
			}
			free(ret1), ret1 = NULL;
			free(tmpstr), tmpstr = NULL;
			free(tmpstr1), tmpstr1 = NULL;
			free(tmpstr2), tmpstr2 = NULL;
		}

		line = readfiletomem(titheklocalfile, 1);
		free(titheklocalfile), titheklocalfile = NULL;
		writesys(tithekfile, line, 3);

		clearscreen(load);
	}

}

char* localparser_hoster(char* link)
{
	debug(99, "link: %s", link);
	int ret = 1;
	char* tmpstr = NULL, *streamurl = NULL;

	tmpstr = command(link);
	tmpstr = string_newline(tmpstr);
	debug(99, "tmpstr: %s", tmpstr);

	if(ostrstr(tmpstr, "gethoster2") != NULL)
	{
		tmpstr = string_replace_all("gethoster2 ", "", tmpstr, 1);
		streamurl = hoster(tmpstr);
	}
	else if(ostrstr(link, ".sh play ") != NULL)
		streamurl = ostrcat(tmpstr, NULL, 0, 0);
	else if(ostrstr(link, ".sh hoster ") != NULL)
		streamurl = hoster(tmpstr);

	free(tmpstr), tmpstr = NULL;

	debug(99, "streamurl: %s", streamurl);

	return streamurl;
}

int localparser_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	char* tmpstr = NULL, *menu = NULL, *search = NULL, *cmd = NULL;
	int ret = 1;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	if(searchstr == NULL)
		search = textinputhist(_("Search"), " ", "searchhist");
	else
		search = textinputhist(_("Search"), searchstr, "searchhist");

	if(search != NULL)
	{
		drawscreen(load, 0, 0);

		strstrip(search);
		string_tolower(search);
		search = stringreplacechar(search, ' ', '+');

		cmd = ostrcat(link, NULL, 0, 0);

		if(ostrstr(cmd, "%search%") != NULL)
			cmd = string_replace_all("%search%", search, cmd, 1);
		else
			cmd = ostrcat(link, search, 0, 0);

		debug(99, "cmd: %s", cmd);
		char* filename = command(cmd);
		filename = string_newline(filename);
		tmpstr = readfiletomem(filename, 1);
		free(cmd), cmd = NULL;

		if(tmpstr != NULL)
		{
			tmpstr = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", tmpstr, 1);
			menu = ostrcat(filename, NULL, 0, 0);
//			writesys(menu, tmpstr, 0);
			struct tithek* tnode = (struct tithek*)listbox->select->handle;
			createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
			ret = 0;
		}

		free(filename), filename = NULL;
		free(tmpstr), tmpstr = NULL;
	}
	free(search), search = NULL;
	return ret;
}

#endif
