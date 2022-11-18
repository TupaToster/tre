#include "akinator.h"

void AkinatorPlay (Tree* tree) {


    for (;;) {

        printf ("Welcome to akinator special software incorporated (a.k.a. as ASS inc.)" "\n"
                "Choose what u prefer to do:" "\n"
                "1) [L]et akinator guess" "\n"
                "2) [C]reate definition" "\n"
                "3) S[h]ow difference between two objects" "\n"
                "4) [S]ave a guessing tree to file" "\n"
                "5) [R]ead tree from file" "\n"
                "6) L[e]ave" "\n"
                ": ");

        char command = '\0';

        scanf ("%c%*c", &command);

        char name[MAX_STATIC_STR_LEN] = {0};
        char name2[MAX_STATIC_STR_LEN] = {0};
        switch (command) {

            case 'L':
                AkinatorGuess (tree, tree->root);
            break;

            case 'C':

                printf ("Please insert name of object to define (not longer than %d chars): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%[^\n\0\r]%*c", name);
                AkinatorMakeDefinition (tree, name);
            break;

            case 'S':
                printf ("Insert filename (not longer than %d characters): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%[^\n\0\r]%*c", name);
                TreeWriteToFile (tree, name);
            break;

            case 'R':
                printf ("Insert filename (not longer than %d characters): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%[^\0\n\r]%*c", name);
                TreeReadFromFile (tree, name);
            break;

            case 'h':
                printf ("Please insert 1 object's name (not longer than %d characters): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%[^\0\n\r]%*c", name);
                printf ("Please insert 2 object's name (not longer than %d characters): ", MAX_STATIC_STR_LEN - 1);
                scanf ("%[^\0\n\r]%*c", name2);
                AkinatorShowDifference (tree, name, name2);
            case 'e': return;

            case '\0':
                printf ("blyat");
                return;

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

        char answer = '\0';
        scanf ("%c%*c", &answer);

        if (answer == 'y') AkinatorGuess (tree, nod->right);
        else if (answer == 'n') AkinatorGuess (tree, nod->left);
        else {

            printf ("I'll consider it a no" "\n");
            AkinatorGuess (tree, nod->left);
        }
    }
    else {

        printf ("Your subject is %s?" "\n"
                "[y/n] : ", nod->str);
        char answer = '\0';
        scanf ("%c%*c", &answer);

        if (answer == 'y') {

            printf ("Akinator always wins!" "\n");
            return;
        }
        else if (answer == 'n') {

            printf ("No fucking way! Please let me know what or who it is!" "\n"
                    "Insert a distinctive feature of your subject to distinguish it from my guess in format (please not longer than %d chars):" "\n"
                    "Your subject \"the feature\"?" "\n"
                    ": ", MAX_STATIC_STR_LEN - 1);

            char feature[MAX_STATIC_STR_LEN] = {0};
            scanf ("%c%[^\n\0]%*c", feature, feature);

            Nod* newNod = NodCtor (nod->prev, feature, nod);
            if (nod->prev->left == nod) nod->prev->left = newNod;
            else nod->prev->right = newNod;
            nod->prev = newNod;

            printf ("And now please be kind to name your subject (again not longer than %d chars): ", MAX_STATIC_STR_LEN - 1);
            scanf ("%[^\n\0]%*c", feature);

            NodAddRight (newNod, tree);
            newNod->right->str = (char*) calloc (strlen (feature) + 1, sizeof (char));
            assert (newNod->right->str != NULL);
            strcpy (newNod->right->str, feature);

            return;
        }
    }
}

void AkinatorMakeDefinition (Tree* tree, char name[MAX_STATIC_STR_LEN]) {

    Nod* nod = NULL;

    nod = TreeDFS (tree, tree->root, name);

    if (nod == NULL) {

        printf ("This object does not exist in the tree" "\n");
        return;
    }

    int size = nod->NodNum;
    char** def = (char**) calloc (size, sizeof (char*));
    assert (def != NULL);
    char*   defType = (char*) calloc (size, sizeof (char));
    assert (defType != NULL);

    for (int i = 0; nod->prev != NULL and i < size; i++) {

        def[i] = nod->str;
        if (nod->prev->right == nod) defType[i + 1] = 1;
        else defType[i + 1] = 0;

        nod = nod->prev;
    }

    def[size - 1] = tree->root->str;

    printf ("%s ", def[0]);

    for (int i = size - 1; i > 1; i--) {

        if (defType[i]) printf ("%s, ", def[i]);
        else {

            while (isalpha (def[i][0])) printf ("%c", *def[i]++);
            printf (" not%s, ", def[i]);
        }
    }

    if (defType[1]) printf ("%s." "\n", def[1]);
    else {

        while (isalpha (def[1][0])) printf ("%c", *def[1]++);
        printf (" not%s." "\n", def[1]);
    }

    memset (def, 0, size * sizeof (char*));
    free (def);
    memset (defType, 0, size * sizeof (char));
    free (defType);
}

void AkinatorShowDifference (Tree* tree, char object1[MAX_STATIC_STR_LEN], char object2[MAX_STATIC_STR_LEN]) {

    Nod* nod1 = TreeDFS (tree, tree->root, object1);
    Nod* nod2 = TreeDFS (tree, tree->root, object2);

    if (nod1 == NULL or nod2 == NULL) {

        printf ("One or more objects is not in the tree" "\n");
        return;
    }

    Nod* LCA = TreeLCA (tree, nod1, nod2);
    AkinatorMakeDefinition (tree, nod1->str);

    printf ("But %s is different in next categories :" "\n", nod2->str);

    int size = nod2->NodNum - LCA->NodNum + 1;
    char** def = (char**) calloc (size, sizeof (char*));
    assert (def != NULL);
    char*   defType = (char*) calloc (size, sizeof (char));
    assert (defType != NULL);

    for (int i = 0; nod2 != LCA and i < size; i++) {

        def[i] = nod2->str;
        if (nod2->prev->right == nod2) defType[i + 1] = 1;
        else defType[i + 1] = 0;

        nod2 = nod2->prev;
    }

    def[size - 1] = LCA->str;

    printf ("%s ", def[0]);

    for (int i = size - 1; i > 1; i--) {

        if (defType[i]) printf ("%s, ", def[i]);
        else {

            while (isalpha (def[i][0])) printf ("%c", *def[i]++);
            printf (" not%s, ", def[i]);
        }
    }

    if (defType[1]) printf ("%s." "\n", def[1]);
    else {

        while (isalpha (def[1][0])) printf ("%c", *def[1]++);
        printf (" not%s." "\n", def[1]);
    }

    memset (def, 0, size * sizeof (char*));
    free (def);
    memset (defType, 0, size * sizeof (char));
    free (defType);

}