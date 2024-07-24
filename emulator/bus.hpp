#pragma once

#include <cstdint>
#include <array>
#include <optional>
#include <future>
#include <mutex>
#include <stdexcept>

class Bus
{
public:
    Bus();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

    void _toggle_bios_write();

private:
    bool BIOS_WRITE = false;
    static constexpr std::size_t BIOS_SIZE = 0x1000;
    static constexpr std::size_t IVT_SIZE = 0x0100;
    static constexpr std::size_t IO_PORTS_SIZE = 0x0100;
    static constexpr std::size_t PROGRAM_SIZE = 0xE800;

    std::array<uint8_t, BIOS_SIZE> bios;
    std::array<uint8_t, IVT_SIZE> interrupt_vectors;
    std::array<uint8_t, IO_PORTS_SIZE> io_ports;
    std::array<uint8_t, PROGRAM_SIZE> program_code;

    std::mutex bus_mutex;

    // Address range checks
    bool is_bios(uint16_t address) const;
    bool is_interrupt_vectors(uint16_t address) const;
    bool is_io_ports(uint16_t address) const;
    bool is_program_code(uint16_t address) const;

    // Helper methods for async operations
    std::optional<uint8_t> read_internal(uint16_t address) const;
    void write_internal(uint16_t address, uint8_t value);

    // Asynchronous read and write operations
    std::future<std::optional<uint8_t>> async_read(uint16_t address);
    std::future<void> async_write(uint16_t address, uint8_t value);
};
