#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>


///////////////////////////
//ToDo: move to helper file
int compareDates(const void *first, const void *second){
	
	DateTime* tmp1;
	DateTime* tmp2;
	int ord;
	char *tmpStr1;
	char *tmpStr2;
	if (first == NULL || second == NULL){
		return 0;
	}
	tmp1 = (DateTime*)first;
	tmp2 = (DateTime*)second;

	tmpStr1 = printDate(tmp1);
	tmpStr2 = printDate(tmp2);

	ord = strcmp(tmpStr1, tmpStr2);
	free(tmpStr1);
	free(tmpStr2);
	return ord;
}


//We need to free the first and last names, and then the Name struct itself
void deleteDate(void *toBeDeleted){}


char* printDate(void *toBePrinted){
	char* tmpStr = malloc(100);
	tmpStr[0] = '\0';
	DateTime *tmpDate;
	if (toBePrinted == NULL){
		free(tmpStr);
		return NULL;
	}
	
	tmpDate = (DateTime*)toBePrinted;
		

	strcpy(tmpStr, tmpDate->date);
	strcat(tmpStr, "T");
	strcat(tmpStr, tmpDate->time);
	if(tmpDate->UTC){
		strcat(tmpStr, "Z");
	}
	strcat(tmpStr, "\r\n");

		
	return tmpStr;
}

int compareAlarms(const void *first, const void *second){
	Alarm* tmp1;
	Alarm* tmp2;
	int ord;
	char *tmpStr1;
	char *tmpStr2;
	if (first == NULL || second == NULL){
		return 0;
	}

	tmp1 = (Alarm*)first;
	tmp2 = (Alarm*)second;

	tmpStr1 = printAlarm(tmp1);
	tmpStr2 = printAlarm(tmp2);

	ord = strcmp(tmpStr1, tmpStr2);
	free(tmpStr1);
	free(tmpStr2);
	return ord;
}


//We need to free the first and last names, and then the Name struct itself
void deleteAlarm(void *toBeDeleted){
	Alarm* tmpAlm;
	
	if (toBeDeleted == NULL){
		return;
	}
	
	tmpAlm = (Alarm*)toBeDeleted;
	if(tmpAlm->trigger != NULL){
		free(tmpAlm->trigger);
	}
	if(tmpAlm->properties != NULL){
		freeList(tmpAlm->properties);
	}
	
	free(tmpAlm);
}


char* printAlarm(void *toBePrinted){
	Property *prop;
	char* str;
	int size = 400;
	char* tmpStr = malloc(400);
	tmpStr[0] = '\0';
	Alarm* tmpAlm;
	
	if (toBePrinted == NULL){
		return NULL;
	}
	
	tmpAlm = (Alarm*)toBePrinted;
		

	strcpy(tmpStr, "BEGIN:VALARM\r\nACTION:");
	strcat(tmpStr, tmpAlm->action);
	strcat(tmpStr, "\r\n");
	strcat(tmpStr, "TRIGGER:");
	strcat(tmpStr, tmpAlm->trigger);
	strcat(tmpStr, "\r\n");
	ListIterator iter = createIterator(tmpAlm->properties);
	while((prop = (Property*)nextElement(&iter) )!= NULL){
		str = printProperty(prop);
		size+=strlen(str);
		tmpStr = realloc(tmpStr, size);
		strcat(tmpStr, str);
		free(str);
	}
	strcat(tmpStr, "END:VALARM\r\n");
		
	return tmpStr;
}

int compareProperties(const void *first, const void *second){
	Property* tmp1;
	Property* tmp2;
	int ord;
	char *tmpStr1;
	char *tmpStr2;
	if (first == NULL || second == NULL){
		return 0;
	}

	tmp1 = (Property*)first;
	tmp2 = (Property*)second;

	tmpStr1 = printProperty(tmp1);
	tmpStr2 = printProperty(tmp2);

	ord = strcmp(tmpStr1, tmpStr2);
	free(tmpStr1);
	free(tmpStr2);
	return ord;
}


//We need to free the first and last names, and then the Name struct itself
void deleteProperty(void *toBeDeleted){
	
	Property* tmpProp;
	
	if (toBeDeleted == NULL){
		return;
	}
	
	tmpProp = (Property*)toBeDeleted;
	
	free(tmpProp);
}


char* printProperty(void *toBePrinted){
	int size = 2000;
	char* tmpStr = malloc(2000);
	tmpStr[0] = '\0';
	Property* tmpProp;
	if (toBePrinted == NULL){
		free(tmpStr);
		return NULL;
	}
	tmpProp = (Property*)toBePrinted;
		
	size+=strlen(tmpProp->propName);
	tmpStr = realloc(tmpStr, size);
	strcpy(tmpStr, tmpProp->propName);
	strcat(tmpStr, ":");
	size+=strlen(tmpProp->propDescr);
	tmpStr = realloc(tmpStr, size);
	strcat(tmpStr, tmpProp->propDescr);
	strcat(tmpStr, "\r\n");
	return tmpStr;
}


int compareEvents(const void *first, const void *second){
	Event* tmp1;
	Event* tmp2;
	int ord;
	char *tmpStr1;
	char *tmpStr2;
	if (first == NULL || second == NULL){
		return 0;
	}

	tmp1 = (Event*)first;
	tmp2 = (Event*)second;

	tmpStr1 = printEvent(tmp1);
	tmpStr2 = printEvent(tmp2);

	ord = strcmp(tmpStr1, tmpStr2);
	free(tmpStr1);
	free(tmpStr2);
	return ord;
}


void deleteEvent(void *toBeDeleted){
	Event* tmpEvt;
	if (toBeDeleted == NULL){
		return;
	}

	tmpEvt = (Event*)toBeDeleted;

	if(tmpEvt->properties != NULL){

		freeList(tmpEvt->properties);
	}

	if(tmpEvt->alarms != NULL){

		freeList(tmpEvt->alarms);

	}
	
	
	free(tmpEvt);
	tmpEvt = NULL;
}


