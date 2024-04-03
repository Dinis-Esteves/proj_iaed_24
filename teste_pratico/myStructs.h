#define DAYS_UP_TO_MONTH (int[]){0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}
#define TRUE 1
#define FALSE 0
#define DIM_Parks 20
#define DIM_PLATE 9
#define COMMAND_DIM BUFSIZ
#define CEILING_POS(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))

typedef struct {
    int day;
    int month;
    int year;
    int hours;
    int minutes;
} Date;

typedef struct Node_Exit Node_Exit;

struct Node_Exit {
    Date date;
    double payment;
    char *plate;
    Node_Exit *next;
    Node_Exit *previous;
};

typedef struct{
    Node_Exit *beginning;
    Node_Exit *final;
} List_Exits;

typedef struct{
    char* name;
    int capacity;
    float value_firstH;
    float value_H;
    float max_daily_value;
    int available_space;
    List_Exits Exits;
} Park;

typedef struct Node Node;

struct Node {
    Date date;
    Node *next;
    Node *previous;
};

typedef struct{
    Park *park;
    Node *begining;
    Node *final;
} List_Entries;

typedef struct{
    char plate[9];
    Date entry_date;
    Park *park;
    int num_entries;
    List_Entries **Entries;
} Car;

typedef struct{
    Park** All_Parks;
    Date current_time;
    int num_parks;
    char command[BUFSIZ];
    int num_cars;
    int hashtable_len;
    Car **hashtable;
} Main;

//Hashtable

unsigned int hash1(Car *c);

unsigned int hash2(Car *c);

void insert(Main *ctrl, Car *c);

int search(Main *ctrl, Car *c);

void resize(Main *ctrl);

//Date 

double Date_to_Hours(Date d);

double Subtract_Date(Date d_in, Date d_out);

Date Read_Date(char input[]);

void Print_Date(Date d);

int check_Date(Date d, Main *ctrl);

int Date_cmp(Date d1, Date d2);

int Date_cmp_command_f(Date d1, Date d2);

//Car

Car* Read_Car_Entry(char input[]);

int check_Plate(Car *c);

Date is_Parked(Car *c, Main *ctrl);

void add_entry(Car *car, Park *park);

void add_exit(Car *c, Park *park, double bill);

void free_node(Node *node);

void free_list_entries(List_Entries *entries);

//Park

Park* Read_Park(char input[]);

void free_park(Park *p);
// COMMANDS

//command p

void command_p(Main *ctrl);

int Aux_command_p(Main *ctrl, Park *new_park);
//command e

void command_e(Main *ctrl);

void free_commmand_e(char *park_name, Car *c);

void Aux_Command_e(Main *ctrl, Car *c, Park *p);

//commands s

void command_s(Main *ctrl);

void Aux_Command_s(Main *ctrl, Car *c, Park *p, Date entry);

//command v

void command_v(Main *Ctrl);

//command f

void command_f(Main *ctrl);

int check_Date_command_f(Date d);

void aux_command_f_oneArg(Park *p);

void aux_command_f_twoArg(Park *p, Date d);

//command r
void command_r(Main *ctrl);

void free_cars_parking(Park *p, Main *ctrl);

void print_parks_sorted(Main *ctrl);

// command u
void command_u(Main *ctrl);

// ON/OFF

void init(Main *ctrl);

void close(Main *ctrl);