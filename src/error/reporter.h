//
// Error reporter: turns a CompileError (message + byte offset) into a
// human-readable diagnostic. This is the single place line/column and caret
// formatting live, so throw sites only have to say what went wrong and where.
//
//   2:9: error: unexpected character '@'
//       let y = @;
//               ^
//

#ifndef COMPILER_REPORTER_H
#define COMPILER_REPORTER_H

#include <cstddef>
#include <string>

#include "error/error.h"

inline std::string formatError(const std::string& source, const CompileError& err) {
    const std::size_t offset = err.offset();

    // One pass up to the offset gives us both the 1-based line number and the
    // start index of the line the error sits on.
    std::size_t line = 1;
    std::size_t lineStart = 0;
    for (std::size_t i = 0; i < offset && i < source.size(); ++i) {
        if (source[i] == '\n') {
            line += 1;
            lineStart = i + 1;
        }
    }
    const std::size_t column = offset - lineStart + 1;   // 1-based

    // The offending source line: from lineStart up to the next newline (or end).
    std::size_t lineEnd = source.find('\n', lineStart);
    if (lineEnd == std::string::npos) lineEnd = source.size();
    const std::string sourceLine = source.substr(lineStart, lineEnd - lineStart);

    // Caret sits under the offending column. Same indent as the source line, so
    // they align.
    const std::string caret = std::string(column - 1, ' ') + "^";

    return std::to_string(line) + ":" + std::to_string(column) +
           ": error: " + err.what() + "\n" +
           "    " + sourceLine + "\n" +
           "    " + caret;
}

#endif // COMPILER_REPORTER_H
