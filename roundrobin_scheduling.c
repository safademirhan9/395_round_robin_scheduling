#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASK_NAME_LEN 20

// Struct for a task
typedef struct Task {
    char name[MAX_TASK_NAME_LEN];
    int runtime;
    struct Task *next;
} Task;

// Struct for a completed task
typedef struct CompletedTask {
    char name[MAX_TASK_NAME_LEN];
    int runtime;
    int completion_time;
    struct CompletedTask *next;
} CompletedTask;

// Struct for a queue
typedef struct Queue {
    Task *first;
    Task *last;
} Queue;

// Global variables
int system_time = 0;
int idle_time = 0;
Queue task_queue = {NULL, NULL};
CompletedTask *completed_task_queue = NULL;

// Function to add a task to the task queue
void add_task_to_queue(char *name, int runtime) {
    Task *new_task = malloc(sizeof(Task));
    if (new_task == NULL) {
        printf("Error: Out of memory\n");
        return;
    }
    strncpy(new_task->name, name, MAX_TASK_NAME_LEN - 1);
    new_task->name[MAX_TASK_NAME_LEN - 1] = '\0';
    new_task->runtime = runtime;
    new_task->next = NULL;

    if (task_queue.first == NULL) {
        task_queue.first = new_task;
        task_queue.last = new_task;
    } else {
        task_queue.last->next = new_task;
        task_queue.last = new_task;
    }
}

// Function to remove a task from the task queue
Task *remove_task_from_queue() {
    if (task_queue.first == NULL) {
        return NULL;
    }

    Task *task = task_queue.first;
    task_queue.first = task->next;
    if (task_queue.first == NULL) {
        task_queue.last = NULL;
    }
    return task;
}

// Function to add a completed task to the completed task queue
void add_completed_task_to_queue(char *name, int runtime, int completion_time) {
    CompletedTask *new_completed_task = malloc(sizeof(CompletedTask));
    if (new_completed_task == NULL) {
        printf("Error: Out of memory\n");
        return;
    }
    strncpy(new_completed_task->name, name, MAX_TASK_NAME_LEN - 1);
    new_completed_task->name[MAX_TASK_NAME_LEN - 1] = '\0';
    new_completed_task->runtime = runtime;
    new_completed_task->completion_time = completion_time;
    new_completed_task->next = NULL;

    if (completed_task_queue == NULL) {
        completed_task_queue = new_completed_task;
    } else {
        CompletedTask *curr = completed_task_queue;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_completed_task;
    }
}

// Function to display the system time
void display_system_time() {
    printf("System time: %d\n", system_time);
}

// Function to display the total (idle) time
void display_total_time() {
    printf("Total (idle) time: %d\n", idle_time);
}

void display_most_recently_completed_task() {
    if (completed_task_queue == NULL) {
        printf("No tasks completed yet.\n");
        return;
    }

    printf("Task Name: %s\n", completed_task_queue->name);
    printf("Initial Runtime: %d\n", completed_task_queue->runtime);
    printf("Completion Time: %d\n", completed_task_queue->completion_time);
}

void dump_completed_task_queue(char *file_name) {
    FILE *fp = fopen(file_name, "w");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Task *current_task = completed_task_queue;
    while (current_task != NULL) {
        fprintf(fp, "Task Name: %s\n", current_task->name);
        fprintf(fp, "Initial Runtime: %d\n", current_task->runtime);
        // fprintf(fp, "Completion Time: %d\n", current_task->completion_time);
        fprintf(fp, "\n");

        current_task = current_task->next;
    }

    fclose(fp);
}

void empty_completed_task_queue() {
    while (completed_task_queue != NULL) {
        struct Task *current_task = completed_task_queue;
        completed_task_queue = completed_task_queue->next;
        free(current_task);
    }

    printf("Completed tasks queue emptied.\n");
}

int main() {
    char input[50];
    int system_time = 0;
    int idle_time = 0;

    task_queue;
    completed_task_queue = NULL;

    while (1) {
        printf("\nEnter command: ");
        fgets(input, 50, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strncmp(input, "SNT", 3) == 0) {
            char task_name[20];
            int runtime;
            sscanf(input + 4, "%s %d", task_name, &runtime);
            add_task_to_queue(task_name, runtime);
        } else if (strncmp(input, "S", 1) == 0) {
            int k = atoi(input + 2);
            // simulate(k);
            system_time += k;
        } else if (strncmp(input, "DST", 3) == 0) {
            printf("System time: %d\n", system_time);
        } else if (strncmp(input, "DTIT", 4) == 0) {
            printf("Idle time: %d\n", idle_time);
        } else if (strncmp(input, "DIMRCT", 6) == 0) {
            display_most_recently_completed_task();
        } else if (strncmp(input, "DCTQ", 4) == 0) {
            char file_name[20];
            sscanf(input + 5, "%s", file_name);
            dump_completed_task_queue(file_name);
        } else if (strncmp(input, "ECTS", 4) == 0) {
            empty_completed_task_queue();
        } else if (strncmp(input, "QUIT", 4) == 0) {
            break;
        }
    }

    // empty_task_list();
    // empty_completed_task_queue();

    return 0;
}
