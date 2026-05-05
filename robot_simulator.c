/*
Degree: Artificial Intelligence
Subject: Fundamentals of Programming 2
Practical project: 1

Simulator - main program
*/
//----------------------------------------------------------Declarations: Diego A. 24/03
#define _CRT_SECURE_NO_WARNINGS // I use clang for compiling and it won't shut up about deprecated functions.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project1.h" // header with all data structures

void SimulationLoop(int EventNumbers);

enum EventType GenerateEventType(); //can't be changed
void CheckArguments(int argc, char ** argv);

struct RobotPackage * GenerateRobotPackage(); //can't be changed
struct RobotPackage * SearchRobotPackage();
void PrintRobotPackages();
void SimulateManagingRobotPackages(struct RobotPackage * RobotPackage);
void RemoveAllRobotPackages();

struct Package * GeneratePackage(); //can't be changed
void InitStacks();
void PrintPackages();
void RemoveStack(struct Package ** head);
void SimulateClassifyPackage(struct Package * Package);
void CleanPackageStacks();

struct Shopping * GenerateShopping(); //can't be changed
void PrintShopping();
void AddToQueue(struct Shopping * r_shopping);
int Dequeue ();
void UpdateShoppingQueue (int * time);
void SimulateGoForShopping(struct Shopping * shopping);
void CleanShoppingQueue(/*...*/);

//----------------------------------------------------------main
int main (int argc, char ** argv)
{
	int EventNumbers;
	CheckArguments(argc, argv);

	// initialize simulation
	EventNumbers = atoi(argv[1]);
	SimulationLoop(EventNumbers);

	return 0;
}

// This is the main loop of the program.
// It generates and consumes events.
void SimulationLoop(int EventNumbers){
	// declare and initialize necessary variables
	enum EventType event;
	//int eventCounter = 0; debug
	int shopping_time = 0;
	InitStacks();

	for (int i = 1; i <= EventNumbers; i++){
		event = GenerateEventType();

		switch(event) {
			case 0:
				struct RobotPackage* r_package = GenerateRobotPackage();
				SimulateManagingRobotPackages(r_package);
				break;
			case 1:
				struct Package* package = GeneratePackage();
				SimulateClassifyPackage(package);
				break;
			case 2:
				struct Shopping* shopping = GenerateShopping();
				SimulateGoForShopping(shopping);
				break;
		}
		UpdateShoppingQueue(&shopping_time);
		//eventCounter++; debug
	}
	printf("\n============STATISTICS WHEN CLEANING THE SIMULATION:============\n");
	printf("\tRemoving Packages...\n\t\t");
   	RemoveAllRobotPackages();
	printf("\tRemoving all stacks of packages...\n\t\t");
   	CleanPackageStacks();
	printf("\tCleaning shopping queue...\n\t\t");
    CleanShoppingQueue();
}


//----------------------------------------------------------General
// WARNING: do not change this function
enum EventType GenerateEventType(){
	return rand()%3;
}

void CheckArguments (int argc, char ** argv){
    if(argc != 2){
        fprintf(stderr, "This progrma requires and parses only 1 argument. Number of arguments you passed: %d", argc-1);
        exit(EXIT_FAILURE);
    }
    if(atoi(argv[1]) < 1){
        fprintf(stderr, "The argument must be a positive integer. You passed: %s", argv[1]);
        exit(EXIT_FAILURE);
    }
    printf("\n============STARTING SIMULATION WITH %s EVENTS...============\n", argv[1]);
}

//----------------------------------------------------------Logic
//-----------------------------------------------RobotPackages -> Sorted list
// WARNING: do not change this function
struct RobotPackage * GenerateRobotPackage(){
	// reserve memory for a RobotPackage
	struct RobotPackage * RobotPackage=malloc(sizeof(struct RobotPackage));
	int RobotPackageNum=rand()%8;
	// initialize the RobotPackage's fields
	strcpy (RobotPackage->supplier, suppliers[RobotPackageNum]);
	strcpy (RobotPackage->id, ids[RobotPackageNum]);
	int year=rand()%40+1980;
	RobotPackage->year=year;
	return RobotPackage;
}

// function to print a list of RobotPackages
void PrintRobotPackages(){
    struct RobotPackage * curr = RobotPackagesHead;
    int i = 1;
    while (curr != NULL){
        printf("Package %i: %s, supplier: %s, year: %d\n", i, curr->id, curr->supplier, curr->year);
        curr = curr->next;
        i++;
    }
}

// function to search for a RobotPackage
/* struct RobotPackage * SearchRobotPackage(){
    unused.
}*/

// function to simulate an insertion of RobotPackages in a ordered way (sorted by supplier)
void SimulateManagingRobotPackages(struct RobotPackage * r_package){
    if (RobotPackagesHead == NULL){
        RobotPackagesHead = r_package;
        RobotPackagesHead->next = NULL;
        return;
    }

    // The Packages are sorted by supplier A-Z.
    // IF they share a supplier they are ordered by order of generation/arrival oldest-newest. Diego A. 05/05/26
    if (strcmp(RobotPackagesHead->supplier, r_package->supplier) > 0){
        r_package->next = RobotPackagesHead;
        RobotPackagesHead = r_package;
        return;
    }

    struct RobotPackage * curr = RobotPackagesHead;
    while (curr->next != NULL && strcmp(curr->next->supplier, r_package->supplier) <= 0){
        curr = curr->next;
    }

    r_package->next = curr->next;
    curr->next = r_package;
}