char* printEvent(void *toBePrinted){
	Property *prop;
	Alarm *alm;
	char *str;
	char* tmpStr = malloc(400);
	int size = 400;
	tmpStr[0] = '\0';
	Event* tmpEvt;
	
	if (toBePrinted == NULL){
		free(tmpStr);
		return NULL;
	}
	
	tmpEvt = (Event*)toBePrinted;
	
	strcpy(tmpStr, "BEGIN:VEVENT\r\nDTSTAMP:");
	str = printDate(&(tmpEvt->creationDateTime));
	strcat(tmpStr, str);
	free(str);

	strcat(tmpStr, "DTSTART:");
	str = printDate(&(tmpEvt->startDateTime));
	strcat(tmpStr, str);
	free(str);

	strcat(tmpStr, "UID:");
	strcat(tmpStr, tmpEvt->UID);
	strcat(tmpStr, "\r\n");

	ListIterator iter = createIterator(tmpEvt->properties);
	while((prop = (Property*)nextElement(&iter) )!= NULL){
		str = printProperty(prop);
		size+=strlen(str);
		tmpStr = realloc(tmpStr, size);
		strcat(tmpStr, str);
		free(str);
	}

	iter = createIterator(tmpEvt->alarms);
	while((alm = (Alarm*)nextElement(&iter)) != NULL){
		str =  printAlarm(alm);
		size+=strlen(str);
		tmpStr = realloc(tmpStr, size);
		strcat(tmpStr, str);
		free(str);
	}
	strcat(tmpStr, "END:VEVENT\r\n");
	
	return tmpStr;
}
/////////////////////////

void raiseCase(char *word){
	for(int i = 0; word[i]!='\0'; i++){
		word[i] = toupper(word[i]);
	}
}

char *valuePuller(FILE **file){
	int valSize = 75;
	char buffer;
	char *val = malloc(sizeof(char) * valSize);
	for(int i = 0; i<valSize; i++){
		fread(&buffer, 1, 1, *file);
		if(buffer=='\r'||feof(*file)/*\r*/){
			fread(&buffer, 1, 1, *file);
			if(feof(*file)){
				val[i] = '\0';
				if(strlen(val) == 0){
					free(val);
					return NULL;
				}
				return val;
			}
			if(buffer == '\n'){
				fread(&buffer, 1, 1, *file);
				if(buffer == ' ' || buffer == '\t'){
					valSize +=75;
					val = realloc(val, sizeof(char)*valSize);
					fread(&buffer, 1, 1, *file);
					val[i] = buffer;
				}else{
					val[i] = '\0';
					fseek(*file, -1L, SEEK_CUR);
					if(strlen(val) == 0){
						free(val);
						return NULL;
					}
					return val;
				}
			}else{
				free(val);
				return NULL;
			}
			
		}else{
			val[i] = buffer;
		}
		if(valSize - 1 == i){
			valSize +=75;
			val = realloc(val, sizeof(char)*valSize);
		}
	}
	
	return val;
}

ICalErrorCode createProp(FILE **file, Property **prop, char *head){
	char *what = valuePuller(file);
	if(what == NULL){
		return OTHER_ERROR;
	}
	Property *tempProp = malloc(sizeof(Property)+strlen(what)*sizeof(char)+1);
	memcpy(tempProp->propName, head, 200);
	memcpy(tempProp->propDescr, what, strlen(what)+1);
	free(what);
	*prop = tempProp;
	return OK;
}

ICalErrorCode getDateTime(FILE **f, DateTime *dt){
	FILE *file = *f;
	char *what = valuePuller(&file);
	char *dtportion;
	
	if(what == NULL){
		return INV_DT;
	}

	dtportion = strtok(what, "T");
	if(dtportion == NULL || (strlen(dtportion)!=8)){
		free(what);
		return INV_DT;
	}
	memcpy(dt->date, dtportion, 9);

	dtportion = strtok(NULL, "T");
	if(dtportion == NULL || (strlen(dtportion)!=7 && strlen(dtportion) != 6)){
		free(what);
		return INV_DT;
	}
	memcpy(dt->time, dtportion, 7);

	dt->time[6] = '\0';
	if(dtportion[6] == 'Z'){
		dt->UTC = true;
	}else{
		dt->UTC = false;
	}
	
	free(what);
	*f = file;
	return OK;
}

ICalErrorCode createAlarm(FILE **f, Alarm **alm){
	*alm = malloc(sizeof(Alarm));
	(*alm)->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
	(*alm)->trigger = NULL;
	int got[2] = {0, 0};
	char *what = NULL;
	FILE *file = *f;
	char buffer;
	char head[200];
	int lineCount = 0;
	while(!feof(file)){
		for(int i = 0; i<200; i++){
			fread(&buffer, 1, 1, file);
			if(buffer==';'||buffer==':'||feof(file)){
				head[i] = '\0';
				break;

			}else if(buffer == '\r'){
				i--;
				fread(&buffer, 1, 1, file);
				if(buffer == '\n'){
					fread(&buffer, 1, 1, file);
					if(buffer != '\t' && buffer != ' '){
						fseek(file, -1L, SEEK_CUR);
						i++;
						head[i] = '\0';
						break;
						//May need to send back error here;
					}
				}else{
					i++;
					head[i] = buffer;

				}
				
			}else{
				head[i] = buffer;
			}
		}

		
		raiseCase(head);
		if(strcmp(head, "BEGIN") == 0){
			fclose(file);
			return INV_ALARM;
		}else if(strcmp(head, "END") == 0){
			what = valuePuller(&file);
			if(what == NULL){
				return INV_ALARM;
			}
			raiseCase(what);
			if(strcmp(what, "VALARM") == 0 && got[0] == 1 && got[1] == 1){
				*f = file;
				free(what);
				return OK;
			}else{
				*f = file;
				free(what);
				return INV_ALARM;
			}

		}else if(strcmp(head, "ACTION") == 0){
			what = valuePuller(&file);
			if(what != NULL){
				memcpy((*alm)->action, what, strlen(what)+1);
			}else{
				free(what);
				return INV_ALARM;
			}
			
			free(what);
			got[1] = 1;
		}else if(strcmp(head, "TRIGGER") == 0){
		
			what = valuePuller(&file);

			if(what != NULL&&strlen(what)!=0){
				(*alm)->trigger = malloc(strlen(what)+1);
				memcpy((*alm)->trigger, what, strlen(what)+1);
			}else{
				if(what != NULL){
					free(what);
				}

				return INV_ALARM;
			}
			
			free(what);
			got[0] = 1;
		}else if(buffer == ';' && strlen(head) == 0){
			//deals with comment lines
			what = valuePuller(&file);
			if(what != NULL){
				free(what);
			}
			
			lineCount--;
		}else{
			if(strlen(head) == 0){
				return INV_ALARM;
			}
			Property *prop;
			if(OK!=createProp(&file, &prop, head)){
				
				return INV_ALARM;
			}
			insertSorted((*alm)->properties, (void *)prop);
			
		}
		lineCount++;

	}
	*f = file;
	return INV_ALARM;
}


