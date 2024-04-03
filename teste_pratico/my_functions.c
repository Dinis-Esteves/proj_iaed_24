#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "myStructs.h"

// initializes the Main structure of the code
void init(Main *control_struct) {
    Park **All_Parks;
    Car **All_Cars;
    Date init = {1, 1, 1, 1, 1};
    control_struct->current_time = init;
    All_Parks = malloc(20*sizeof(Park*));
    All_Cars = calloc(101, sizeof(Car*));
    control_struct->hashtable=All_Cars;
    control_struct->All_Parks = All_Parks;
    control_struct->num_parks = 0;
    control_struct->num_cars = 0;
    control_struct->hashtable = All_Cars;
    control_struct->hashtable_len = 101;
}

// Converts Date structures to hours
double Date_to_Hours(Date d) {
    double hours = ((double)d.hours);
    
    hours += (double)d.minutes/60.0;
    
    hours += d.day * 24.0;
    
    hours += DAYS_UP_TO_MONTH[d.month - 1] * 24.0;

    hours += ((double)d.year * 365.0*24.0);
    
    return hours;
}

// Subtracts Date Structures
double Subtract_Date(Date d_in, Date d_out) {
    double time = 0.0;
    time = Date_to_Hours(d_out) - Date_to_Hours(d_in);
    return time;
}

// Reads an returns a Date Structure
Date Read_Date(char input[]) {
    Date d;
    sscanf(input, "%2d-%2d-%4d %2d:%2d\n", &d.day, &d.month, &d.year, &d.hours, &d.minutes);
    return d;
}

// Prints a Date Struct: day-month-year hours:minutes
void Print_Date(Date d) {
    printf("%02d-%02d-%d %02d:%02d", d.day, d.month, d.year, d.hours, d.minutes);
}

// checks if a Date Structure is valid
int check_Date(Date d, Main *ctrl) {
    if(d.year <= 0) {
        return FALSE;
    }
    if(d.month < 1 || d.month > 12) {
        return FALSE;
    }
    if(d.day < 1 || d.day > 31) {
        return FALSE;
    }
    if(d.day == 31 && (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11)) {
        return FALSE;
    }
    if(d.day > 28 && d.month == 2){
        return FALSE;
    }
    if(d.hours < 0 || d.hours > 23) {
        return FALSE;
    }
    if(d.minutes < 0 || d.minutes > 59) {
        return FALSE;
    }
    if (Subtract_Date(ctrl->current_time, d) < 0.0) {
        return FALSE;
    }

    return TRUE;
}

// compares 2 Date Sctrutres, returning TRUE if they are the same
int Date_cmp(Date d1, Date d2) {
    if (d1.day == d2.day && d1.hours == d2.hours && d1.minutes == d2.minutes && d1.month == d2.month && d1.year == d2.year) {
        return TRUE;
    }
    return FALSE;
}

// adds an exit to the Park logs
void add_exit(Car *c, Park *park, double bill) {
    Node_Exit *exit = malloc(sizeof(Node_Exit));

    exit->date = c->entry_date;
    exit->next = NULL;
    exit->payment = bill;
    exit->plate = c->plate;
    if (park->Exits.beginning == NULL) {
        park->Exits.beginning = exit;
        park->Exits.final = exit;
    } else {
        park->Exits.beginning->next = exit;
        exit->previous = park->Exits.beginning;
        park->Exits.beginning = exit;
    }
    
}

