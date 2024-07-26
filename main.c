#include "header.h"

Task *tasks;
int taskCount;

int main(int argc, char *argv[])
{
    FILE *file = read_count();
    loadTasksFromFile(file);

    if (argc < 2)
    {
        print_menu();
    }

    if (strcmp(argv[1], "add") == 0)
    {
        if (argc < 10)
        {
            print_menu();
        }

        char *title = NULL, *description = NULL, *priorityStr = NULL, *dueDateStr = NULL;
        for (int i = 2; i < argc; i += 2)
        {
            if (strcmp(argv[i], "--title") == 0)
            {
                title = argv[i + 1];
            }
            else if (strcmp(argv[i], "--desc") == 0)
            {
                description = argv[i + 1];
            }
            else if (strcmp(argv[i], "--priority") == 0)
            {
                priorityStr = argv[i + 1];
            }
            else if (strcmp(argv[i], "--due") == 0)
            {
                dueDateStr = argv[i + 1];
            }
        }

        if (title && description && priorityStr && dueDateStr)
        {
            Priority priority = parsePriority(priorityStr);

            struct tm tm = {0};
            if (sscanf(dueDateStr, "%4d-%2d-%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3)
            {
                fprintf(stderr, "Invalid date format. Use YYYY-MM-DD.\n");
                return 1;
            }

            if (tm.tm_year < 1900 || tm.tm_year > 2100)
            {
                fprintf(stderr, "Invalid year for due date: must be greater than 1900 and less than 2100\n");
                print_menu();
            }
            if (tm.tm_mon < 1 || tm.tm_mon > 12)
            {
                fprintf(stderr, "Invalid month for due date: must be greater than 0 and less than 13\n");
                print_menu();
            }
            if (tm.tm_mday < 0 || tm.tm_mday > 31)
            {
                fprintf(stderr, "Invalid day for due date: must be greater than 0 and less than 32\n");
                print_menu();
            }

            tm.tm_year -= 1900;
            tm.tm_mon -= 1;     

            time_t due_date = mktime(&tm);
            if (due_date == -1)
            {
                fprintf(stderr, "Failed to parse date.\n");
                return 1;
            }

            addTask(title, description, priority, due_date);
            saveTasksToFile();
        }
        else
        {
            print_menu();
        }
    }
    else if (strcmp(argv[1], "view") == 0)
    {
        if (argc != 3 || strcmp(argv[2], "--all") != 0)
        {
            print_menu();
        }
        viewTasks(tasks, taskCount);
    }
    else if (strcmp(argv[1], "update") == 0)
    {
        if (argc < 8)
        {
            print_menu();
        }
        int id = 0;
        char *field = NULL, *value = NULL;
        for (int i = 2; i < argc; i += 2)
        {
            if (strcmp(argv[i], "--id") == 0)
            {
                id = atoi(argv[i + 1]);
            }
            else if (strcmp(argv[i], "--field") == 0)
            {
                field = argv[i + 1];
            }
            else if (strcmp(argv[i], "--value") == 0)
            {
                value = argv[i + 1];
            }
        }
        if (id && field && value)
        {
            updateTask(id, field, value);
            saveTasksToFile();
        }
        else
        {
            print_menu();
        }
    }
    else if (strcmp(argv[1], "delete") == 0)
    {
        if (argc != 4 || strcmp(argv[2], "--id") != 0)
        {
            print_menu();
        }
        int id = atoi(argv[3]);
        deleteTask(id);
        saveTasksToFile();
    }
    else
    {
        print_menu();
    }

    return 0;
}