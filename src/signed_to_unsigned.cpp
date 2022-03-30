#include <iostream>
#include <thread>
#include <string>
#include "lib/queue.h"
#include "lib/waveheader.h"
#include "lib/QueueDataSource.h"
#include "lib/ConvertDataSource.h"
#include "lib/FileDataSink.h"

// Returns true when the read fails.
bool ProcessQueue(IDataWriter &writer, IDataSource &queue)
{
    const unsigned int max_size = 1000000000;
    unsigned int writesize = 0;
    while (true)
    {
        auto buffer = queue.GetData();
        if (buffer.size() == 0)
        {
            return true;
        }

        writer.PutData(buffer);

        writesize += buffer.size();
        if (writesize > max_size)
        {
            return false;
        }
    }
}

bool ProcessFile(IDataSink &writer, IDataSource &queue)
{
    const int sample_rate = 20000000;
    const int channels = 2;
    const int bitdepth = 8;
    WriteWaveFileHeader(writer, sample_rate, 0, channels, bitdepth);

    auto headersize = writer.Size();

    bool ret = ProcessQueue(writer, queue);

    auto filesize = writer.Size();
    writer.Rewind();
    WriteWaveFileHeader(writer, sample_rate, filesize - headersize, channels, bitdepth);
    return ret;
}

void WorkerThread(IDataSource &queue)
{
    std::cout << "Starting worker thread\n";
    int file_index = 0;
    std::string file_prefix = "/mnt/hackrf";
    std::string file_extension = "-100000kHz-IQ.wav";

    while (true)
    {
        std::string filename = file_prefix + std::to_string(file_index) + file_extension;
        std::cout << "Opening file: " << filename << std::endl;

        FileSink sink(filename.c_str());

        bool failed = ProcessFile(sink, queue);

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
    QueueDataSource queue_data_source(queue);
    ConvertDataSource convert_data_source(queue_data_source);

    // start workthread in a new thread
    std::thread workthread(WorkerThread, std::ref(convert_data_source));

    // Read 4k from stdin
    while (true)
    {
        std::vector<char> buf(readsize);
        int n = fread(buf.data(), 1, readsize, stdin);
        if (n <= 0)
        {
            buf.resize(0);
            queue.push(buf);
            break;
        }
        if ((int)buf.size() != n)
        {
            buf.resize(n);
        }
        queue.push(buf);
    }
    std::cout << "main thread stopping\n";
    workthread.join();

    return 0;
}
