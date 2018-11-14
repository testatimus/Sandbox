#include <iostream>

using Real = double;
constexpr Real epsilon = 1e-8;


template <typename ...> class Point {};

template <typename Coordinate, typename ...Rest_of_basis> class Point <Coordinate, Rest_of_basis...> : public Point<Rest_of_basis...>
{
public:
    Point(Coordinate c, Rest_of_basis... tail) : Point<Rest_of_basis...>(tail...), coordinate(c) {}
    
private:
    Coordinate    coordinate;
};

template <typename ...> class Function
{
};


template <typename Argument_variable, typename ...Rest_of_arguments> class Function <Argument_variable, Rest_of_arguments...> : public Function<Rest_of_arguments...>
{
public:
    Function(Argument_variable variable, Rest_of_arguments... tail) : Function<Rest_of_arguments...>(tail...), x(variable) {}
    
    Function<Argument_variable, Rest_of_arguments...> operator()(Argument_variable) const
    {
        return *this;
    }
  
private:
    Argument_variable x;
};

template <typename F, typename Argument_variable, typename ...Rest_of_arguments> class Function <F, Argument_variable, Rest_of_arguments...> : public Function<Argument_variable, Rest_of_arguments...>
{
public:
    Function(F f, Argument_variable x, Rest_of_arguments... tail) : Function<Argument_variable, Rest_of_arguments...>(x, tail...), f(f) {}
    
    Function<F, Argument_variable, Rest_of_arguments...> operator/(Function g) const
    {
        return Function {[this,g](Point<Argument_variable, Rest_of_arguments...> x) -> Real { return f(x) / g(x); }};
    }
    
    Real operator()(Point<Argument_variable, Rest_of_arguments...> x)
    {
        return f(x);
    }
    
private:
    F f;
};
    
    
//template <typename F, typename X, typename ...YZ> Function<F, X, YZ...> d(Function<F, X, YZ...> f)
//{
    
    
//return Function<F, X, YZ...> {[f](Point<X, YZ...> x) -> Real { return f(x + epsilon) - f(x); }};
//}

template <typename X> auto d(X x)
{
    auto f = [](Point<Real>) -> Real { return epsilon; };
    return Function<decltype(f), X>(f, x);
}

class X {};
class Y {};
class Z {};

auto Derive(X x, Y y, Z z)
{
    auto dx = d(x);
    
    Point<Real> v {0.0};
    
    Real foo = dx(v);
    
    std::cout << foo << std::endl;
}

int main()
{
    X x;
    Y y;
    Z z;
    Derive(x, y, z);
    return 0;
}


/*Function d(Variable)
{
    return Function {[](Real) -> Real { return epsilon; }};
}

Function Derive(Function f, Variable x)
{
    return d(f(x)) / d(x);
}

Real Derive(Function f, Variable x, Real point)
{
    return (d(f(x)) / d(x))(point);
}*/
