# LUA Interpreter

A small interpreter supporting a subset of the LUA programming language.

## Tools
This project is using [flex](http://flex.sourceforge.net/) for lexing and [Bison](https://www.gnu.org/software/bison/) for parsing. 

## Build
```
make
```

## Run
```
./int file.lua
```

## Visualizing the parse tree
The parse tree can be visualized with [Graphviz](http://www.graphviz.org/), by using the following command:
```
dot -Tpdf tree.dot -otree.pdf
```

## Supported features
- Affectation ```a = "string"```, ```a, b = 3, 4```, ```array[i], array[i+1] = array[i+1], array[i]```
- Control flow ```if ... then ... elseif ... then ... else ...```, ```while```, ```for```
- Input & output  ```io.write()```, ```print()```, ```io.read()```
- Numerical Arrays ```a = {1,2,3}```
- Function definition and (recursive) function calls

## Examples
### Bubble sort:
```
list = { 5, 6, 1, 2, 14, 2, 15, 6, 7, 8, 97 }
itemCount=#list
repeat
  hasChanged = false
  itemCount=itemCount - 1
  for i = 1, itemCount do
    if list[i] > list[i + 1] then
      list[i], list[i + 1] = list[i + 1], list[i]
      hasChanged = true
    end
  end
until hasChanged == false
for i = 1,#list do
  print (list[i])
end
```
### Fibonacci Sequence:
```
function fibonacci(n)
    if n<3 then
        return 1
    else
        return fibonacci(n-1) + fibonacci(n-2)
    end
end

io.write(fibonacci(16))
```
