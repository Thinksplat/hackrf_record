#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include "lib/queue.h"
#include "lib/waveheader.h"

bool ProcessQueue(FILE *fp, BufferQueue &queue)
{
    int max_size = 1000000000;
    int size = 0;
    while (true)
    {
        auto buffer = queue.pop();
        if (buffer.size() == 0)
        {
            return true;
        }

        char *buf = &buffer[0];
        // convert each byte to unsigned
        for (unsigned int i = 0; i < buffer.size(); i++)
        {
            buf[i] ^= 0x80;
        }

        fwrite(buf, 1, buffer.size(), fp);

        size += buffer.size();
        if(size > max_size)
        {
            return false;
        }

    }
}

bool ProcessFile(FILE *fp, BufferQueue &queue)
{
    int sample_rate = 20000000;
    WriteWaveFileHeader(fp, sample_rate, 0, 2, 8);
    bool ret = ProcessQueue(fp, queue);
    auto size = ftell(fp);
    // move to beginning
    fseek(fp, 0, SEEK_SET);
    WriteWaveFileHeader(fp, sample_rate, size, 2, 8);
    return ret;
}

void WorkerThread(BufferQueue &queue)
{
    std::cout << "Starting worker thread\n";
    int file_index = 0;
    std::string file_prefix = "/mnt/hackrf";
    std::string file_extension = ".wav";

    while (true)
    {
        std::string filename = file_prefix + std::to_string(file_index) + file_extension;
        std::cout << "Opening file: " << filename << std::endl;

        // Open file
        FILE *fp = fopen(filename.c_str(), "wb");
        bool failed = ProcessFile(fp, queue);
        fclose(fp);
        if (failed)
        {
            break;
        }
        file_index++;
    }

    std::cout << "worker thread stopping\n";
}

int main(int argc, char *argv[])
{
    BufferQueue queue;
    // start workthread in a new thread
    std::thread workthread(WorkerThread, std::ref(queue));

    // Read 4k from stdin
    while (true)
    {
        std::vector<char> buf(4086);
        int n = fread(&buf[0], 1, 4096, stdin);
        if (n <= 0)
        {
            buf.resize(0);
            queue.push(buf);
            break;
        }
        buf.resize(n);
        queue.push(buf);
    }
    std::cout << "main thread stopping\n";
    workthread.join();

    return 0;
}