ICalErrorCode createEvent(FILE **f, Event **evt){
	*evt = malloc(sizeof(Event));
	(*evt)->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
	(*evt)->alarms = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);
	
	char *what = NULL;
	FILE *file = *f;
	char buffer;
	char head[200];
	int lineCount = 0;
	ICalErrorCode err = OK;
	int got[3] = {0, 0, 0};
	while(!feof(file)){
		for(int i = 0; i<200; i++){
			fread(&buffer, 1, 1, file);
			if(buffer==';'||buffer==':'||feof(file)){
				head[i] = '\0';
				break;

			}else if(buffer == '\r'){
				i--;
				fread(&buffer, 1, 1, file);
				if(buffer == '\n'){
					fread(&buffer, 1, 1, file);
					if(buffer != '\t' && buffer != ' '){
						fseek(file, -1L, SEEK_CUR);
						i++;
						head[i] = '\0';
						break;
						//May need to send back error here;
					}
				}else{
					i++;
					head[i] = buffer;

				}
				
			}else{
				head[i] = buffer;
			}
		}

		
		raiseCase(head);
		if(strcmp(head, "BEGIN") == 0){
			what = valuePuller(&file);
			if(what == NULL){
				return INV_EVENT;
			}
			raiseCase(what);

			if(strcmp(what, "VALARM") == 0){
				Alarm *alm = NULL;
				err = createAlarm(&file, &alm);
				free(what);
				if(err != OK){
					deleteAlarm(alm);
					return err;
				}
				insertSorted((*evt)->alarms, (void *)alm);
				
			}else{
				free(what);
				return INV_EVENT;
			}
			
		}else if(strcmp(head, "UID") == 0){
			
			what = valuePuller(&file);
			if(what == NULL){
				return INV_EVENT;
			}
			if(got[2] == 1){
				free(what);
				return INV_EVENT;
			}
			got[2] = 1;
			memcpy((*evt)->UID, what, strlen(what)+1);
			free(what);

		}else if(strcmp(head, "DTSTAMP") == 0){
			err = getDateTime(&file, &((*evt)->creationDateTime));
			
			if(err != OK || got[0] == 1){
				return err;
			}
			got[0] = 1;
		}else if(strcmp(head, "DTSTART") == 0){
			
			err = getDateTime(&file, &((*evt)->startDateTime));	
			if(err != OK || got[1] == 1){
				return err;
			}
			got[1] = 1;
		}else if(strcmp(head, "END") == 0){
			what = valuePuller(&file);
			if(what == NULL){
				return INV_EVENT;
			}
			raiseCase(what);

			if(strcmp(what, "VEVENT") == 0 && 
			got[0] == 1 && 
			got[1] == 1 && 
			got[2] == 1){
				*f = file;
				free(what);

				return OK;
			}else{
				free(what);
				return INV_EVENT;
			}
		}else if(buffer == ';' && strlen(head) == 0){
			//deals with comment lines
			what = valuePuller(&file);
			if(what != NULL){
				free(what);
			}
			
			lineCount--;
		}else{
			if(strlen(head) == 0){
				return INV_EVENT;
			}
			Property *prop;
			if(OK!=createProp(&file, &prop, head)){
				return INV_EVENT;
			}
			insertSorted((*evt)->properties, (void *)prop);
			
		}
		lineCount++;

	}
	*f = file;
	return INV_EVENT;
}

