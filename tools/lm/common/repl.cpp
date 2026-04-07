#include <chrono>

#include "repl.hpp"
#include "../compiler/lexer.hpp"
#include "../compiler/parser.hpp"
#include "../compiler/generator/generator.hpp"
#include "../compiler/generator/emit.hpp"
#include "../runtime/vm.hpp"
#include "../tools/lm/debug.hpp"

int run_repl() {
    std::string input;
    lmx::Lexer l(input, "<shell#>");
    lmx::Generator generator;
    lmx::runtime::VirtualCore core;
    core.set_program(&generator.ops);

    const std::string prompt = "\033[35m>>> \033[0m";
    while (true) {
        LOG("Getting input");
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin, input)) break;
        if (input == ":lastret") std::cout << core.look_register(0) << std::endl;
        else if (input == ":exit") break;
        else if (input == ":op") generator.print_ops();
        else if (input == ":vars") generator.print_vars();
        else {
            auto tks = l.tokenize(input);
            if (l.has_err) continue;

            lmx::Parser parser(tks, input, "<shell#>");
            auto node = parser.parse();
            if (!node || parser.has_error()) continue;

            lmx::Generator::node_has_error = false;

            const size_t op = generator.gen(node);
            if (lmx::Generator::node_has_error) continue;

            core.set_constant(generator.constant_pool.data());
            core.run();

            if (op != -1) {
                generator.regs.free(op);
                std::cout << core.look_register(op) << std::endl;
            }

            if (generator.ops.back().op == lmx::runtime::Opcode::HALT) generator.ops.pop_back();
        }
    }
    return 0;

}
