#ifndef TEMPLATE_EXPR_H
#define TEMPLATE_EXPR_H
#include <cmath>

struct expr {};

template<char, typename>
struct equation;
template<char>
struct placeholder;
template<typename, typename>
struct add_expr;
template<typename, typename>
struct sub_expr;
template<typename, typename>
struct mul_expr;
template<typename, typename, typename>
struct pow_expr;
template<typename, typename>
struct log_expr;

template<typename T, typename ...Rest>
auto invoke(T keep, Rest... rest) {
  return keep; // 自由变元、常量、字面值等
}

// x(x = 1, ...)
template<char Name, typename Val, typename ...Rest>
auto invoke(placeholder<Name> const& ph, equation<Name, Val> const& eq, Rest... rest) {
  return eq.val;
}

// x(y = 1, ...)
template<char Name, char AnotherName, typename Val, typename ...Rest>
auto invoke(placeholder<Name> const& ph, equation<AnotherName, Val> const& eq, Rest... rest) {
  return invoke(ph, rest...);
}

template<typename Lhs, typename Rhs, typename ...Args>
auto invoke(add_expr<Lhs, Rhs> const& add, Args... args) {
  return invoke(add.lhs, args...) + invoke(add.rhs, args...);
}

template<typename Lhs, typename Rhs, typename ...Args>
auto invoke(sub_expr<Lhs, Rhs> const& sub, Args... args) {
  return invoke(sub.lhs, args...) - invoke(sub.rhs, args...);
}

template<typename Lhs, typename Rhs, typename ...Args>
auto invoke(mul_expr<Lhs, Rhs> const& mul, Args... args) {
  return invoke(mul.lhs, args...) * invoke(mul.rhs, args...);
}

template<typename Lhs, typename Rhs, typename = void>
struct pow_helper {
  pow_helper(Lhs lhs, Rhs rhs): lhs(lhs), rhs(rhs) {}

  Lhs lhs;
  Rhs rhs;

  auto operator()() {
    return std::pow(lhs, rhs);
  }
};

template<typename Lhs, typename Rhs>
struct pow_helper<Lhs, Rhs, std::enable_if_t<std::is_base_of_v<expr, Rhs> || std::is_base_of_v<expr, Lhs>>> {
  pow_helper(Lhs lhs, Rhs rhs): lhs(lhs), rhs(rhs) {}

  Lhs lhs;
  Rhs rhs;

  auto operator()() {
    return lhs ^ rhs;
  }
};

template<typename Lhs, typename Rhs, typename ...Args>
auto invoke(pow_expr<Lhs, Rhs, std::enable_if_t<!(std::is_base_of_v<expr, Lhs> && std::is_base_of_v<expr, Rhs>)>> const& pow, Args... args) {
  auto lhs = invoke(pow.lhs, args...);
  auto rhs = invoke(pow.rhs, args...);

  return pow_helper{lhs, rhs}();
}

template<char Name, typename Val>
struct equation: expr {
  explicit equation(Val val): val(val) {}

  Val val;

  friend std::ostream& operator<<(std::ostream& out, equation<Name, Val> const& eq) {
    return out << "(" << Name << " = " << eq.val << ")";
  }
};

template<char Name>
struct placeholder: expr {
  static constexpr char name = Name;

  template<typename Val>
  auto operator=(Val val) const {
    return equation<Name, Val> { val };
  }

  template<typename ...Args>
  auto operator()(Args... args) const {
    return invoke(*this, args...);
  }

  friend std::ostream& operator<<(std::ostream& out, placeholder<Name> const& ph) {
    return out << ph.name;
  }
};

static constexpr auto x = placeholder<'x'>{};
static constexpr auto y = placeholder<'y'>{};
static constexpr auto z = placeholder<'z'>{};

template<typename Lhs, typename Rhs>
struct add_expr: expr {
  explicit add_expr(Lhs lhs, Rhs rhs): lhs(lhs), rhs(rhs) {}

  Lhs lhs;
  Rhs rhs;

  template<typename ...Args>
  auto operator()(Args... args) const {
    return invoke(*this, args...);
  }

  friend std::ostream& operator<<(std::ostream& out, add_expr<Lhs, Rhs> const& expr) {
    return out << "(" << expr.lhs << " + " << expr.rhs << ")";
  }
};

