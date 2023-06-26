#include "parser.hpp"

int main() {
    constexpr char json_file[]{"euraud.json"};
    fx::Parser(json_file).Run();
}
