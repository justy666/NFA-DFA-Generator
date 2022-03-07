

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfa.h"
#include "IntHashSet.h"
#include <math.h>

typedef struct States States;
struct States
{
    /*TESTING
    hashset possibleNextStates;
    bool accepting
    struct IntHashSets hashSet;
TESTING*/

    struct IntHashSet *possibleNextStates[128];
    IntHashSet setOfStates;
    IntHashSet setOfDestinations;
    bool accepting;
    bool acceptingSet;
    int symbol;
};

struct NFA
{
    int size;
    int nsize;
    struct States *boolStates;
    int numAcceptingStates;
    int numStates;
    //IntHashSet acceptingStates; //TESTING
    int acceptingState;
    int *acceptingStates;
    //IntHashSet dfaTransitionTable[][128];
    //int existingStates; //TESTING
};

/*METHOD 1?
    typedef struct DFA_States;
    struct DFA_States{
        IntHashSet setOfStates;
        IntHashSet setOfDestinations;
    };

    struct Set_DFA{
        DFA_States *dfaState;
        int numStates;
        bool acceptingSet;
    };
METHOD 1?*/
//NVM FAILED

/**
 * Allocate and return a new NFA containing the given number of states.
 */
NFA new_NFA(int nstates)
{
    NFA this = (NFA)malloc(sizeof(struct NFA));
    this->boolStates = (States *)malloc(nstates * sizeof(struct States)); //NICE

    if (this == NULL)
    {
        return NULL;
    }

    this->size = nstates;
    this->numAcceptingStates = 0;
    this->acceptingState = -1;
    this->nsize = (nstates * nstates);
    this->acceptingStates = (int *)malloc(nstates * sizeof(int));
    this->boolStates->setOfStates = new_IntHashSet(nstates);
    this->boolStates->setOfDestinations = new_IntHashSet(nstates);
    //this -> dfaTransitionTable
    //printf("line 36\n"); //TESTING
    for (int i = 0; i < nstates; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            //printf("i:%d\n", i); TESTING
            this->boolStates[i].possibleNextStates[j] = new_IntHashSet(128);
        }
    }

    //printf("line 40\n"); //TESTING
    for (int i = 0; i < nstates; i++)
    {
        this->boolStates[i].accepting = false;
    }
    return this;
}

/**
 * Free the given NFA.
 */
void NFA_free(NFA nfa)
{
    /*METHOD 1?
     for(int i = 0; i < nfa->size; i++){
        States* stateHolder = &nfa -> boolStates[i];
        //IntHashSet_free(nfa->boolStates->possibleNextStates[i]); //TESTING
        for(int i = 0; i < 128; i++){
            IntHashSet_free(stateHolder->possibleNextStates[i]);
        }
    }
METHOD 1?*/
    //NVM METHOD FAILED
    for (int i = 0; i < nfa->size; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            IntHashSet_free(nfa->boolStates[i].possibleNextStates[j]);
        }
    }
    free(nfa->boolStates);
    //IntHashSet_free(nfa -> boolStates -> possibleNextStates); //TESTING
    free(nfa->acceptingStates);
    free(nfa);
}

/**
 * Return the number of states in the given NFA.
 */
int NFA_get_size(NFA nfa)
{
    return nfa->size;
}

/**
 * Return the set of next states specified by the given NFA's transition
 * function from the given state on input symbol sym.
 */
Set NFA_get_transitions(NFA nfa, int state, char sym)
{
    return nfa->boolStates[state].possibleNextStates[(int)sym];
}

/**
 * For the given NFA, add the state dst to the set of next states from
 * state src on input symbol sym.
 */
void NFA_add_transition(NFA nfa, int src, char sym, int dst)
{
    IntHashSet_insert(nfa->boolStates[src].possibleNextStates[(int)sym], dst);
}

/**
 * Add a transition for the given NFA for each symbol in the given str.
 */
