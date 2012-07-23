f1 = open("data.txt")
f2 = open("pareto.txt")
out = open("result.txt", 'w')

for line in zip(f1.readlines(), f2.readlines()):
    if line[0].split() != []:
        l1 = line[0].split()
        l2 = line[1].split()
        s1 = ' '.join(l1[:-1])
        s2 = float(l2[-1]) / float(l1[-1])
        out.write(s1 + ' ' + str(s2) + '\n')
    else:
        out.write('\n')
