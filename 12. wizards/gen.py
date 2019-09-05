from random import randrange, random, choice

print(200, randrange(200), randrange(200))
print(" ".join(map(str,[randrange(101) for i in range(200)])))
print(" ".join(map(str,[choice([randrange(5),-1,-1,-1,-1,-1,-1]) for i in range(200)])))