void NFA_add_transition_str(NFA nfa, int src, char *str, int dst)
{
    for (int i = 0; i < strlen(str); i++)
    {
        IntHashSet_insert(nfa->boolStates[src].possibleNextStates[(int)str[i]], dst);
    }
}

/**
 * Add a transition for the given NFA for each input symbol. GIVES LAMBDA CASE
 */
void NFA_add_transition_all(NFA nfa, int src, int dst)
{
    for (int i = 0; i < 128; i++)
    {
        if (IntHashSet_isEmpty(nfa->boolStates[src].possibleNextStates[i]))
        {
            nfa->boolStates[src].possibleNextStates[i] = new_IntHashSet(nfa->size);
            IntHashSet_insert(nfa->boolStates[src].possibleNextStates[i], dst);
        }
        else
        {
            IntHashSet_insert(nfa->boolStates[src].possibleNextStates[i], dst);
        }
    }
}

/**
 * Set whether the given NFA's state is accepting or not.
 */
void NFA_set_accepting(NFA nfa, int state, bool value)
{
    nfa->boolStates[state].accepting = value;
    nfa->acceptingStates[nfa->numAcceptingStates] = state;
    nfa->numAcceptingStates++;
    nfa->acceptingState = state;
}

/**
 * Return true if the given NFA's state is an accepting state.
 */
bool NFA_get_accepting(NFA nfa, int state)
{
    return nfa->boolStates[state].accepting;
}