ICalErrorCode createCalendar(char *fileName, Calendar **obj){
	char *what = NULL;
	char buffer;
	char head[200];
	int lineCount = 0;
	int fnl;
	int size;
	int got[3] = {0,0,0};
	FILE *file;
	ICalErrorCode err = OK;
	//filename checking

	if(fileName == NULL){
		*obj = NULL;
		return INV_FILE;
	}

	fnl = strlen(fileName);

	if(strcmp(fileName, "") == 0 ||
	fnl<4||
	strcmp(&fileName[fnl-4], ".ics") != 0||
	(file = fopen(fileName, "r")) == NULL){//attempts to open file to read in
		*obj = NULL;
		return INV_FILE;
	}


	while(!feof(file)){
		fread(&buffer, 1, 1, file);
		if(buffer == '\r'){
			fread(&buffer, 1, 1, file);
			if(buffer != '\n'){
				got[1] = -1;
			}
		}
		
	}

    size = ftell(file);
    if(size == 0 || got[1] == -1){
        fclose(file);
        *obj = NULL;
        return INV_FILE;
    }
    rewind(file);

	*obj = malloc(sizeof(Calendar));
	(*obj)->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
	(*obj)->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
	(*obj)->version = -1;
	(*obj)->prodID[0] = '\0';
	while(!feof(file)){
		for(int i = 0; i<200; i++){
			fread(&buffer, 1, 1, file);
			if(buffer==';'||buffer==':'||feof(file)){
				head[i] = '\0';
				break;

			}else if(buffer == '\r'){
				i--;
				fread(&buffer, 1, 1, file);
				if(buffer == '\n'){
					fread(&buffer, 1, 1, file);
					if(buffer != '\t' && buffer != ' '){
						fseek(file, -1L, SEEK_CUR);
						i++;
						head[i] = '\0';
						break;
						//May need to send back error here;
					}
				}else{
					i++;
					head[i] = buffer;

				}
				
			}else{
				head[i] = buffer;
			}
		}


		raiseCase(head);
		if(lineCount == 0 && strcmp(head, "BEGIN") != 0){
			deleteCalendar(*obj);
			fclose(file);
			*obj = NULL;
			return INV_CAL;
		}
		if(strcmp(head, "BEGIN") == 0){
			what = valuePuller(&file);
			if(what == NULL){
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return INV_CAL;
			}
			raiseCase(what);

			if(strcmp(what, "VCALENDAR") == 0){
				free(what);
				if (lineCount!=0){
					deleteCalendar(*obj);
					fclose(file);
					*obj = NULL;
					return INV_CAL;
				}
			}else if(strcmp(what, "VEVENT") == 0){
				got[0] = 1;
				Event *evt = NULL;
				err = createEvent(&file, &evt);
				free(what);
				if(err != OK){
					deleteEvent(evt);
					deleteCalendar(*obj);
					fclose(file);
					*obj = NULL;
					return err;
				}
				insertSorted((*obj)->events, (void *)evt);
				
			}else{
				free(what);
				fclose(file);
				deleteCalendar(*obj);
				*obj = NULL;
				return INV_CAL;
			}
		}else if(strcmp(head, "VERSION") == 0){
			if((*obj)->version != -1){
				deleteCalendar(*obj);
				fclose(file);
				*obj = NULL;
				return DUP_VER;
			}
			what = valuePuller(&file);
			if(what == NULL){
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return INV_VER;
			}
			if(atof(what) > 0){
				(*obj)->version = atof(what);
			}else{
				free(what);
				deleteCalendar(*obj);
				fclose(file);
				*obj = NULL;
				return INV_VER;
			}
			free(what);
			got[1] = 1;
		}else if(strcmp(head, "PRODID") == 0){
			if(strcmp((*obj)->prodID, "") != 0){
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return DUP_PRODID;
			}

			what = valuePuller(&file);
			if(what == NULL){
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return INV_PRODID;
			}

			if(strlen(what)>0){
				memcpy((*obj)->prodID, what, strlen(what)+1);
				free(what);
			}else{
				free(what);
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return INV_PRODID;
			}
			got[2] = 1;
		}else if(strcmp(head, "END") == 0){
			what = valuePuller(&file);
			if(what == NULL){
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return INV_CAL;
			}
			raiseCase(what);
			fclose(file);
			if(strcmp(what, "VCALENDAR") == 0 && 
			got[0] == 1 &&
			got[1] == 1 &&
			got[2] == 1){
				free(what);
				return OK;
			}else{
				free(what);
				deleteCalendar(*obj);
				*obj = NULL;
				return INV_CAL;
			}
		}else if(buffer == ';' && strlen(head) == 0){
			//deals with comment lines
			what = valuePuller(&file);
			if(what != NULL){
				free(what);
			}
			
			lineCount--;
		
		}else{
			if(strlen(head) == 0){
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return INV_CAL;
			}
			Property *prop;
			if(OK!=createProp(&file, &prop, head)){
				deleteCalendar(*obj);
				*obj = NULL;
				fclose(file);
				return INV_CAL;
			}
			insertSorted((*obj)->properties, (void *)prop);
			//add the property to the linked list inside obj;
		}
		
		lineCount++;

	}

	fclose(file);
	deleteCalendar(*obj);
	*obj = NULL;
	return INV_CAL;
}

void deleteCalendar(Calendar* obj){

	if(obj == NULL){
		return;
	}
	if(obj->properties != NULL){
		freeList(obj->properties);	
	}
	if(obj->events != NULL){
		freeList(obj->events);	
	}
	free(obj);
}

char* printError(ICalErrorCode err){
	char *errStrRet = malloc(100);
	char *errStr;
	switch(err){
		case OK:
			errStr = "Success";
			break;
		case INV_FILE:
			errStr = "Invalid file";
			break;
		case INV_CAL:
			errStr = "Invalid calendar";
			break;
		case INV_PRODID:
			errStr = "Invalid product ID";
			break;
		case INV_EVENT:
			errStr = "Invalid event";
			break;
		case INV_ALARM:
			errStr = "Invalid alarm.";
			break;
		case INV_DT:
			errStr = "Invalid DateTime format.";
			break;
		case INV_VER:
			errStr = "Invalid version number.";
			break;
		case DUP_PRODID:
			errStr = "Duplicate product ID.";
			break;
		case DUP_VER:
			errStr = "Duplicate version";
			break;
		case WRITE_ERROR:
			errStr = "Error while writing to file";
			break;
		default:
			errStr = "Other unidentified error";
			break;
	}
	strcpy(errStrRet, errStr);
	return errStrRet;
}

//MOVE to separtae file

ICalErrorCode validateDateTime(const DateTime obj){
	if(strlen(obj.date) != 8){
		return INV_DT;
	}

	if(strlen(obj.time) != 6){
		return INV_DT;
	}
	
	return OK;
}

ICalErrorCode validateProperty(const Property* obj){
	if(obj == NULL){
		return OTHER_ERROR;
	}

	if(strlen(obj->propName) == 0 || strlen(obj->propName) > 199){
		return OTHER_ERROR;
	}

	if(strlen(obj->propDescr) == 0){
		return OTHER_ERROR;
	}
	

	return OK;
}

