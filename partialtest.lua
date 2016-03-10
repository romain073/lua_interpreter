function fibonacci(n)
    if n<3 then
        return 1
    else
        return fibonacci(n-1) + fibonacci(n-2)
    end
end

for n = 1, 16 do
<<<<<<< HEAD
    io.write(fibonacci(n), ", ")
end
io.write("...\n")

print "enter a number:"
n = io.read("*number")
for i = 2,n do
  if n%i==0 then
    io.write(i)
    io.write(" is a factor of ")
    print(n)
  end
end

print "enter a number:"
n = io.read("*number")
factorial = 1
x=1
for i = 2,n do
  x = x * i
end
io.write("factorial of ")
io.write(n)
io.write(" is ")
print(x)
=======
    io.write(fibonacci(n))
    io.write(", ")
end
io.write("...\n")
>>>>>>> 57d86227d31cdeca15ac1930b90c182978331716
