# About B-Splines and refinement

The two following references :
 - [A subdivision-based implementation of thierarchical b-spline finite element method. Computer Methods in Applied Mechanics and Engineering] [bornemann_2012]
 - [Subdivision for Modeling and Animation : SIGGRAPH 2000 Course Notes] [zorin_2000]

provide the theoretical basis of this document.

[bornemann_2012]: http://doi.org/10.1016/j.cma.2012.06.023

[zorin_2000]: https://mrl.nyu.edu/publications/subdiv-course2000/coursenotes00.pdf

In this document the **refinement factor** is denoted by \f$\alpha\f$, and the **interpolation order** by \f$\mu\f$.

In 1D, a mother macro-particle will give birth to \f$N_{1d}\f$ daughter macro-particles :
\f[
N_{1d} = (2\alpha - 1) + (\alpha -1)(\mu -1)
\f]

In 3D, a mother macro-particle will give birth to \f$N_{3d}\f$ daughter macro-particles :
\f[
N_{3d} = [ (2\alpha - 1) + (\alpha -1)(\mu -1) ] ^ 3
\f]

# Refinement of B-splines

A remarkable property of B-splines is that they obey a refinement equation. This specific property
paves the way to connect splines and subdivision ([zorin, sec 2.2.3, p. 25] [zorin_2000]).
The B-Spline of degree 0 can be written as a sum of translated and contracted transforms of itself
\f[ B_0(t) = B_0(2t) + B_0(2t-1). \f]

And the B-spline of degree \f$l\f$ is defined by
##### {#eq_spline_l}
\f[
B_l(t) = \bigotimes_{i=0}^l B_0(t),
\f]

where \f$\otimes\f$ denotes the continuous convolution operator.

This operator is classically defined for two functions \f$f(t)\f$ and \f$g(t)\f$ by
\f[
(f \otimes g)(t) = \int f(s)g(t-s) ds
\f]

Prior to developing \f$B_1\f$, let us recall basic properties of convolution for functions \f$f\f$, \f$g\f$ and \f$h\f$
\f[
\begin{eqnarray}
f(t)\otimes(g(t)+h(t)) & = & f(t) \otimes g(t) +f(t) \otimes h(t) \\
f(t-i)\otimes g(t-k)   & = & m(t-i-k) \\
f(2t)\otimes g(2t)     & = & \frac{1}{2}m(2t)
\end{eqnarray}
\f]

Let us denote the refinement factor \f$\alpha\f$.

##Arbitrary spline order and \f$\alpha = 2\f$
The **1st order spline** decomposition can be formally derived using [eq.](#eq_spline_l) , we get
\f[
\begin{eqnarray}
B_1(t) & = & B_0(t) \otimes B_0(t) \\
       & = & (B_0(2t) + B_0(2t-1)) \otimes (B_0(2t) + B_0(2t-1)) \\
       & = & \frac{1}{2}\left( B_1(2t) + 2B_1(2t-1) + B_1(2t-2) \right) \\
       & = & \frac{1}{2^{1}} \sum_{k=0}^2 \binom{2}{k} B_1(2t - k)
\end{eqnarray}
\f]

We can prove by recurrence
the following relation for the **\f$\mu\f$th order spline** decomposition
\f[
\begin{equation}
B_{\mu} (t) = \frac{1}{2^{\mu}} \sum_{k=0}^{\mu +1} \binom{\mu +1}{k}
B_{\mu} (2t - k)
\end{equation}
\f]

##Arbitrary spline order and \f$\alpha = 3\f$
In this case the B-Spline of degree 0 is defined by
\f[
\begin{equation}
B_0(t) = B_0(3t) + B_0(3t-1) + B_0(3t-2).
\end{equation}
\f]

The **1st order spline** decomposition can be formally derived using [eq.](#eq_spline_l), we get
\f[
\begin{eqnarray}
B_1(t) & = & B_0(t) \otimes B_0(t) \\
       & = & (B_0(3t) + B_0(3t-1) + B_0(3t-2)) \otimes (B_0(3t) + B_0(3t-1) + B_0(3t-2)) \\
       & = & \frac{1}{3} [ B_1(3t) + B_1(3t-1) + B_1(3t-2) +  \nonumber \\
       &   & + B_1(3t-1) + B_1(3t-2) + B_1(3t-3) + \nonumber \\
       &   & + B_1(3t-2) + B_1(3t-3) + B_1(3t-4) ] \nonumber \\
       & = & \frac{1}{3} \left[ B_1(3t) + 2B_1(3t-1) + 3B_1(3t-2) + 2B_1(3t-3) + B_1(3t-4) \right]
\end{eqnarray}
\f]

We can prove by recurrence
the following relation for the **\f$\mu\f$th order spline** decomposition
\f[
\begin{equation}
B_{\mu} (t) = \frac{1}{3^{\mu}} \sum_{k=0}^{2(\mu +1)} \binom{\mu +1}{k}_2
B_{\mu} (3t - k)
\end{equation}
\f]

Where \f$\binom{n}{k}_2\f$ corresponds to the trinomial coefficients, _i.e._ the
coefficients of the following polynome:
\f[
\begin{equation}
P(x) = (1+x+x^2)^n.
\end{equation}
\f]

As in the binomial case, a triangle can also be drawn in the trinomial case
\f[
\begin{array}{rccccccccc}
n=0:&  1 \\
n=1:&  1 &  1  &  1 \\
n=2:&  1 &  2  &  3  & 2  &  1 \\
n=3:&  1 &  3  &  6  & 7  &  6  & 3  & 1 \\
n=4:&  1 &  4  &  10 & 16 &  19 & 16 & 10 & 4 & 1 \\
\end{array}
\f]


## Arbitrary spline order with arbitrary \f$\alpha \in \mathcal{Z}^{\star}\f$

The general case must be ruled by the following formula
\f[
\begin{equation}
B_{\mu} (t) = \frac{1}{\alpha^{\mu}} \sum_{k=0}^{(\alpha-1)(\mu +1)} \binom{\mu +1}{k}_{\alpha-1}
B_{\mu} (\alpha t - k)
\end{equation}
\f]

## Special case : arbitrary \f$\alpha \in \mathbb{Z}^{\star}\f$ with \f$\mu = 1\f$ (piecewise linear)

In general, computing the multinomial coefficients can be quite puzzling.
But, we note that the 2nd line of a multinomial triangle is straightforward
\f[
\begin{array}{rccccccccc}
n=2:&  1 &  2  &  ... & \alpha -1  & \alpha  & \alpha -1  & ... & 2  &  1 \\
\end{array}
\f]

This is of major interest for the piecewise linear case as we can write the base function as a combination
of contracted transforms with arbitrary refinement factor.
In this particular case, the base function reads
\f[
\begin{equation}
B_{1} (t) = \frac{1}{\alpha} \sum_{k=0}^{2(\alpha-1)} \binom{2}{k}_{\alpha-1}
B_{1} (\alpha t - k)
\end{equation}
\f]
