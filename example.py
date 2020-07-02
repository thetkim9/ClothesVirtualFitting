from subprocess import Popen, PIPE
import sys
p = Popen(['./abc.out'], shell=True, stdout=PIPE, stdin=PIPE)
for i in range(10):
    temp = str(i) + '\n'
    value = temp.encode('UTF-8')  # Needed in Python 3.
    p.stdin.write(value)
    p.stdin.flush()
    result = p.stdout.readline().strip()
    sys.stdout.write(result)
