//
// Created by geguj on 2025/12/28.
//

#pragma once
#include <iostream>
#include <memory>
#include <utility>

#include "../include/lmx_export.hpp"
#include "lexer.hpp"
#include "ast.hpp"

namespace lmx {

class ParserError final : public std::runtime_error {
public:
    explicit ParserError(const std::string& msg)
        : std::runtime_error(msg) {}
};

class LMC_API Parser {
    bool in_module{false};
    bool has_err{false};
    size_t pos{0};

    std::vector<Token>& tokens;
    std::string code, filename;

    void parse_args(std::vector<std::shared_ptr<ASTNode>> &args);

    void advance(ssize_t steps = 1);
    [[nodiscard]] Token& cur() const;
    [[nodiscard]] bool match(TokenType t) const;
    [[nodiscard]] bool is_eof() const;
    std::shared_ptr<ExprNode> expr();
    std::shared_ptr<ExprNode> term();


    std::shared_ptr<ExprNode> factor();

    [[nodiscard]] bool peek_match(TokenType type) const;

    void error(const std::string& msg);

    std::shared_ptr<BlockStmtNode> parse_block();
    std::shared_ptr<StringNode> parse_string();
    std::shared_ptr<ASTNode> parse_if();
    std::shared_ptr<ExprNode> parse_expr();
    std::shared_ptr<ExprNode> parse_logical_and();
    std::shared_ptr<ExprNode> parse_relational();
    std::shared_ptr<ExprNode> parse_logical_or();
    std::shared_ptr<ASTNode> parse_func_decl(bool has_block);
    std::shared_ptr<ExprNode> parse_func_call();

    std::shared_ptr<TypeNode> parse_type();
    std::shared_ptr<ASTNode> parse_module();
public:
    explicit Parser(
        std::vector<Token>& tokens,
        std::string code = "",
        std::string filename = "<unknown>"
    ): tokens(tokens), code(std::move(code)), filename(std::move(filename)) {}

    std::shared_ptr<ASTNode> parse();
    std::shared_ptr<ProgramASTNode> parse_program();

    [[nodiscard]] bool has_error() const {return has_err;}
    void print_error(const ParserError& error) const;
};

} // lmx