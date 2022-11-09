#include "struct.h"

void setPoisonInside (void* var, size_t sizeofVar) {

    if (var == NULL) return;

    switch(sizeofVar) {
        case 1 : *((unsigned char*     ) var)      = POISON1; break;
        case 2 : *((unsigned short*    ) var)      = POISON2; break;
        case 4 : *((unsigned int*      ) var)      = POISON4; break;
        case 8 : *((unsigned long long*) var)      = POISON8; break;

        default:
            memset (var, POISON1, sizeofVar);
        break;
    }
}

bool isPoisonInside (void* var, size_t sizeofVar) {

    if (var == NULL) return 1;

    switch(sizeofVar) {
        case 1:
            if ( *( (unsigned char*     ) var) == POISON1) return 1;
            else                                           return 0;
            break;
        case 2:
            if ( *( (unsigned short*    ) var) == POISON2) return 1;
            else                                           return 0;
            break;
        case 4:
            if ( *( (unsigned int*      ) var) == POISON4) return 1;
            else                                           return 0;
            break;
        case 8:
            if ( *( (unsigned long long*) var) == POISON8) return 1;
            else                                           return 0;
            break;
        default:
            for (int i = 0; i < sizeofVar; i++) {

                if (*( ( (unsigned char*) var) + i) != POISON1) return 0;
            }
            return 1;
            break;
    }
}

void NodCtor (Nod* nod) {

    nod->str   = NULL;
    nod->left  = NULL;
    nod->right = NULL;
    nod->prev  = NULL;
}

void NodDtorRec (Nod* nod) {

    if (nod->right != NULL) NodDtorRec (nod->right);
    if (nod->left != NULL) NodDtorRec (nod->left);

    free (nod->str);
    setPoison (nod->str);
    setPoison (nod->left);
    setPoison (nod->right);
    setPoison (nod->prev);
    free (nod);
}

void TreeCtor (Tree* tree) {

    tree->canL      = CANL;
    tree->canR      = CANR;
    tree->hash      = 0;
    tree->errCode   = ok;
    tree->rootCanL  = (unsigned int*) calloc (sizeof (unsigned int) * 2 + sizeof (Nod), 1);
    tree->root      = (Nod*) (tree->rootCanL + 1);
    tree->rootCanR  = (unsigned int*) (tree->root + 1);

    NodCtor (tree->root);

   *tree->rootCanL = CANL;
   *tree->rootCanR = CANR;

    TreeCountHash (tree);
}

void TreeDtor (Tree* tree) {

    if (tree == NULL) return;

    setPoison ( tree->canL      );
    setPoison ( tree->canR      );
    setPoison (*tree->rootCanL  );
    setPoison (*tree->rootCanR  );
    setPoison ( tree->rootCanR  );
    setPoison ( tree->errCode   );
    setPoison ( tree->hash      );

    NodDtorRec (tree->root);

    free      ( tree->rootCanL );
    setPoison ( tree->rootCanL );
    setPoison ( tree->root     );
}

void TreeDumpInside (Tree* tree, const char* TreeName, const char* fileName, const char* funcName, size_t line) {

    if (tree == NULL) return;


    TreeVerifyHash (tree);

    if (tree == NULL){
        flogprintf ( "In file %s, function %s, line %u, Tree named %s is a NULL ptr <br>", fileName, funcName, line, TreeName);
        return;
    }

    flogprintf ( "In file %s, function %s, line %u, Tree named %s was dumped : <br>", fileName, funcName, line, TreeName);

    flogprintf ( "<pre>    " "Errors : <br>");
    TreeLogPrintErrors (tree);

                                           flogprintf ( "<pre>    " "hash = %u (", tree->hash);
    if      ( isPoison (tree->hash)) flogprintf ( "POISONED)<br>")
    else                                   flogprintf ( "ok)<br>")

                                           flogprintf ( "<pre>    " "canL = 0x%X (", tree->canL);
    if      ( isPoison (tree->canL)) flogprintf ( "POISONED)<br>")
    else if ( tree->canL      == CANL   ) flogprintf ( "ok)<br>")
    else                                   flogprintf ( "NOT_OK)<br>")

                                           flogprintf ( "<pre>    " "canR = 0x%X (", tree->canR);
    if      ( isPoison (tree->canR)) flogprintf ( "POISONED)<br>")
    else if ( tree->canR      == CANR   ) flogprintf ( "ok)<br>")
    else                                   flogprintf ( "NOT_OK)<br>")

                                          flogprintf ( "<pre>    " "rootCanL = 0x%X (", *tree->rootCanL);
    if      (isPoison (*tree->rootCanL)) flogprintf ( "POISONED)<br>")
    else if (*tree->rootCanL == CANL   ) flogprintf ( "ok)<br>")
    else                                  flogprintf ( "NOT_OK)<br>")

                                          flogprintf ( "<pre>    " "rootCanR = 0x%X (", *tree->rootCanR);
    if      (isPoison (*tree->rootCanR)) flogprintf ( "POISONED)<br>")
    else if (*tree->rootCanR == CANR   ) flogprintf ( "ok)<br>")
    else                                  flogprintf ( "NOT_OK)<br>")

    if (isPoison (tree->root) or tree->root == NULL) goto A;

    TreeGraphicDump (tree);

A:
    flogprintf ( "<pre>    " "End of root" "<br>" "End of dump <br>");

    TreeCountHash (tree);
}

