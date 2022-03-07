/**
   main.c
   CSC 173: Project1
   Created by Sam Frank and Alex Martin on 9/4/21.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "nfa.h"
#include "IntHashSet.h"

void test_DFA(void)
{
    char input[128]; //ASCII list
    printf("\n-----------------BEGIN TESTING DFA--------------------");

    printf("\nTesting DFA that recognizes exactly \"computer\"\n\n");
    char *computer = "computer"; //accepting condition
    DFA partOne = new_DFA(10);   //initializes dfa with 8 states
    for (int i = 0; i < strlen(computer); i++)
    { //reads accepting conditions and "actually" sets up dfa
        DFA_set_transition(partOne, i, computer[i], i + 1);
    }
    DFA_set_transition_all(partOne, 8, 9);
    DFA_set_accepting(partOne, 8, true); //sets accepting state

    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (DFA_execute(partOne, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    DFA_free(partOne);
    printf("Finished"); //testing purposes
    printf("\n-----------------NEW TEST--------------------\n");

    //case 2: Any string that starts with 123
    printf("Testing DFA that recognizes any string that starts with \"123\"\n\n");
    DFA partTwo = new_DFA(3);               //inititalizes DFA with 3 states
                                            //  _| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    DFA_set_transition(partTwo, 0, '1', 1); //  0| - | 1 | - | - | - | - | - | - | - | -
    DFA_set_transition(partTwo, 1, '2', 2); //  1| - | - | 2 | - | - | - | - | - | - | -
    DFA_set_transition(partTwo, 2, '3', 3); //  2| - | - | - | 3 | - | - | - | - | - | -
    DFA_set_transition_all(partTwo, 3, 3);  //  3| 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3 | 3
    DFA_set_accepting(partTwo, 3, true);    //Sets accepting state

    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (DFA_execute(partTwo, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    DFA_free(partTwo);
    printf("Finished"); //testing purposes
    printf("\n-----------------NEW TEST--------------------\n");

    //case 3: Any string that starts with either two 2’s or three 3’s
    //SAM 9/10/21: I think we should hardcode this in... bit tricky with for statement
    printf("Testing DFA that recognizes any string that starts with either two \"2's\" or three \"3's\"\n\n");

    DFA partThree = new_DFA(5); //initializes DFA with 5 states
    /* Transition table
        accepting state 2
        _| 2 | 3 | all |
        0| 1 | 3 |  -  |
        1| 2 | - |  -  |
        2| 2 | 2 |  2  |
        3| - | 4 |  -  |
        4| - | 2 |  -  |
    */
    //  _| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    DFA_set_transition(partThree, 0, '2', 1); //  0| - | - | 1 | 3 | - | - | - | - | - | -
    DFA_set_transition(partThree, 0, '3', 3);
    DFA_set_transition(partThree, 1, '2', 2); //  1| - | - | 2 | - | - | - | - | - | - | -
    DFA_set_transition_all(partThree, 2, 2);  //  2| 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 | 2 (for lamdba)
    DFA_set_transition(partThree, 3, '3', 4); //  3| - | - | - | 4 | - | - | - | - | - | -
    DFA_set_transition(partThree, 4, '3', 2); //  4| - | - | - | 5 | - | - | - | - | - | -
    DFA_set_accepting(partThree, 2, true);    //Sets accepting state

    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (DFA_execute(partThree, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    DFA_free(partThree);
    printf("Finished"); //testing purposes
    printf("\n-----------------NEW TEST--------------------\n");

    //case 4: Binary input with an even number of 0’s and an even number of 1’s
    printf("Testing DFA that recognizes any Binary input with an even number of \"0's\" and an even number of \"1's\"\n\n");
    DFA partFour = new_DFA(4); //inititalizes DFA with 2 states (nodes)
    /*
        _| 0 | 1 |
        0| 1 | 2 |
        1| 0 | 3 |
        2| 3 | 0 |
        3| 2 | 1 |
    */
    //Sets up transition table
    DFA_set_transition(partFour, 0, '0', 1);
    DFA_set_transition(partFour, 1, '0', 0);
    DFA_set_transition(partFour, 2, '0', 3);
    DFA_set_transition(partFour, 3, '0', 2);
    DFA_set_transition(partFour, 0, '1', 2);
    DFA_set_transition(partFour, 1, '1', 3);
    DFA_set_transition(partFour, 2, '1', 0);
    DFA_set_transition(partFour, 3, '1', 1);
    DFA_set_accepting(partFour, 0, true); //sets accepting state

    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (DFA_execute(partFour, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    DFA_free(partFour);
    printf("Finished"); //testing purposes
    printf("\n-----------------NEW TEST--------------------\n");

    //case 5: At least one other pattern that you think is interesting (and hopefully different from other students)
    //ours is ... testing the first 6 nums of the fibonacci sequence "112358"
    printf("Testing DFA that ONLY recognizes \"112358\"\n\n"); //need creative
    DFA partFive = new_DFA(7);                                 //inititalizes DFA with 7 states
    /* accepting state 6
        _| 1 | 2 | 3 | 5 | 8 | all |
        0| 1 | - | - | - | - |  -  |
        1| 2 | - | - | - | - |  -  |
        2| - | 3 | - | - | - |  -  |
        3| - | - | 4 | - | - |  -  |
        4| - | - | - | 5 | - |  -  |
        5| - | - | - | - | 6 |  -  |
        6| - | - | - | - | - |  6  |
    */
    //sets up transition table
    DFA_set_transition(partFive, 0, '1', 1);
    DFA_set_transition(partFive, 1, '1', 2);
    DFA_set_transition(partFive, 2, '2', 3);
    DFA_set_transition(partFive, 3, '3', 4);
    DFA_set_transition(partFive, 4, '5', 5);
    DFA_set_transition(partFive, 5, '8', 6);
    DFA_set_accepting(partFive, 6, true); //sets accepting state

    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (DFA_execute(partFive, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    DFA_free(partFive);
    printf("Finished"); //testing purposes
    printf("\n-----------------Finished Testing DFA--------------------\n");
}

void test_NFA(void)
{
    printf("\n-----------------BEGIN TESTING NFA--------------------");
    char input[128]; //ASCII list

    //case 1: strings ending in "ter"
    /*
    //accepting state 3
    _| t | e | r | all |
    0|0,1| 0 | 0 |  0  |
    1| - | 2 | - | --- |
    2| - | - | 3 | --- |
    3| - | - | - | --- |
    */
    printf("\nTesting NFA that accepts Strings ending in \"ter\"\n\n");
    NFA partOne = new_NFA(5);
    NFA_add_transition_all(partOne, 0, 0);
    NFA_add_transition(partOne, 0, 't', 1);
    NFA_add_transition(partOne, 1, 'e', 2);
    NFA_add_transition(partOne, 2, 'r', 3);
    NFA_add_transition_all(partOne, 3, 4);
    NFA_set_accepting(partOne, 3, true);
    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (NFA_execute(partOne, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    NFA_free(partOne);
    printf("Finished"); //testing purposes
    printf("\n-----------------NEW TEST--------------------\n");
    //printf("NFA \"Strings end ter\" runs with: %d\n",NFA_execute(partOne, "tlatter")); //TESTING

    //case 2: strings containing "ter"
    /*
    //accepting state 3
    _| t | e | r | all |
    0|0,1| 0 | 0 |  0  |
    1| - | 2 | - | --- |
    2| - | - | 3 | --- |
    3| 3 | 3 | 3 |  3  |
    */
    printf("\nTesting NFA that accepts Strings that contain \"ter\"\n\n");
    NFA partTwo = new_NFA(4);
    NFA_add_transition_all(partTwo, 0, 0);
    NFA_add_transition(partTwo, 0, 't', 1);
    NFA_add_transition(partTwo, 1, 'e', 2);
    NFA_add_transition(partTwo, 2, 'r', 3);
    NFA_add_transition_all(partTwo, 3, 3);
    NFA_set_accepting(partTwo, 3, true);
    //printf("NFA \"contain ter \" runs with: %d\n",NFA_execute(partTwo, "tlaterrr")); //TESTING
    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (NFA_execute(partTwo, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    NFA_free(partTwo);
    printf("Finished"); //testing purposes
    printf("\n-----------------NEW TEST--------------------\n");
    /*
        case 3: Strings with more than one a, g, i, o, t, or u, or more than two c’s or n’s.
        Such strings cannot be anagrams of the word “accounting” since they have too many
        of some letter.
    */
    /* accepting state = 2
     _| a | g | i | o | t | u | c | n |
     0| 1 | 3 | 4 | 5 | 6 | 7 | 8 | 10 |
     1| 2 | 1 | 1-->
     2| 2 -->
     3| 3 | 2 | 3 -->
     4| 4 | 4 | 2 | 4-->
     5| 5 | 5 | 5 | 2 | 5-->
     6| 6 | 6 | 6 | 6 | 2 | 6-->
     7| 7 | 7 | 7 | 7 | 7 | 2 | 7-->
     8| 8 | 8 | 8 | 8 | 8 | 8 | 9 | 8-->
     9| 9 | 9 | 9 | 9 | 9 | 9 | 2 | 9 -->
    10| 10| 10| 10| 10| 10| 10| 10| 11| 10 -->
    11| 11| 11| 11| 11| 11| 11| 11| 2| 11 -->
     */
    printf("Testing NFA that recognizes Strings with more than one \"a\", \"g\", \"i\", \"o\", \"t\", or \"u\" or more than two \"c's\" or \"n's\".\n Such Strings cannot be anagrams of the word \"accounting\"\n\n");
    NFA partThree = new_NFA(20);
    NFA_add_transition_all(partThree, 0, 0);

    NFA_add_transition(partThree, 0, 'a', 1);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'a')
        {
            NFA_add_transition(partThree, 1, (char)i, 1);
        }
    }
    NFA_add_transition(partThree, 1, 'a', 2);
    NFA_add_transition_all(partThree, 2, 2);

    NFA_add_transition(partThree, 0, 'g', 3);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'g')
        {
            NFA_add_transition(partThree, 3, (char)i, 3);
        }
    }
    NFA_add_transition(partThree, 3, 'g', 4);
    NFA_add_transition_all(partThree, 4, 4);

    NFA_add_transition(partThree, 0, 'i', 5);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'i')
        {
            NFA_add_transition(partThree, 5, (char)i, 5);
        }
    }
    NFA_add_transition(partThree, 5, 'i', 6);
    NFA_add_transition_all(partThree, 6, 6);

    NFA_add_transition(partThree, 0, 'o', 7);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'o')
        {
            NFA_add_transition(partThree, 7, (char)i, 7);
        }
    }
    NFA_add_transition(partThree, 7, 'o', 8);
    NFA_add_transition_all(partThree, 8, 8);

    NFA_add_transition(partThree, 0, 't', 9);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'t')
        {
            NFA_add_transition(partThree, 9, (char)i, 9);
        }
    }
    NFA_add_transition(partThree, 9, 't', 10);
    NFA_add_transition_all(partThree, 10, 10);

    NFA_add_transition(partThree, 0, 'u', 11);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'u')
        {
            NFA_add_transition(partThree, 11, (char)i, 11);
        }
    }
    NFA_add_transition(partThree, 11, 'u', 12);
    NFA_add_transition_all(partThree, 12, 12);

    NFA_add_transition(partThree, 0, 'c', 13);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'c')
        {
            NFA_add_transition(partThree, 13, (char)i, 13);
        }
    }
    NFA_add_transition(partThree, 13, 'c', 14);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'c')
        {
            NFA_add_transition(partThree, 14, (char)i, 14);
        }
    }
    NFA_add_transition(partThree, 14, 'c', 15);
    NFA_add_transition_all(partThree, 15, 15);

    NFA_add_transition(partThree, 0, 'n', 16);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'n')
        {
            NFA_add_transition(partThree, 16, (char)i, 16);
        }
    }
    NFA_add_transition(partThree, 16, 'c', 17);
    for (int i = 0; i < 128; i++)
    {
        if (i != (int)'n')
        {
            NFA_add_transition(partThree, 17, (char)i, 17);
        }
    }
    NFA_add_transition(partThree, 17, 'c', 18);
    NFA_add_transition_all(partThree, 18, 18);
    NFA_set_accepting(partThree, 2, true);
    NFA_set_accepting(partThree, 4, true);
    NFA_set_accepting(partThree, 6, true);
    NFA_set_accepting(partThree, 8, true);
    NFA_set_accepting(partThree, 10, true);
    NFA_set_accepting(partThree, 12, true);
    NFA_set_accepting(partThree, 15, true);
    NFA_set_accepting(partThree, 18, true);
    //printf("NFA \"case3 \" runs with: %d\n",NFA_execute(partThree, "agg")); //TESTING
    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        //anagrams of "accounting"
        int a = 1;
        int c = 2;
        int o = 1;
        int u = 1;
        int n = 2;
        int t = 1;
        int i = 1;
        int g = 1;
        int ac = 0;
        int cc = 0;
        int oc = 0;
        int uc = 0;
        int nc = 0;
        int tc = 0;
        int ic = 0;
        int gc = 0;

        for (int i = 0; i < strlen(input); i++)
        {
            if (input[i] == 'a')
            {
                ac++;
            }
            if (input[i] == 'c')
            {
                cc++;
            }
            if (input[i] == 'o')
            {
                oc++;
            }
            if (input[i] == 'u')
            {
                uc++;
            }
            if (input[i] == 'n')
            {
                nc++;
            }
            if (input[i] == 't')
            {
                tc++;
            }
            if (input[i] == 'i')
            {
                ic++;
            }
            if (input[i] == 'g')
            {
                gc++;
            }
        }
        if (ac == a)
        {
            if (cc == c)
            {
                if (oc == o)
                {
                    if (uc == u)
                    {
                        if (nc == n)
                        {
                            if (tc == t)
                            {
                                if (ic == i)
                                {
                                    if (gc == g)
                                    {
                                        printf("%s: rejected because its an anagram of \"accounting\" \n", input);
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (NFA_execute(partThree, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    NFA_free(partThree);
    printf("Finished"); //testing purposes
    printf("\n-----------------TESTING NFA--------------------\n");

    /*//case 4 strings ending in man
     //accepting state 3
     _| m | a | n | all |
     0|0,1| 0 | 0 |  0  |
     1| - | 2 | - | --- |
     2| - | - | 3 | --- |
     3| 3 | 3 | 3 |  3  |
     */
    printf("Testing NFA that recognizes Strings ending in \"man\"\n\n");
    NFA partFour = new_NFA(5);
    NFA_add_transition_all(partFour, 0, 0);
    NFA_add_transition(partFour, 0, 'm', 1);
    NFA_add_transition(partFour, 1, 'a', 2);
    NFA_add_transition(partFour, 2, 'n', 3);
    //NFA_add_transition_all(partFour, 3, 4); //TESTING
    NFA_set_accepting(partFour, 3, true);
    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (NFA_execute(partFour, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    NFA_free(partFour);
    printf("Finished"); //testing purposes
    printf("\n-----------------FINISHED TESTING NFA--------------------\n");
    //    printf("NFA \"end man\" runs with: %d\n",NFA_execute(partFour, "mann")); //TESTING
}

void test_NFAtoDFA(void)
{
    printf("\n-----------------BEGIN TESTING NFA TO DFA--------------------");
    char input[128]; //ASCII list

    //case 1: strings ending in "ter"
    /*
    //accepting state 3
    _| t | e | r | all |
    0|0,1| 0 | 0 |  0  |
    1| - | 2 | - | --- |
    2| - | - | 3 | --- |
    3| - | - | - | --- |
    */
    printf("\nTesting NFA to DFA that accepts Strings ending in \"ter\"\n\n");

    NFA partOnea = new_NFA(5);
    //NFA translated = nfa_to_dfa(partOnea);
    NFA_add_transition_all(partOnea, 0, 0);
    NFA_add_transition(partOnea, 0, 't', 1);
    NFA_add_transition(partOnea, 1, 'e', 2);
    NFA_add_transition(partOnea, 2, 'r', 3);
    NFA_add_transition_all(partOnea, 3, 4);
    NFA_set_accepting(partOnea, 3, true);
    NFA translated = nfa_to_dfa(partOnea);
    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (nfa_to_dfa_Execute(translated, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    NFA_free(partOnea);
    printf("Finished"); //testing purposes
    printf("\n-----------------NEW TEST--------------------\n");

    //case 2: strings containing "ter"
    /*
    //accepting state 3
    _| t | e | r | all |
    0|0,1| 0 | 0 |  0  |
    1| - | 2 | - | --- |
    2| - | - | 3 | --- |
    3| 3 | 3 | 3 |  3  |
    */
    printf("\nTesting NFA to DFA that accepts Strings that contain \"ter\"\n\n");
    NFA partOneb = new_NFA(4);
    //NFA translatedTwo = nfa_to_dfa(partOneb);
    NFA_add_transition_all(partOneb, 0, 0);
    NFA_add_transition(partOneb, 0, 't', 1);
    NFA_add_transition(partOneb, 1, 'e', 2);
    NFA_add_transition(partOneb, 2, 'r', 3);
    NFA_add_transition_all(partOneb, 3, 3);
    NFA_set_accepting(partOneb, 3, true);
    NFA translatedTwo = nfa_to_dfa(partOneb);
    do
    {
        printf("\tEnter an input (\"QUIT\" to quit): ");
        scanf("%s", input); //user input
        if (nfa_to_dfa_Execute(translatedTwo, input) == 1)
        {
            printf("%s: accepted\n", input);
        }
        else
        {
            printf("%s: rejected\n", input);
        }
    } while (strcmp(input, "QUIT") != 0);
    NFA_free(partOneb);
    printf("Finished"); //testing purposes
    printf("\n-----------------FINISHED TESTING NFA TO DFA--------------------\n");
}

int main(int argc, char *argv[])
{
    // can we make this a method to clean up our main method --> YES
    //printf("I don't understand anything\n"); //This is still factual
    printf("CSC 173 Project 1, Alexander Martin (amart50) & Samuel Frank (sfrank8)\n");
    test_DFA();
    test_NFA();
    test_NFAtoDFA();
    printf("\n\t\tTHANK YOU FOR YOUR TIME!\n\t\t-Alex Martin and Sam Frank\n\n");
}