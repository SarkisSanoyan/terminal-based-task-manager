# Task Manager Application

This Task Manager application is designed to help users manage their tasks efficiently through a command-line interface. Users can add, view, update, and delete tasks with various functionalities such as setting priorities, due dates, and tracking task statuses.

## Features

- **Add Task:** Add a new task with a title, description, priority (high, medium, low), and due date.
- **View Tasks:** Display all tasks sorted by priority, showing their title, priority, status, and due date.
- **Update Task:** Modify existing task details including title, description, priority, or status by specifying the task ID.
- **Delete Task:** Remove a task from the list based on its ID.

## Getting Started

### Prerequisites

- C compiler (GCC recommended)
- POSIX compliant operating system (Linux, macOS, etc.)

### Installation

#### 1. Clone the repository:

```bash
git clone https://github.com/SarkisSanoyan/terminal-based-task-manager.git
cd terminal-based-task-manager
```

#### 2. Compile the application:

```bash
gcc main.c functions.c -o taskmanager
```

### Usage

The application is driven via command-line arguments:

```bash
./taskmanager add --title "Task Title" --desc "Task Description" --priority high --due 2024-12-31
./taskmanager view --all
./taskmanager update --id 1 --field title --value "New Title"
./taskmanager delete --id 1


