#include "protos.h"


int main (int argc, char* argv[]) {

    Tree lol = {};

    TreeCtor (&lol);

    NodAddLeft (lol.root);
    NodAddRight (lol.root);
    NodAddRight (lol.root->left);
    TreeDump (lol);
    NodAddLeft (lol.root->left);
    NodAddRight (lol.root->right);
    TreeDump (lol);
}