import os

cmd = "time ./q -qn2 -r10"

def main():
    for i in range(1,10):
        for j in range(1,10):
            r = os.popen(cmd+" "+str(i)+" "+str(j))
            for line in r:
                print(line[:-1],"meow")
            return

main()
