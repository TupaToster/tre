#include "protos.h"


int main (int argc, char* argv[]) {

    Tree lol = {};

    TreeCtor (&lol);

    ReadTreeFromFile (&lol, "lol.save.pohuy");
    TreeDump (lol);

    // NodAddLeft (lol.root, &lol, "le left");
    // NodAddRight (lol.root, &lol, "le right");
    // NodAddRight (lol.root->left, &lol, "le left to right");
    // TreeDump (lol);
    // NodAddLeft (lol.root->left, &lol);
    // NodAddRight (lol.root->right, &lol);
    // TreeDump (lol);
    // WriteTreeToFile (&lol, "lol.save.pohuy");
}