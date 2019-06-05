//
// Chicago CTA Ridership Analysis: top-5 and bottom-5 stations, with search. //
// Srinivas C Lingutla
// UIN: 655115444
// MAC OS 10.11.5 , XCODE
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #02
//





#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct to store the stations info
typedef struct Stations
{
    int station_id;
    char name[255];
    char handicap_accessable[10];
    double lattitude;
    double longitude;
    long long int totalrides;
    int weekday;
    int sunday;
    int saturday;
}Stations;

//Struct/linkedlist to store the ridership info
typedef struct ridership
{
    int station_id;
    char date[10];
    char datatype[2];
    int rides;
    struct ridership *next;
    
}ridership;



void binary_search(int station_id , int rides , char datatype[], int length, long long int *totalridershiprides , Stations *stat);

//****************************************************************************************
//input ridership data from file into linked lists
struct ridership *InputRiderShip(FILE *file, int length , Stations *stat){
    
    long long int totalridershiprides = 0;
    ridership *current, *head;
    head = current = NULL;
    
    //Store the dates
    char firstdate[15];
    char lastdate[15];
    
    char str[350];
    
    //get the first line
    fgets(str, 350, file);
    
    int counter = 0;
    while (fgets(str, 350, file)){
        
        ridership *node  = (ridership *)malloc(sizeof(ridership));
        
        //printf("%s", str);
        
        char *token;
        token = strtok(str, ",");
        node->station_id = atoi(str);
        
        token = strtok(NULL, ",");
        strcpy(node->date, token);
        
        
        
        token = strtok(NULL, ",");
        strcpy(node->datatype, token);
        
        
        token = strtok(NULL, ",");
        node->rides = atoi(token);
        strtok(str, "\n");
        //update the counter for total number of ridership data
        counter++;
        binary_search(node->station_id , node->rides, node->datatype, length, &totalridershiprides , stat);
        
        //get the first date
        if(counter==1)
            strcpy(firstdate, node->date);
        
        //get the last date
        strcpy(lastdate, node->date);
        
        if(head == NULL){
            current = head = node;
        } else {
            current = current->next = node;
        }
    }
    
    printf(">> Number of Ridership data points: %d\n", counter );
    printf(">> Date Range:  %s  -  %s\n\n\n", firstdate, lastdate);
    printf(">> Total Riders:  %lld\n", totalridershiprides);
    return head;
}

//****************************************************************************************


//binary search to input the rides info from linked lists to structs.
void binary_search(int station_id , int rides , char datatype[], int length, long long int *totalridershiprides , Stations *stat)
{
    int low, high, mid;
    
    //updating the total rides
    *totalridershiprides = *totalridershiprides + rides;
    
    low = 0;
    high = length - 1;
    
    while (high >= low) {
        mid = (low + high) / 2;
        if (stat[mid].station_id < station_id)
            low = mid + 1;
        else if (stat[mid].station_id > station_id)
            high = mid - 1;
        else if (station_id == stat[mid].station_id) {   //if found
            stat[mid].totalrides = stat[mid].totalrides + rides;
            
            //storing the ride datatypes into appropriate categories
            if(strcmp(datatype, "U")==0)
                stat[mid].sunday = stat[mid].sunday + rides;
            else if(strcmp(datatype, "W")==0)
                stat[mid].weekday = stat[mid].weekday + rides;
            else
                stat[mid].saturday = stat[mid].saturday + rides;
            break;
        }
        else
            break;
    }
}

//****************************************************************************************


//function to input the stations into structs
struct Stations *InputStations (FILE *file, int *size)
{
    
    Stations *stat;
    
    char file_length[5];
    int length;
    //get the size of file
    fgets(file_length, sizeof(file_length), file);
    length = atoi(file_length);
    *size = length;
    printf(">> Number of stations: %d \n", length);
    
    //allocate the memory
    stat = (Stations *)malloc(sizeof(Stations) * length);
    
    int counter = 0;
    
    char str[350];
    
    fgets(str, 350, file);
    
    while (fgets(str, 350, file)){
        
        char *token;
        token = strtok(str, ",");
        stat[counter].station_id = atoi(token);
        
        token = strtok(NULL, ",");
        strcpy(stat[counter].name, token);
        
        token = strtok(NULL, ",");
        strcpy(stat[counter].handicap_accessable, token);
        
        token = strtok(NULL, ",");
        stat[counter].lattitude = atof(token);
        
        token = strtok(NULL, ",");
        stat[counter].longitude = atof(token);
        strtok(str, "\n");
        
        counter++;
        
    }
    //sorting stations by id
    