/**
 * Run the given NFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
bool NFA_execute(NFA nfa, char *input)
{
    IntHashSet final = new_IntHashSet(nfa->size);
    IntHashSet current = new_IntHashSet(nfa->size);
    IntHashSet iteratorSets = new_IntHashSet(nfa->size);
    IntHashSet_insert(current, 0);
    IntHashSet holder;
    /*TESTING
        printf("current is: ");
        IntHashSet_print(current);
    TESTING*/
    for (int i = 0; i < strlen(input); i++)
    {
        /*TESTING
        printf("current is: ");
        IntHashSet_print(current);
        printf("\naddedSets is: ");
        IntHashSet_print(iteratorSets);
    TESTING*/
        iteratorSets = current;
        /*TESTING
        printf("\naddedSets is now: ");
        IntHashSet_print(iteratorSets);
     TESTING*/
        IntHashSetIterator iterator = IntHashSet_iterator(iteratorSets);
        holder = new_IntHashSet(nfa->size);
        while (IntHashSetIterator_hasNext(iterator))
        {
            current = nfa->boolStates[IntHashSetIterator_next(iterator)].possibleNextStates[(int)input[i]];
            IntHashSet_union(holder, current);
            if (!IntHashSetIterator_hasNext(iterator))
            {
                current = holder;
                break;
            }
        }
    }
    //nfa->currentStates = current; //FAILED
    final = current;

    /*TESTING
    printf("final is: ");
    IntHashSet_print(final);
    printf("\n");
    for(int i = 0; i < nfa -> numAcceptingStates; i++){
        printf("acceptingStates at [%d]: %d \n", i, nfa -> acceptingStates[i]);
    }
TESTING*/

    //ACTUALLY WORKS
    for (int i = 0; i < nfa->numAcceptingStates; i++)
    {
        if (IntHashSet_lookup(final, nfa->acceptingStates[i]))
        {
            return true;
        }
    }
    return false;

    /**CONCEPT
    just a heads up, I need to check to make sure a path isnt 0->0->0->0->1
    also need to get rid of a set once it enters a failed state throw that bad boy out and free that memory (MEMORY!!!!)
 
    IntHashSet current = new_IntHashSet(nfa->size);
    IntHashSet final = new_IntHashSet(nfa->size);
    IntHashSet_insert(current, 0);
    IntHashSet temp = new_IntHashSet(nfa->size);
    IntHashSet holder;
    int position = 0;
    for(int i = 0; i < strlen(input); i++){
    final = current;
 
CONCEPT*/

    /**TESTING
          printf("Temp is: ");
         IntHashSet_print(current);
         printf("\n");
         printf("Final is: ");
         IntHashSet_print(final);
         printf("\n");
         IntHashSetIterator iterator = IntHashSet_iterator(final);
 TESTING*/

    /** METHOD 1?
//        temp = new_IntHashSet(nfa->size);
//        holder = new_IntHashSet(nfa -> size);
//        while(IntHashSetIterator_hasNext(iterator)){
//            temp = nfa -> boolStates[IntHashSetIterator_next(iterator)].
//                possibleNextStates[(int) input[i]];
//            IntHashSet_union(holder, temp);
//            if(!IntHashSetIterator_hasNext(iterator)){
//                temp = holder;
//                //current = temp;
//            }
//        }
//        Iterator = iterator current
//        while(iterator)
//            get possible number of states
//            for(possible number of states)
//                Set outcome = new set
//    }
//    final = current;
METHOD 1? */
    //NVM FAILED METHOD

    /** TESTING
 //     printf("Temp is: \n");
 //    IntHashSet_print(current);
 //    printf("\n");
 //    printf("Final is:");
 //    IntHashSet_print(final);
 //    IntHashSet finalStates = new_IntHashSet(nfa->size);
 //    printf("\n");
 //    finalStates = final;
 //    //int acceptingStates = nfa -> numAcceptingStates;
 //    //printf("Num accepting states: %d \n", nfa -> numAcceptingStates);
TESTING */

    /** METHOD 2?
     int acceptingStates = 0;
     for(int i = 0; i < nfa->size; i++){
         if(nfa -> boolStates[i].accepting){
             acceptingStates++;
         }
     }
     printf("NFA accepting state: %d\n", nfa -> acceptingState); //TESTING
     for(int i = 0; i < nfa -> size; i++){
         if(IntHashSet_lookup(finalStates, nfa->acceptingState +1)) {
             printf("returning false\n");
             return false;
         }else if(IntHashSet_lookup(finalStates, nfa -> acceptingState)){
             printf("returning true\n");
             return true;
         }
     }
 METHOD 2?*/
    //NVM FAILED METHOD
    /** METHOD 3?
     for(int i = 0; i < acceptingStates; i++){
         printf("Boolean value for IntHashSet_lookup: %d", IntHashSet_lookup(finalStates, nfa->boolStates[i].accepting));
         printf("\n");
         if(IntHashSet_lookup(finalStates, nfa->boolStates[i].accepting)) {
             printf("returning true\n");
             return true;
         }
     }
     printf("returning false \n");
     return false;
METHOD 3?*/

    /**CONCEPT
 list of sets with possible paths
 loop through input string
     how many path options?
     for(path options)
         have a current set your adding to
         newSet = add possible
         newSet union current set
         add to list of sets

 for(list of set size)          //loops finish
     find set with finishing position = accepting position
     if found
         return true
 return false
CONCEPT*/

    /*CONCEPT
    loop through input string
    get transition with char
    if transition contains accepting state, accept
CONCEPT*/
}
/**
 * Print the given NFA to System.out.
 */
void NFA_print(NFA nfa)
{
    for (int i = 0; i < nfa->size; i++)
    {
        char *setString = IntHashSet_toString(nfa->boolStates->possibleNextStates[i]);
        printf("%d: %s", i, setString);
    }
}
/*
 NFA to DFA
 */