template<typename Lhs, typename Rhs>
auto operator+(Lhs lhs, Rhs rhs) {
  return add_expr<Lhs, Rhs> {lhs, rhs};
}

template<typename Lhs, typename Rhs>
struct sub_expr: expr {
  explicit sub_expr(Lhs lhs, Rhs rhs): lhs(lhs), rhs(rhs) {}

  Lhs lhs;
  Rhs rhs;

  template<typename ...Args>
  auto operator()(Args... args) const {
    return invoke(*this, args...);
  }

  friend std::ostream& operator<<(std::ostream& out, sub_expr<Lhs, Rhs> const& expr) {
    return out << "(" << expr.lhs << " - " << expr.rhs << ")";
  }
};

template<typename Lhs, typename Rhs>
auto operator-(Lhs lhs, Rhs rhs) {
  return sub_expr<Lhs, Rhs> {lhs, rhs};
}

template<typename Lhs, typename Rhs>
struct mul_expr: expr {
  explicit mul_expr(Lhs lhs, Rhs rhs): lhs(lhs), rhs(rhs) {}

  Lhs lhs;
  Rhs rhs;

  template<typename ...Args>
  auto operator()(Args... args) const {
    return invoke(*this, args...);
  }

  friend std::ostream& operator<<(std::ostream& out, mul_expr<Lhs, Rhs> const& expr) {
    return out << "(" << expr.lhs << " * " << expr.rhs << ")";
  }
};

template<typename Lhs, typename Rhs>
auto operator*(Lhs lhs, Rhs rhs) {
  return mul_expr<Lhs, Rhs> {lhs, rhs};
}

template<typename Lhs, typename Rhs, typename _ = void>
struct pow_expr: expr {
  explicit pow_expr(Lhs lhs, Rhs rhs): lhs(lhs), rhs(rhs) {}

  Lhs lhs;
  Rhs rhs;

  template<typename ...Args>
  auto operator()(Args... args) const {
    return invoke(*this, args...);
  }

  friend std::ostream& operator<<(std::ostream& out, pow_expr<Lhs, Rhs> const& expr) {
    return out << "(" << expr.lhs << " ^ " << expr.rhs << ")";
  }
};

template<typename Lhs, typename Rhs>
auto operator^(Lhs lhs, Rhs rhs) {
  return pow_expr<
    Lhs,
    Rhs,
    std::enable_if_t<!(std::is_base_of_v<expr, Lhs> && std::is_base_of_v<expr, Rhs>)>
  > {lhs, rhs};
}

// 求导函数
template<typename Expr, typename Var>
auto d(Expr expr, Var var) {
  return 0; // 常量求导为0
}

template<char X>
auto d(placeholder<X> x, placeholder<X> y) {
  return 1; // x对x求导，1
}

template<typename Lhs, typename Rhs, typename Var>
auto d(add_expr<Lhs, Rhs> add, Var var) {
  return d(add.lhs, var) + d(add.rhs, var); // d(f+g) = d(f) + d(g)
}

template<typename Lhs, typename Rhs, typename Var>
auto d(sub_expr<Lhs, Rhs> sub, Var var) {
  return d(sub.lhs, var) - d(sub.rhs, var); // d(f-g) = d(f) - d(g)
}

template<typename Lhs, typename Rhs, typename Var>
auto d(mul_expr<Lhs, Rhs> mul, Var var) {
  return d(mul.lhs, var) * mul.rhs + mul.lhs * d(mul.rhs, var); // d(f*g) = d(f)*g + f*d(g)
}

template<typename Lhs, typename Rhs, typename Var>
auto d(pow_expr<Lhs, Rhs, std::enable_if_t<std::is_base_of_v<expr, Lhs>>> pow, Var var) {
  return pow.rhs * (pow.lhs ^ (pow.rhs - 1)) * d(pow.lhs, var); // d(f^a) = a * (f^(a-1)) * d(f)
}

template<typename Lhs, typename Rhs, typename Var>
auto d(pow_expr<Lhs, Rhs, std::enable_if_t<std::is_base_of_v<expr, Rhs>>> pow, Var var) {
  return std::log(static_cast<double>(pow.lhs)) * pow * d(pow.rhs, var); // d(a^f) = ln(a) * (a^f) * d(f)
}

#endif