ICalErrorCode validateAlarm(const Alarm* obj){
	Property *prop;
	ICalErrorCode err = OK;
	int duration = 0, repeat = 0, attach = 0;
	if(obj == NULL){
		return INV_ALARM;
	}

	if(strlen(obj->action) == 0 || strlen(obj->action) > 199){
		return INV_ALARM;
	}

	if(obj->trigger == NULL ||strlen(obj->trigger) == 0){
		return INV_ALARM;
	}

	if(obj->properties == NULL){
		return INV_ALARM;
	}

	ListIterator iter = createIterator(obj->properties);
	while((prop = (Property*)nextElement(&iter)) != NULL){
		err = validateProperty(prop);
		if(err != OK){
			return INV_ALARM;//Since there is no invalid prop code, we return the invalid container of the prop
		}

		char *name = malloc(sizeof(char)*200);
		memcpy(name, prop->propName, 200);
		raiseCase(name);
		if(strcmp("DURATION", name) == 0){
			duration++;
		}else if(strcmp("REPEAT", name) == 0){
			repeat++;
		}else if(strcmp("ATTACH", name) == 0){
			attach++;
		}else{
			free(name);
			return INV_ALARM;
		}
		free(name);
	}

	if(duration > 1 || repeat > 1){
		return INV_ALARM;
	}

	if((duration == 1 && repeat == 0)||(duration == 0 && repeat == 1)){
		return INV_ALARM;
	}

	return OK;
}

ICalErrorCode validateEvent(const Event* obj){
	Property *prop;
	Alarm *alm;
	

	ICalErrorCode err = OK;
	if(obj == NULL){
		return INV_EVENT;
	}

	if(strlen(obj->UID) == 0 || strlen(obj->UID) > 199){
		return INV_EVENT;
	}

	err = validateDateTime(obj->creationDateTime);
	if(err != OK){
		return INV_EVENT;
	}

	err = validateDateTime(obj->startDateTime);
	if(err != OK){
		return INV_EVENT;
	}

	if(obj->alarms == NULL){
		return INV_EVENT;
	}
	

	ListIterator iter = createIterator(obj->alarms);
	while((alm = (Alarm*)nextElement(&iter)) != NULL){
		err = validateAlarm(alm);
		if(err != OK){
			return err;
		}
	}
	
	if(obj->properties == NULL){
		return INV_EVENT;
	}

	int class = 0, created =0, description = 0, geo = 0, 
	lastmod = 0, location = 0, organizer = 0, priority = 0, 
	seq = 0, status = 0, summary = 0, transp = 0, url = 0,
	recurid = 0, rrule = 0, attach = 0, attendee = 0, 
	catagories = 0, comment = 0, exdate = 0,
	contact = 0, related = 0, resources = 0, rdate = 0;

	int dtend = 0, duration = 0;

	iter = createIterator(obj->properties);
	while((prop = (Property*)nextElement(&iter)) != NULL){
		err = validateProperty(prop);
		if(err != OK){
			return INV_EVENT;//Since there is no invalid prop code, we return the invalid container of the prop
		}

		char *name = malloc(sizeof(char)*200);
		memcpy(name, prop->propName, 200);
		raiseCase(name);

		//printf("%s\n", name);
		if(strcmp("CLASS", name) == 0){
			class++;
		}else if(strcmp("CREATED", name) == 0){
			created++;
		}else if(strcmp("DESCRIPTION", name) == 0){
			description++;
		}else if(strcmp("GEO", name) == 0){
			geo++;
		}else if(strcmp("LAST-MODIFIED", name) == 0){
			lastmod++;
		}else if(strcmp("LOCATION", name) == 0){
			location++;
		}else if(strcmp("ORGANIZER", name) == 0){
			organizer++;
		}else if(strcmp("PRIORITY", name) == 0){
			priority++;
		}else if(strcmp("SEQUENCE", name) == 0){
			seq++;
		}else if(strcmp("STATUS", name) == 0){
			status++;
		}else if(strcmp("SUMMARY", name) == 0){
			summary++;
		}else if(strcmp("TRANSP", name) == 0){
			transp++;
		}else if(strcmp("URL", name) == 0){
			url++;
		}else if(strcmp("RECURRENCE-ID", name) == 0){
			recurid++;
		}else if(strcmp("ATTACH", name) == 0){
			attach++;
		}else if(strcmp("ATTENDEE", name) == 0){
			attendee++;
		}else if(strcmp("CATEGORIES", name) == 0){
			catagories++;
		}else if(strcmp("COMMENT", name) == 0){
			comment++;
		}else if(strcmp("CONTACT", name) == 0){
			contact++;
		}else if(strcmp("EXDATE", name) == 0){
			exdate++;
		}else if(strcmp("RELATED-TO", name) == 0){
			related++;
		}else if(strcmp("RESOURCES", name) == 0){
			resources++;
		}else if(strcmp("RDATE", name) == 0){
			rdate++;
		}else if(strcmp("RRULE", name) == 0){
			rrule++;
		}else if(strcmp("DTEND", name) == 0){
			dtend++;
		}else if(strcmp("DURATION", name) == 0){
			duration++;
		}else{
			free(name);
			return INV_EVENT;
		}
		free(name);
		
	}

	if(class > 1 || created > 1 || description > 1 || geo > 1 || lastmod > 1 ||
	location > 1 || organizer > 1 || priority > 1 || seq > 1 || status > 1 ||
	summary > 1 || transp > 1 || url > 1 || recurid > 1){
		return INV_EVENT;
	}

	if(dtend > 1 || duration > 1){
		return INV_EVENT;
	}

	if(dtend == 1 && duration == 1){
		return INV_EVENT;
	}
	
	return OK;
}

ICalErrorCode validateCalendar(const Calendar* obj){
	Property *prop;
	Event *evt;
	if(obj == NULL){
		return INV_CAL;
	}

	if(obj->version <= 0){
		return INV_CAL;
	}

	if(strlen(obj->prodID) == 0 || strlen(obj->prodID) > 999){
		return INV_CAL;
	}

	if(obj->events == NULL || obj->events->length <= 0){
		return INV_CAL;
	}
	
	ListIterator iter = createIterator(obj->events);
	ICalErrorCode err = OK;
	while((evt = (Event*)nextElement(&iter)) != NULL){
		err = validateEvent(evt);
		if(err != OK){
			return err;
		}
	}
	
	if(obj->properties == NULL){
		return INV_CAL;
	}

	int calscale = 0, method = 0;

	iter = createIterator(obj->properties);
	while((prop = (Property*)nextElement(&iter)) != NULL){
		err = validateProperty(prop);
		if(err != OK){
			return INV_CAL;//Since there is no invalid prop code, we return the invalid container of the prop
		}
		char *name = malloc(sizeof(char)*200);
		memcpy(name, prop->propName, 200);
		raiseCase(name);
		if(strcmp("CALSCALE", name) == 0){
			calscale++;
		}else if(strcmp("METHOD", name) == 0){
			method++;
		}else{
			free(name);
			return INV_CAL;
		}
		free(name);
	}

	if(calscale > 1 || method > 1){
		return INV_CAL;
	}
	return OK;
}