/* CONCEPT
    Conversion from NFA to DFA
    Suppose there is an NFA N < Q, ∑, q0, δ, F > which recognizes a language L. Then the DFA D < Q’, ∑, q0, δ’, F’ > can be constructed for language L as:
    Step 1: Initially Q’ = ɸ.
    Step 2: Add q0 to Q’.
    Step 3: For each state in Q’, find the possible set of states for each input symbol using transition function of NFA. If this set of states is not in Q’, add it to Q’.
    Step 4: Final state of DFA will be all states with contain F (final states of NFA)
    from: https://www.geeksforgeeks.org/conversion-from-nfa-to-dfa/
    what we need:
    Set of final states
    q0 = initial state
    set of states in the NFA
    E = langauge accepted by NFA
CONCEPT*/
bool nfa_to_dfa_Execute(NFA dfa, char *input)
{
    printf("dfa size: %d\n", dfa->nsize); //prints the size of the new DFA
    IntHashSet final = new_IntHashSet(dfa->size);
    IntHashSet current = new_IntHashSet(dfa->size);
    IntHashSet iteratorSets = new_IntHashSet(dfa->size);
    IntHashSet_insert(current, 0);
    IntHashSet holder;
    /*TESTING
        printf("current is: ");
        IntHashSet_print(current);
     TESTING*/
    for (int i = 0; i < strlen(input); i++)
    {
        /* TESTING
        printf("current is: ");
        IntHashSet_print(current);
        printf("\naddedSets is: ");
        IntHashSet_print(iteratorSets);
    TESTING*/
        iteratorSets = current;
        /*TESTING
        printf("\naddedSets is now: ");
        IntHashSet_print(iteratorSets);
    TESTING*/
        IntHashSetIterator iterator = IntHashSet_iterator(iteratorSets);
        holder = new_IntHashSet(dfa->size);
        while (IntHashSetIterator_hasNext(iterator))
        {
            current = dfa->boolStates[IntHashSetIterator_next(iterator)].possibleNextStates[(int)input[i]];
            IntHashSet_union(holder, current);
            if (!IntHashSetIterator_hasNext(iterator))
            {
                current = holder;
                break;
            }
        }
    }
    //nfa->currentStates = current; //TESTING
    final = current;
    /*TESTING
        printf("final is: ");
        IntHashSet_print(final);
        printf("\n");
        for(int i = 0; i < nfa -> numAcceptingStates; i++){
            printf("acceptingStates at [%d]: %d \n", i, nfa -> acceptingStates[i]);
        }
    TESTING*/
    for (int i = 0; i < dfa->numAcceptingStates; i++)
    {
        if (IntHashSet_lookup(final, dfa->acceptingStates[i]))
        {
            return true;
        }
    }
    return false;
}

