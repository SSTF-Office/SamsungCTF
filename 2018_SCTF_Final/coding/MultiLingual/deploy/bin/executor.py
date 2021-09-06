from unicorn import Uc, UcError
from unicorn import UC_ARCH_X86, UC_ARCH_ARM64, UC_ARCH_MIPS
from unicorn import UC_MODE_32, UC_MODE_64, UC_MODE_ARM
from unicorn import UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from unicorn import UC_HOOK_INSN, UC_HOOK_INTR

ADDRESS = 0x1000000


class Error(Exception):
    def __init__(self, e):
        self.e = str(e)

    def __str__(self):
        return self.e

    def __repr__(self):
        return self.e


def runX86(code):
    output = []

    def hook_interrupt(uc, intno, user_data):
        from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EBX
        from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EDX
        if intno != 0x80:
            uc.emu_stop()
            raise Error("Unknwon Interrupt")
        eax = uc.reg_read(UC_X86_REG_EAX)
        if eax == 4:  # SYS_WRITE
            ebx = uc.reg_read(UC_X86_REG_EBX)
            ecx = uc.reg_read(UC_X86_REG_ECX)
            edx = uc.reg_read(UC_X86_REG_EDX)
            try:
                buf = uc.mem_read(ecx, edx)
                if ebx == 1:
                    user_data.extend(map(chr, buf))
                    uc.reg_write(UC_X86_REG_EAX, edx)
            except UcError:
                uc.emu_stop()
                raise Error("Segmentation fault")
        elif eax == 1:
            uc.emu_stop()
        else:
            raise Error("Unknown system call")
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    mu.mem_map(ADDRESS, 0x1000)
    mu.mem_write(ADDRESS, code)
    mu.hook_add(UC_HOOK_INTR, hook_interrupt, output)
    mu.emu_start(ADDRESS, ADDRESS + len(code))
    return ''.join(output)


def runX86_64(code):
    from unicorn.x86_const import UC_X86_INS_SYSCALL
    output = []

    def hook_syscall(uc, user_data):
        from unicorn.x86_const import UC_X86_REG_RAX, UC_X86_REG_RDI
        from unicorn.x86_const import UC_X86_REG_RSI, UC_X86_REG_RDX
        rax = uc.reg_read(UC_X86_REG_RAX)
        if rax == 1:
            rdi = uc.reg_read(UC_X86_REG_RDI)
            rsi = uc.reg_read(UC_X86_REG_RSI)
            rdx = uc.reg_read(UC_X86_REG_RDX)
            try:
                buf = uc.mem_read(rsi, rdx)
                if rdi == 1:
                    user_data.extend(map(chr, buf))
                    uc.reg_write(UC_X86_REG_RAX, rdx)
            except UcError:
                uc.emu_stop()
                raise Error("Segmentation fault")
        elif rax == 60:
            uc.emu_stop()
        else:
            raise Error("Unknown system call")

    mu = Uc(UC_ARCH_X86, UC_MODE_64)
    mu.mem_map(ADDRESS, 0x1000)
    mu.mem_write(ADDRESS, code)
    mu.hook_add(UC_HOOK_INSN, hook_syscall, output, 1, 0, UC_X86_INS_SYSCALL)
    mu.emu_start(ADDRESS, ADDRESS + len(code))
    return ''.join(output)


def runmips(code):
    output = []

    def hook_interrupt(uc, intno, user_data):
        from unicorn.mips_const import UC_MIPS_REG_2, UC_MIPS_REG_4
        from unicorn.mips_const import UC_MIPS_REG_5, UC_MIPS_REG_6
        if intno != 17:
            uc.emu_stop()
            raise Exception("Unknwon Interrupt")
        v0 = uc.reg_read(UC_MIPS_REG_2)
        if v0 == 4004:  # SYS_WRITE
            a0 = uc.reg_read(UC_MIPS_REG_4)
            a1 = uc.reg_read(UC_MIPS_REG_5)
            a2 = uc.reg_read(UC_MIPS_REG_6)
            try:
                buf = uc.mem_read(a1, a2)
                if a0 == 1:
                    user_data.extend(map(chr, buf))
                    uc.reg_write(UC_MIPS_REG_2, a2)
            except UcError:
                uc.emu_stop()
                raise Error("Segmentation fault")
        elif v0 == 4001:
            uc.emu_stop()
        else:
            uc.emu_stop()
            raise Error("Unknown system call")

    mu = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 + UC_MODE_LITTLE_ENDIAN)
    mu.mem_map(ADDRESS, 0x1000)
    mu.mem_write(ADDRESS, code)
    mu.hook_add(UC_HOOK_INTR, hook_interrupt, output)
    mu.emu_start(ADDRESS, ADDRESS + len(code))
    return ''.join(output)


def runarm64(code):
    output = []

    def hook_interrupt(uc, intno, user_data):
        from unicorn.arm64_const import UC_ARM64_REG_X0, UC_ARM64_REG_X1
        from unicorn.arm64_const import UC_ARM64_REG_X2, UC_ARM64_REG_X8
        if intno != 2:
            uc.emu_stop()
            raise Exception("Unknwon Interrupt")
        x8 = uc.reg_read(UC_ARM64_REG_X8)
        if x8 == 64:  # SYS_WRITE
            x0 = uc.reg_read(UC_ARM64_REG_X0)
            x1 = uc.reg_read(UC_ARM64_REG_X1)
            x2 = uc.reg_read(UC_ARM64_REG_X2)
            try:
                buf = uc.mem_read(x1, x2)
                if x0 == 1:
                    user_data.extend(map(chr, buf))
                    uc.reg_write(UC_ARM64_REG_X0, x2)
            except UcError:
                uc.emu_stop()
                raise Error("Segmentation fault")
        elif x8 == 93:
            uc.emu_stop()
        else:
            uc.emu_stop()
            raise Error("Unknown system call")

    mu = Uc(UC_ARCH_ARM64, UC_MODE_ARM)
    mu.mem_map(ADDRESS, 0x1000)
    mu.mem_write(ADDRESS, code)
    mu.hook_add(UC_HOOK_INTR, hook_interrupt, output)
    mu.emu_start(ADDRESS, ADDRESS + len(code))
    return ''.join(output)


def execute_arch(code, arch):
    try:
        return ({'X86': runX86,
                 'X86_64': runX86_64,
                 'mips': runmips,
                 'aarch64': runarm64}
                [arch](code))
    except KeyError:
        print ("Unknown arch: {}". format(arch))
    except UcError as e:
        raise Error(e)


TARGETS = ['X86', 'X86_64', 'aarch64', 'mips']
