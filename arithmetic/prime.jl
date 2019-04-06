flag = falses(1_000_000_001);
# n	  pi(10^n)
# 7	   664,579
# 8	 5,761,455
# 9	50,847,534
primes = Array{Int32}(undef, 50847534);

function print_primes2(n)
	cnt = 0
	for i = 2 : n
		# i is a prime iff flag[i] == false
		if !flag[i]
			cnt += 1
			primes[cnt] = i
		end
		j = 1
		m = i * primes[j]
		while m <= n
			flag[m] = true
			# only sieve once if i is a compound number containing
			# factor primes[j]
			if i % primes[j] == 0
				break
			end
			j += 1
			m = i * primes[j]
		end
	end
	for i = 1 : cnt
		print(primes[i], ' ')
	end
	println("")
end

n = parse(Int32, ARGS[1])
print_primes2(n)
