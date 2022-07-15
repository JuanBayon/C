#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "appointment.h"

// Initializes a vaccination appointment data list
void appointmentData_init(tAppointmentData* list) {
    assert(list != NULL);
    list->count = 0;
    list->elems = NULL;
}

// Insert a new vaccination appointment
void appointmentData_insert(tAppointmentData* list, tDateTime timestamp, tVaccine* vaccine, tPerson* person) {
    assert(list != NULL);
    assert(vaccine != NULL);
    assert(person != NULL);

    bool found = false;
    bool foundFirst = false;
    int index = 0;
    int firstIndex = 0;
    int lastIndex = 0;

    //The list is empty
    if (list->count == 0) {
        list->elems = (tAppointment*) malloc(sizeof(tAppointment));
    } else {
        list->elems = (tAppointment*) realloc(list->elems, (list->count + 1) * sizeof(tAppointment));
        //Search for the place ordered by date and time
        while(index < list->count && !found){
            //if the date is found the first index is saved
            if (!foundFirst && dateTime_cmp(list->elems[index].timestamp, timestamp) == 0) {
                firstIndex = index;
                foundFirst = true;
            //if the date is not in the list the index where storing the appointment is saved
            // if it is the last element
            } else if (!foundFirst && index == list->count - 1) {
                firstIndex = index;
                found = true;
            // Otherwise if the next element is smaller
            } else if (!foundFirst && dateTime_cmp(list->elems[index + 1].timestamp, timestamp) > 0){
                firstIndex = index;
                found = true;
            }
            //save the last index of the same date
            if (foundFirst && dateTime_cmp(list->elems[index].timestamp, timestamp) > 0) {
                lastIndex = index - 1;
                found = true;
            }
            index++;
        }
        //If there are different appointments on the same date 
        //search for the place ordered alphabetically
        if (lastIndex != 0) {
            found = false;
            index = firstIndex;
            //Search for the place
            while (!found && index <= lastIndex) {
                //if the index is the last
                if (index == lastIndex) {
                    //if the document goes before
                    if (strcmp(list->elems[index].person->document, person->document) > 0) {
                        firstIndex = index;
                    //if the document goes after
                    } else {
                        firstIndex = index + 1;
                    }
                //if the next document is bigger
                } else if (strcmp(list->elems[index + 1].person->document, person->document) > 0) {
                    firstIndex = index;
                    found = true;
                }
                index++;
            }
        }
        //If the place to insert the appointment is not the last
        if (firstIndex <= list->count - 1) {
            //Move the appointments to the right
            for (int i = list->count; i > firstIndex; i--) {
                list->elems[i] = list->elems[i - 1];
            }
        }
    }
    //Place the element
    list->elems[firstIndex].person = person;
    list->elems[firstIndex].timestamp = timestamp;
    list->elems[firstIndex].vaccine = vaccine;
    list->count++;
}

// Remove a vaccination appointment
void appointmentData_remove(tAppointmentData* list, tDateTime timestamp, tPerson* person) {
    assert(list != NULL);
    assert(person != NULL);

    int index = 0;
    bool found = false;
    bool empty = false;
    if (list->count == 0) empty = true;
    //Search for the element
    while(!found && index < list->count) {
        if (dateTime_cmp(list->elems[index].timestamp, timestamp) == 0 && 
            strcmp(list->elems[index].person->document, person->document) == 0) {
            found = true;
        }
        index++;
    }
    //Move the appointments to the left if found
    if (!empty && found) {
        while (index < list->count) {
            list->elems[index - 1] = list->elems[index];
            index++;
        }
        list->count--;
        if (list->count == 0) {
            free(list->elems);
            list->elems = NULL;
        } else {
            list->elems = (tAppointment*) realloc(list->elems, list->count * sizeof(tAppointment));
        }
    }
}

// Find the first instance of a vaccination appointment for given person
int appointmentData_find(tAppointmentData list, tPerson* person, int start_pos) {
    assert(person != NULL);

    if (start_pos <= list.count) {
        if (start_pos == list.count) {
            return -1;
        } else {
            if (list.elems[start_pos].person == person) {
                return start_pos;
            } else {
                appointmentData_find(list, person, start_pos + 1);
            }
        }
    }

    return -1;
}

// Release a vaccination appointment data list
void appointmentData_free(tAppointmentData* list) {
    assert(list != NULL);

    free(list->elems);
    appointmentData_init(list);
}



    //     while (index < list->count && !found) {
    //         //The date is equal
    //         if (dateTime_cmp(list->elems[index].timestamp, timestamp) == 0) {
    //             //si el documento es igual hay que comprobar el dni. pero la fecha siguiente tiene que ser igual tambien para iterar!
    //             if (strcmp(list->elems[index].person->document, person->document) == 0) {
    //                  //The element is the last
    //                 if (index != list->count - 1) {
    //                 } else {
    //                     //move all the elements to the right
    //                     for (int i = list->count; i > index; i--) {
    //                     }
    //                 }
    //             }
    //         //The element is the last (and the date is not equal)
    //         } else if (index != list->count - 1) {

    //         //The next date is bigger and it is not the last element
    //         } else if (dateTime_cmp(list->elems[index + 1].timestamp, timestamp) == 0) {
    //     }
    // }