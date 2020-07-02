from subprocess import Popen, PIPE
import os

p = Popen(['./a.out'], shell=True, stdout=PIPE, stdin=PIPE)
p.wait()

msg, err = p.communicate()
if msg!=None and len(msg)>0:
    print(msg)

#os.remove("images/person.jpg")
#os.remove("images/t-shirt.jpg")

#print("completed")
