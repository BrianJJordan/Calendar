#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define MAX_CAPTION_LENGTH 100
#define MAX_EVENTS 100


typedef struct {
    int day;
    int month;
    int year;
    char caption[MAX_CAPTION_LENGTH];
} Event;


bool leap_year(int year) 
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)); //Check for leap year
}


int days_in_month(int month, int year) 
{
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (leap_year(year) && month == 2) 
    {
        return 29;
    }
    return days[month - 1];
}


int day_of_week(int day, int month, int year) //Zeller's Congruence below
{    
    if (month < 3) 
    {
        year--;
        month += 12;
    }

    int k = year % 100;
    int j = year / 100;
    int secondterm = 13 * (month + 1) / 5;   
    int yr_div = k / 4;                     //  K/4 from Zeller's congruence
    int cnty_div = j / 4;                  //   J/4 from Zeller's congruence
    int sub = (2 * j);                    // 2*J from Zeller's congruence

    int h = (day + secondterm + k + yr_div + cnty_div - sub) % 7;

    h = (h + 5) % 7 + 1;

    return h;
}


void month_calendar(int month, int year) 
{
    char *month_names[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    int start_day = day_of_week(1, month, year);
    int days = days_in_month(month, year);

    int calendar_date[43] = {0};
    int array_index;
    int one = 1;
    int row_cnt = 1;
    int column_cnt = 1;

    array_index = start_day;
    while (array_index <= 42 && one <= days) 
    {
        calendar_date[array_index] = one;
        array_index++;
        one++;
    }

    printf("\n  %s %d\n", month_names[month - 1], year);
    printf("  Sun   Mon   Tue   Wed   Thu   Fri   Sat\n");

    for (int row = 1; row <= 6; row++) {
        column_cnt = 1;
        array_index = 7 * (row - 1);

        while (column_cnt < 8) {
            if (calendar_date[array_index] == 0) {
                printf("      ");
            } else if (calendar_date[array_index] < 10) {
                printf("    %d ", calendar_date[array_index]);
            } else {
                printf("   %d ", calendar_date[array_index]);
            }

            array_index++;
            column_cnt++;
        }

        printf("\n\n");

        if (calendar_date[array_index] == 0) {
            break;
        }
    }
}

void print_year_calendar(int year) 
{
    for (int month = 1; month <= 12; month++) 
    {
        month_calendar(month, year);
    }
}

void full_calendar (){
    int year, month;
    char month_input[20];

    printf("Enter the year: ");
    scanf("%d", &year);

    while (getchar() != '\n');

    printf("Enter the month (1-12) or 'all': ");
    fgets(month_input, sizeof(month_input), stdin);

    // Replace the newline character with a null terminator
    for (int i = 0; i < strlen(month_input); i++) {
        if (month_input[i] == '\n') {
            month_input[i] = '\0';
            break;
        }
    }

    if (strcmp(month_input, "all") == 0) {
        print_year_calendar(year);
    } else {
        sscanf(month_input, "%d", &month);

        if (month >= 1 && month <= 12) {
            month_calendar(month, year);
        } else {
            printf("Invalid month input. Please enter a valid month (1-12) or 'all'.\n");
        }
    }
    
}


void create_file() {
    char filename[50];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error creating file!\n");
        exit(1);
    }
    fclose(fp);
}

void add_event() {
    char filename[50];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    int num_events;
    printf("How many events do you want to add? ");
    scanf("%d", &num_events);
    Event events[MAX_EVENTS];
    for (int i = 0; i < num_events; i++) {
        printf("Event %d:\n", i + 1);
        printf("Date (day month year): ");
        scanf("%d %d %d", &events[i].day, &events[i].month, &events[i].year);
        printf("Caption: ");
        scanf(" %[^\n]", events[i].caption); // note the space before % to skip leading whitespace
        fprintf(fp, "%d/%d/%d - %s\n", events[i].day, events[i].month, events[i].year, events[i].caption);
    }
    fclose(fp);
}



