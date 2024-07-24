#include "bus.hpp"

Bus::Bus()
{
    bios.fill(0);
    interrupt_vectors.fill(0);
    io_ports.fill(0);
    program_code.fill(0);
}

std::future<std::optional<uint8_t>> Bus::async_read(uint16_t address)
{
    return std::async(std::launch::async, [this, address]()
                      {
        std::lock_guard<std::mutex> lock(bus_mutex);
        return read_internal(address); });
}

std::future<void> Bus::async_write(uint16_t address, uint8_t value)
{
    return std::async(std::launch::async, [this, address, value]()
                      {
        std::lock_guard<std::mutex> lock(bus_mutex);
        write_internal(address, value); });
}

uint8_t Bus::read(uint16_t address)
{
    auto future = async_read(address);
    auto result = future.get(); // Block until the async read operation completes
    if (result)
    {
        return *result;
    }
    else
    {
        throw std::out_of_range("Address out of range");
    }
}

void Bus::write(uint16_t address, uint8_t value)
{
    auto future = async_write(address, value);
    future.get(); // Block until the async write operation completes
}

std::optional<uint8_t> Bus::read_internal(uint16_t address) const
{
    if (is_bios(address))
    {
        return bios[address];
    }
    else if (is_interrupt_vectors(address))
    {
        return interrupt_vectors[address - 0x1000];
    }
    else if (is_io_ports(address))
    {
        return io_ports[address - 0x1100];
    }
    else if (is_program_code(address))
    {
        return program_code[address - 0x1200];
    }
    else
    {
        return std::nullopt;
    }
}

void Bus::write_internal(uint16_t address, uint8_t value)
{
    if (is_bios(address))
    {
        if (BIOS_WRITE)
        {
            bios[address] = value;
            _toggle_bios_write();
        }
        else
        {
            throw std::runtime_error("Cannot write to BIOS");
        }
    }
    else if (is_interrupt_vectors(address))
    {
        interrupt_vectors[address - 0x1000] = value;
    }
    else if (is_io_ports(address))
    {
        io_ports[address - 0x1100] = value;
    }
    else if (is_program_code(address))
    {
        program_code[address - 0x1200] = value;
    }
    else
    {
        throw std::out_of_range("Address out of range");
    }
}

bool Bus::is_bios(uint16_t address) const
{
    return address >= 0x0000 && address < 0x1000;
}

bool Bus::is_interrupt_vectors(uint16_t address) const
{
    return address >= 0x1000 && address < 0x1100;
}

bool Bus::is_io_ports(uint16_t address) const
{
    return address >= 0x1100 && address < 0x1200;
}

bool Bus::is_program_code(uint16_t address) const
{
    return address >= 0x1200 && address < 0x10000;
}

void Bus::_toggle_bios_write()
{
    BIOS_WRITE = !BIOS_WRITE;
}