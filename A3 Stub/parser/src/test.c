#include "CalendarParser.h"
#include "LinkedListAPI.h"

int main(){
	Calendar *cal = NULL, *reCal = NULL;
	ICalErrorCode err = OK;
	char *errStr;
	char *str;
	err = createCalendar("megaCal1.ics", &cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	if(cal == NULL){
		return 0;
	}
	str = printCalendar(cal);
	//printf("\n\n%s", str);
	free(str);
	writeCalendar("write.ics", cal);
	



	err = createCalendar("write.ics", &reCal);
	printf("%s\n", errStr = printError(err));
	free(errStr);
	if(reCal == NULL){
		return 0;
	}
	str = printCalendar(reCal);
	//printf("\n\n%s\n", str);
	free(str);

	err = validateCalendar(cal);
	printf("%s\n", errStr = printError(err));
	free(errStr);

	cal->version = 0;
	err = validateCalendar(cal);
	printf("err: %s\n", errStr = printError(err));
	free(errStr);
	deleteCalendar(cal);

	createCalendar("megaCalFolded1.ics", &cal);
	printf("Here I am\n");
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


	createCalendar("megaCalFolded1.ics", &cal);
	addEvent(cal, JSONtoEvent("{\"UID\":\"uidtest\",\"creationDT\":{\"date\":\"11112233\",\"time\":\"112233\",\"isUTC\":true},\"startDT\":{\"date\":\"11112233\",\"time\":\"112233\",\"isUTC\":false},\"summary\":\"summarytest\",\"numProps\":0,\"numAlarms\":0}"));
	printf("%s\n", printError(writeCalendar("megaCalFolded1.ics", cal)));
	deleteCalendar(cal);
	return 0;
}