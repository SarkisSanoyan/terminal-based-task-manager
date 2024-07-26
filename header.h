#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TASKS_FILE "tasks.dat"
#define TITLE_SIZE 100
#define DESC_SIZE 512

typedef enum
{
    UNKNOWN = -1,
    LOW,
    MEDIUM,
    HIGH
} Priority;

typedef enum
{
    PENDING,
    COMPLETED,
    FAILED
} Status;

typedef struct
{
    int id;
    char title[TITLE_SIZE];
    char description[DESC_SIZE];
    Priority priority;
    Status status;
    time_t due_date;
} Task;

void print_menu();

// priority and sorting functions
Priority parsePriority(const char *priority);
int comparePriority(Priority p1, Priority p2);
void sortTasksByPriority();
const char *priorityToString(Priority priority);
const char *statusToString(Status status);

// task managing functions
FILE *read_count();
void addTask(const char *title, const char *description, Priority priority, time_t due_date);
void viewTasks();
void updateTask(int id, const char *field, const char *value);
void deleteTask(int id);

// file updating functions
void saveTasksToFile();
int loadTasksFromFile(FILE *file);

#endif