//MOVE TO Separate file

char* printCalendar(const Calendar* obj){
	Property *prop;
	Event *evt;
	char* str;
	char* tmpStr = malloc(1400);
	int size = 1400;
	tmpStr[0] = '\0';

	
	if (obj == NULL){
		return NULL;
	}
		

	char buf[100]; 
  
    sprintf(buf, "%g", obj->version);
	strcpy(tmpStr, "BEGIN:VCALENDAR\r\nVERSION:");
	strcat(tmpStr, buf);
	strcat(tmpStr, "\r\nPRODID:");
	strcat(tmpStr, obj->prodID);
	strcat(tmpStr, "\r\n");

	ListIterator iter = createIterator(obj->events);
	while((evt = (Event*)nextElement(&iter)) != NULL){
		str = printEvent(evt);
		size+=strlen(str);
		tmpStr = realloc(tmpStr, size);
		strcat(tmpStr, str);
		free(str);
	}


	iter = createIterator(obj->properties);
	while((prop = (Property*)nextElement(&iter)) != NULL){
		str = printProperty(prop);
		size+=strlen(str);
		tmpStr = realloc(tmpStr, size);
		strcat(tmpStr, str);
		free(str);
	}
	

	
	strcat(tmpStr, "END:VCALENDAR\r\n");
	
	return tmpStr;
}


ICalErrorCode writeCalendar(char* fileName, const Calendar* obj){
	//Make sure this is the write error codes
	if(!obj){
		return WRITE_ERROR;
	}
	if(!fileName){
		return WRITE_ERROR;
	}
	FILE *file = fopen(fileName, "wb");
	char *cal = printCalendar(obj);
	fwrite(cal, strlen(cal), 1, file);
	fclose(file);
	free(cal);
	return OK;
}


///JSON stuff. move to own file
char* dtToJSON(DateTime prop){
	char *str = malloc(sizeof(char)*300);
	str[0] = '\0';
	strcat(str, "{\"date\":\"");
	strcat(str, prop.date);
	strcat(str, "\",\"time\":\"");
	strcat(str, prop.time);
	strcat(str, "\",\"isUTC\":");
	strcat(str, prop.UTC?"true":"false");
	strcat(str, "}");

	return str;
}

char* eventToJSON(const Event* event){
	char *str = calloc(150, sizeof(char));
	int len = 150;
	if(event == NULL){
		strcat(str, "{}");
		return str;
	}
	char *sum;
	
	char buf[100];
	Property *prop;
	str[0] = '\0';
	strcat(str, "{\"startDT\":");
	sum = dtToJSON(event->startDateTime);
	strcat(str, sum);
	free(sum);
	strcat(str, ",\"numProps\":");
	sprintf(buf, "%d", event->properties->length+3);
	strcat(str, buf);
	strcat(str, ",\"numAlarms\":");
	sprintf(buf, "%d", event->alarms->length);
	strcat(str, buf);
	strcat(str, ",\"summary\":\"");

	sum = "";
	ListIterator iter = createIterator(event->properties);
	while((prop = (Property*)nextElement(&iter)) != NULL){
		if(strcmp(prop->propName,"SUMMARY") == 0){
			str = realloc(str, sizeof(char)*(strlen(prop->propDescr)+strlen(str)));
			len = strlen(prop->propDescr)+strlen(str);
			sum = prop->propDescr;
		}
	}
	str = realloc(str, sizeof(char)*(len+25));
	strcat(str, sum);
	strcat(str, "\"");

	strcat(str, ",\"organizer\":\"");

	sum = "";
	iter = createIterator(event->properties);
	while((prop = (Property*)nextElement(&iter)) != NULL){
		if(strcmp(prop->propName,"ORGANIZER") == 0){
			str = realloc(str, sizeof(char)*(strlen(prop->propDescr)+strlen(str)));
			len = strlen(prop->propDescr)+strlen(str);
			sum = prop->propDescr;
		}
	}
	str = realloc(str, sizeof(char)*(len+25));
	strcat(str, sum);
	strcat(str, "\"");

	strcat(str, ",\"location\":\"");

	sum = "";
	iter = createIterator(event->properties);
	while((prop = (Property*)nextElement(&iter)) != NULL){
		if(strcmp(prop->propName,"LOCATION") == 0){
			str = realloc(str, sizeof(char)*(strlen(prop->propDescr)+strlen(str)));
			len = strlen(prop->propDescr)+strlen(str);
			sum = prop->propDescr;
		}
	}
	str = realloc(str, len*(strlen(str)+25));
	strcat(str, sum);

	strcat(str, "\"}");
	//printf("%s\n", str);
	return str;
}


char* eventListToJSON(const List* eventList){
	char *str = malloc(sizeof(char)*500);
	char *jvt;
	Event *evt;
	str[0] = '\0';
	if(eventList == NULL){
		strcat(str, "[]");
		return str;
	}
	ListIterator iter = createIterator((List *)eventList);

	strcat(str, "[");

	while((evt = (Event*)nextElement(&iter)) != NULL){
		jvt = eventToJSON(evt);
		str = realloc(str, sizeof(char)*(strlen(jvt)+strlen(str)+2));
		strcat(str, jvt);
		strcat(str, ",");
		printf("length: %ld", strlen(str));
		free(jvt);
	}
	str[strlen(str)-1] = '\0';
	strcat(str, "]");
	return str;
}