    int j,i;
    
    for(i=1;i<length;i++)
    {
        for(j=0;j<length-i;j++)
        {
            if(stat[j].station_id >stat[j+1].station_id)
            {
                struct Stations temp = stat[j];
                stat[j] =stat[j+1];
                stat[j+1] = temp;
            }
        }
    }
    
    return stat;
}

//****************************************************************************************


//sorting stations by rides from top and bottom
void sortStationsbyRidesTOP( int length , Stations *stat){
    
    //TOP
    int j,i;
    
    for(i=1;i<length;i++)
    {
        for(j=0;j<length-i;j++)
        {
            if(stat[j].totalrides < stat[j+1].totalrides)
            {
                struct Stations temp = stat[j];
                stat[j] =stat[j+1];
                stat[j+1] = temp;
            }
        }
    }
    
    
}

//****************************************************************************************

void sortStationsbyRidesBOTTOM( int length, Stations *stat){
    
    //BOTTOM
    int l,m;
    
    for(m=1;m<length;m++)
    {
        for(l=0;l<length-m;l++)
        {
            if(stat[l].totalrides > stat[l+1].totalrides)
            {
                struct Stations temp = stat[l];
                stat[l] =stat[l+1];
                stat[l+1] = temp;
            }
        }
    }
    
    int n;
    printf("\n\n>> Bottom 5 Stations:\n");
    for ( n = 4; n >= 0; n--)
    {
        printf("   %d: %s, %lld\n", length-n, stat[n].name, stat[n].totalrides);
    }
    printf("\n\n");
    
    //return 0;
}


//****************************************************************************************


//interactive search of stations for the user.
int search_stations (int length, Stations *stat){
    
    while(1){
        
        char name[255];
        
        printf(">> Please enter station name (full or substring): ");
        //getting the user input
        fgets(name, sizeof(name), stdin);
        int i=0;
        
        //removing the tailing newline character from fgets
        size_t ln = strlen(name) - 1;
        if (name[ln] == '\n')
            name[ln] = '\0';
        
        //checking if user pressed enter
        if(strcmp(name, "")==0)
            exit(0);
        
        //for loop to check if user pressed enter
        for(i = 0 ; i< length ; i++){
            if(  strstr(stat[i].name , name )!=NULL ||strcmp(stat[i].name , name )==0){
                
                printf("%s:\n", stat[i].name);
                printf("  ID: %d\n", stat[i].station_id);
                printf("  @(%f , %f)\n", stat[i].lattitude, stat[i].longitude);
                if(strcmp(stat[i].handicap_accessable, "TRUE" )==0)
                    printf("  handicap accessable\n");
                else
                    printf("  *not* handicap accessable\n");
                printf("  %lld Total riders\n", stat[i].totalrides);
                printf("     Weekday:        %d\n" , stat[i].weekday);
                printf("     Saturday:       %d\n" , stat[i].saturday);
                printf("     Sunday/Holiday: %d\n\n\n" , stat[i].sunday);
            }
        }
    }
    return 0;
}

//****************************************************************************************


int main()
{
    
    Stations *stat;
    
    printf("** CTA Ridership Analysis **\n\n");
    //OPEING THE FILE
    FILE* file = fopen("stations.csv", "r");
    if (!file)
        return -1;
    
    int size = 0;
    stat = InputStations(file, &size );
    
    //opening the file2
    FILE* file2 = fopen("ridership.csv", "r");
    if (!file2)
        return -1;
    
    InputRiderShip(file2, size, stat);
    
    
    
    //Sorting and printing the top 5
    sortStationsbyRidesTOP(size , stat);
    
    int k;
    printf(">> Top 5 Stations:\n");
    for ( k = 0; k < 5; k++)
    {
        printf("   %d: %s, %lld\n", k+1, stat[k].name, stat[k].totalrides);
    }
    
    //sorting and printing the bottom 5 and resorting back to top 5
    sortStationsbyRidesBOTTOM(size , stat);
    sortStationsbyRidesTOP(size, stat);
    
    //interactive search function
    search_stations(size, stat);
    
    //closing the file!
    fclose(file);
    fclose(file2);
    return 0;
}
