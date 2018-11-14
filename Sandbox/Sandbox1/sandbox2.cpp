#include <functional>

using Real = double;

constexpr Real epsilon = 1e-8;

enum class Variable
{
    x,
    y,
    z,
};

template <typename ...Rest_of_basis> class Point {};

template <typename Coordinate, typename ...Rest_of_basis> class Point <Coordinate, Rest_of_basis...> : public Point<Rest_of_basis...>
{
public:
    Point(Coordinate c, Rest_of_basis... tail) : Point<Rest_of_basis...>(tail...), coordinate(c) {}
    
private:
    Real    coordinate;
};

template <typename ...Rest_of_arguments> class Function {};


template <typename Argument_variable, typename ...Rest_of_arguments> class Function <Argument_variable, Rest_of_arguments...> : public Function<Rest_of_arguments...>
{
public:
     
    Function(const std::function<Real(Real)>&& f) : f(f) {}
    
    Function operator()(Variable) const
    {
        return *this;
    }
    
    Real operator()(Real x) const
    {
        return f(x);
    }
    
    Function operator/(Function g) const
    {
        return Function {[this,g](Real x) -> Real { return f(x) / g(x); }};
    }
    

private:
    std::function<Real(Real)> f;
};

Function d(Function f)
{
    return Function {[f](Real x) -> Real { return f(x + epsilon) - f(x); }};
}

Function d(Variable)
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
}
