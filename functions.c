#include "header.h"

extern Task *tasks;
extern int taskCount;

void print_menu()
{
    printf("Usage:\n");
    printf("./taskmanager add --title <title> --desc <description> --priority <high|medium|low> --due <YYYY-MM-DD>\n");
    printf("./taskmanager view --all\n");
    printf("./taskmanager update --id <id> --field <title|description|priority|status> --value <valid_input>\n");
    printf("./taskmanager delete --id <id>\n");

    free(tasks);
    exit(EXIT_SUCCESS);
}

// task managing functions
void addTask(const char *title, const char *description, Priority priority, time_t due_date)
{
    tasks[taskCount].id = taskCount + 1;
    strncpy(tasks[taskCount].title, title, sizeof(tasks[taskCount].title) - 1);
    tasks[taskCount].title[sizeof(tasks[taskCount].title) - 1] = '\0';

    strncpy(tasks[taskCount].description, description, sizeof(tasks[taskCount].description) - 1);
    tasks[taskCount].description[sizeof(tasks[taskCount].description) - 1] = '\0';

    tasks[taskCount].priority = priority;
    tasks[taskCount].due_date = due_date;

    time_t currentTime = time(NULL);
    if (difftime(due_date, currentTime) < 0)
    {
        tasks[taskCount].status = FAILED;
    }
    else
    {
        tasks[taskCount].status = PENDING;
    }

    printf("ID: %d Title: %s Priority: %s Status: %s\n",
           tasks[taskCount].id, tasks[taskCount].title, priorityToString(priority), statusToString(tasks[taskCount].status));
    printf("Task added successfully.\n");

    taskCount++;
}

void viewTasks()
{
    sortTasksByPriority(tasks, taskCount);

    printf("Viewing tasks... (Count: %d)\n", taskCount);
    if (taskCount == 0)
    {
        printf("No tasks available.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].priority == UNKNOWN)
            continue;

        const char *priorityStr = priorityToString(tasks[i].priority);

        const char *statusStr = statusToString(tasks[i].status);

        char dueDateStr[11];                                
        struct tm *tm_info = localtime(&tasks[i].due_date); 
        strftime(dueDateStr, sizeof(dueDateStr), "%Y-%m-%d", tm_info);

        printf("%d. %s [%s] Status: %s Due: %s\n",
               tasks[i].id, tasks[i].title, priorityStr, statusStr, dueDateStr);
    }
    printf("Tasks are listed in order of priority.\n");
}

void updateTask(int id, const char *field, const char *value)
{
    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].id == id)
        {
            if (strcmp(field, "title") == 0)
            {
                strncpy(tasks[i].title, value, sizeof(tasks[i].title) - 1);
                tasks[i].title[sizeof(tasks[i].title) - 1] = '\0';
                printf("Title updated successfully.\n");
            }
            else if (strcmp(field, "description") == 0)
            {
                strncpy(tasks[i].description, value, sizeof(tasks[i].description) - 1);
                tasks[i].description[sizeof(tasks[i].description) - 1] = '\0';
                printf("Description updated successfully.\n");
            }
            else if (strcmp(field, "priority") == 0)
            {
                Priority newPriority = parsePriority(value);
                if (newPriority != -1)
                {
                    tasks[i].priority = newPriority;
                    printf("Priority updated successfully.\n");
                }
                else
                {
                    printf("Invalid priority value.\n");
                }
            }
            else if (strcmp(field, "status") == 0)
            {
                if (strcmp(value, "pending") == 0)
                    tasks[i].status = PENDING;
                else if (strcmp(value, "completed") == 0)
                    tasks[i].status = COMPLETED;
                else if (strcmp(value, "failed") == 0)
                    tasks[i].status = FAILED;
                else
                {
                    printf("Invalid status value.\n");
                    return;
                }
                printf("Status updated successfully.\n");
            }
            else
            {
                printf("Invalid field.\n");
                return;
            }
            return;
        }
    }
    printf("Task not found.\n");
}

void deleteTask(int id)
{
    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].id == id)
        {
            for (int j = i; j < taskCount - 1; j++)
            {
                tasks[j] = tasks[j + 1];
            }
            taskCount--;
            printf("Task deleted successfully.\n");
            return;
        }
    }
    printf("Task not found.\n");
}

// file updating functions
void saveTasksToFile()
{
    FILE *file = fopen(TASKS_FILE, "wb");
    if (file)
    {
        fwrite(&taskCount, sizeof(int), 1, file);
        fwrite(tasks, sizeof(Task), taskCount, file);
        fclose(file);
    }
    else
    {
        perror("Failed to save tasks \n");
    }

    free(tasks);
}

int loadTasksFromFile(FILE *file)
{
    tasks = (Task *)malloc(sizeof(Task) * (taskCount + 1));

    fread(tasks, sizeof(Task), taskCount, file);
    fclose(file);
    return 0;
}

FILE *read_count()
{
    FILE *file = fopen(TASKS_FILE, "rb");

    fread(&taskCount, sizeof(int), 1, file);
    return file;
}

// priority and sorting functions
Priority parsePriority(const char *priority)
{
    if (strcmp(priority, "high") == 0)
        return HIGH;
    else if (strcmp(priority, "medium") == 0)
        return MEDIUM;
    else if (strcmp(priority, "low") == 0)
        return LOW;

    fprintf(stderr, "Invalid priority\n");
    return UNKNOWN; // default priority
}

int comparePriority(Priority p1, Priority p2)
{
    return p2 - p1;
}

void sortTasksByPriority() // bubble sort
{
    for (int i = 0; i < taskCount - 1; i++)
    {
        for (int j = i + 1; j < taskCount; j++)
        {
            if (comparePriority(tasks[i].priority, tasks[j].priority) > 0)
            {
                Task temp = tasks[i];
                tasks[i] = tasks[j];
                tasks[j] = temp;
            }
        }
    }
}

const char *priorityToString(Priority priority)
{
    switch (priority)
    {
    case HIGH:
        return "High";
    case MEDIUM:
        return "Medium";
    case LOW:
        return "Low";
    default:
        return "Unknown";
    }
}

const char *statusToString(Status status)
{
    switch (status)
    {
    case PENDING:
        return "Pending";
    case COMPLETED:
        return "Completed";
    case FAILED:
        return "Failed";
    default:
        return NULL;
    }
}