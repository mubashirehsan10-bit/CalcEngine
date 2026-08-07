#include "src/ui/ui.h"
#include <iostream>

int main()
{
    try
    {
        CalcApp app;
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
    catch (const std::string& s)
    {
        std::cerr << "Fatal error: " << s << std::endl;
    }
    catch (const char* s)
    {
        std::cerr << "Fatal error: " << s << std::endl;
    }
    catch (...)
    {
        std::cerr << "Fatal error: unknown exception." << std::endl;
    }
    return 0;
}