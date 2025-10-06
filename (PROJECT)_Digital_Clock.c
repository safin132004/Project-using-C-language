#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

// ASCII color values
#define RESET       "\033[0m"
#define RED         "\033[1;31m"
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[1;33m"
#define BLUE        "\033[1;34m"
#define CYAN        "\033[1;36m"
#define MAGENTA     "\033[1;35m"

// Function to center text in 80-character width
void print_center(const char *text, const char *color) {
    int padding = (80 - strlen(text)) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

// Cross-platform screen clear
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Get user format input
int input_format() {
    int format;
    print_center("Choose the time format:", CYAN);
    print_center("1. 24-HOUR FORMAT", YELLOW);
    print_center("2. 12-HOUR FORMAT (Default)", YELLOW);
    printf("\n%sMake a choice (1/2): %s", BLUE, RESET);
    scanf("%d", &format);
    return format;
}

// Fill current time into buffer
void fill_time(char* buffer, int format) {
    time_t raw_time;
    struct tm *current_time;
    time(&raw_time);
    current_time = localtime(&raw_time);

    if (format == 1) {
        strftime(buffer, 50, "%H:%M:%S", current_time);
    } else {
        strftime(buffer, 50, "%I:%M:%S %p", current_time);
    }
}

// Fill current date into buffer
void fill_date(char* buffer) {
    time_t raw_time;
    struct tm *current_time;
    time(&raw_time);
    current_time = localtime(&raw_time);
    strftime(buffer, 100, "%A, %B %d, %Y", current_time);
}

int main() {
    char time_str[50], date_str[100];
    int format = input_format();

    while (1) {
        fill_time(time_str, format);
        fill_date(date_str);
        clear_screen();

        print_center("============================", CYAN);
        print_center("LIVE CLOCK DISPLAY", GREEN);
        print_center("============================", CYAN);
        printf("\n");
        print_center(time_str, YELLOW);
        print_center(date_str, MAGENTA);
        printf("\n");
        print_center("Press Ctrl + C to exit.", RED);

        sleep(1);  // Wait 1 second
    }

    return 0;
}