void PrintNod (Nod* nod, int NodNumber, int depth, FILE* picSource, char ranking[][1000]) {

    #define picprintf(...) fprintf (picSource, __VA_ARGS__)

    picprintf ("\t" "\"Nod_%d\" [shape = \"record\", style = \"filled\", fillcolor = \"#9feb83\", label = \"{ <prev> Prev = %p | Value = \"%s\" | { <left> Left = %p | <right> Right = %p} \"]\n",
                NodNumber, nod->prev, nod->str == NULL ? "" : nod->str, nod->left, nod->right);


    if (ranking[depth][0] == 0) {

        sprintf (ranking[depth], "{ rank = same; ");
    }

    sprintf (ranking[depth], "Nod_%d ; ");

    if (nod->left != NULL) {

        picprintf ("\t" "\"Nod_%d\":left -> \"Nod_%d\":prev\n", NodNumber, NodNumber + 1);
        PrintNod(nod->left, NodNumber + 1, depth + 1, picSource, ranking);
    }
    if (nod->right != NULL) {

        picprintf ("\t" "\"Nod_%d\":right -> \"Nod_%d\":prev\n", NodNumber, NodNumber + 2);
        PrintNod (nod->right, NodNumber + 2, depth + 1, picSource, ranking);
    }

    #undef picprintf
}

void TreeGraphicDump (Tree* tree) {

    #define picprintf(...) fprintf (picSource, __VA_ARGS__)

    assert (tree != NULL);

    static int GraphDumpCounter = 0;
    static char ranking[1000][1000];

    for (int i = 0; i < 1000;i++) ranking[i][0] = '\0';

    char srcName[] = "GraphDumpSrc.dot";
    char picName[30] = "GraphDumpPic";
    sprintf (picName, "%d.png", GraphDumpCounter);

    FILE* picSource = fopen (srcName, "w");
    assert (picSource != NULL);

    picprintf ("digraph List_%d {" "\n", GraphDumpCounter);
    picprintf ("\t" "graph [dpi = 100];" "\n");
    picprintf ("\t" "splines = ortho" "\n");
    picprintf ("\t" "rankdir = TB" "\n");

    PrintNod (tree->root, 0, 0, picSource, ranking);

    picprintf ( "}");

    fclose (picSource);

    char command[100] = "";
    sprintf (command, "dot -Tpng %s -o %s", srcName, picName);

    system (command);

    flogprintf("<pre>\n");
    flogprintf("<h2>Tree dump</h2>\n");
    flogprintf("<img src = \"%s\">\n", picName);
    flogprintf("<hr>\n");

    GraphDumpCounter++;

    #undef picprintf
}