// adds an entry to the car log
void add_entry(Car *car, Park *park) {
    Node *node = malloc(sizeof(Node));

    for (int i = 0; i < car->num_entries; i++) {
        if (strcmp(car->Entries[i]->park->name, park->name) == 0) {
            node->next = NULL;
            node->previous = car->Entries[i]->begining;
            node->date = car->entry_date;
            car->Entries[i]->begining->next = node;
            car->Entries[i]->begining = node;
            return;
        }
    }

    List_Entries *new_entry = (List_Entries*) malloc(sizeof(List_Entries));
    new_entry->park = park;
    new_entry->begining = node;
    new_entry->final = node;
    node->next = NULL;
    node->previous = NULL;
    node->date = car->entry_date;

    car->Entries = (List_Entries**) realloc(car->Entries, (car->num_entries + 1) * sizeof(List_Entries*));
    car->Entries[car->num_entries] = new_entry;
    car->num_entries++;
}

// Reads an Retuns a Car Strucure
Car* Read_Car_Entry(char input[])  {
    Car *c;
    char date[17], placa[9], *park_name;
    park_name = malloc(BUFSIZ);
    c = malloc(sizeof(Car));
    c->num_entries = 0;
    c->Entries = malloc(sizeof(List_Entries));

    if (input[2] == '\"') {
        sscanf(input + 3, " %[^\"]\" %9s %17[^\n]", park_name, placa, date);
    } else {
        sscanf(input + 2, "%s %9s %17[^\n]", park_name, placa, date);
    }
    
    free(park_name);  

    strcpy(c->plate, placa);
    c->entry_date = Read_Date(date);
    return c;
}

// sorting alghoritm
void merge(Car *car, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    List_Entries *L[n1], *R[n2];

    for (i = 0; i < n1; i++)
        L[i] = car->Entries[l + i];
    for (j = 0; j < n2; j++)
        R[j] = car->Entries[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i]->park->name, R[j]->park->name) <= 0) {
            car->Entries[k] = L[i];
            i++;
        } else {
            car->Entries[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        car->Entries[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        car->Entries[k] = R[j];
        j++;
        k++;
    }
}


// sorting alghoritm
void mergeSort(Car *car, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(car, l, m);
        mergeSort(car, m + 1, r);

        merge(car, l, m, r);
    }
}

// prints all car entries in the right format
void print_Entries(Car *c) {
    int i = 0;
    Node *node;
    for (i = 0; i < c->num_entries; i++) {
            if (c->Entries[i]->park != NULL) {
                node = c->Entries[i]->final;
                while (node != NULL) {
                    printf("%s", c->Entries[i]->park->name); printf(" ");
                    Print_Date(node->date);
                    node = node->next;
                    if (node == NULL) {printf("\n"); break;}
                    else {printf(" "); Print_Date(node->date); printf("\n");}
                    node = node->next;
            }
        }
    }
}

// checks if a car's plate is valid
int check_Plate(Car *c) {
   int i, letterCount = 0, digitCount = 0;

    if(strlen(c->plate) != 8 || c->plate[2] != '-' || c->plate[5] != '-') {
        return FALSE;
    }

    for(i = 0; i < 8; i += 3) {
        if(isalpha(c->plate[i]) && isalpha(c->plate[i+1])) {
            if(isupper(c->plate[i]) && isupper(c->plate[i+1])) {
                letterCount++;
            } else {
                return FALSE;
            }
        } else if(isdigit(c->plate[i]) && isdigit(c->plate[i+1])) {
            digitCount++;
        } else {
            return FALSE;
        }
    }

    if(letterCount == 3) {
        return FALSE;
    }

    return TRUE; 
}

// reads and returns a Park Structure
Park* Read_Park(char input[]) {
    Park *p;
    p = malloc(sizeof(Park));
    p->Exits.final = NULL;
    p->Exits.beginning = NULL;
    p->name = malloc(BUFSIZ);

    // case for the more then one word names
    if (input[0] == '\"') {
        sscanf(input + 1, "%[^\"]\" %d %f %f %f", p->name, &p->capacity, &p->value_firstH, &p->value_H, &p->max_daily_value);
        
    // case for the one word names
    } else {
        sscanf(input, "%s %d %f %f %f", p->name, &p->capacity, &p->value_firstH, &p->value_H, &p->max_daily_value);
    }
    p->name = (char *) realloc(p->name, strlen(p->name) + 1);
    p->available_space = p->capacity;
    return p;
}