void del_event() {
    char filename[50];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    Event events[MAX_EVENTS];
    int num_events = 0;
    while (fscanf(fp, "%d/%d/%d - %[^\n]", &events[num_events].day, &events[num_events].month, &events[num_events].year, events[num_events].caption) != EOF) {
        num_events++;
    }
    fclose(fp);

    if (num_events == 0) {
        printf("No events found in file.\n");
        return;
    }

    printf("Events in file '%s':\n", filename);
    for (int i = 0; i < num_events; i++) {
        printf("%d. %d/%d/%d - %s\n", i + 1, events[i].day, events[i].month, events[i].year, events[i].caption);
    }

    int event_num;
    printf("Enter the number of the event you want to delete: ");
    scanf("%d", &event_num);
    if (event_num < 1 || event_num > num_events) {
        printf("Invalid event number.\n");
        return;
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i = 0; i < num_events; i++) {
        if (i + 1 != event_num) {
            fprintf(fp, "%d/%d/%d - %s\n", events[i].day, events[i].month, events[i].year, events[i].caption);
        }
    }
    fclose(fp);

    printf("Event %d deleted successfully from file '%s'.\n", event_num, filename);
}

void edit_event() {
    char filename[50];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    int num_events = 0;
    Event events[MAX_EVENTS];
    char line[MAX_CAPTION_LENGTH];
    while (fgets(line, MAX_CAPTION_LENGTH, fp) != NULL) {
        sscanf(line, "%d/%d/%d - %[^\n]", &events[num_events].day, &events[num_events].month, &events[num_events].year, events[num_events].caption);
        num_events++;
    }
    fclose(fp);

    printf("Which event would you like to edit? (Enter the number): ");
    int index_to_edit;
    scanf("%d", &index_to_edit);
    if (index_to_edit < 1 || index_to_edit > num_events) {
        printf("Invalid event index!\n");
        exit(1);
    }

    printf("Enter new date (day month year): ");
    scanf("%d %d %d", &events[index_to_edit-1].day, &events[index_to_edit-1].month, &events[index_to_edit-1].year);
    printf("Enter new caption: ");
    scanf(" %[^\n]", events[index_to_edit-1].caption);

    fp = fopen(filename, "w");
    for (int i = 0; i < num_events; i++) {
        if (i == index_to_edit-1) {
            fprintf(fp, "%d/%d/%d - %s\n", events[i].day, events[i].month, events[i].year, events[i].caption);
        } else {
            fprintf(fp, "%d/%d/%d - %s\n", events[i].day, events[i].month, events[i].year, events[i].caption);
        }
    }
    fclose(fp);
    printf("Event updated successfully!\n");
}

void view_events() {
    char filename[50];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    int choice;
    printf("View events for:\n");
    printf("1. Entire year\n");
    printf("2. Specific month in a year\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    int year, month;
    if (choice == 1) {
        printf("Enter year: ");
        scanf("%d", &year);
    } else if (choice == 2) {
        printf("Enter month (1-12): ");
        scanf("%d", &month);
        printf("Enter year: ");
        scanf("%d", &year);
    } else {
        printf("Invalid choice!\n");
        exit(1);
    }
    
    Event events[MAX_EVENTS];
    int num_events = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (num_events == MAX_EVENTS) {
            printf("Max number of events reached!\n");
            break;
        }
        if (sscanf(line, "%d/%d/%d - %[^\n]", &events[num_events].day, &events[num_events].month, &events[num_events].year, events[num_events].caption) != 4) {
            printf("Error reading event from file!\n");
            continue;
        }
        if (choice == 1 && events[num_events].year == year) {
            num_events++;
        } else if (choice == 2 && events[num_events].year == year && events[num_events].month == month) {
            num_events++;
        }
    }
    fclose(fp);
    
    printf("Found %d events:\n", num_events);
    for (int i = 0; i < num_events; i++) {
        printf("%d/%d/%d - %s\n", events[i].day, events[i].month, events[i].year, events[i].caption);
    }
}



int main() {
    
    
    int choice;
    
    
    while (true){
        
        printf("Choose an option 1-6:\n\n1. Create a new file\n2. Add an event\n3. Delete events\n4. Edit events\n5. View events\n6. View Calendar\n\nOption: ");
        scanf("%i", &choice);
        if(choice == 0){
            break;
        }
        else if(choice == 1){
            create_file();
        }
        else if(choice == 2){
            add_event();
        }
        else if(choice == 3){
            del_event();
        }
        else if(choice == 4){
            edit_event();
        }
        else if(choice == 5){
            view_events();
        }
        else if(choice == 6){
            full_calendar();
        }
    
    }
    return 0;
}
