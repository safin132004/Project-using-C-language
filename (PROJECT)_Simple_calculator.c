#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#define RESET       "\033[0m"
#define RED         "\033[1;31m"
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[1;33m"
#define BLUE        "\033[1;34m"
#define MAGENTA     "\033[1;35m"
#define CYAN        "\033[1;36m"
#define WHITE       "\033[1;37m"
#define BOLD        "\033[1m"
#define REVERSE     "\033[7m"  // Reverse video for highlighting

// Function declarations
void print_menu(int selected);
void print_center(const char *text, const char *color);
int get_terminal_width();
int visible_length(const char *text);
int cursor_menu_select();

// Calculator operations
double division(double, double);
double modulus(int, int);

int main()
{
    int choice;
    double first, second, result;

    while (1)
    {
        system("clear");

        // ASCII Art Title
        print_center("============================================", CYAN);
        print_center("    SIMPLE CALCULATOR - BY SAFIN", GREEN);
        print_center("============================================", CYAN);

        // Cursor-based menu selection
        choice = cursor_menu_select();

        if (choice == 7)
        {
            print_center("Thank you for using the calculator!", GREEN);
            print_center("Developed by SAFIN (KGCoding)", CYAN);
            break;
        }

        printf(BLUE "\nEnter the first number: " RESET);
        scanf("%lf", &first);
        printf(BLUE "Enter the second number: " RESET);
        scanf("%lf", &second);

        switch (choice)
        {
        case 1:
            result = first + second;
            break;
        case 2:
            result = first - second;
            break;
        case 3:
            result = first * second;
            break;
        case 4:
            result = division(first, second);
            break;
        case 5:
            result = modulus((int)first, (int)second);
            break;
        case 6:
            result = pow(first, second);
            break;
        default:
            result = NAN; // Invalid operation
            break;
        }

        printf("\n");
        if (!isnan(result))
        {
            char result_msg[100];
            sprintf(result_msg, "✅ Result of the operation: %.2f", result);
            print_center(result_msg, GREEN);
        }
        else
        {
            print_center("❌ Operation could not be performed due to invalid input.", RED);
        }

        printf(YELLOW "\nPress Enter to continue..." RESET);
        while(getchar() != '\n'); // clear input buffer
        getchar(); // wait for Enter
    }

    return 0;
}

double division(double a, double b)
{
    if (b == 0)
    {
        print_center(" Division by zero is not allowed.", RED);
        return NAN;
    }
    return a / b;
}

double modulus(int a, int b)
{
    if (b == 0)
    {
        print_center(" Modulus by zero is not allowed.", RED);
        return NAN;
    }
    return a % b;
}

// Prints the menu with a highlight on the selected item
void print_menu(int selected)
{
    // Clear screen before printing menu
    printf("\033[H\033[J");

    print_center("============================================", CYAN);
    print_center("    SIMPLE CALCULATOR - BY SAFIN", GREEN);
    print_center("============================================", CYAN);
    print_center("Select an operation:", WHITE);

    const char *items[] = {
        "1. ADD",
        "2. SUBTRACTION",
        "3. MULTIPLY",
        "4. DIVIDE",
        "5. MODULUS",
        "6. POWER",
        "7. EXIT"
    };

    int count = sizeof(items) / sizeof(items[0]);

    for (int i = 0; i < count; i++)
    {
        char line[50];
        snprintf(line, sizeof(line), "%s", items[i]);

        if (i == selected)
        {
            // Highlight selected line using reverse video
            int width = get_terminal_width();
            int pad = (width - visible_length(line)) / 2;
            for (int p = 0; p < pad; p++) printf(" ");
            printf("%s%s%s\n", REVERSE, line, RESET);
        }
        else
        {
            print_center(line, MAGENTA);
        }
    }
}

// Dynamically gets the terminal width
int get_terminal_width()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col > 0 ? w.ws_col : 80;
}

// Calculates visible length ignoring ANSI escape codes
int visible_length(const char *text)
{
    int len = 0, in_escape = 0;
    for (int i = 0; text[i]; ++i)
    {
        if (text[i] == '\033')
            in_escape = 1;
        else if (in_escape && text[i] == 'm')
            in_escape = 0;
        else if (!in_escape)
            len++;
    }
    return len;
}

// Centered text with color support
void print_center(const char *text, const char *color)
{
    int width = get_terminal_width();
    int pad = (width - visible_length(text)) / 2;
    for (int i = 0; i < pad; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

// Reads arrow keys and enter to select menu item
int cursor_menu_select()
{
    struct termios orig_term, new_term;
    tcgetattr(STDIN_FILENO, &orig_term);
    new_term = orig_term;
    new_term.c_lflag &= ~(ICANON | ECHO); // disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    int selected = 0;
    int count = 7;

    print_menu(selected);

    while (1)
    {
        char c;
        ssize_t nread = read(STDIN_FILENO, &c, 1);
        if (nread <= 0) continue;

        if (c == '\033') // Escape sequence (arrow keys)
        {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[')
            {
                if (seq[1] == 'A') // Up arrow
                {
                    selected = (selected - 1 + count) % count;
                    print_menu(selected);
                }
                else if (seq[1] == 'B') // Down arrow
                {
                    selected = (selected + 1) % count;
                    print_menu(selected);
                }
            }
        }
        else if (c == '\n' || c == '\r') // Enter key
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &orig_term); // restore terminal
            return selected + 1; // menu options 1-based
        }
        else if (c == 'q' || c == 'Q') // optional quit with q
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
            return 7; // exit option
        }
    }
}