// Calculates the ammount of money a car needs to pay when it leaves the park
double Calculate_Value(Date d_in, Date d_out, Park *p) {
    double time_stayed = Subtract_Date(d_in, d_out);
    int days = (int)(time_stayed / 24);
    double price = days * p->max_daily_value;
    int leftover_quarters = CEILING_POS(((time_stayed - (days * 24.)) * 4.));
    double aux;
 
    if (leftover_quarters <= 4) {

        aux = CEILING_POS(leftover_quarters) * p->value_firstH;
    } else {

        aux = 4 * p->value_firstH + (CEILING_POS(leftover_quarters) - 4) * p->value_H;
    }

    if (aux > p->max_daily_value) {
        aux = p->max_daily_value;
    }
    
    return price + aux;
}

// Checks is a car is parked
Date is_Parked(Car *c, Main *ctrl) {

    int index = search(ctrl, c);
    Date null = {0, 0, 0, 0, 0};
    
    if (index == -1 || ctrl->hashtable[index] == NULL || ctrl->hashtable[index]->park == NULL) {
            return null;
    } else {
            return ctrl->hashtable[index]->entry_date;
    }

}

// free's the command_e stuff
void free_command_e(char *park_name, Car *c) {
    free(park_name); 
    free(c->Entries);
    free(c);
}

// turns a Car Struct into hash
unsigned int hash1(Car *c) {
    unsigned int hash = 0, i;
    for (i = 0; i < strlen(c->plate); i++) {
        hash = (hash * 31 + c->plate[i]);
    }
    return hash ;
}

// turns a Car Struct into hash
unsigned int hash2(Car *c) {
    unsigned int hash = 0;
    char *p = c->plate;
    while (*p) {
    hash = (hash << 5) ^ (hash >> 27) ^ (*p++);
    }
    return hash;    
}

// inserts a Car in the hash table
void insert(Main *ctrl, Car *c) {
    int index = hash1(c), i = 0;
    while (ctrl->hashtable[(index + (i * hash2(c))) % ctrl->hashtable_len] != NULL) {
        i++;
    }
    ctrl->hashtable[(index + (i * hash2(c))) % ctrl->hashtable_len] = c;
    if (ctrl->num_cars + 1 >= (ctrl->hashtable_len / 2)) {
        resize(ctrl);
    }
}

// resizes the hash table
void resize(Main *ctrl) {
    int old_size = ctrl->hashtable_len;
    Car **old_hashtable = ctrl->hashtable;
    ctrl->hashtable_len *= 2;
    ctrl->hashtable = calloc(ctrl->hashtable_len, sizeof(Car*));
    for (int i = 0; i < old_size; i++) {
        if (old_hashtable[i] != NULL) {
            insert(ctrl, old_hashtable[i]);
        }
    }
    free(old_hashtable);
}

// retuns the index of a Car in the hashtable, returns -1 if not find
int search(Main *ctrl, Car *c) {
    int index = hash1(c), i = 0;
    while (TRUE) {
        if (ctrl->hashtable[(index + (i * hash2(c))) % ctrl->hashtable_len] == NULL) {
            return -1;
        }
        if (strcmp(ctrl->hashtable[(index + (i * hash2(c))) % ctrl->hashtable_len]->plate, c->plate) == 0) {
            return ((index + (i * hash2(c))) % ctrl->hashtable_len);
        }
        i++;
    }
    return -1;
}

