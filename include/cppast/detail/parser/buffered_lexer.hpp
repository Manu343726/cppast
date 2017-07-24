#ifndef CPPAST_DETAIL_PARSER_BUFFERED_LEXER_INCLUDED
#define CPPAST_DETAIL_PARSER_BUFFERED_LEXER_INCLUDED

#include <cppast/detail/parser/lexer.hpp>
#include <vector>

namespace cppast
{

namespace detail
{

namespace parser
{

class buffered_lexer : public lexer
{
public:
    buffered_lexer(lexer& lexer, std::size_t size);

    bool read_next_token() override;
    const token& current_token() const override;

    const token& next_token(std::size_t i) const;
    std::size_t buffer_size() const;

    bool good() const override;
    bool eof() const override;

    void reset() override;

private:
    lexer& _lexer;
    token _current_token;
    std::vector<token> _lookahead_buffer;
};

}

}

}

#endif // CPPAST_DETAIL_PARSER_BUFFERED_LEXER_INCLUDED
