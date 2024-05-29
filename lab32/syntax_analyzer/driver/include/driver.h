#pragma once



class Driver {
public:
  Driver();
  Driver(Driver &&) = default;
  Driver(const Driver &) = default;
  Driver &operator=(Driver &&) = default;
  Driver &operator=(const Driver &) = default;
  ~Driver();

private:
  
};

Driver::Driver() {
}

Driver::~Driver() {
}