// Auxiliar function to the command_e function
void Aux_Command_e(Main *ctrl, Car *c, Park *p) {
    int index;
    index = search(ctrl, c);

    if (index == -1) {
        c->park = p;
        insert(ctrl, c);
        index = search(ctrl, c);
        ctrl->current_time = c->entry_date;
        add_entry(c, p);
    } else {
        ctrl->hashtable[index]->park = p;
        ctrl->hashtable[index]->entry_date = c->entry_date;
        ctrl->current_time = c->entry_date;
        free(c->Entries);
        free(c);
        add_entry(ctrl->hashtable[index], p);
    }
    p->available_space -= 1;
    printf("%s %d\n", p->name, p->available_space);
    ctrl->num_cars += 1;
}

// Auxiliar function to the command_s function
void Aux_Command_s(Main *ctrl, Car *c, Park *p, Date entry) {
    double bill;
    int index;
    Date test = c->entry_date;
    index = search(ctrl, c);
    ctrl->hashtable[index]->park = NULL;
    bill = Calculate_Value(entry, c->entry_date, p);
    printf("%s ", c->plate);
    Print_Date(entry); printf(" ") ;Print_Date(c->entry_date); printf(" %.2f\n", bill);
    p->available_space += 1;
    ctrl->current_time = c->entry_date;
    ctrl->hashtable[index]->entry_date = test;
    add_entry(ctrl->hashtable[index], p);
    add_exit(ctrl->hashtable[index], p, bill);
    free(c->Entries);
    free(c);
}

// processes a Car entry
void command_e(Main *ctrl) {
    char *input = ctrl->command, *park_name = malloc(BUFSIZ);
    int i, checked = 0;
    Date null = {0, 0, 0, 0, 0};
    Car *c = Read_Car_Entry(input);
    if (input[2] == '\"') { 
        sscanf(input + 3, "%[^\"]\"", park_name);
    } else {
        sscanf(input + 2, "%s", park_name);
    }

    if (ctrl->num_parks == 0) {printf("%s: no such parking.\n", park_name); free_command_e(park_name, c); return;}
    if (Date_cmp(is_Parked(c, ctrl), null) == FALSE && ctrl->hashtable[search(ctrl, c)]->park != NULL) {
        printf("%s: invalid vehicle entry.\n", c->plate); free_command_e(park_name, c); return;}
    if (check_Plate(c) == FALSE) {printf("%s: invalid licence plate.\n", c->plate); free_command_e(park_name, c); return;}
    if (check_Date(c->entry_date, ctrl) == FALSE) {printf("invalid date.\n"); free_command_e(park_name, c); return;}

    for (i = 0; i < ctrl->num_parks; i++) {
        if (strcmp(park_name, ctrl->All_Parks[i]->name) == 0) {
            if (ctrl->All_Parks[i]->available_space == 0) {printf("%s: parking is full.\n", park_name); free_command_e(park_name, c); return;};
            Aux_Command_e(ctrl, c, ctrl->All_Parks[i]);
            free(park_name);   
            return;

        } else  {
            checked += 1;
            }
        if (checked == ctrl->num_parks) {
            printf("%s: no such parking.\n", park_name); free_command_e(park_name, c); return;
        } 
    }
    return;
}

// processes a Car Exit
void command_s(Main *ctrl) {
    char *input = ctrl->command, *park_name = malloc(BUFSIZ);
    int i, checked = 0;
    Car *c = Read_Car_Entry(input);
    Date entry = {0,0,0,0,0}, null = {0, 0, 0, 0, 0};
    
    if (input[2] == '\"') {
        sscanf(input + 3, "%[^\"]\"", park_name);
    } else {
        sscanf(input + 2, "%s", park_name);
    }

    if (ctrl->num_parks == 0) {printf("%s: no such parking.\n", park_name); free_command_e(park_name, c); return;}
    entry = is_Parked(c, ctrl);
    if (check_Plate(c) == FALSE) {printf("%s: invalid licence plate.\n", c->plate);free_command_e(park_name, c); return;}
    if (Date_cmp(entry, null) == TRUE) {printf("%s: invalid vehicle exit.\n", c->plate); free_command_e(park_name, c); return;}
    if (check_Date(c->entry_date, ctrl) == FALSE) {printf("invalid date.\n"); free_command_e(park_name, c); return;}
    
    for (i = 0; i < ctrl->num_parks; i++) {
        if (strcmp(park_name, ctrl->All_Parks[i]->name) == 0) {
            Aux_Command_s(ctrl, c, ctrl->All_Parks[i], entry);
            free(park_name);
            return;
        } else  {
            checked += 1;
            }
    }

    if (checked == ctrl->num_parks) {
        printf("%s: no such parking.\n", park_name); free_command_e(park_name, c); return;
    } 
}

