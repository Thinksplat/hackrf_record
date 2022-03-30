# hackrf_transfer -r - -f 100000000  -s 20000000 | python buffer.py  > /mnt/data.raw
hackrf_transfer -r - -f 100000000  -s 20000000 | ./signed_to_unsigned # | python bufferrecord.py
