//read pyc file
#ifndef BUFFERED_INPUT_STREAM_HPP
#define BUFFERED_INPUT_STREAM_HPP

#include <stdio.h>
#define BUFFER_LEN 256

class BufferedInputStream {
    private:
        FILE *fp;
        char szBuffer[BUFFER_LEN];
        unsigned short index;

    public:
        BufferedInputStream(char const *filename) {
            if ((fp = fopen(filename,"rb")) == NULL) {
                throw "Error:can't find the file!";
            }
            fread(szBuffer,BUFFER_LEN * sizeof(char),1,fp);
            index = 0;
        }

        BufferedInputStream() {
            close();
        }

        char read() {
            if (index < BUFFER_LEN) 
                return szBuffer[index++];
            else {
                index = 0;
                fread(szBuffer,BUFFER_LEN * sizeof(char),1,fp);
                printf("%c",szBuffer[index]);
                return szBuffer[index++];
            }
        }

        int read_int() {
            int b1 = read() & 0xff;
            int b2 = read() & 0xff;
            int b3 = read() & 0xff;
            int b4 = read() & 0xff;

            return b4 << 24 | b3 << 16 | b2 << 8 | b1;
        }

        void unread() {
            index--;
        }

        void close() {
            if (fp != NULL) {
                fclose(fp);
                fp = NULL;
            }
        }
};

#endif
