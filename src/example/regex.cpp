#include "./regex.h"

void show_matches(const std::string& in, const std::regex& re) {
  std::smatch m;
  std::regex_search(in, m, re);

  if (!m.empty()) {
    std::cout << "input=[" << in << "]\n prefix=[" << m.prefix() << "]\n  smatch: ";

    for (std::size_t n = 0; n < m.size(); ++n) std::cout << "m[" << n << "]=[" << m[n] << "] ";

    std::cout << "\n  suffix=[" << m.suffix() << "]\n";
  } else {
    std::cout << "input=[" << in << "]: NO MATCH\n";
  }
}

void test_backref() {
  std::regex re("^(?:\\(\1*\\))*$");

  show_matches("()", re);
  show_matches("()(", re);
  show_matches("())", re);
  show_matches("(())", re);
  show_matches("()()", re);
}
