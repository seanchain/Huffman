#include "FileEncode.h"
#include "FileDecode.h"

using namespace std;
int main(int argc, char const *argv[])
{
	string path = argv[1];
	FileEncode<char>* encodedFile;
	FileDecode<char>* decodedFile;
	decodedFile = new FileDecode<char>(path);
	delete decodedFile;
    return 0;
}
