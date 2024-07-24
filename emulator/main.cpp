#include <iostream>
#include "bus.hpp"

int main()
{
    Bus bus;

    try
    {
        bus._toggle_bios_write();
        bus.write(0x0000, 69);

        uint8_t v = bus.read(0x0000);
        std::cout << "Read from 0x0000: " << static_cast<int>(v) << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error writing to bus: " << e.what() << std::endl;
    }

    return 0;
}
