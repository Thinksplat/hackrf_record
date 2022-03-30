import threading, queue, sys, time, struct

def MakeHeader(sample_rate, sampleNum, bits, channels):
    """
    Create a wav header
    """
    header = bytearray(44)
    header[0:4] = b'RIFF'
    header[4:8] = struct.pack('<I', int(sampleNum * channels * bits / 8 + 36))
    header[8:12] = b'WAVE'
    header[12:16] = b'fmt '
    header[16:20] = struct.pack('<I', 16)
    header[20:22] = struct.pack('<H', 1)
    header[22:24] = struct.pack('<H', channels)
    header[24:28] = struct.pack('<I', sample_rate)
    header[28:32] = struct.pack('<I', int(sample_rate * channels * bits / 8))
    header[32:34] = struct.pack('<H', int(channels * bits / 8))
    header[34:36] = struct.pack('<H', bits)
    header[36:40] = b'data'
    header[40:44] = struct.pack('<I', int(sampleNum * channels * bits / 8))
    return header


q = queue.Queue()

def WriteToWav(f,q):
    max_size = 1000000000
    samplerate = 20000000
    size = 0

    # Write the header
    f.write(MakeHeader(samplerate, 0, 8, 2))

    while True:
        data = q.get()
        if data is None:
            break

        # convert data buffer to array of chars
        #data = bytearray(data)        
        # for i in range(0, len(data)):
        #     # convert from signed to unsigned
        #     data[i] =  data[i] ^ 0x80;
        # convert byte array back to buffer
        #data = bytes(data)

        # Write the data to stdout
        # Write bytes to stdout
        f.write(data)
        size += len(data);
        if size > max_size:
            break

    pos = f.tell()
    # Seek to the beginning of the file
    f.seek(0, 0)
    # Write the header again
    f.write(MakeHeader(samplerate, pos - 44, 8, 2))
    return size > max_size

def worker():
    fileindex = 0
    fileprefix = "/mnt/hackrf"

    stop = False
    while not stop:
        # open the file
        filename = fileprefix + str(fileindex) + ".wav"
        print("Opening file: " + filename)
        with open(filename, 'wb') as f:   
            if False == WriteToWav(f, q):
                break
            fileindex += 1  

    q.task_done()

# Turn-on the worker thread.
threading.Thread(target=worker, daemon=True).start()

# get the current time
start = time.time()
maxdepth = 0
while True:
    # read 4k from stdin
    data = sys.stdin.buffer.read(4096)
    # If no more data, exit.
    if not data or len(data) == 0:
        q.put(None);
        break
    # Put the data on the queue.
    q.put(data)
    # See how deep the queue is
    depth = q.qsize()

    # time now
    now = time.time()
    # how long since start
    elapsed = now - start

    if depth > maxdepth or elapsed > 5:
        start = now
        maxdepth = depth
        # print to stderr
        print("Queue depth: %d" % depth, file=sys.stderr)

# Block until all tasks are done.
q.join()