unsigned long long TreeErrCheck (Tree* tree) {

    if (tree == NULL) return 0;

    //checking for poison
    if (isPoison ( tree->errCode  )) {
        tree->errCode = POISONED_ERRCOD;
        return tree->errCode;
    }
    if (isPoison ( tree->canL     ) or
        isPoison ( tree->canR     ) or
        isPoison ( tree->root    ) or
        isPoison ( tree->rootCanL) or
        isPoison (*tree->rootCanL) or
        isPoison ( tree->rootCanR) or
        isPoison (*tree->rootCanR) or
        isPoison ( tree->hash     )   ) tree->errCode |= POISON_ACCESS;

    //end of check

    if ( tree->canL      != CANL         ) tree->errCode |= BAD_CAN_L;
    if ( tree->canR      != CANR         ) tree->errCode |= BAD_CAN_R;
    if ( tree->rootCanL == NULL         ) tree->errCode |= NULL_root_CAN_L_PTR;
    if ( tree->rootCanR == NULL         ) tree->errCode |= NULL_root_CAN_R_PTR;
    if (*tree->rootCanL != CANL         ) tree->errCode |= BAD_root_CAN_L;
    if (*tree->rootCanR != CANR         ) tree->errCode |= BAD_root_CAN_R;
    if ( tree->root     == NULL         ) tree->errCode |= NULL_root_PTR;

    TreeCountHash (tree);

    return tree->errCode;
}

void TreeLogPrintErrors (Tree* tree) {

    if (tree == NULL) return;

    //TreeCountHash (tree);

    char names [11][40] = {};
    int iter = 0;
    if (tree->errCode & POISON_ACCESS        ) strcpy (names[iter++], "\t\t[POISON_ACCESS       ]\n");
    if (tree->errCode & BAD_CAN_L            ) strcpy (names[iter++], "\t\t[BAD_CAN_L           ]\n");
    if (tree->errCode & BAD_CAN_R            ) strcpy (names[iter++], "\t\t[BAD_CAN_R           ]\n");
    if (tree->errCode & BAD_root_CAN_L      ) strcpy (names[iter++],  "\t\t[BAD_root_CAN_L      ]\n");
    if (tree->errCode & BAD_root_CAN_R      ) strcpy (names[iter++],  "\t\t[BAD_root_CAN_R      ]\n");
    if (tree->errCode & NULL_root_PTR       ) strcpy (names[iter++],  "\t\t[NULL_root_PTR       ]\n");
    if (tree->errCode & NULL_root_CAN_L_PTR ) strcpy (names[iter++],  "\t\t[NULL_root_CAN_L_PTR ]\n");
    if (tree->errCode & NULL_root_CAN_R_PTR ) strcpy (names[iter++],  "\t\t[NULL_root_CAN_R_PTR ]\n");
    if (tree->errCode & POISONED_ERRCOD      ) strcpy (names[iter++], "\t\t[POISONED_ERRCOD     ]\n");
    if (tree->errCode & WRONG_HASH           ) strcpy (names[iter++], "\t\t[WRONG_HASH          ]\n");

    if (iter == 0) flogprintf ( "\t\t[ok]\n")
    else
        for (int i = 0; i < iter; i++) flogprintf ( "%s", names[i])

}

void TreeCountHash (Tree* tree) {

    if (tree == NULL) return;

    unsigned int newHash = 0, multiplier = 1;
    char* ptr = (char*) &tree->canL;

    for (int i = 0; i < sizeof (unsigned int); i++) {

        newHash += ((unsigned int) *ptr++) * multiplier;
        multiplier *= MULT;
    }

    ptr = (char*) &tree->root;

    while (ptr != ((char*) &tree->canR) + sizeof (unsigned int)) {

        newHash += ((unsigned int) *ptr++) * multiplier;
        multiplier *= MULT;
    }

    ptr = (char*) tree->rootCanL;

    if (tree->rootCanL != NULL) {
        while (ptr != ((char*) tree->rootCanR) + sizeof (unsigned int)) {

            newHash += ((unsigned int) *ptr++) * multiplier;
            multiplier *= MULT;
        }
    }

    tree->hash = newHash;

}

void TreeVerifyHash (Tree* tree) {

    if (tree == NULL) return;

    unsigned int oldHash = tree->hash;
    TreeCountHash (tree);

    if (tree->hash != oldHash) {

        tree->errCode |= WRONG_HASH;
    }

}

void NodAddLeft (Nod* nod, char* value) {

    assert (nod != NULL);
    if (nod->left != NULL) return;

    nod->left = (Nod*) calloc (1, sizeof (Nod));
    assert (nod->left != NULL);

    NodCtor (nod->left);
    nod->left->prev = nod;
    nod->left->str  = value;
}

void NodAddRight (Nod* nod, char* value) {

    assert (nod != NULL);
    if (nod->right != NULL) return;

    nod->right = (Nod*) calloc (1, sizeof (Nod));
    assert (nod->right != NULL);

    NodCtor (nod->right);
    nod->right->prev = nod;
    nod->right->str  = value;
}
