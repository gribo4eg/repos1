#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#include "worker.h"
#include "../JSON/cJSON.h"


struct worker_s
{
    int id;
    char name[50];
    char surname[50];
    struct tm birthdate;
    int exp;
    double rating;
};

worker_t* worker_new(void){
    worker_t * worker = malloc(sizeof(struct worker_s));
    worker->id = -1;
    strcpy(worker->name, "");
    strcpy(worker->surname, "");
    memset(&worker->birthdate, 0, sizeof(worker->birthdate));
    worker->birthdate.tm_year = 0;
    worker->birthdate.tm_mday = 0;
    worker->birthdate.tm_mon = 0;
    worker->exp = 0;
    worker->rating = 0;
    return worker;
}

void worker_free(worker_t * worker){
    free(worker);
}

int worker_getId(worker_t* worker){
    return worker->id;
}

char* worker_getName(worker_t* worker)
{
    return worker->name;
}

char* worker_getSurname(worker_t* worker){
    return worker->surname;
}

char* worker_getBirthdate(worker_t* worker){
    char buffer[256];
    sprintf(buffer, "%i-%i-%i", worker->birthdate.tm_year,
                                worker->birthdate.tm_mon,
                                worker->birthdate.tm_mday);
    return buffer;
}

int worker_getExp(worker_t* worker){
    return worker->exp;
}

double worker_getRate(worker_t* worker){
    return worker->rating;
}

void worker_fill(worker_t* worker, int id, char* name, char* surname, char* birthdate, int exp, double rating){
    char* str = NULL;
    char buffer[256];
    worker->id = id;

    strcpy(worker->name, name);
    strcpy(worker->surname, surname);
    worker->exp = exp;
    worker->rating = rating;

    strcpy(buffer, birthdate);

    str = strtok(buffer, "-");
    worker->birthdate.tm_year = atoi(str);
    str = strtok(NULL, "-");
    worker->birthdate.tm_mon = atoi(str);
    str = strtok(NULL, "\0");
    worker->birthdate.tm_mday = atoi(str);
}

char* worker_makeWorkerJSON(worker_t *worker){
    char* inJsn = NULL;
    char buffer[256];
    cJSON* workerJsn = cJSON_CreateObject();

    cJSON_AddItemToObject(workerJsn, "Id", cJSON_CreateNumber(worker->id));
    cJSON_AddItemToObject(workerJsn, "Name", cJSON_CreateString(worker->name));
    cJSON_AddItemToObject(workerJsn, "Surname", cJSON_CreateString(worker->surname));
    sprintf(buffer, "%i-%i-%i", worker->birthdate.tm_year,
                                worker->birthdate.tm_mon,
                                worker->birthdate.tm_mday);
    cJSON_AddItemToObject(workerJsn, "Birth date", cJSON_CreateString(buffer));
    cJSON_AddItemToObject(workerJsn, "Experience", cJSON_CreateNumber(worker->exp));
    cJSON_AddItemToObject(workerJsn, "Rating", cJSON_CreateNumber(worker->rating));

    inJsn = cJSON_Print(workerJsn);
    cJSON_Delete(workerJsn);
    return inJsn;
}

void worker_print(worker_t * worker){
    printf("Id: %i\nName: %s\nSurname: %s\nBirth date: %i-%i-%i\nExperience: %i\nRating: %.2f\n\n----------------------\n\n",
        worker->id, worker->name, worker->surname, worker->birthdate.tm_year,
        worker->birthdate.tm_mon, worker->birthdate.tm_mday,
        worker->exp, worker->rating);
}

void worker_parseWorker(worker_t** workers){
    FILE* fp = fopen("worker.json", "r");
    char buffer[10000];
    char line[100];

    while(fgets(line, 100, fp) != NULL)
    {
        strcat(buffer, line);
    }
    fclose(fp);

    cJSON* textJsn = cJSON_Parse(buffer);
    if(!textJsn){
        printf("Some troubles with [%s]\n", cJSON_GetErrorPtr());
        return;
    }

    for(int i = 0; i<cJSON_GetArraySize(textJsn); i++)
    {

        cJSON* workerJsn = cJSON_GetArrayItem(textJsn, i);
        int id = cJSON_GetObjectItem(workerJsn, "id")->valueint;
        char* name = cJSON_GetObjectItem(workerJsn, "name")->valuestring;
        char* surname = cJSON_GetObjectItem(workerJsn, "surname")->valuestring;
        char* birthdate = cJSON_GetObjectItem(workerJsn, "birthdate")->valuestring;
        int experience = cJSON_GetObjectItem(workerJsn, "experience")->valueint;
        double rating = cJSON_GetObjectItem(workerJsn, "rating")->valuedouble;

        worker_fill(workers[i], id, name, surname, birthdate, experience, rating);
    }

    cJSON_Delete(textJsn);
}

int worker_workersCount(worker_t** workers){
    return sizeof(workers)/sizeof(workers[0]);
}