// Shows all the entries a Car did on all Parks 
void command_v(Main *ctrl) {
    Car *c = malloc(sizeof(Car));
    
    sscanf(ctrl->command + 2, "%s", c->plate);
    int index = search(ctrl, c);
    
    if (check_Plate(c) == FALSE) {
        printf("%s: invalid licence plate.\n", c->plate);
        free(c);
        return;
    } else if (index == -1 || ctrl->hashtable[index]->num_entries == 0) {
        printf("%s: no entries found in any parking.\n", c->plate);
        free(c);
        return;
    } else {
        mergeSort(ctrl->hashtable[index], 0, ctrl->hashtable[index]->num_entries - 1);
        print_Entries(ctrl->hashtable[index]);
    }
    free(c);
}

// Compares 2 Date Strucures not caring to the time, just the date
int Date_cmp_command_f(Date d1, Date d2) {
    if (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year) 
        return TRUE;
    return FALSE;
}

// checks Dates not caring to the actual time 
int check_Date_command_f(Date d) {
    if(d.year <= 0) {
        return FALSE;
    }
    if(d.month < 1 || d.month > 12) {
        return FALSE;
    }
    if(d.day < 1 || d.day > 31) {
        return FALSE;
    }
    if(d.day == 31 && (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11)) {
        return FALSE;
    }
    if(d.day > 28 && d.month == 2){
        return FALSE;
    }
    return TRUE;
}

// processes the command_f in the one argument case
void aux_command_f_oneArg(Park *p) {
    double bill = 0;
    Node_Exit *Current_Exit = p->Exits.final;
    Date current_date;

    
    while (Current_Exit != NULL) {
        current_date = Current_Exit->date;
        while (Current_Exit != NULL && Date_cmp_command_f(Current_Exit->date, current_date)) {
            bill += Current_Exit->payment;
            Current_Exit = Current_Exit->next;
        }
        printf("%02d-%02d-%d ", current_date.day, current_date.month, current_date.year);
        printf("%.2f\n", bill);
        bill = 0;
    }
}

// processes the command_f in the two argument case
void aux_command_f_twoArg(Park *p, Date d) {
    Node_Exit *Current_Exit = p->Exits.final;
    int Entered = FALSE;
    while (Current_Exit != NULL) {
        while (Current_Exit != NULL && Date_cmp_command_f(Current_Exit->date, d)) {
            Entered = TRUE;
            printf("%s %02d:%02d %.2f\n", Current_Exit->plate ,Current_Exit->date.hours, Current_Exit->date.minutes, Current_Exit->payment);
            Current_Exit = Current_Exit->next;
        }
        if (Entered == TRUE) 
            break;
        Current_Exit = Current_Exit->next;
    }
}

// Shows all Cars Exits
void command_f(Main *ctrl) {
    char *input = ctrl->command, *park_name = malloc(sizeof(char) * COMMAND_DIM), *date = calloc(sizeof(char), 11);
    int i, index = -1, test;


    if (input[2] == '\"') {
        test = sscanf(input + 3, "%[^\"]\" %s", park_name, date);
    } else {
        test = sscanf(input + 2, "%s %s", park_name, date);
    }
    for (i = 0; i < ctrl->num_parks; i++) {
        if (strcmp(park_name, ctrl->All_Parks[i]->name) == 0) {
            index = i;
            break;
        }
    }

    if (i == ctrl->num_parks && index == -1) {
        printf("%s: no such parking.\n", park_name);
        return;
    }
    //one argument case
    if (test == 1)  {
        aux_command_f_oneArg(ctrl->All_Parks[index]);
    } 
    //two argument case
    else {
        if (check_Date_command_f(Read_Date(date)) == FALSE) {
            printf("invalid date.\n");
            return;
        }
        aux_command_f_twoArg(ctrl->All_Parks[index], Read_Date(date));
    }
    free(date);
    free(park_name);
}

