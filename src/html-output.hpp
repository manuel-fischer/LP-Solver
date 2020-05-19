#pragma once

class html_output
{
private:
    std::ostream* file_output;

public:

    html_output(std::ostream* file_output);
    ~html_output();
    std::ostream& file();
};


auto HTML_START =
R"(<!DOCTYPE html>
<html>
<head><title>LP-Solver output</title></head>
<body>
)";

auto HTML_END =
R"(
</body>
</html>
)";


html_output::html_output(std::ostream* file_output)
{
    this->file_output = file_output;

    out() << HTML_START;
}

html_output::~html_output()
{
    out() << HTML_END;
}
