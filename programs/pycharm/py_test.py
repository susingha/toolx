"""
4 3 2 1
1 3 2 4
1 2 3 4
"""


def badplace(num, idx, arr):
    if idx+1 == num:
        return True
    else:
        return False


class Solution(object):
    def firstMissingPositive(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """

        lnums = len(nums)
        for i in range(lnums):
            while badplace(nums[i], i, nums) == False:
                goodplace = nums[i]
                j = goodplace - 1
                if j >= 0 and j < lnums:
                    nums[i], nums[j] = nums[j], nums[i]
                    if nums[i] == nums[j]:
                        nums[j] = 0
                else:
                    nums[i] = 0
                    break

        print nums
        for i in range(lnums):
            if i + 1 != nums[i]:
                return i + 1

        return lnums + 1

ar = [3, 4, -1, 1]
ar = [1, 4, 3, 2, 1, 0]
ar = [1, 1]
print Solution().firstMissingPositive(ar)