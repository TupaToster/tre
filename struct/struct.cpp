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

void TreeCtor (Tree* tree) {

    tree->canL      = CANL;
    tree->canR      = CANR;
    tree->hash      = 0;
    tree->errCode   = ok;

    //Allocate root and set root cannaries here

   *tree->rootCanL = CANL;
   *tree->rootCanR = CANR;

    TreeCountHash (tree);

}

void TreeDtor (Tree* tree) {

    if (tree == NULL) return;

    setPoison ( tree->canL      );
    setPoison ( tree->canR      );
    setPoison (*tree->rootCanL );
    setPoison (*tree->rootCanR );
    setPoison ( tree->rootCanL );
    setPoison ( tree->rootCanR );
    setPoison ( tree->errCode   );
    setPoison ( tree->hash      );

    //Poison root here

    free      ( tree->rootCanL );
    setPoison ( tree->root     );
}

void TreeDumpInside (Tree* tree, const char* TreeName, const char* fileName, const char* funcName, size_t line) {

    if (tree == NULL) return;


    TreeVerifyHash (tree);

    if (tree == NULL){
        flogprintf ( "In file %s, function %s, line %u, Tree named %s is a NULL ptr \n", fileName, funcName, line, TreeName);
        return;
    }

    flogprintf ( "In file %s, function %s, line %u, Tree named %s was dumped : \n", fileName, funcName, line, TreeName);

    flogprintf ( "\t" "Errors : \n");
    TreeLogPrintErrors (tree);

                                           flogprintf ( "\t" "hash = %u (", tree->hash);
    if      ( tree->hash      == POISON4) flogprintf ( "POISONED)\n")
    else                                   flogprintf ( "ok)\n")

                                           flogprintf ( "\t" "canL = 0x%X (", tree->canL);
    if      ( tree->canL      == POISON4) flogprintf ( "POISONED)\n")
    else if ( tree->canL      == CANL   ) flogprintf ( "ok)\n")
    else                                   flogprintf ( "NOT_OK)\n")

                                           flogprintf ( "\t" "canR = 0x%X (", tree->canR);
    if      ( tree->canR      == POISON4) flogprintf ( "POISONED)\n")
    else if ( tree->canR      == CANR   ) flogprintf ( "ok)\n")
    else                                   flogprintf ( "NOT_OK)\n")

                                          flogprintf ( "\t" "rootCanL = 0x%X (", *tree->rootCanL);
    if      (*tree->rootCanL == POISON4) flogprintf ( "POISONED)\n")
    else if (*tree->rootCanL == CANL   ) flogprintf ( "ok)\n")
    else                                  flogprintf ( "NOT_OK)\n")

                                          flogprintf ( "\t" "rootCanR = 0x%X (", *tree->rootCanR);
    if      (*tree->rootCanR == POISON4) flogprintf ( "POISONED)\n")
    else if (*tree->rootCanR == CANR   ) flogprintf ( "ok)\n")
    else                                  flogprintf ( "NOT_OK)\n")

    if ((size_t) tree->root  == POISON4) goto A;

    //Dump Data Here

A:
    flogprintf ( "\t" "End of root" "\n" "End of dump \n");

    TreeCountHash (tree);
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

    //flog (tree->hash);
}