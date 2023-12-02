#pragma once


enum TrapCode {
	/// The trap has no associated trap code.
    Undefined = -1,

    /// The trap was the result of exhausting the available stack space.
    StackOverflow = 0,

    /// The trap was the result of an out-of-bounds memory access.
    MemoryOutOfBounds = 1,

    /// The trap was the result of a wasm atomic operation that was presented with a misaligned linear-memory address.
    HeapMisaligned = 2,

    /// The trap was the result of an out-of-bounds access to a table.
    TableOutOfBounds = 3,

    /// The trap was the result of an indirect call to a null table entry.
    IndirectCallToNull = 4,

    /// The trap was the result of a signature mismatch on indirect call.
    BadSignature = 5,

    /// The trap was the result of an integer arithmetic operation that overflowed.
    IntegerOverflow = 6,

    /// The trap was the result of an integer division by zero.
    IntegerDivisionByZero = 7,

    /// The trap was the result of a failed float-to-int conversion.
    BadConversionToInteger = 8,

    /// The trap was the result of executing the `unreachable` instruction.
    Unreachable = 9,

    /// The trap was the result of interrupting execution.
    Interrupt = 10,

    /// The trap was the result of running out of the configured fuel amount.
    OutOfFuel = 11,
};
const inline char* TrapCodeToString(enum TrapCode code) {
    switch (code)
    {
    case Undefined:
        return "Undefined";
    case StackOverflow:
        return "StackOverflow";
    case MemoryOutOfBounds:
        return "MemoryOutOfBounds";
    case HeapMisaligned:
        return "HeapMisaligned";
    case TableOutOfBounds:
        return "TableOutOfBounds";
    case IndirectCallToNull:
        return "IndirectCallToNull";
    case BadSignature:
        return "BadSignature";
    case IntegerOverflow:
        return "IntegerOverflow";
    case IntegerDivisionByZero:
        return "IntegerDivisionByZero";
    case BadConversionToInteger:
        return "BadConversionToInteger";
    case Interrupt:
        return "Interrupt";
    case OutOfFuel:
        return "OutOfFuel";
    default:
        return "?";
    }
}

enum TargetType {
    SpaceBattleShip = 0,
    SpaceHulk = 1,
    Missile = 2,
    Shell = 3,
    // Asteroid = 4,  // Actually, it's nothing.
    Asteroid = 5,
};
const inline char* TargetTypeToString(enum TargetType type) {
    switch (type)
    {
    case SpaceBattleShip:
        return "SpaceBattleShip";
    case SpaceHulk:
        return "SpaceHulk";
    case Missile:
        return "Missile";
    case Shell:
        return "Shell";
    case Asteroid:
        return "Asteroid";
    default:
        return "Unknown";
    }
}