char* calendarToJSON(const Calendar* cal){
	char *str = malloc(sizeof(char)*300);

	if(cal == NULL){
		strcat(str, "{}");
		return str;
	}

	char buf[100];
	str[0] = '\0';
	strcat(str, "{\"version\":");
	sprintf(buf, "%d", (int)cal->version);
	strcat(str, buf);
	strcat(str, ",\"prodID\":\"");
	strcat(str, cal->prodID);
	strcat(str, "\",\"numProps\":");
	sprintf(buf, "%d", cal->properties->length+2);
	strcat(str, buf);
	strcat(str, ",\"numEvents\":");
	sprintf(buf, "%d", cal->events->length);
	strcat(str, buf);
	strcat(str, "}");

	return str;
}

DateTime JSONtoDT(char* str){
	DateTime dt;
	char buf[1000] = "";
	int count = 0;
	int holder;

	while(str[count] != ':'){
		count++;
	}
	count+=2;

	holder = count;
	while(str[count] != '\"'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '\0';
	memcpy(dt.date, buf, sizeof(char)*9);

	while(str[count] != ':'){
		count++;
	}
	count+=2;

	holder = count;
	while(str[count] != '\"'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '\0';
	memcpy(dt.time, buf, sizeof(char)*7);
	while(str[count] != ':'){
		count++;
	}
	count+=1;

	holder = count;
	if(str[count] == 't'){
		dt.UTC = true;
	}else{
		dt.UTC = false;
	}

	return dt;

}

Event* JSONtoEvent(const char* str){
	char buf[1000] = "";
	int count = 0;
	int holder;
	if(str == NULL){
		return NULL;
	}
	Event *evt = malloc(sizeof(Event));
	evt->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
	evt->alarms = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);

	while(str[count] != ':'){
		count++;
	}
	count+=2;

	holder = count;
	while(str[count] != '\"'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '\0';
	printf("uid: %s\n", buf);
	memcpy(evt->UID, buf, sizeof(char)*1000);

	while(str[count] != ':'){
		count++;
	}
	count+=1;

	holder = count;
	while(str[count] != '}'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '}';
	buf[count+1-holder] = '\0';
	evt->startDateTime = JSONtoDT(buf);

	while(str[count] != ':'){
		count++;
	}
	count+=1;

	holder = count;
	while(str[count] != '}'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '}';
	buf[count+1-holder] = '\0';

	evt->creationDateTime = JSONtoDT(buf);

	while(str[count] != ':'){
		count++;
	}
	count+=2;

	holder = count;
	while(str[count] != '\"'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '\0';

	Property *tempProp = malloc(sizeof(Property)+strlen(buf)*sizeof(char)+1);
	memcpy(tempProp->propName, "SUMMARY", 200);
	memcpy(tempProp->propDescr, buf, strlen(buf)+1);
	
	insertSorted(evt->properties, tempProp);

	return evt;
}

Calendar* JSONtoCalendar(const char* str){
	char buf[1000] = "";
	int count = 0;
	int holder;
	if(str == NULL){
		return NULL;
	}
	Calendar *cal = malloc(sizeof(Calendar));
	cal->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
	cal->events = initializeList(&printEvent, &deleteEvent, &compareEvents);
	
	while(str[count] != ':'){
		count++;
	}
	count++;
	holder = count;
	while(str[count] != ','){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '\0';

	cal->version = atof(buf);
	count++;

	while(str[count] != ':'){
		count++;
	}
	count+=2;

	holder = count;
	while(str[count] != '\"'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '\0';
	memcpy(cal->prodID, buf, sizeof(char)*1000);

	while(str[count] != ':'){
		count++;
	}
	count+=2;

	holder = count;
	while(str[count] != '}'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count - holder] = str[count];
	count++;
	while(str[count] != '}'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count - holder] = str[count];
	count++;
	while(str[count] != '}'){
		buf[count - holder] = str[count];
		count++;
	}
	buf[count-holder] = '}';
	buf[count+1-holder] = '\0';
	addEvent(cal, JSONtoEvent(buf));
	return cal;
}

void addEvent(Calendar* cal, Event* toBeAdded){
	if(cal == NULL || toBeAdded == NULL){

		return;
	}
	insertBack(cal->events, toBeAdded);//check if you can switch this to insert sorted
}
///


char* createCalJSONWrap(char *fileName){
	Calendar *cal;
	createCalendar(fileName, &cal);
	ICalErrorCode err;
	if((err = validateCalendar(cal))!=OK){
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		deleteCalendar(cal);
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,printError(err));
		strcat(errStr,"\"}");
		return errStr;
	}
	return calendarToJSON(cal);
}

char* createEvtListJSONWrap(char *fileName){
	Calendar *cal;
	createCalendar(fileName, &cal);
	printf("filename: %s\n", fileName);
	ICalErrorCode err;
	if((err = validateCalendar(cal))!=OK){
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		deleteCalendar(cal);
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,printError(err));
		strcat(errStr,"\"}");
		return errStr;
	}
	return eventListToJSON(cal->events);
}

char* addEventWrap(char *fileName, char* evtJSON){
	Calendar *cal;
	createCalendar(fileName, &cal);
	addEvent(cal, JSONtoEvent(evtJSON));
	ICalErrorCode err = validateCalendar(cal);
	if(err!=OK){
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		deleteCalendar(cal);
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,printError(err));
		strcat(errStr,"\"}");
		return errStr;
	}
	err = writeCalendar(fileName, cal);

	char *errStr = malloc(sizeof(char)*100);
	errStr[0]='\0';
	deleteCalendar(cal);
	strcat(errStr, "{\"error\":\"");
	strcat(errStr,printError(err));
	strcat(errStr,"\"}");
	return errStr;

}

char* addCalWrap(char *fileName, char* calJSON){
	FILE *file;
    if ((file = fopen(fileName, "r"))){
        fclose(file);
        printf("\n\n\nITS the same\n\n\n");
    // file exists
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,"File with this name already exists.");
		strcat(errStr,"\"}");
		return errStr;
	}
	Calendar *cal = JSONtoCalendar(calJSON);
	ICalErrorCode err= validateCalendar(cal);

	if(err!=OK){
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		deleteCalendar(cal);
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,printError(err));
		strcat(errStr,"\"}");
		return errStr;
	}

	err = writeCalendar(fileName, cal);

	char *errStr = malloc(sizeof(char)*100);
	errStr[0]='\0';
	deleteCalendar(cal);
	strcat(errStr, "{\"error\":\"");
	strcat(errStr,printError(err));
	strcat(errStr,"\"}");
	return errStr;
}

char* alarmToJSON(Alarm *alm){
	char *str = calloc(300, sizeof(char));
	if(alm == NULL){
		strcat(str, "{}");
		return str;
	}
	
	str[0] = '\0';
	strcat(str, "{\"action\":\"");
	strcat(str, alm->action);
	strcat(str, "\",\"trigger\":\"");
	str = realloc(str, sizeof(char)*(strlen(str)+strlen(alm->trigger)+4));
	strcat(str, alm->trigger);
	strcat(str, "\"}");
	return str;
}

char* createAlmListJSONWrap(char* fileName, int num){
	Calendar *cal;
	Event *evt;
	int evtNum = 0;
	createCalendar(fileName, &cal);
	ICalErrorCode err;
	if((err = validateCalendar(cal))!=OK){
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		deleteCalendar(cal);
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,printError(err));
		strcat(errStr,"\"}");
		return errStr;
	}
	ListIterator iter = createIterator((List *)(cal->events));

	if(cal->events== NULL){
		deleteCalendar(cal);
		return "{\"error\":\"Bad Event\"}";
	}
	while((evt = (Event*)nextElement(&iter)) != NULL&& evtNum!=num){
		evtNum++;
	}
	if(evt == NULL){
		deleteCalendar(cal);
		return "{\"error\":\"Bad Event\"}";
	}


	char *str = malloc(sizeof(char)*300);
	char *jlm;
	Alarm *alm;
	str[0] = '\0';
	if(evt->alarms == NULL||getLength(evt->alarms) == 0){
		strcat(str, "[]");
		deleteCalendar(cal);
		return str;
	}
	ListIterator aiter = createIterator((List *)(evt->alarms));

	strcat(str, "[");

	while((alm = (Alarm*)nextElement(&aiter)) != NULL){
		jlm = alarmToJSON(alm);
		str = realloc(str, sizeof(char)*(strlen(jlm)+strlen(str)+4));
		strcat(str, jlm);
		strcat(str, ",");
		free(jlm);
	}
	//printf("%s\n",str);
	str[strlen(str)-1] = '\0';
	strcat(str, "]");
	//printf("%s\n",str);
	deleteCalendar(cal);
	return str;
}

char* readableAlmListJSONWrap(char *fileName, int num){
	Calendar *cal;
	Event *evt;
	int evtNum = 0;
	createCalendar(fileName, &cal);
	ICalErrorCode err;
	if((err = validateCalendar(cal))!=OK){
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		deleteCalendar(cal);
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,printError(err));
		strcat(errStr,"\"}");
		return errStr;
	}
	ListIterator iter = createIterator((List *)(cal->events));

	if(cal->events== NULL){
		deleteCalendar(cal);
		return "{\"error\":\"Bad Event\"}";
	}
	while((evt = (Event*)nextElement(&iter)) != NULL&& evtNum!=num){
		evtNum++;
	}
	if(evt == NULL){
		deleteCalendar(cal);
		return "{\"error\":\"Bad Event\"}";
	}


	char *str = malloc(sizeof(char)*300);
	char *jlm;
	Alarm *alm;
	str[0] = '\0';
	if(evt->alarms == NULL||getLength(evt->alarms) == 0){
		strcat(str, "[]");
		deleteCalendar(cal);
		return str;
	}
	ListIterator aiter = createIterator((List *)(evt->alarms));

	strcat(str, "[");

	while((alm = (Alarm*)nextElement(&aiter)) != NULL){
		jlm = printAlarm(alm);
		str = realloc(str, sizeof(char)*(strlen(jlm)+strlen(str)+4));
		strcat(str, "\"");
		strcat(str, jlm);
		strcat(str, "\",");
		free(jlm);
	}
	//printf("%s\n",str);
	str[strlen(str)-1] = '\0';
	strcat(str, "]");
	//printf("%s\n",str);
	deleteCalendar(cal);
	return str;
}

