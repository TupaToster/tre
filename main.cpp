#include "protos.h"


int main (int argc, char* argv[]) {

    Tree lol = {};

    TreeCtor (&lol);

    TreeReadFromFile (&lol, "lol.save");
    AkinatorPlay (&lol);
    TreeDump(lol);
    return 0;

    TreeReadFromFile (&lol, "lol.save");
    TreeDump (lol);
    return 0;

    lol.root->str = (char*) calloc (strlen ("is zhivotnoe") + 1, sizeof (char));
    strcpy (lol.root->str, "is zhivotnoe");
    NodAddLeft (lol.root, &lol, "is kurator");
    NodAddRight (lol.root, &lol, "is bidlo");
    NodAddLeft (lol.root->left, &lol, "Rtshnik");
    NodAddRight (lol.root->left, &lol, "Leorius Maltz");
    NodAddLeft (lol.root->right, &lol, "Ded");
    NodAddRight (lol.root->right, &lol, "Maks");
    TreeWriteToFile (&lol, "lol.save");
    TreeDump (lol);
    return 0;


    AkinatorPlay (&lol);
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