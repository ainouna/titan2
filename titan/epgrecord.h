#ifndef EPGRECORD_H
#define EPGRECORD_H

void debugepgrecord(struct epgrecord* first)
{
	struct epgrecord* node = first;

	while(node != NULL)
	{
		printf("%ld - %ld\n", node->begin, node->end);
		node = node->next;
	}
}

struct epgrecord* addepgrecord(time_t epgbegin, time_t epgend, time_t begin, time_t end, struct epgrecord** first, struct epgrecord* last)
{
	//debug(1000, "in");
	struct epgrecord *newnode = NULL, *prev = NULL, *node = *first;

	newnode = (struct epgrecord*)malloc(sizeof(struct epgrecord));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct epgrecord));

	newnode->begin = begin;
	newnode->end = end;

	if(begin < epgbegin) begin = epgbegin;
	if(end > epgend) end = epgend;
	float p1 = (epgend - epgbegin) / 100;
	newnode->posx = (begin - epgbegin) / p1;
	newnode->size = (end - epgbegin) / p1;

	if(last == NULL)
	{
		while(node != NULL && newnode->begin > node->begin)
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
		*first = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	//debug(1000, "out");
	return newnode;
}

void delepgrecord(struct epgrecord* epgrecordnode, struct epgrecord** first)
{
	debug(1000, "in");
	struct epgrecord *node = *first, *prev = *first;

	while(node != NULL)
	{
		if(node == epgrecordnode)
		{
			if(node == *first)
			{
				*first = node->next;
				if(*first != NULL)
					(*first)->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

void freeepgrecord(struct epgrecord** first)
{
	debug(1000, "in");
	struct epgrecord *node = *first, *prev = *first;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delepgrecord(prev, first);
	}
	debug(1000, "out");
}

struct epgrecord* getepgrecord(struct channel* chnode, struct epg* epgnode)
{
	struct rectimer* rectimernode = rectimer;
	struct epgrecord* first = NULL;
	int recbegin = 0, recend = 0;

	int recforerun = getconfigint("recforerun", NULL) * 60;
	int recoverrun = getconfigint("recoverrun", NULL) * 60;

	while(rectimernode != NULL)
	{
		recbegin = rectimernode->begin - recforerun;
		recend = rectimernode->end + recoverrun;

		if(recbegin >= epgnode->endtime || recend <= epgnode->starttime || rectimernode->status == 2 || rectimernode->status == 3 || rectimernode->justplay != 0)
		{
			rectimernode = rectimernode->next;
			continue;
		}
		if(chnode == getchannel(rectimernode->serviceid, rectimernode->transponderid))
			addepgrecord(epgnode->starttime, epgnode->endtime, recbegin, recend, &first, NULL);

		rectimernode = rectimernode->next;
	}

	return first;
}

#endif
