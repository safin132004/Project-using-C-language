#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ASCII color value
#define RESET       "\033[0m"
#define RED         "\033[1;31m"
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[1;33m"
#define BLUE        "\033[1;34m"
#define CYAN        "\033[1;36m"
#define MAGENTA     "\033[1;35m"

// function for putting everything in middle
void print_center(const char *text, const char *color) {
    int padding = (80 - strlen(text)) / 2; // center assuming 80-char terminal
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

int main()
{
    int random, guess;
    int no_of_guess = 0;
    srand(time(NULL));

    print_center("Welcome to the world of Guessing Numbers", CYAN);

    random = rand() % 100 + 1;
    // unmount the line for seeing the generated number.
    // printf("Number generated is: %d\n", random);

    do {
        printf("\n%s Please enter your Guess between (1 to 100): %s", YELLOW, RESET);
        scanf("%d", &guess);
        no_of_guess++;

        if (guess < random) {
            print_center("Guess a larger number.", BLUE);
        }
        else if (guess > random) {
            print_center("Guess a smaller number.", MAGENTA);
        }
        else {
            char congrats[100];
            sprintf(congrats, "Congratulations! You guessed it in %d attempts.", no_of_guess);
            print_center(congrats, GREEN);
        }

    } while (guess != random);

    print_center("BYE BYE, Thanks for Playing.", RED);
    print_center("DEVELOPED BY SAFIN (COPYRIGHT: KGCoding).", CYAN);

    return 0;
}
