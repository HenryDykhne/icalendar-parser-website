#include "CalendarParser.h"
#include "LinkedListAPI.h"

int main(){
	Calendar *cal = NULL, *reCal = NULL;
	ICalErrorCode err = OK;
	char *errStr;
	char *str;
	err = createCalendar("megaCalFolded1.ics", &cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	if(cal == NULL){
		return 0;
	}
	str = printCalendar(cal);
	printf("\n\n%s", str);
	free(str);
	writeCalendar("write.ics", cal);
	



	err = createCalendar("write.ics", &reCal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	if(reCal == NULL){
		return 0;
	}
	str = printCalendar(reCal);
	printf("\n\n%s\n", str);
	free(str);

	err = validateCalendar(cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);

	cal->version = 0;
	err = validateCalendar(cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	deleteCalendar(cal);

	createCalendar("megaCalFolded1.ics", &cal);
	memcpy(((Property*)(((Event*)(cal->events->head->data))->properties->head->data))->propName, "DTEND", 6);
	memcpy(((Property*)(((Event*)(cal->events->head->data))->properties->head->next->data))->propName, "DURATION", 9);
	err = validateCalendar(cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	deleteCalendar(cal);

	createCalendar("megaCalFolded1.ics", &cal);
	memcpy(((Property*)((Alarm*)((Event*)(cal->events->head->data))->alarms->head->data)->properties->head->data)->propName, "REPEAT", 7);
	err = validateCalendar(cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	deleteCalendar(cal);

	createCalendar("megaCalFolded1.ics", &cal);
	clearList(((Alarm*)((Event*)(cal->events->head->data))->alarms->head->data)->properties);
	clearList(((Alarm*)((Event*)(cal->events->head->data))->alarms->head->data)->properties);
	err = validateCalendar(cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	deleteCalendar(cal);

	createCalendar("megaCalFolded1.ics", &cal);

	str = dtToJSON(((Event*)(cal->events->head->data))->creationDateTime);
	printf("%s\n", str);
	free(str);

	str = eventToJSON((Event*)(cal->events->head->data));
	printf("%s\n", str);
	free(str);


	str = eventListToJSON((cal->events));
	printf("%s\n", str);
	free(str);

	str = calendarToJSON(cal);
	printf("%s\n", str);
	free(str);

	deleteCalendar(cal);
	deleteCalendar(reCal);


	cal = JSONtoCalendar("{\"version\":2,\"prodID\":\"-//hacksw/handcal//NONSGML v1.0//EN\"}");
	printf("%f\n", cal->version);
	printf("%s\n", cal->prodID);

	deleteCalendar(cal);

	Event *evt = JSONtoEvent("{\"UID\":\"1234\"}");
	printf("%s\n", evt->UID);
	deleteEvent(evt);

	cal = JSONtoCalendar("{\"version\":2,\"prodID\":\"-//hacksw/handcal//NONSGML v1.0//EN\"}");
	evt = JSONtoEvent("{\"UID\":\"1234\"}");
	addEvent(cal, evt);
	printf("%s\n", ((Event*)(cal->events->head->data))->UID);

	deleteCalendar(cal);
	return 0;
}