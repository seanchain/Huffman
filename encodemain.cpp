#include "FileEncode.h"
#include "FileDecode.h"

using namespace std;
int main(int argc, char const *argv[])
{
	string path = argv[1];
	FileEncode<char>* encodedFile;
	encodedFile = new FileEncode<char>(path);
	delete encodedFile;
    return 0;
}
