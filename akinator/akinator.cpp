#include "akinator.h"

void AkinatorPlay (Tree* tree) {


    for (;;) {

        printf ("Welcome to akinator special software incorporated (a.k.a. as ASS inc.)" "\n"
                "Choose what u prefer to do:" "\n"
                "1) [L]et akinator guess" "\n"
                "2) [C]reate definition" "\n"
                "3) [S]ave a guessing tree to file" "\n"
                "4) [R]ead tree from file" "\n"
                "5) L[e]ave" "\n"
                ": ");

        char command[10] = {0};

        scanf ("%s", command);

        char name[MAX_STATIC_STR_LEN] = {0};
        switch ( command[0]) {

            case 'L':
                AkinatorGuess (tree, tree->root);
            break;

            case 'C':

                printf ("Please insert name of object to define (not longer than %d chars): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%c%[^\n\0]", name, name);
                AkinatorMakeDefinition (tree, name);
            break;

            case 'S':
                printf ("Insert filename (not longer than %d characters): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%s", name);
                TreeWriteToFile (tree, name);
            break;

            case 'R':
                printf ("Insert filename (not longer than %d characters): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%s", name);
                TreeReadFromFile (tree, name);
            break;

            case 'e': return;

            default:
                printf ("Unknown command, go again" "\n");
            break;
        }
    }
}

void AkinatorGuess (Tree* tree, Nod* nod) {

    if (nod->right != NULL and nod->left != NULL) {

        printf ("Your subject %s?" "\n"
                "[y/n] : ", nod->str);

        char answer[10] = {0};
        scanf ("%s", answer);

        if (answer[0] == 'y') AkinatorGuess (tree, nod->right);
        else if (answer[0] == 'n') AkinatorGuess (tree, nod->left);
        else {

            printf ("I'll consider it a no" "\n");
            AkinatorGuess (tree, nod->left);
        }
    }
    else {

        printf ("Your subject is %s?" "\n"
                "[y/n] : ", nod->str);
        char answer[10] = {0};
        scanf ("%s", answer);

        if (answer[0] == 'y') {

            printf ("Akinator always wins!" "\n");
            return;
        }
        else if (answer[0] == 'n') {

            printf ("No fucking way! Please let me know what or who it is!" "\n"
                    "Insert a distinctive feature of your subject to distinguish it from my guess in format (please not longer than %d chars):" "\n"
                    "Your subject \"the feature\"?" "\n"
                    ": ", MAX_STATIC_STR_LEN - 1);

            char feature[MAX_STATIC_STR_LEN] = {0};
            scanf ("%c%[^\n\0]", feature, feature);

            Nod* newNod = (Nod*) calloc (1, sizeof (Nod));
            NodCtor (newNod);
            if (nod->prev->left == nod) nod->prev->left = newNod;
            else nod->prev->right = newNod;
            newNod->prev = nod->prev;
            newNod->left = nod;
            nod->prev = newNod;
            newNod->str = (char*) calloc (strlen (feature) + 1, sizeof (char));
            assert (newNod->str != NULL);
            strcpy (newNod->str, feature);

            printf ("And now please be kind to name your subject (again not longer than %d chars): ", MAX_STATIC_STR_LEN - 1);
            scanf ("%c%[^\n\0]", feature, feature);

            NodAddRight (newNod, tree);
            newNod->right->str = (char*) calloc (strlen (feature) + 1, sizeof (char));
            assert (newNod->right->str != NULL);
            strcpy (newNod->right->str, feature);

            return;
        }
    }
}

void AkinatorMakeDefinition (Tree* tree, char object[MAX_STATIC_STR_LEN]) {

    char** definition = (char**) calloc (4, sizeof (char*));
    bool*  defType    = (bool*) calloc (4, sizeof (bool));
    int    defIter    = 0;
    int    defSize    = 4;

    Nod* iter = tree->root;

    while (strcmp (object, iter->str)) {

        iter->NodNum = 0;

        if (iter->left == NULL and iter->right == NULL or
            iter->left == NULL and iter->right->NodNum == 1 or
            iter->right == NULL and iter->left->NodNum == 1 or
            iter->left->NodNum == 1 and iter->right->NodNum == 1) {

            iter->NodNum = 1;
            defIter--;
            if (iter->prev == NULL) break;
            else iter = iter->prev;
        }
        else if (iter->left != NULL and iter->left->NodNum != 1) {

            if (defIter == defSize) {

                void* temp = calloc (2 * defSize, sizeof (char*));
                assert (temp != NULL);
                memcpy (temp, definition, defSize);
                definition = (char**) temp;
                temp = calloc (2*defSize, sizeof (bool));
                assert (temp != NULL);
                memcpy (temp, defType, defSize);
                defType = (bool*) temp;
                defSize *= 2;
            }
            definition[defIter] = iter->str;
            defType[defIter]    = false;
            defIter++;
            iter = iter->left;
        }
        else if (iter->right != NULL and (iter->left == NULL or iter->left->NodNum == 1)) {

            if (defIter == defSize) {

                void* temp = calloc (2 * defSize, sizeof (char*));
                assert (temp != NULL);
                memcpy (temp, definition, defSize);
                definition = (char**) temp;
                temp = calloc (2*defSize, sizeof (bool));
                assert (temp != NULL);
                memcpy (temp, defType, defSize);
                defType = (bool*) temp;
                defSize *= 2;
            }
            definition[defIter] = iter->str;
            defType[defIter]    = true;
            defIter++;
            iter = iter->right;
        }
    }

    printf ("%s ", object);

    for (int i = 0; i < defIter - 1; i++) {

        if (defType[i]) printf ("%s, ", definition[i]);
        else {

            while (definition[i][0] != ' ') printf ("%c", *definition[i]++);
            printf (" not%s, " , definition[i]);
        }
    }

    if (defType[defIter - 1]) printf ("%s." "\n", definition[defIter - 1]);
    else {

        while (definition[defIter - 1][0] != ' ') printf ("%c", *definition[defIter - 1]++);
        printf (" not%s." "\n", definition[defIter - 1]);
    }

}

