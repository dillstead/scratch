import fileinput

def next_num(nums):
    if all(num == 0 for num in nums):
        return 0
    pairs = [(nums[i], nums[i + 1]) for i in range(len(nums) - 1)]
    diffs = list(map(lambda x: x[1] - x[0], pairs))
    return nums[-1] + next_num(diffs)

sum = 0;
for line in fileinput.input():
    sum += next_num(list(map(int, line.strip().split())))
print(sum)

    




    
    
    


    
                