void command_u(Main *ctrl) {
    int i;
    double value = 0.00;
    Car *c = malloc(sizeof(Car));
    Node_Exit *Current = malloc(sizeof(Node_Exit));
    sscanf(ctrl->command + 2, "%s", c->plate);
    
    if (check_Plate(c) == FALSE) {
        printf("%s: invalid licence plate.\n", c->plate);
        free(c);
        free(Current);
        return;
    }

    for (i = 0; i < ctrl->num_parks; i++) {
        Current = ctrl->All_Parks[i]->Exits.final;
        while (Current != NULL) {
            if (strcmp(Current->plate, c->plate) == 0) {
                value += Current->payment;
            }
            Current = Current->next;
        }    
    }
    free(c);
    free(Current);
    printf("%.2f\n", value); 
}

// Auxiliar function to command_p function
int Aux_command_p(Main *ctrl, Park *new_park) {
    int i;
    //num parks verification
        if (ctrl->num_parks >= 20) {
            printf("too many parks.\n");
            free(new_park->name); 
            free(new_park);  
            return FALSE;                    
        }
        //duplicate verification
        for (i = 0; i < ctrl->num_parks; i++) {
            if (strcmp(new_park->name, ctrl->All_Parks[i]->name) == 0) {
                printf("%s: parking already exists.\n", new_park->name);
                free(new_park->name);
                free(new_park);
                return FALSE;
            }
        }
        //capacity verification
        if (new_park->capacity <= 0) {
            printf("%d: invalid capacity.\n", new_park->capacity);
            free(new_park->name);   
            free(new_park);
            return FALSE;
        }
        //cost verification                    
        if ((new_park->value_firstH <= 0 || new_park->value_H <= 0 ||
        new_park->max_daily_value <=0 || new_park->value_firstH > new_park->value_H
        || new_park->value_H > new_park->max_daily_value)) {
            printf("invalid cost.\n");
            free(new_park->name);   
            free(new_park);
            return FALSE;
        }
    return TRUE;
}

// processes a Park Creation
void command_p(Main *ctrl) {
    int i;
    unsigned long j;
    Park *new_park;
    //if the command is single p
    if (strlen(ctrl->command) == 2) {
        for (i = 0 ;i < ctrl->num_parks; i++) {
            printf("%s %d %d\n", ctrl->All_Parks[i]->name, ctrl->All_Parks[i]->capacity, ctrl->All_Parks[i]->available_space);
        }
    } 
    // if the command is p + Arguments
    else {
        new_park = Read_Park(ctrl->command + 2);
        for (j = 0; j < strlen(new_park->name); j++) {
            if (isdigit(new_park->name[j])) {
                printf("invalid parking name.\n");
                free(new_park->name);
                free(new_park);
                return;
            }
        }
        if (Aux_command_p(ctrl, new_park) == FALSE) return;
        //Adding the new park to our park vector
            ctrl->All_Parks[ctrl->num_parks] = new_park;
            ctrl->num_parks++;
    }
    return;
}

