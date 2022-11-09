#include "protos.h"


int main (int argc, char* argv[]) {

    Tree lol = {};

    TreeCtor (&lol);

    NodAddLeft (lol.root);
    NodAddRight (lol.root);
    NodAddLeft (lol.root->left);
    TreeDump (lol);

}