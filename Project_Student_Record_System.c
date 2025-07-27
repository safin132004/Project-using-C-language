#include <stdio.h>
#include <string.h>

#define name_len 30
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
    int id;
    char name[name_len];
    int marks[sub];
    int total;
    float average;
    int attendance;
} Student;

Student students[max_std];
int std_count = 0;

void addStudent()
{
    if (std_count == max_std)
    {
        printf(RED "Student list full!\n" RESET);
        return;
    }

    Student s;
    s.id = std_count + 1;

    printf("Enter student name: ");
    getchar(); // Clear newline
    fgets(s.name, name_len, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    s.total = 0;
    for (int i = 0; i < sub;)
    {
        int mark;
        printf("Enter marks for subject %d: ", i + 1);
        scanf("%d", &mark);
        if (mark >= 0 && mark <= 100)
        {
            s.marks[i] = mark;
            s.total += mark;
            i++;
        }
        else
        {
            printf(RED "Invalid marks! Enter 0-100.\n" RESET);
        }
    }

    s.average = (float)s.total / sub;

    printf("Enter attendance: ");
    scanf("%d", &s.attendance);

    students[std_count++] = s;
    printf(GREEN "Student added with ID: %d\n" RESET, s.id);
}

void displayStudents()
{
    if (std_count == 0)
    {
        printf(RED "No students to display.\n" RESET);
        return;
    }

    printf(CYAN "\n%-4s %-15s %-20s %-6s %-8s %-10s\n" RESET,
           "ID", "Name", "Marks", "Total", "Average", "Attendance");

    for (int i = 0; i < std_count; i++)
    {
        printf("%-4d %-15s ", students[i].id, students[i].name);
        for (int j = 0; j < sub; j++)
        {
            printf("%3d ", students[i].marks[j]);
        }
        printf("   %-6d %-8.2f %-10d\n",
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

    int search_id, found = 0;
    char search_name[name_len];

    printf("Search by: 1. ID  2. Name\nChoose: ");
    int option;
    scanf("%d", &option);

    if (option == 1)
    {
        printf("Enter student ID: ");
        scanf("%d", &search_id);
        for (int i = 0; i < std_count; i++)
        {
            if (students[i].id == search_id)
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
        getchar();
        fgets(search_name, name_len, stdin);
        search_name[strcspn(search_name, "\n")] = '\0';
        for (int i = 0; i < std_count; i++)
        {
            if (strcmp(students[i].name, search_name) == 0)
            {
                printf(GREEN "Found: ID %d, Total: %d, Average: %.2f\n" RESET,
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
        printf(RED "Cannot open file for saving.\n" RESET);
        return;
    }

    fprintf(fp, "%d\n", std_count);
    for (int i = 0; i < std_count; i++)
    {
        fprintf(fp, "%s\n", students[i].name);
        for (int j = 0; j < sub; j++)
        {
            fprintf(fp, "%d ", students[i].marks[j]);
        }
        fprintf(fp, "\n%d\n%.2f\n%d\n",
                students[i].total, students[i].average, students[i].attendance);
    }

    fclose(fp);
    printf(GREEN "Data saved to file successfully.\n" RESET);
}

void loadFromFile()
{
    FILE *fp = fopen("students.txt", "r");
    if (!fp)
    {
        printf(RED "No saved data found.\n" RESET);
        return;
    }

    fscanf(fp, "%d\n", &std_count);
    for (int i = 0; i < std_count; i++)
    {
        fgets(students[i].name, name_len, fp);
        students[i].name[strcspn(students[i].name, "\n")] = '\0';

        for (int j = 0; j < sub; j++)
        {
            fscanf(fp, "%d", &students[i].marks[j]);
        }
        fgetc(fp); // newline

        fscanf(fp, "%d\n", &students[i].total);
        fscanf(fp, "%f\n", &students[i].average);
        fscanf(fp, "%d\n", &students[i].attendance);
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

        scanf("%d", &choice);
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