// function to remove all the RobotPackages from the list at the end of the program
void RemoveAllRobotPackages(){
    struct RobotPackage *curr = RobotPackagesHead;
    int count = 0;
    while (curr != NULL) {
        struct RobotPackage *temp = curr;
        curr = curr->next;
        free(temp);
        count++;
    }
    RobotPackagesHead = NULL;
    printf("%d RobotPackages removed after cleanup.\n", count);
}

//-----------------------------------------------Packages -> different Stacks
// WARNING: do not change this function
struct Package * GeneratePackage(){
	// reserve memory for a Package
	struct Package * Package=malloc(sizeof(struct Package));
	// initialize the Package's fields
	enum PackageType type=rand()%3;
	enum Colors color=rand()%4;
	Package->type = type;
	Package->color = color;
	return Package;
}

// function to initialize all stacks of Packages
void InitStacks(){
    for (int i = 0; i < NUMBER_OF_STACK; i++){
        Top_ofPackageStacks[i] = NULL;
        CurrentState[i] = 0;
    }
}

// function to print all stacks with all Packages
void PrintPackages(){
    for (int i = 0; i < NUMBER_OF_STACK; i++){
        struct Package * curr = Top_ofPackageStacks[i];
        if (curr == NULL){
            printf("The stack for package type \"%d\" is empty\n", i);
        } else{
            while (curr != NULL){
                printf("Package: type->%d, color->%d\n", curr->type, curr->color);
                curr = curr->next;
            }
        }
    }
}

// function to remove all packages from a given stack when its MAX_CAPACITY is reached
void RemoveStack(struct Package ** head){
    while ((*head) != NULL){
        struct Package * temp = *head;
        (*head) = (*head)->next;
        free(temp);
    }
}

// function to simulate putting a generated Package to a corresponding stack depending on the type (size)
void SimulateClassifyPackage(struct Package * package){
    package->next = Top_ofPackageStacks[package->type];
    Top_ofPackageStacks[package->type] = package;
    CurrentState[package->type]++;

    if (CurrentState[package->type] == MAX_CAPACITY){
        //printf("Cleaning Stack %d...\n",package->type); debug
        RemoveStack(&Top_ofPackageStacks[package->type]);
        Top_ofPackageStacks[package->type] = NULL;
        CurrentState[package->type] = 0;
    }
}

// function to clean all stacks before the end of the program
void CleanPackageStacks() {
    int count = 0;
    for (int i = 0; i < NUMBER_OF_STACK; i++) {
        struct Package *curr = Top_ofPackageStacks[i];
        while (curr != NULL) {
            struct Package *temp = curr;
            curr = curr->next;
            free(temp);
            count++;
        }
        Top_ofPackageStacks[i] = NULL;
        CurrentState[i] = 0;
    }
    printf("%d Packages removed after cleanup.\n", count);
}

//-----------------------------------------------Shopping -> Queue
// WARNING: do not change this function
struct Shopping * GenerateShopping(){
	// reserve memory for a Shopping
	struct Shopping * shopping=malloc(sizeof(struct Shopping));
	// initialize the shopping's fields
	int n=rand()%5+1;
	shopping->numberThingsToBuy = n;
	nextRobotID++;
	shopping->robot_id=nextRobotID;
	return shopping;
}

// function to print a list of robots in a shopping queue
void PrintShopping(){
    struct Shopping * curr = queueFirst;

    while (curr != NULL){
        printf("Shopping robot: %d, wants to buy: %d\n", curr->robot_id,curr->numberThingsToBuy);
        curr = curr->next;
    }

}

// function to add a robot to a shopping queue
void AddToQueue(struct Shopping * r_shopping){

    if (queueFirst == NULL) {
        queueFirst = r_shopping;
        queueLast = r_shopping;
        queueFirst->next = NULL;
        return;
    }

    queueLast->next = r_shopping;
    queueLast = r_shopping;
    queueLast->next = NULL;
}

// function to remove a robot from the queue and serve it
// it may return the number of things to buy to simulate the time
int Dequeue (){
    int buy_list;

    if (queueFirst == NULL){
        return -1;
    }

    struct Shopping * temp = queueFirst;
    buy_list = temp->numberThingsToBuy;
    queueFirst = temp->next;

    if (queueFirst == NULL){
        queueLast = NULL;
    }

    free(temp);
    return buy_list;
}

// function to simulate the time the robot is in the queue
void UpdateShoppingQueue(int * time){

    if(*time == 0){
        int new_time = Dequeue();
        if (new_time != -1){
            //printf("a robot left the queue\n"); debug
            *time = new_time;
        }
    } else if(*time > 0){
        (*time)--;
    }
}

// function to simulate a robot going for shopping - add to the queue
void SimulateGoForShopping(struct Shopping * shopping){
    AddToQueue(shopping);
    //printf("Added to queue\n"); debug
}

// function to clean shopping queue before the end of the program
void CleanShoppingQueue() {
    struct Shopping *curr = queueFirst;
    int count = 0;
    while (curr != NULL) {
        struct Shopping *temp = curr;
        curr = curr->next;
        free(temp);
        count++;
    }
    queueFirst = NULL;
    queueLast = NULL;
    printf("%d Shopping Robots removed after cleanup.\n", count);
}
