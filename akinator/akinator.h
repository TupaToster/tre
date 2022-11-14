#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "..\lib\flog.h"
#include "..\struct\struct.h"

#define MAX_STATIC_STR_LEN 1000

void AkinatorPlay (Tree* tree);

void AkinatorGuess (Tree* tree, Nod* nod);

void AkinatorMakeDefinition (Tree* tree, char object[MAX_STATIC_STR_LEN]);

