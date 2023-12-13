def recursionSum(x):
    print(x)

    #base case: if x == 1 -> stop
    if (x == 1):
        return 1
    return x + recursionSum(x-1)    #recursion

print("sum:", recursionSum(5))