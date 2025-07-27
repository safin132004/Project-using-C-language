#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define name_len 30
#define id_len 15
#define sub 3
#define max_std 50

#define RESET "\x1b[0m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"

typedef struct
{
    char id[id_len];
    char name[name_len];
    int marks[sub];
    int total;
    float average;
    int attendance;
} Student;

Student students[max_std];
int std_count = 0;

void clearInputBuffer()
{
    while (getchar() != '\n')
        ;
}

int isDuplicateID(const char *id)
{
    for (int i = 0; i < std_count; i++)
    {
        if (strcmp(students[i].id, id) == 0)
            return 1;
    }
    return 0;
}

void addStudent()
{
    if (std_count == max_std)
    {
        printf(RED "Student list is full!\n" RESET);
        return;
    }

    Student s;

    clearInputBuffer();
    printf("Enter student ID (e.g., 251-15-858): ");
    fgets(s.id, id_len, stdin);
    s.id[strcspn(s.id, "\n")] = '\0';

    if (isDuplicateID(s.id))
    {
        printf(RED "Duplicate ID found! Try again.\n" RESET);
        return;
    }

    printf("Enter student name: ");
    fgets(s.name, name_len, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    s.total = 0;
    for (int i = 0; i < sub;)
    {
        int mark;
        printf("Enter marks for subject %d: ", i + 1);
        if (scanf("%d", &mark) != 1)
        {
            printf(RED "Invalid input! Please enter an integer.\n" RESET);
            clearInputBuffer();
            continue;
        }
        if (mark >= 0 && mark <= 100)
        {
            s.marks[i] = mark;
            s.total += mark;
            i++;
        }
        else
        {
            printf(RED "Invalid marks! Enter a value between 0-100.\n" RESET);
        }
    }

    s.average = (float)s.total / sub;

    printf("Enter attendance: ");
    while (scanf("%d", &s.attendance) != 1)
    {
        printf(RED "Invalid input! Enter a number.\n" RESET);
        clearInputBuffer();
    }

    students[std_count++] = s;
    printf(GREEN "Student added successfully!\n" RESET);
}

void displayStudents()
{
    if (std_count == 0)
    {
        printf(RED "No students to display.\n" RESET);
        return;
    }

    printf(CYAN "\n%-15s %-20s %-15s %-7s %-9s %-10s\n" RESET,
           "ID", "Name", "Marks", "Total", "Average", "Attendance");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < std_count; i++)
    {
        printf("%-15s %-20s ", students[i].id, students[i].name);
        for (int j = 0; j < sub; j++)
        {
            printf("%3d ", students[i].marks[j]);
        }
        printf("   %-7d %-9.2f %-10d\n",
               students[i].total, students[i].average, students[i].attendance);
    }
}

void searchStudent()
{
    if (std_count == 0)
    {
        printf(RED "No students to search.\n" RESET);
        return;
    }

    int found = 0;
    char input[30];

    printf("Search by: 1. ID  2. Name\nChoose: ");
    int option;
    scanf("%d", &option);
    clearInputBuffer();

    if (option == 1)
    {
        printf("Enter student ID (e.g., 2xx-xx-xxx): ");
        fgets(input, id_len, stdin);
        input[strcspn(input, "\n")] = '\0';

        for (int i = 0; i < std_count; i++)
        {
            if (strcmp(students[i].id, input) == 0)
            {
                printf(GREEN "Found: %s, Total: %d, Average: %.2f\n" RESET,
                       students[i].name, students[i].total, students[i].average);
                found = 1;
                break;
            }
        }
    }
    else if (option == 2)
    {
        printf("Enter student name: ");
        fgets(input, name_len, stdin);
        input[strcspn(input, "\n")] = '\0';

        for (int i = 0; i < std_count; i++)
        {
            if (strcmp(students[i].name, input) == 0)
            {
                printf(GREEN "Found: ID %s, Total: %d, Average: %.2f\n" RESET,
                       students[i].id, students[i].total, students[i].average);
                found = 1;
                break;
            }
        }
    }

    if (!found)
    {
        printf(RED "Student not found.\n" RESET);
    }
}

void save_file()
{
    FILE *fp = fopen("students.txt", "w");
    if (!fp)
    {
        printf(RED "Cannot open file to save.\n" RESET);
        return;
    }

    fprintf(fp, "%d\n", std_count);
    for (int i = 0; i < std_count; i++)
    {
        fprintf(fp, "%s\n", students[i].id);
        fprintf(fp, "%s\n", students[i].name);
        for (int j = 0; j < sub; j++)
        {
            fprintf(fp, "%d ", students[i].marks[j]);
        }
        fprintf(fp, "\n%d\n%.2f\n%d\n",
                students[i].total, students[i].average, students[i].attendance);
    }

    fclose(fp);
    printf(GREEN "Data saved successfully to students.txt\n" RESET);
}

void loadFromFile()
{
    FILE *fp = fopen("students.txt", "r");
    if (!fp)
    {
        printf(YELLOW "No previous data found. Starting fresh.\n" RESET);
        return;
    }

    fscanf(fp, "%d\n", &std_count);
    for (int i = 0; i < std_count; i++)
    {
        fgets(students[i].id, id_len, fp);
        students[i].id[strcspn(students[i].id, "\n")] = '\0';

        fgets(students[i].name, name_len, fp);
        students[i].name[strcspn(students[i].name, "\n")] = '\0';

        for (int j = 0; j < sub; j++)
        {
            fscanf(fp, "%d", &students[i].marks[j]);
        }
        fgetc(fp);
        fscanf(fp, "%d\n%f\n%d\n", &students[i].total, &students[i].average, &students[i].attendance);
    }

    fclose(fp);
    printf(CYAN "Data loaded from file.\n" RESET);
}

int main()
{
    loadFromFile();
    int choice;

    while (1)
    {
        printf("\n%s=====================================\n", BLUE);
        printf("       🎓 Student Record System       \n");
        printf("=====================================%s\n", RESET);
        printf("%s1.%s Add Student\n", YELLOW, RESET);
        printf("%s2.%s Display Students\n", YELLOW, RESET);
        printf("%s3.%s Search Student\n", YELLOW, RESET);
        printf("%s4.%s Save and Exit\n", YELLOW, RESET);
        printf("Choose option: ");

        if (scanf("%d", &choice) != 1)
        {
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            displayStudents();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            save_file();
            printf(GREEN "Exiting... Goodbye!\n" RESET);
            return 0;
        default:
            printf(RED "Invalid option! Please try again.\n" RESET);
        }
    }

    return 0;
}
