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

Nod* NodCtor (Nod* prev, const char* str, Nod* left, Nod* right) {

    Nod* nod = (Nod*) calloc (1, sizeof (Nod));
    assert (nod != NULL);

    if (str != NULL) {

        nod->str = (char*) calloc (strlen (str) + 1, sizeof (char));
        assert (nod->str != NULL);
        strcpy (nod->str, str);
    }
    else nod->str = NULL;
    nod->prev  = prev;
    nod->left  = left;
    nod->right = right;

    return nod;
}

void NodDtorRec (Nod* nod) {

    if (nod->right != NULL) NodDtorRec (nod->right);
    if (nod->left  != NULL) NodDtorRec (nod->left);

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

    tree->root = NodCtor ();

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
    TreeCountHash (tree);
}

void PrintNod (Nod* nod, int* NodNumber, int depth, FILE* picSource, int ranks[][MAX_RANKS + 1]) {

    #define picprintf(...) fprintf (picSource, __VA_ARGS__)

    nod->NodNum = *NodNumber;
    ranks[depth][0]++;
    ranks[depth][ranks[depth][0]] = *NodNumber;

    picprintf ("\t" "\"Nod_%d\" [shape = \"Mrecord\", style = \"filled\", fillcolor = \"#9feb83\", label = \"{ <prev> Prev = %p | Current = %p | Value = \\\"%s\\\" | { <left> Left = %p | <right> Right = %p} }\"]\n",
                *NodNumber, nod->prev, nod, nod->str == NULL ? "" : nod->str, nod->left, nod->right);

    *NodNumber += 1;
    if (nod->left != NULL) {

        PrintNod(nod->left, NodNumber, depth + 1, picSource, ranks);
    }
    if (nod->right != NULL) {

        PrintNod (nod->right, NodNumber, depth + 1, picSource, ranks);
    }

    #undef picprintf
}

void PrintConnections (Nod* nod, FILE* picSource) {

    #define picprintf(...) fprintf (picSource, __VA_ARGS__)

    if (nod->left != NULL) {

        picprintf ("\t" "\"Nod_%d\":left -> \"Nod_%d\";\n", nod->NodNum, nod->left->NodNum);
        PrintConnections (nod->left, picSource);
    }
    if (nod->right != NULL) {

        picprintf ("\t" "\"Nod_%d\":right -> \"Nod_%d\";\n", nod->NodNum, nod->right->NodNum);
        PrintConnections (nod->right, picSource);
    }

    #undef picprintf
}

void TreeGraphicDump (Tree* tree) {

    static int GraphDumpCounter = 0;

    #define picprintf(...) fprintf (picSource, __VA_ARGS__)

    assert (tree != NULL);

    char srcName[] = "GraphDumpSrc.dot";
    char picName[30] = "GraphDumpPic";
    sprintf (picName, "%d.png", GraphDumpCounter);

    FILE* picSource = fopen (srcName, "w");
    assert (picSource != NULL);

    picprintf ("digraph List_%d {" "\n", GraphDumpCounter);
    picprintf ("\t" "graph [dpi = 300];" "\n");
    picprintf ("\t" "rankdir = TB" "\n");

    int ranks[MAX_RANKS][MAX_RANKS + 1] = {0};
    int NodNum = 0;
    PrintNod (tree->root, &NodNum, 0, picSource, ranks);

    for (int i = 0; i < MAX_RANKS and ranks[i][0] != 0; i++) {

        picprintf ("\t" "{ rank = same; ");

        for (int j = 1; j <= ranks[i][0];j++) {

            picprintf (" Nod_%d; ", ranks[i][j]);
        }

        picprintf ("}\n");
    }

    PrintConnections (tree->root, picSource);

    picprintf ( "}");

    fclose (picSource);

    char command[200] = "";
    sprintf (command, "D:\\Graphviz\\bin\\dot.exe -Tpng %s -o %s", srcName, picName);

    system (command);

    flogprintf("<pre>\n");
    flogprintf("<h2>Tree dump</h2>\n");
    flogprintf("<img src = \"%s\" style = \"width: 55%%; height: auto\"/>\n", picName);
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

    DataCountHash (tree->root, &newHash, &multiplier);

    tree->hash = newHash;

}

void DataCountHash (Nod* nod, unsigned int* hash, unsigned int* multiplier) {

    char* iter = (char*) &(nod->left);

    for (char* iter = (char*)&nod->left; iter < (char*) (&nod->prev + 1);iter++) {

        *hash += *multiplier * (unsigned int)(*iter);
        *multiplier *= MULT;
    }



    for (int i = 0;nod->str != NULL and i < strlen (nod->str); i++) {

        *hash += *multiplier * ((unsigned int)nod->str[i]);
        *multiplier *= MULT;
    }

    if (nod->left != NULL) (nod->left, hash, multiplier);
    if (nod->right != NULL) (nod->right, hash, multiplier);
}

