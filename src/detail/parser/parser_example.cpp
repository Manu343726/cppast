#include <cppast/detail/parser/parser.hpp>
#include <iostream>

using namespace cppast::detail::parser;

class print_visitor : public detailed_visitor
{
public:
    void on_node(const ast_terminal_integer& node) override
    {
        print() << "integer literal (" << node.value << ")\n";
    }

    void on_node(const ast_terminal_float& node) override
    {
        print() << "float literal (" << node.value << ")\n";
    }

    void on_node(const ast_terminal_string& node) override
    {
        print() << "string literal (" << node.value << ")\n";
    }

    void on_node(const ast_identifier& node) override
    {
        print() << "identifier (full name: \"" << node.full_qualified_name() << "\")\n";
    }

    void on_node(const ast_expression_invoke& node) override
    {
        print() << "invoke expression (\"" << node.callee->full_qualified_name() << "(<"
            << node.args.size() << " args>)\")\n";
    }

    void on_node(const ast_expression_cpp_attribute& node) override
    {
        print() << "C++ attribute expression (\"" << node.body->callee->full_qualified_name() << "(<"
            << node.body->args.size() << " args>)\")\n";
    }

private:
    std::size_t _depth = 0;

    std::ostream& print()
    {
        return std::cout << std::string(_depth, ' ');
    }

    void on_event(event event) override
    {
        switch(event)
        {
        case event::children_enter:
            _depth++; break;
        case event::children_exit:
            _depth--; break;
        default:
            (void)42;
        }
    }
};

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        std::istringstream input{argv[1]};
        buffered_lexer lexer{input, 10};

        std::shared_ptr<ast_node> expr = parse_invoke(lexer);

        // Try again with an attribute
        if(expr == nullptr)
        {
            input.str(argv[1]);
            expr = parse_cpp_attribute(lexer);
        }

        if(expr != nullptr)
        {
            std::cout << "ok\n";

            print_visitor visitor;
            expr->visit(visitor);
        }
        else
        {
            std::cout << "error\n";
        }
    }
    else
    {
        std::cerr << "usage: parser_example <intput>";
    }
}