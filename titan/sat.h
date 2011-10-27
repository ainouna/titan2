#ifndef SAT_H
#define SAT_H

void resetsatscan()
{
	struct sat *node = sat;

	while(node != NULL)
	{
		node->scan = 0;
		node = node->next;
	}
}

struct sat* getsatbyorbitalpos(int orbitalpos)
{
	debug(1000, "in");
	struct sat *node = sat;

	while(node != NULL)
	{
		if(node->orbitalpos == orbitalpos)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "sat not found (%d)", orbitalpos);
	return NULL;
}

char* getorbitalposstring(char* feshortname)
{
	int i = 0, orbitalpos = 0;
	struct sat* node = sat;
	char* str = NULL, *tmp = NULL, *tmpstr = NULL, *tmpnr = NULL;

	if(feshortname != NULL)
	{
		tmpstr = ostrcat(feshortname, "_sat", 0, 0);
		for(i = 1; i <= status.maxsat; i++)
		{
			tmpnr = oitoa(i);
			orbitalpos = getconfigint(tmpstr, tmpnr);
			free(tmpnr); tmpnr = NULL;

			node = getsatbyorbitalpos(orbitalpos);
			if(node != NULL)
			{
				tmp = oitoa(node->orbitalpos);
				str = ostrcat(str, tmp, 1, 1);
				str = ostrcat(str, "\n", 1, 0);
			}
		}
		free(tmpstr); tmpstr = NULL;

	}
	else
	{
		while(node != NULL)
		{
			if(node->fetype == FE_QPSK)
			{
				tmp = oitoa(node->orbitalpos);
				str = ostrcat(str, tmp, 1, 1);
				str = ostrcat(str, "\n", 1, 0);
			}
			node = node->next;
		}
	}

	return str;
}

char* getsatstring(char* feshortname)
{
	int i = 0, orbitalpos = 0;
	struct sat* node = sat;
	char* str = NULL, *tmpstr = NULL, *tmpnr = NULL;

	if(feshortname != NULL)
	{
		tmpstr = ostrcat(feshortname, "_sat", 0, 0);
		for(i = 1; i <= status.maxsat; i++)
		{
			tmpnr = oitoa(i);
			orbitalpos = getconfigint(tmpstr, tmpnr);
			free(tmpnr); tmpnr = NULL;

			node = getsatbyorbitalpos(orbitalpos);
			if(node != NULL)
			{
				str = ostrcat(str, node->name, 1, 0);
				str = ostrcat(str, "\n", 1, 0);
			}
		}
		free(tmpstr); tmpstr = NULL;

	}
	else
	{
		while(node != NULL)
		{
			if(node->fetype == FE_QPSK)
			{
				str = ostrcat(str, node->name, 1, 0);
				str = ostrcat(str, "\n", 1, 0);
			}
			node = node->next;
		}
	}

	return str;
}

struct sat* getlastsat(struct sat* node)
{
	debug(1000, "in");
	struct sat *prev = NULL;

	while(node != NULL || sat == NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

int movesatdown(struct sat* node)
{
	struct sat* prev = NULL, *next = NULL;

	if(node == NULL || sat == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//last node
	if(node->next == NULL)
	{
		if(node->prev != NULL)
			node->prev->next = NULL;
		node->prev = NULL;
		node->next = sat;
		sat->prev = node;
		sat = node;
		return 0;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		sat = node->next;
	node->next->prev = node->prev;

	//save nodes next and prev
	next = node->next;
	prev = node->prev;

	//haenge es eine pos nacher ein
	node->next = next->next;
	node->prev = next;
	
	if(next->next != NULL)
		next->next->prev = node;
	next->next = node;

	status.writesat = 1;
	return 0;
}

int movesatup(struct sat* node)
{
	struct sat* prev = NULL, *next = NULL, *last = NULL;

	if(node == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//first node
	if(node->prev == NULL)
	{
		last = getlastsat(sat);

		if(node->next != NULL)
			node->next->prev = NULL;
		sat = node->next;
		node->next = NULL;
		last->next = node;
		node->prev = last;
		return 0;
	}

	//haenge node aus 
	node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;

	//save nodes next and prev
	next = node->next;
	prev = node->prev;

	//haenge es eine pos voher ein
	node->next = prev;
	node->prev = prev->prev;
	
	if(prev->prev != NULL)
		prev->prev->next = node;
	else
		sat = node;
	prev->prev = node;

	status.writesat = 1;
	return 0;
}

struct sat* addsat(char *line, int count, struct sat* last)
{
	debug(1000, "in");
	struct sat *newnode = NULL, *prev = NULL, *node = sat;
	char *name = NULL;
	int ret = 0;

	newnode = (struct sat*)malloc(sizeof(struct sat));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	name = malloc(MINMALLOC);
	if(name == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct sat));
	status.writesat = 1;

	ret = sscanf(line, "%[^#]#%d#%d#%d", name, &newnode->flag, &newnode->orbitalpos, &newnode->fetype);
	if(ret != 4)
	{
		if(count > 0)
		{
			err("satlist line %d not ok", count);
		}
		else
		{
			err("add sat");
		}
		free(name);
		free(newnode);
		return NULL;
	}

	newnode->name = ostrcat(name, "", 1, 0);

	if(last == NULL)
	{
		while(node != NULL && strcoll(newnode->name, node->name) > 0)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		sat = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	debug(1000, "out");
	return newnode;
}

int readsat(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct sat* last = NULL, *tmplast = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addsat(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writesat = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

void delsat(char *name)
{
	debug(1000, "in");
	struct sat *node = sat, *prev = sat;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			status.writesat = 1;
			if(node == sat)
			{
				sat = node->next;
				if(sat != NULL)
					sat->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;	
			}

			free(node->name);
			node->name = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

struct sat* getsat(char *name)
{
	debug(1000, "in");
	struct sat *node = sat;

	while(node != NULL)
	{
		if(strstr(node->name, name) != NULL)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "sat not found (%s)", name);
	return NULL;
}

void freesat()
{
	debug(1000, "in");
	struct sat *node = sat, *prev = sat;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delsat(prev->name);
	}
	debug(1000, "out");
}

int writesat(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct sat *node = sat;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%s#%d#%d#%d\n", node->name, node->flag, node->orbitalpos, node->fetype);

		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	debug(1000, "out");
	return 0;
}

#endif
