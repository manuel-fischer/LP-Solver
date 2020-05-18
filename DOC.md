# LP-Solver
A simple commandline program, that solves LP-models with the simplex algorithm,
and outputs each involved step.

## Syntax
```
lp-solve <input> [-s]

<input>     text file containing the model
-s          only print out the solution(s)
-d          use dual model
```


## Input
The input is a text file with the following **syntax:**
```ebnf
eol = "\n" | line-comment
line-comment = "#", all-characters - "\n", "\n"
sign = "+" | "-"
const = [sign], {digit}, [".", {digit}]
var = letter, {letter | digit}
linear-var = [const], var
linear = [sign] linear-var, {sign, linear-var}

objective = "min" | "max"
objective-function = objective, linear-var, "=", linear, eol
comp = "<=" | ">=" | "="
limits = ["s.d."], linear, {",", linear}, comp, const, eol
model = {eol | objective-function | limits}
```
There should only be one objective function.

An example input is:

```
max z = 3x1 + 5x2

s.d. 2x1 + 4x2 <= 40
      x1       <= 20
            x2 <= 40
        x1, x2 >= 0
```