NFA nfa_to_dfa(NFA dfa)
{
    /* CONCEPT
     We need state hashsets
     We need position hashsets
     we refer to these by the character symbol
     */
    /*IntHashSet currentStateSet = new_IntHashSet(nfa -> size);
    IntHashSet setFinalStates;
    IntHashSet initialState = new_IntHashSet(1);
    int dfaStates = pow(2.0, (double) nfa -> size);
    //nfa -> size;
    IntHashSet transitionTableSets[dfaStates];
CONCEPT */

    NFA setDFA = new_NFA(dfa->size * dfa->size);         //sets size
    IntHashSet setNfaStates = new_IntHashSet(dfa->size); //sets up with size
    //int listNfaStates[dfa -> size]; //TESTING
    for (int i = 0; i < dfa->size; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            IntHashSet_union(setNfaStates, dfa->boolStates[i].possibleNextStates[j]);
        }
    }
    //    IntHashSetIterator iterator = IntHashSet_iterator(setNfaStates);
    //    int x = 0;
    /*while(IntHashSetIterator_hasNext(iterator)){
        //listNfaStates[x] = IntHashSetIterator_next(iterator); //TESTING
        x++;
    }*/
    //IntHashSet currentStateSet = new_IntHashSet(nfa -> size); //TESTING
    IntHashSet previousState = new_IntHashSet(dfa->size);
    IntHashSet_insert(previousState, 0);
    IntHashSet listOfStateSets[dfa->size];
    listOfStateSets[0] = previousState;
    for (int i = 0; i < dfa->size; i++)
    {
        IntHashSet holder = new_IntHashSet(dfa->size);
        for (int j = 0; j < 128; j++)
        {
            IntHashSet_union(holder, dfa->boolStates[i].possibleNextStates[j]);
        }
        IntHashSet_union(holder, previousState);
        //IntHashSet_print(holder);
        listOfStateSets[i + 1] = holder;
        /*TESTING
            printf("i = %d", i);
            IntHashSet_print(holder);
            int currentState = listNfaStates[i];
            setDFA -> symbol = listN
            currentStateSet = nfa -> boolStates[i].possibleNextStates;
         TESTING*/
    }
    //printf("\n");
    for (int i = 0; i < dfa->size; i++)
    {
        //IntHashSet_print(listOfStateSets[i]);
        setDFA->boolStates[i].setOfStates = listOfStateSets[i];
        //IntHashSet_print(setDFA -> boolStates[i].setOfStates);
    }
    //printf("\n");
    //setDFA -> boolStates[1].setOfDestinations;
    IntHashSet nextStates[128];
    IntHashSet dfaTransitionTable[dfa->size][128];
    //int transition[][128];
    for (int i = 0; i < dfa->size; i++)
    {
        IntHashSet holder = new_IntHashSet(dfa->size);
        IntHashSet set = setDFA->boolStates[i].setOfStates;
        IntHashSetIterator iterator = IntHashSet_iterator(set);
        while (IntHashSetIterator_hasNext(iterator))
        {
            int position = IntHashSetIterator_next(iterator);
            for (int j = 0; j < 128; j++)
            {
                nextStates[j] = dfa->boolStates[position].possibleNextStates[j];
                //IntHashSet_print(dfa->boolStates[position].possibleNextStates[j]);
                dfaTransitionTable[i][j] = dfa->boolStates[position].possibleNextStates[j];
                IntHashSet_union(holder, dfa->boolStates[position].possibleNextStates[j]);
            }
            //printf("\nHolder at position %d: ", position);
            //IntHashSet_print(holder);
        }
    }

    //    for(int i = 0; i < dfa -> size; i++){
    //        for(int j = 0; j < 128; j++){
    //
    //        }
    //    }

    /** METHOD 1?
     for(int i = 0 ; i < nfa -> size; i++){
         for(int j = 0; j < 128; j++){
             IntHashSet current = nfa -> boolStates[i].possibleNextStates[j];
        //printf("\ncurrent: "); //TESTING
            IntHashSet_print(current);
             IntHashSet_union(currentStateSet, current);
         }
         printf("current state set: ");
         IntHashSet_print(currentStateSet);
     }
     for(int i = 0; i < nfa -> size; i++){
         printf("%d : %d ", i, listNfaStates[i]); //TESTING
     }
     
     for(int i =0; i < nfa->size; i++) {
         IntHashSet_print(setNfaStates[i]); //TESTING
     }
     for(int i = 0; i < nfa -> size; i++){
         printf("\n%d: ", i);  //TESTING
         IntHashSet_print(listNFASets[i]);
     }
METHOD 1?*/
    //NVM FAILED

    /**CONCEPT
   //Set of final States
   //need pointer nfa -> numberOfAcceptingStates in NFA
   IntHashSet finalStates = new_IntHashSet(nfa -> numAcceptingStates);
   Q' needs to be a set of sets not whatever this is
   maybe a list of sets where index is set of possibleStates and the item in the set is the set of possibleNextStates;

   IntHashShet qApostraphe = new_IntHashSet();
   //add q0 to Q'
   IntHashSet stateSetLittleQ = new_IntHashSet();
   for(int i = 0; i < nfa -> size; i++){
     for(int j = 0; j < 128; j++){
       //If this set of states is not in Q’, add it to Q'-https://www.geeksforgeeks.org/conversion-from-nfa-to-dfa/
 CONCEPT*/

    return dfa;
}

/*TESTING
    int main(int argc, char* argv[]) { //TESTING
        NFA test = new_NFA(2);
        NFA_execute(test, "youramazingmom");
        NFA_print(test);
        NFA_get_size(test);
        NFA_free(test);
    }
TESTING*/