#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "..\struct\struct.h"

#define MAX_STATIC_STR_LEN 1000

void AkinatorPlay (Tree* tree);

void AkinatorGuess (Tree* tree, Nod* nod);

void AkinatorMakeDefinition (Tree* tree, char name[MAX_STATIC_STR_LEN]);

void AkinatorShowDifference (Tree* tree, char object1[MAX_STATIC_STR_LEN], char object2[MAX_STATIC_STR_LEN]);
