#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "..\lib\flog.h"

// DEFINE NDEBUG TO DISABLE LOGS
//Constants to specify Tree element type and element output mode

#define ELEM_TYPE int ///< Type of element a Tree operates with
#define OUT_FORM "%d" ///< Output format for printf functions

//end of block

//inside defines

#define CANL 0xDEADBEEF ///< Left cannary of a structure
#define CANR 0xD34DB33F ///< Right cannary of a structure

#define POISON1 0xBD ///< 1 byte Poison
#define POISON2 0xBDCF ///< 2 byte Poison
#define POISON4 0xBADC0FEE ///< 4 byte Poison
#define POISON8 0xBADC0FEEF04DED32 ///< 8 byte Poison

#define MULT 37u ///< Multiplier for hash

#define setPoison(var) setPoisonInside (&var, sizeof (var)) ///< Macros for auto-poison

#define isPoison(var) isPoisonInside (&var, sizeof (var)) ///< Macros for checking for poison

#ifndef NDEBUG
#define TreeDump(tree) TreeDumpInside (&tree, #tree, __FILE__, __FUNCTION__, __LINE__) ///< Tree dump macros
#else
#define TreeDump(tree) ;
#endif
//endof inside defines

/// @brief Describes errors within Tree
enum TreeErrorCodes {
    ok                   = 0,    ///< All ok
    POISON_ACCESS        = 1<<0, ///< One or more struct elements is poison
    BAD_CAN_L            = 1<<1, ///< Dead left cannary of structure
    BAD_CAN_R            = 1<<2, ///< Dead right cannary of structure
    BAD_root_CAN_L       = 1<<3, ///< Dead left cannary of  root
    BAD_root_CAN_R       = 1<<4, ///< Dead right cannary of root
    NULL_root_PTR        = 1<<5, ///< NULL ptr for root
    NULL_root_CAN_L_PTR  = 1<<6, ///< NULL ptr for left  root cannary
    NULL_root_CAN_R_PTR  = 1<<7, ///< NULL ptr for right root cannary
    POISONED_ERRCOD      = 1<<8, ///< Errcod variable is poisoned; Ususally means that struct has been destructed
    WRONG_HASH           = 1<<9 ///< Hash was changed without any changes from specified function
};

/// @brief Stores a string with links to next elements
struct Nod {

    char* str   = NULL;
    Nod*  left  = NULL;
    Nod*  right = NULL;
    Nod*  prev  = NULL;
};

/// @brief Stores a Tree with several support variables
struct Tree {

    unsigned int  canL      = 0xDEADBEEF; ///< left cannary of struct
    unsigned int  hash      = 0;          ///< hash value
    size_t        errCode   = ok;         ///< error code
    Nod*          root      = NULL;       ///< Ptr to root
    unsigned int* rootCanL  = NULL;       ///< left cannary of root
    unsigned int* rootCanR  = NULL;       ///< right cannary of root
    unsigned int  canR      = 0xD34DB33F; ///< right cannary of struct
};

/// @brief Constructs a Tree var
/// @param tree ptr to a var to construct
void TreeCtor (Tree* tree);

/// @brief Destroys a Tree var.
/// @param tree ptr to a destroyable variable
void TreeDtor (Tree* tree);

/// @brief Dumps Tree to logs_out
/// @param tree ptr to Tree
/// @param TreeName name of Tree
/// @param fileName name of file function was called at
/// @param funcName name of func function was called at
/// @param line line function was called at
void TreeDumpInside (Tree* tree, const char* TreeName, const char* fileName, const char* funcName, size_t line);

/// @brief Prints errors drom a Tree variable
/// @param tree ptr to Tree
/// @return error code tree->errCode
unsigned long long TreeErrCheck (Tree* tree);

/// @brief Prints errors for a TreeDump func
/// @param tree ptr to Tree
void TreeLogPrintErrors (Tree* tree);

/// @brief Counts hash for Tree
/// @param tree ptr to Tree
void TreeCountHash (Tree* tree);

/// @brief Verifies hash for Tree (tells if it was changed without any Tree specified functions)
/// @param tree ptr to Tree
void TreeVerifyHash (Tree* tree);

/// @brief Sets var to a poisoned state
/// @param var ptr to var
/// @param sizeofVar sizeof var
void setPoisonInside (void* var, size_t sizeofVar);

/// @brief Checks for a variable to be poisoned
/// @param var ptr to var
/// @param sizeofVar size of var
/// @return 1 if poisoned, 0 if not
bool isPoisonInside (void* var, size_t sizeofVar);

void NodAddLeft (Nod* nod, char* value = NULL);

void NodAddRight (Nod* nod, char* value = NULL);

void TreeGraphicDump (Tree* tree);

void PrintNod (Nod* nod, int NodNumber, int depth, FILE* picSource, char ranking[][1000]);