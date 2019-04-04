function move(a, b)
	return string(a, " -> ", b)
end

function hanoi(n, a, b, c)
	if n == 1
		move(a, c)
	else
		hanoi(n-1, a, c, b)
		move(a, c)
		hanoi(n-1, b, a, c)
	end
end

# 1.817s
hanoi(24, 'a', 'b', 'c')
