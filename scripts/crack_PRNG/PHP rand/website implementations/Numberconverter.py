seeds = "vLblmrouMX1VaMhrTLrwpW" + "seriRxcgY0imbDrwK9yZti" + "OuwNZCvGAcaLqaRIXephUd"
chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
nums2 = []
nums = [29, 52, 12, 33, 41, 41, 6, 21, 42, 45, 52, 50, 12, 38, 60, 42, 6, 46, 10, 6, 27, 56, 60, 39, 0, 53, 32, 57, 60, 36, 50, 27, 26, 1, 61, 5, 42, 5, 27, 22, 50, 17, 11, 1, 55, 10, 44, 0, 56, 54, 6, 22, 48, 5, 0, 49, 58, 33, 45, 56, 7, 33, 22, 33, 35, 21, 39, 15, 26, 4]
seed = ""


for i in range(len(nums)):
	seed += chars[nums[i]]

for i in range(len(seed)):
	nums2.append(chars.index(seeds[i]))
	if i > 30:
		print(str(nums2[i-31]) + " + " + str(nums2[i-3]) + " = " + str(nums2[i]))



