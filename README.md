# THE INTENT
it has been a curiosy of mine to create math libraries that are independent of
IEEE standards for no reason once so ever. There libraries that support
'BigNumbers' (numbers supporting  memory size larger than doubles) but they lack
some key components about number representation. Some things that I would like
to explore:


- create a fraction library. such that the numbers are represented linearly in
  the form of `f = n + 1/d` where `n` and `d` use same amount of memory
  allocation and `d=0` represents a point at infinity. currently `float`s and
  `double`s support numbers in the form of `r= s(1+1/d)*2^e` (kinda) where `s`
  is the sign bit, `d` is the fractional bits in base 2 and `e` is the exponent
  bits.
- explore number representations in the form of `q=n^|n_e| + d^-|d_e|`.
- explore number representations in the form of `c*b^e`? `z=x^y` => `x=nthroot()`
- if `z=x*y`, `x=div(z,y)`, `y=div(z,x)` and `z=x^y` lead to `x=nthrt(z,y)`,
  `y=log(z,x)` then for any higher order function `z=f(x,y)` leads to split
  inverses of the kind `x=f^{-1}_{rhs}(z,y)` and `y=f^{-1}_{lhs}(z,x)`. for
  example the rational number $n \times \sqrt[1]{d} =n+d1$
- explore number representations in the form of.

## TODO List
- have better documentation. I am kinda lost.