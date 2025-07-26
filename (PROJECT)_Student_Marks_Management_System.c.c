#include <stdio.h>
#include <string.h>

#define name_len 30
#define sub 3
#define max_std 50

typedef struct
{
    char name[name_len];
    int marks[sub];
    int total;
    float average;
} Student;

Student students[max_std];
int std_count = 0;

void addStudent()
{
    if (std_count == max_std)
    {
        printf("Sorry, student list is full! Can't add more students.\n");
        return;
    }

    Student s;

    printf("\nEnter student name: ");
    getchar();
    fgets(s.name, name_len, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    s.total = 0;

    for (int i = 0; i < sub;)
    {
        int mark;
        printf("Enter marks for subject %d : ", i + 1);
        scanf("%d", &mark);

        if (mark >= 0 && mark <= 100)
        {
            s.marks[i] = mark;
            s.total += mark;
            i++;
        }
        else
        {
            printf("Invalid marks! Please enter a value between 0 and 100.\n");
        }
    }

    s.average = s.total / sub;

    students[std_count++] = s;

    printf("Student '%s' added successfully!\n", s.name);
}

void displayStudents()
{
    if (std_count == 0)
    {
        printf("\nNo students to display yet.\n");
        return;
    }

    printf("\nName\t\tMarks (Sub1 Sub2 Sub3)\tTotal\tAverage\n");
    printf("---------------------------------------------\n");

    for (int i = 0; i < std_count; i++)
    {
        printf("%s\t", students[i].name);
        for (int j = 0; j < sub; j++)
        {
            printf("%d ", students[i].marks[j]);
        }
        printf("Total: %d  Average: %.2f\n", students[i].total, students[i].average);
    }
}

void findTopper()
{
    if (std_count == 0)
    {
        printf("\nNo students added yet.\n");
        return;
    }

    int top = 0;
    for (int i = 1; i < std_count; i++)
    {
        if (students[i].total > students[top].total)
        {
            top = i;
        }
    }

    printf("\n--- Topper Details ---\n");
    printf("Name        : %s\n", students[top].name);
    printf("Total Marks : %d\n", students[top].total);
    printf("Average     : %.2f\n", students[top].average);
}

int main()
{
    int choice;
    while (1)
    {
        printf("\n=== Student Marks Management ===\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Find Topper\n");
        printf("4. Exit\n");
        printf("Choose an option (1-4): ");

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
            findTopper();
            break;
        case 4:
            printf("Goodbye! Exiting program.\n");
            return 0;
        default:
            printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    }

    return 0;
}
