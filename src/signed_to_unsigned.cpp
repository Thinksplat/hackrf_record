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
        // Make sure this gets freed
        std::unique_ptr<std::vector<char>> bufferobj(queue.pop());

        auto &buffer = *bufferobj;
        if (buffer.size() == 0)
        {
            return true;
        }

        char *buf = buffer.data();
        auto size = buffer.size();
        // convert each byte to unsigned
        for (unsigned int i = 0; i < size; i++)
        {
            buf[i] ^= 0x80;
        }

        fwrite(buf, 1, buffer.size(), fp);

        size += buffer.size();
        if (size > max_size)
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
    std::string file_extension = "-100000kHz-IQ.wav";

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
    const int readsize = 4096;

    BufferQueue queue;
    // start workthread in a new thread
    std::thread workthread(WorkerThread, std::ref(queue));

    // Read 4k from stdin
    while (true)
    {
        std::vector<char> *bufobj = new std::vector<char>(readsize);
        std::vector<char> &buf = *bufobj;
        int n = fread(buf.data(), 1, readsize, stdin);
        if (n <= 0)
        {
            buf.resize(0);
            queue.push(bufobj);
            break;
        }
        buf.resize(n);
        queue.push(bufobj);
    }
    std::cout << "main thread stopping\n";
    workthread.join();

    return 0;
}
