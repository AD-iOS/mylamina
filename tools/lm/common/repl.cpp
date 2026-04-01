#include "repl.hpp"
#include "../compiler/lexer.hpp"
#include "../compiler/parser.hpp"
#include "../compiler/generator/generator.hpp"
#include "../compiler/generator/emit.hpp"
#include "../runtime/vm.hpp"
#include "../compiler/ast.hpp"
#include <chrono>

int run_repl() {
    std::string input;
    lmx::Lexer l(input);
    lmx::Generator generator;
    lmx::runtime::VirtualCore core;
    core.set_program(&generator.ops);

    const std::string prompt = "\033[35m>>> \033[0m";
    while (true) {
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin, input)) break;
        if (input == ":lastret") std::cout << core.look_register(0) << std::endl;
        else if (input == ":exit") break;
        else if (input == ":op") generator.print_ops();
        else if (input == ":vars") generator.print_vars();
        else {
            auto tks = l.tokenize(input);

            lmx::Parser parser(tks, input, "<shell#>");
            auto node = parser.parse();
            if (!node || parser.has_error()) continue;

            lmx::Generator::node_has_error = false;
            size_t op;

            try {
                op = generator.gen(node);
            } catch (const lmx::GenerError& e) {
                lmx::Generator::print_error(e);
            }
            if (lmx::Generator::node_has_error) continue;
            generator.ops.emplace_back(lmx::runtime::Opcode::HALT);

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