void TreeVerifyHash (Tree* tree) {

    if (tree == NULL) return;

    unsigned int oldHash = tree->hash;
    TreeCountHash (tree);

    if (tree->hash != oldHash) {

        tree->errCode |= WRONG_HASH;
    }

}

void NodAddLeft (Nod* nod, Tree* tree, char* value) {

    TreeVerifyHash (tree);

    assert (nod != NULL);
    if (nod->left != NULL) return;

    nod->left = NodCtor (nod, value);

    TreeCountHash (tree);
}

void NodAddRight (Nod* nod, Tree* tree, char* value) {

    TreeVerifyHash (tree);

    assert (nod != NULL);
    if (nod->right != NULL) return;

    nod->right = NodCtor (nod, value);

    TreeCountHash (tree);
}

void TreeWriteToFile (Tree* tree, char* fileName) {

    assert (tree != NULL);
    assert (fileName != NULL);

    FILE* outFile = fopen (fileName, "wb");
    assert (outFile != NULL);

    WriteNodRec (tree->root, outFile);

    fclose (outFile);
    free (outFile);
}

void WriteNodRec (Nod* nod, FILE* outFile) {

    if (nod == NULL) return;
    assert (outFile != NULL);

    #define print(...) fprintf (outFile, __VA_ARGS__)

    if (nod->str != NULL) print ("{<%s>", nod->str);
    else print ("{<>");

    if (nod->left == NULL) print ("{}");
    else WriteNodRec (nod->left, outFile);

    if (nod->right == NULL) print ("{}");
    else WriteNodRec (nod->right, outFile);

    print("}");
    #undef print
}

void TreeReadFromFile (Tree* tree, char* fileName) {

    assert (tree);
    assert (fileName);

    TreeVerifyHash (tree);

    Text txt = read_Text (fileName);
    Nod* iter = tree->root;
    bool backFromTheLeft = false;

    for (int i = 1; i < txt.TextSize; i++) {

        if (txt.str[i] == '{') {

            if (txt.str[i + 1] == '}') {

                if (backFromTheLeft == 1) backFromTheLeft = 0;
                else backFromTheLeft = 1;
                i++;
            }
            else if (backFromTheLeft) {

                NodAddRight (iter, tree);
                backFromTheLeft = 0;
                iter = iter->right;
            }
            else {

                NodAddLeft (iter, tree);
                backFromTheLeft = 0;
                iter = iter->left;
            }
        }
        else if (txt.str[i] == '<') {

            for (int j = 0; i + 1 + j < txt.TextSize; j++) {

                if (txt.str[i + 1 + j] == '>') {

                    if (j != 0) {

                        iter->str = (char*) calloc (j + 1, sizeof (char));
                        assert (iter->str != NULL);

                        strncpy (iter->str, txt.str + i + 1, j);
                        iter->str[j] = '\0';
                    }
                    else {

                        iter->str = NULL;
                    }

                    i += j + 1;
                    break;
                }
            }
        }
        else if (txt.str[i] == '}') {

            if (iter->prev == NULL) break;
            else if (iter->prev->left == iter) {

                backFromTheLeft = true;
                iter = iter->prev;
            }
            else {

                backFromTheLeft = false;
                iter = iter->prev;
            }
        }
    }

    killText (&txt);
    free (iter);

    tree->errCode -= WRONG_HASH;
    TreeCountHash (tree);
}

Nod* TreeDFS (Tree* tree, Nod* nod, const char* key) {

    if (nod == NULL) return NULL;
    nod->NodNum = (nod->prev == NULL ? 1 : nod->prev->NodNum + 1);
    if (key != NULL and strcmp (nod->str, key) == 0) return nod;
    Nod* lft = TreeDFS (tree, nod->left, key);
    if (lft != NULL) return lft;
    Nod* rgt = TreeDFS (tree, nod->right, key);
    return rgt;
}

Nod* TreeLCA (Tree* tree, Nod* fst, Nod* scd) {

    TreeDFS (tree, tree->root, NULL);
    if (fst->NodNum > scd->NodNum) {

        Nod* tmp = fst;
        fst = scd;
        scd = tmp;
    }

    while (scd->NodNum > fst->NodNum) scd = scd->prev;

    while (scd != fst) {

        scd = scd->prev;
        fst = fst->prev;
    }

    return fst;
}