// prints all the existing parks sorted by ther name
void print_parks_sorted(Main *ctrl) {
    Park **temp_array = malloc(ctrl->num_parks * sizeof(Park *));
    for (int i = 0; i < ctrl->num_parks; i++) {
        temp_array[i] = ctrl->All_Parks[i];
    }

    for (int i = 0; i < ctrl->num_parks; i++) {
        for (int j = 0; j < ctrl->num_parks - i - 1; j++) {
            if (strcmp(temp_array[j]->name, temp_array[j + 1]->name) > 0) {
                Park *temp = temp_array[j];
                temp_array[j] = temp_array[j + 1];
                temp_array[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < ctrl->num_parks; i++) {
        printf("%s\n", temp_array[i]->name);
    }

    free(temp_array);
}

// deletes a park
void command_r(Main *ctrl) {
    char *input = ctrl->command, *park_name = malloc(sizeof(char) * COMMAND_DIM);
    int i, index = -1;
    
    if (input[2] == '\"') {
        sscanf(input + 3, "%[^\"]\"", park_name);
    } else {
        sscanf(input + 2, "%s", park_name);
    }
    for (i = 0 ;i < ctrl->num_parks; i++) {
        if (strcmp(ctrl->All_Parks[i]->name, park_name) == 0) { 
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("%s: no such parking.\n", park_name);
        free(park_name);
        return;
    }
    free_park(ctrl->All_Parks[index]);
    free_cars_parking(ctrl->All_Parks[index], ctrl);
    for (int i = index; i < ctrl->num_parks - 1; i++) {
        ctrl->All_Parks[i] = ctrl->All_Parks[i + 1];
    }
    ctrl->num_parks -= 1;

    print_parks_sorted(ctrl);
    free(park_name); 
}

// Frees all nodes in a linked list
void free_node(Node *node) {
    if (node != NULL) {
        free_node(node->previous);
        free(node);
    }
}

// Frees all Node_Exit in a linked list
void free_Node_Exits(Park *park) {
    Node_Exit *current_node = park->Exits.final;
    Node_Exit *next_node;
    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }

    park->Exits.beginning = NULL;
    park->Exits.final = NULL;
}

// Frees a List_Entrie
void free_list_entries(List_Entries *entries) {
    if (entries != NULL) {
        free_node(entries->begining);
        free(entries);
    }
}

// Frees a Park
void free_park(Park *p) {
    free_Node_Exits(p);
    free(p->name);
    free(p);
}

// deletes all the entries of a deleted park
void free_cars_parking(Park *p, Main *ctrl) {
    int i, j, k;
    for (i = 0; i < ctrl->hashtable_len; i++) {
        if (ctrl->hashtable[i] != NULL) {
            Car *car = ctrl->hashtable[i];
            if (car->park == p) 
                car->park = NULL;
            for (j = 0; j < car->num_entries; j++) {
                if (car->Entries[j]->park == p) {
                    // Free the nodes in the Entries list
                    Node *node = car->Entries[j]->final;
                    while (node != NULL) {
                        Node *next_node = node->next;
                        free(node);
                        node = next_node;
                    }
                    free(car->Entries[j]);
                    for (k = j; k < car->num_entries - 1; k++) {
                        car->Entries[k] = car->Entries[k + 1];
                    }
                    car->num_entries -= 1;
                    j--;
                }
            }
        }
    }
}

// closes the program freeing all the used stuff
void close(Main *ctrl) {
    int i, j;
    for (i = 0; i < ctrl->num_parks; i++) {
        //set the momry occupied by the parking lots free
        free_park(ctrl->All_Parks[i]);
    }
    
    //set the momry occupied by the Parks free
    free(ctrl->All_Parks);
    for (i = 0; i < ctrl->hashtable_len; i++) {
        if (ctrl->hashtable[i] != NULL) {
            for (j = 0; j < ctrl->hashtable[i]->num_entries; j++) {
                free_list_entries(ctrl->hashtable[i]->Entries[j]);
            }
            free(ctrl->hashtable[i]->Entries);
            free(ctrl->hashtable[i]);
        }
    }
    free(ctrl->hashtable);
    free(ctrl);
    return;

}