char* readablePropListJSONWrap(char *fileName, int num){
	Calendar *cal;
	Event *evt;
	int evtNum = 0;
	createCalendar(fileName, &cal);
	ICalErrorCode err;
	if((err = validateCalendar(cal))!=OK){
		char *errStr = malloc(sizeof(char)*100);
		errStr[0]='\0';
		deleteCalendar(cal);
		strcat(errStr, "{\"error\":\"");
		strcat(errStr,printError(err));
		strcat(errStr,"\"}");
		return errStr;
	}
	ListIterator iter = createIterator((List *)(cal->events));
	if(cal->events== NULL){
		deleteCalendar(cal);
		return "{\"error\":\"Bad Event\"}";
	}
	while((evt = (Event*)nextElement(&iter)) != NULL&& evtNum!=num){
		evtNum++;
	}
	if(evt == NULL){
		deleteCalendar(cal);
		return "{\"error\":\"Bad Event\"}";
	}

	char *str = malloc(sizeof(char)*300);
	char *jlm;
	Property *prop;
	str[0] = '\0';
	if(evt->properties == NULL||getLength(evt->properties) == 0){
		strcat(str, "[]");
		deleteCalendar(cal);
		return str;
	}
	ListIterator aiter = createIterator((List *)(evt->properties));

	strcat(str, "[");
	while((prop = (Property*)nextElement(&aiter)) != NULL){
		jlm = printProperty(prop);
		printf("reallocProp\n");
		str = realloc(str, sizeof(char)*(strlen(jlm)+strlen(str)+4));
		strcat(str, "\"");
		strcat(str, jlm);
		strcat(str, "\",");
		free(jlm);
	}
	//printf("%s\n",str);
	str[strlen(str)-1] = '\0';
	strcat(str, "]");
	//printf("%s\n",str);
	deleteCalendar(cal);
	return str;
}