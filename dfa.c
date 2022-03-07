

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dfa.h"
#include <string.h>

const int ASCII = 128;
typedef struct States States;

struct States
{
    //int transitions[ASCII]; //state transitions for all 128 ascii characters //FAILED DFA
    int possibleNextStates[128]; //(will be useful for NFA) (multiple edges from a state) //change to hashset
    bool acceptance;
};

struct DFA
{
    int size; //size of DFA (number of states)
    //int **transitiontable; //transition table failed dfa
    bool accepting; //bool value for acceptance of DFA;
    struct States *boolStates;
};

DFA new_DFA(int nstates)
{
    DFA this = (DFA)malloc(sizeof(struct DFA)); //actual
    //DFA this = (DFA)malloc(sizeof(DFA*)); //failed dfa (MEMORY PROBLEM)
    if (this == NULL)
    {
        return NULL;
    }

    /** METHOD 1?
     for (int i = 0; i < nstates; i++) {
         this->transitiontable[i] = (int*)malloc(128*sizeof(int));
     }
     int **t_t = (int**)malloc(nstates*sizeof(int*)); //failed dfa
     
     for(int i=0; i< nstates; i++) { //failed dfa
         t_t[i] = (int*)malloc(128*sizeof(int));
     }
     this->transitiontable = t_t; //failed dfa
 METHOD 1?*/
    //NVM FAILED

    this->size = nstates;
    this->boolStates = (States *)malloc(128 * sizeof(struct States)); //ACTUAL
    for (int i = 0; i < nstates; i++)
    { // set all states of the DFA to "Not accepting"
        this->boolStates[i].acceptance = false;
        for (int j = 0; j < ASCII; j++)
        {
            this->boolStates[i].possibleNextStates[j] = -1;
        }
    }
    return this;
}

void DFA_free(DFA dfa)
{
    if (dfa->boolStates == NULL)
    {
        printf("dfa->boolStates is null\n"); //TESTING
        return;
    }
    if (dfa == NULL)
    {
        printf("dfa is null\n"); //TESTING
        return;
    }
    /**METHOD 1?
 free(dfa);
 for (int i =0; i < 1; i++) {
     free(dfa->boolStates->possibleNextStates);
 }
METHOD 1?*/
    free(dfa->boolStates); //TOOK WAY TOO LONG AHHHHHH
    free(dfa);             //TOOK WAY TOO LONG AHHHHH
}

int DFA_get_size(DFA dfa)
{
    return dfa->size;
}

/**
 * Return the state specified by the given DFA's transition function from
 * state src on input symbol sym.
 */
int DFA_get_transition(DFA dfa, int src, char sym)
{
    //return dfa->transitiontable[src][(int) sym]; //failed dfa
    return dfa->boolStates[src].possibleNextStates[(int)sym]; //actual
}

/**
 * For the given DFA, set the transition from state src on input symbol
 * sym to be the state dst.
 */
void DFA_set_transition(DFA dfa, int src, char sym, int dst)
{
    //row = states (transition ->) column is the ascii symbols //CONCEPT
    //dfa->transitiontable[src][(int)sym] = dst; //failed dfa
    dfa->boolStates[src].possibleNextStates[(int)sym] = dst; //actual
}

/**
 * Set the transitions of the given DFA for each symbol in the given str.
 * This is a nice shortcut when you have multiple labels on an edge between
 * two states.
 */
void DFA_set_transition_str(DFA dfa, int src, char *str, int dst)
{
    for (int i = 0; i < strlen(str); i++)
    { //loops through accepting string
        //int letter = (int) str[i]; //ASCII representation of the char //CONCEPT
        DFA_set_transition(dfa, i, str[i], i + 1);
    }
}

/**
 * Set the transitions of the given DFA for all input symbols.
 * Another shortcut method.
 */
void DFA_set_transition_all(DFA dfa, int src, int dst)
{
    for (int i = 0; i < ASCII; i++)
    {
        DFA_set_transition(dfa, src, (char)i, dst);
    }
}

/**
 * Set whether the given DFA's state is accepting or not.
 */
void DFA_set_accepting(DFA dfa, int state, bool value)
{
    /** CONCEPT
        get last transition number
        if in the accepting position,
            valeu = true
        else
        false
    CONCEPT*/
    /*METHOD 1?
    if(state == dfa->size-1) {
        value = true;
    }
    else {
        value = false;
    }
    for (int i =0; i < ASCII; i++) {
        if(dfa->transitiontable[state-1][i] == state) { //failed dfa
            dfa->accepting = value;
        }
    }
METHOD 1?*/
    dfa->boolStates[state].acceptance = value; //ACTUAL
}

/**
 * Return true if the given DFA's state is an accepting state.
 */
bool DFA_get_accepting(DFA dfa, int state)
{
    /**CONCEPT
 return boolean value set in accepting
 return dfa -> accepting;
  if state is final{
  return true
  return false
 CONCEPT*/

    /*TESTING
    if(state == dfa->size-1) {
        return true;
    }
    return false;
 
    if(dfa->accepting == true) { //failed dfa
        return true;
    }
    else{
        return false;
    }
TESTING*/

    if (dfa->boolStates[state].acceptance)
    { //ACTUAL
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool DFA_execute(DFA dfa, char *input)
{
    /** CONCEPT
         loop through input str
            check char with transition table
            iterate through transition table
            set_accepting(with current position in transition table
         exit loop
    CONCEPT*/
    int current = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        //char c = input[i]; //TESTING
        //        int destination = dfa->transitiontable[current][(int) input[i]]; //failed dfa
        int destination = dfa->boolStates[current].possibleNextStates[(int)input[i]]; //ACTUAL
        if (destination == -1)
        {
            return false;
        }
        current = destination;
    }
    //int n = dfa -> current; //TESTING
    return DFA_get_accepting(dfa, current);
}

/**
 * Print the given DFA to System.out.
 */
void DFA_print(DFA dfa)
{
    if (dfa == NULL)
    {
        printf("dfa is NULL");
    }
    for (int i = 0; i < dfa->size; i++)
    {
        printf("New Row: %d\n", i);
        for (int j = 0; j < ASCII; j++)
        {
            printf("%d ", dfa->boolStates[i].possibleNextStates[j]); //ACTUAL
                                                                     //            printf("%d ", dfa->transitiontable[i][j]); //failed dfa
        }
    }
}