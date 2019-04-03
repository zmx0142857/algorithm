#bool flag[1000000001];
# n	  pi(10^n)
# 7	   664,579
# 8	 5,761,455
# 9	50,847,534
#int primes[50847534];

###
function print_primes2(n)
	cnt = 0
	for i = 2 : n
		# i is a prime iff flag[i] == false
		if !flag[i]
			primes[cnt++] = i;
		end
		j = 0
		m = i * primes[j]
		while m <= n
			flag[m] = true
			# only sieve once if i is a compound number containing
			# factor primes[j]
			if i % primes[j] == 0
				break
			end
			++j
			m = i * primes[j]
		end
	end
	for i = 0 : cnt-1
		print(primes[i], ' ')
	println("")
end
##

n = ARGS[0]
println(n)
#print_primes2(n)
