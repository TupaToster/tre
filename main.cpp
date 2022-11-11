#include "protos.h"


int main (int argc, char* argv[]) {

    Tree lol = {};

    TreeCtor (&lol);


    NodAddLeft (lol.root, &lol);
    NodAddRight (lol.root, &lol);
    NodAddRight (lol.root->left, &lol);
    TreeDump (lol);
    NodAddLeft (lol.root->left, &lol);
    NodAddRight (lol.root->right, &lol);
    TreeDump (lol);
}