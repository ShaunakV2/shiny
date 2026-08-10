//
// Compile-time error type, shared by the lexer, parser, and semantic analysis.
//

#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include <cstddef>
#include <stdexcept>
#include <string>

// A compile-time error (lexing, parsing, or semantic analysis), tagged with the
// byte offset in the source where it occurred. The reporter turns the offset
// into line:col + a caret when printing; throw sites just say what went wrong
// and where. Inheriting std::runtime_error gives us what() (the message) for
// free; the offset is our addition.
class CompileError : public std::runtime_error {
public:
    CompileError(std::string message, std::size_t offset)
        : std::runtime_error(std::move(message)), offset_(offset) {}

    std::size_t offset() const { return offset_; }

private:
    std::size_t offset_;
};

#endif // COMPILER_ERROR_H
