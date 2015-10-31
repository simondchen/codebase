def power(x,n):
	result=1
	num=1
	for i in range(n):
		result*=x	
	num+=1
	return (result,num)
	
print power(2,3)
