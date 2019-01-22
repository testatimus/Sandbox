#include <functional>
#include <tuple>

using Real = double;
constexpr Real epsilon = 1e-8;

template <typename ...XYZ> using Point = std::tuple<XYZ...>;

template <std::size_t I, typename X, typename ...YZ> X coordinate(Point<X, YZ...> x) { return std::get<I>(x); }
template <std::size_t I, typename X, typename ...YZ> X variable(Point<X, YZ...> x) { return coordinate<I>(x); }

template <typename ...XYZ> using Function_body = std::function<Real(Point<XYZ...>)>;

template <typename ...XYZ> class Function
{
public:
    Function(Function_body<XYZ...> f) : f(f) {}
    
    Real operator()(Point<XYZ...> x) { return f(x); }
    
private:
    Function_body<XYZ...> f;
    
};

template <typename ...XYZ> Function<XYZ...> d(Real)
{
    return Function<XYZ...> {[](Point<XYZ...>) -> Real { return epsilon; }};
}

template <std::size_t I, typename ...XYZ> Function<XYZ...> d(Function<XYZ...> f)
{
    return Function<XYZ...> {[&f](Point<XYZ...> point) ->Real { return f(point) / d<XYZ...>(coordinate<I>(point))(point); }};
}

template <std::size_t I, typename ...XYZ> Function<XYZ...> div(Function<XYZ...> f)
{
    return Function<XYZ...> {[&f](Point<XYZ...> point) ->Real { return d<I, XYZ...>(f)(point); }};
}

template <typename X, std::size_t I, typename ...YZ> Function<X, YZ...> div(Function<X, YZ...> f)
{
    return Function<X, YZ...> {[&f](Point<X, YZ...> point) ->Real { return d<I, X, YZ...>(f) + div<I + 1, YZ...>(f)(point); }};
}

template <typename ...XYZ> Function<XYZ...> div(Function<XYZ...> f)
{
    return div<0, XYZ...>(f);
}

using Point_3d = Point<Real,Real,Real>;
using Function_3d = Function<Real, Real, Real>;

Real x(Point_3d point) { return coordinate<0>(point); }
Real y(Point_3d point) { return coordinate<1>(point); }
Real z(Point_3d point) { return coordinate<2>(point); }

template <std::size_t I> Function_3d d(Function_3d f) { return d<I, Real,Real,Real>(f); }

Function_3d div(Function_3d f) { return div<Real,Real,Real>(f); }

void test()
{
    Point_3d p {0.0,5.5,6.6};
    
    Function_3d f {[](Point_3d p) -> Real
    {      
        return x(p) + y(p) + z(p);
    }};
    
    Function_3d df_dx { d<0>(f) };
    Function_3d divF { div(f) };
    
}
